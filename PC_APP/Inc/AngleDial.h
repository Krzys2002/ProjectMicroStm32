//
// Created by Krzysztof Sawicki on 31/01/2026.
//

#ifndef PC_APP_ANGLEDIAL_H
#define PC_APP_ANGLEDIAL_H
#include <QDial>

class AngleDial : public QDial {
    Q_OBJECT
  public:
        explicit AngleDial(QWidget *parent=nullptr);

        void setAngleText(double deg) { m_angleDeg = deg; update(); }
    protected:
        void paintEvent(QPaintEvent *e) override;

    private:
        double m_angleDeg = 0.0;
};
#endif //PC_APP_ANGLEDIAL_H