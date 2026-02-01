//
// Created by Krzysztof Sawicki on 31/01/2026.
//
#include "../Inc/MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
  QWidget *c = new QWidget(this);
  setCentralWidget(c);

  m_posDial = new AngleDial;
  m_posDial->setRange(0, 360);
  m_posDial->setEnabled(false);

  m_speedLabel = new QLabel("Speed: 0.0 deg/s");

  m_setDial = new AngleDial;
  m_setDial->setRange(0, 360);
  m_setDial->setValue(0);
  m_setDial->setAngleText(0);
  connect(m_setDial, &AngleDial::valueChanged, this, [this](int val){
      m_setDial->setAngleText(val);
  });

  QPushButton *sendSp = new QPushButton("Send Setpoint");

  m_modeBtn = new QPushButton;
  m_modeLabel = new QLabel;

  m_discoverBtn = new QPushButton("UDP Discover");
  m_udpAddrEdit = new QLineEdit("255.255.255.255");
  m_udpAddrEdit->setFixedWidth(120);
  m_serialPorts = new QComboBox;
  m_connectSerialBtn = new QPushButton("Open Serial");
  QPushButton *consoleBtn = new QPushButton("Serial Console");

  // Fill serial ports
  for (auto &p : QSerialPortInfo::availablePorts())
    m_serialPorts->addItem(p.portName());

  // Layout
  auto *top = new QHBoxLayout;
  top->addWidget(m_modeLabel);
  top->addWidget(m_modeBtn);
  top->addStretch();
  top->addWidget(m_discoverBtn);
  top->addWidget(m_udpAddrEdit);
  top->addWidget(m_serialPorts);
  top->addWidget(m_connectSerialBtn);
  top->addWidget(consoleBtn);

  auto *v = new QVBoxLayout(c);
  v->addLayout(top);
  v->addWidget(new QLabel("Current Position"));
  v->addWidget(m_posDial);
  v->addWidget(m_speedLabel);
  v->addSpacing(10);
  v->addWidget(new QLabel("Set Target Position"));
  v->addWidget(m_setDial);
  v->addWidget(sendSp);

  // Client signals
  connect(&m_client, &DeviceClient::deviceFound, this, &MainWindow::onFound);
  connect(&m_client, &DeviceClient::jsonLineReceived, this, &MainWindow::onJson);
  connect(&m_client, &DeviceClient::discoveryTimeout, this, &MainWindow::onDiscoveryTimeout);

  connect(m_modeBtn, &QPushButton::clicked, this, &MainWindow::toggleMode);
  connect(sendSp, &QPushButton::clicked, this, &MainWindow::sendSetpoint);
  connect(m_discoverBtn, &QPushButton::clicked, this, &MainWindow::discover);
  connect(consoleBtn, &QPushButton::clicked, this, &MainWindow::toggleConsole);

  connect(m_connectSerialBtn, &QPushButton::clicked, this, [this]{
    m_mode = DeviceClient::SP;
    m_client.setMode(DeviceClient::SP);
    updateModeUi();
    m_client.openSerial(m_serialPorts->currentText(), 115200);
    sendOutCmd(); // tell STM to use UART output
  });

  // Default mode ENT
  m_client.setMode(DeviceClient::ENT);
  updateModeUi();
}

void MainWindow::updateModeUi()
{
  if (m_mode == DeviceClient::ENT) {
    m_modeLabel->setText("Mode: ENT (Ethernet)");
    m_modeBtn->setText("Switch to SP");
    m_discoverBtn->setEnabled(true);
    m_connectSerialBtn->setEnabled(true);
  } else {
    m_modeLabel->setText("Mode: SP (Serial)");
    m_modeBtn->setText("Switch to ENT");
    m_discoverBtn->setEnabled(true);
    m_connectSerialBtn->setEnabled(true);
  }
}

void MainWindow::toggleMode()
{
  if (m_mode == DeviceClient::ENT) {
    m_mode = DeviceClient::SP;
    m_client.setMode(DeviceClient::SP);
    updateModeUi();
    sendOutCmd();
  } else {
    m_mode = DeviceClient::ENT;
    m_client.setMode(DeviceClient::ENT);
    updateModeUi();
    // if we already discovered, reconnect
    if (!m_lastIp.isNull()) m_client.connectTcp(m_lastIp, m_lastPort);
    sendOutCmd();
  }
}

void MainWindow::discover()
{
  m_mode = DeviceClient::ENT;
  m_client.setMode(DeviceClient::ENT);
  updateModeUi();
  m_client.startDiscovery(m_udpAddrEdit->text(), 1000);
}

void MainWindow::onFound(QHostAddress ip, quint16 port, QString name)
{
  m_lastIp = ip;
  m_lastPort = port;
  setWindowTitle("STM32: " + name + " @ " + ip.toString());

  // Connect TCP
  m_client.connectTcp(ip, port);

  // Ask STM to switch output to ETH
  m_mode = DeviceClient::ENT;
  m_client.setMode(DeviceClient::ENT);
  updateModeUi();
  sendOutCmd();
}

void MainWindow::onDiscoveryTimeout()
{
  QMessageBox::warning(this, "Discovery Timeout", "No STM32 devices found on the network.");
}

void MainWindow::sendOutCmd()
{
  if (m_mode == DeviceClient::ENT)
    m_client.sendJson(R"({"cmd":"out","value":"ETH"})");
  else
    m_client.sendJson(R"({"cmd":"out","value":"UART"})");
}

void MainWindow::sendSetpoint()
{
  double deg = m_setDial->value();
  QByteArray cmd = QByteArray::fromStdString(
    std::string("{\"cmd\":\"setpoint\",\"deg\":") + std::to_string(deg) + "}"
  );
  m_client.sendJson(cmd);
}

void MainWindow::onJson(QByteArray line)
{
  if (m_console) m_console->appendText(line);

  // Parse telemetry JSON
  auto doc = QJsonDocument::fromJson(line);
  if (!doc.isObject()) return;
  auto o = doc.object();

  if (o.value("type").toString() == "pid") {
    double pos = o.value("pos").toDouble();
    double spd = o.value("spd").toDouble();

    // update current position dial + center text
    int dialVal = (int)pos % 360;
    if (dialVal < 0) dialVal += 360;
    m_posDial->setValue(dialVal);
    m_posDial->setAngleText(pos);

    m_speedLabel->setText(QString("Speed: %1 deg/s").arg(spd, 0, 'f', 2));
  }
}

void MainWindow::toggleConsole()
{
  if (!m_console) {
    m_console = new SerialConsole;
  }
  if (m_console->isVisible()) {
    m_console->hide();
  } else {
    m_console->show();
  }
}