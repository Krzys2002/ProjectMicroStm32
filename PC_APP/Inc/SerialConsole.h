//
// Created by Junie on 31/01/2026.
//

#ifndef PC_APP_SERIALCONSOLE_H
#define PC_APP_SERIALCONSOLE_H

#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>

class SerialConsole : public QWidget {
    Q_OBJECT
public:
    explicit SerialConsole(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Serial Console");
        resize(400, 300);

        auto *layout = new QVBoxLayout(this);
        m_textEdit = new QTextEdit(this);
        m_textEdit->setReadOnly(true);
        layout->addWidget(m_textEdit);

        auto *clearBtn = new QPushButton("Clear", this);
        connect(clearBtn, &QPushButton::clicked, m_textEdit, &QTextEdit::clear);
        layout->addWidget(clearBtn);
    }

public slots:
    void appendText(const QByteArray &data) {
        m_textEdit->append(QString::fromUtf8(data));
    }

private:
    QTextEdit *m_textEdit;
};

#endif //PC_APP_SERIALCONSOLE_H
