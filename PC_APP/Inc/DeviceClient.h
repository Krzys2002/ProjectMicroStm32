//
// Created by Krzysztof Sawicki on 31/01/2026.
//

#ifndef PC_APP_DEVICECLIENT_H
#define PC_APP_DEVICECLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QSerialPort>
#include <QTimer>

class DeviceClient : public QObject {
    Q_OBJECT
  public:
        enum Mode { SP, ENT };
        explicit DeviceClient(QObject *parent=nullptr);

        void setMode(Mode m);
        Mode mode() const { return m_mode; }

        // Serial
        void openSerial(const QString &portName, int baud=115200);
        void closeSerial();

        // Ethernet
        void startDiscovery(int timeoutMs=300);
        void connectTcp(const QHostAddress &ip, quint16 port);
        void disconnectTcp();

        // Send NDJSON command (adds \n)
        void sendJson(const QByteArray &json);

        signals:
          void deviceFound(QHostAddress ip, quint16 port, QString name);
        void connected();
        void disconnected();
        void jsonLineReceived(QByteArray line);
        void error(QString msg);

    private slots:
      void onUdpReady();
        void onDiscoveryTimeout();
        void onTcpReady();
        void onSerialReady();

    private:
        Mode m_mode = ENT;

        QUdpSocket m_udp;
        QTcpSocket m_tcp;
        QSerialPort m_serial;
        QTimer m_discTimer;

        QByteArray m_rxTcp;
        QByteArray m_rxSerial;
};
#endif //PC_APP_DEVICECLIENT_H