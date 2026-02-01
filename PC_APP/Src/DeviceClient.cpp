//
// Created by Krzysztof Sawicki on 31/01/2026.
//

#include "../Inc/DeviceClient.h"
#include <QNetworkDatagram>

static const quint16 DISCOVERY_PORT = 40000;

DeviceClient::DeviceClient(QObject *parent): QObject(parent) {
  connect(&m_udp, &QUdpSocket::readyRead, this, &DeviceClient::onUdpReady);
  connect(&m_discTimer, &QTimer::timeout, this, &DeviceClient::onDiscoveryTimeout);
  m_discTimer.setSingleShot(true);

  connect(&m_tcp, &QTcpSocket::readyRead, this, &DeviceClient::onTcpReady);
  connect(&m_tcp, &QTcpSocket::connected, this, &DeviceClient::connected);
  connect(&m_tcp, &QTcpSocket::disconnected, this, &DeviceClient::disconnected);
  connect(&m_tcp, &QTcpSocket::errorOccurred, this, [this](auto){
    emit error(m_tcp.errorString());
  });

  connect(&m_serial, &QSerialPort::readyRead, this, &DeviceClient::onSerialReady);
  connect(&m_serial, &QSerialPort::errorOccurred, this, [this](auto){
    if (m_serial.error() != QSerialPort::NoError)
      emit error(m_serial.errorString());
  });
}

void DeviceClient::setMode(Mode m) { m_mode = m; }

void DeviceClient::openSerial(const QString &portName, int baud) {
  m_serial.close();
  m_serial.setPortName(portName);
  m_serial.setBaudRate(baud);
  if (!m_serial.open(QIODevice::ReadWrite))
    emit error("Serial open failed: " + m_serial.errorString());
}

void DeviceClient::closeSerial(){ m_serial.close(); }

void DeviceClient::startDiscovery(const QString &broadcastAddr, int timeoutMs) {
  if (!m_udp.isValid()) {
    if (!m_udp.bind(QHostAddress::AnyIPv4, DISCOVERY_PORT,
                    QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
      emit error("UDP bind failed: " + m_udp.errorString());
      return;
    }
  }

  QByteArray msg = "WHO_IS_STM32?\n";
  m_udp.writeDatagram(msg, QHostAddress(broadcastAddr), DISCOVERY_PORT);

  m_discTimer.start(timeoutMs);
}

void DeviceClient::connectTcp(const QHostAddress &ip, quint16 port) {
  if (m_tcp.state() != QAbstractSocket::UnconnectedState) m_tcp.abort();
  m_tcp.connectToHost(ip, port);
}

void DeviceClient::disconnectTcp(){ m_tcp.disconnectFromHost(); }

void DeviceClient::sendJson(const QByteArray &json) {
  QByteArray line = json;
  if (!line.endsWith('\n')) line.append('\n');

  if (m_mode == ENT) {
    if (m_tcp.state() == QAbstractSocket::ConnectedState) m_tcp.write(line);
  } else {
    if (m_serial.isOpen()) m_serial.write(line);
  }
}

void DeviceClient::onUdpReady() {
  while (m_udp.hasPendingDatagrams()) {
    QNetworkDatagram dg = m_udp.receiveDatagram();
    QString s = QString::fromUtf8(dg.data()).trimmed();
    if (!s.startsWith("STM32")) continue;

    m_discTimer.stop(); // Stop timer if we found a device

    quint16 port = 5000;
    QString name;

    const auto parts = s.split(';');
    for (const auto &p : parts) {
      if (p.startsWith("TCP=")) port = p.mid(4).toUShort();
      if (p.startsWith("NAME=")) name = p.mid(5);
    }
    emit deviceFound(dg.senderAddress(), port, name);
  }
}

void DeviceClient::onDiscoveryTimeout() {
  emit discoveryTimeout();
}

static void emitLinesFromBuffer(QByteArray &buf, QObject *obj, void (DeviceClient::*sig)(QByteArray)) {
  int idx;
  while ((idx = buf.indexOf('\n')) >= 0) {
    QByteArray line = buf.left(idx);
    buf.remove(0, idx + 1);
    if (!line.isEmpty()) QMetaObject::invokeMethod(obj, [obj, sig, line]{
      (static_cast<DeviceClient*>(obj)->*sig)(line);
    }, Qt::QueuedConnection);
  }
}

void DeviceClient::onTcpReady() {
  m_rxTcp.append(m_tcp.readAll());
  emitLinesFromBuffer(m_rxTcp, this, &DeviceClient::jsonLineReceived);
}

void DeviceClient::onSerialReady() {
  m_rxSerial.append(m_serial.readAll());
  emitLinesFromBuffer(m_rxSerial, this, &DeviceClient::jsonLineReceived);
}