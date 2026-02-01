//
// Created by Krzysztof Sawicki on 31/01/2026.
//

#ifndef PC_APP_MAINWINDOW_H
#define PC_APP_MAINWINDOW_H
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "DeviceClient.h"
#include "AngleDial.h"
#include "SerialConsole.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
        explicit MainWindow(QWidget *parent=nullptr);

    private slots:
      void onFound(QHostAddress ip, quint16 port, QString name);
        void onJson(QByteArray line);
        void onDiscoveryTimeout();
        void toggleMode();
        void sendSetpoint();
        void discover();
        void toggleConsole();

    private:
        void updateModeUi();
        void sendOutCmd();

        DeviceClient m_client;
        SerialConsole *m_console = nullptr;

        // UI
        AngleDial *m_posDial = nullptr;
        QLabel *m_speedLabel = nullptr;

        AngleDial *m_setDial = nullptr;

        QPushButton *m_modeBtn = nullptr;
        QLabel *m_modeLabel = nullptr;

        QPushButton *m_discoverBtn = nullptr;
        QLineEdit *m_udpAddrEdit = nullptr;
        QComboBox *m_serialPorts = nullptr;
        QPushButton *m_connectSerialBtn = nullptr;

        QHostAddress m_lastIp;
        quint16 m_lastPort = 5000;

        // current mode shown/used
        DeviceClient::Mode m_mode = DeviceClient::ENT;
};
#endif //PC_APP_MAINWINDOW_H