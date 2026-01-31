//
// Created by Krzysztof Sawicki on 31/01/2026.
//

#include "../Inc/AngleDial.h"
#include <QPainter>
#include <QStyleOptionSlider>

AngleDial::AngleDial(QWidget *parent): QDial(parent) {
    setNotchesVisible(true);
}

void AngleDial::paintEvent(QPaintEvent *e)
{
    QDial::paintEvent(e);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Draw angle in center
    QString txt = QString::number(m_angleDeg, 'f', 1) + QChar(0x00B0);
    QFont f = p.font();
    f.setBold(true);
    f.setPointSize(qMax(8, width()/20));
    p.setFont(f);

    p.drawText(rect(), Qt::AlignCenter, txt);
}