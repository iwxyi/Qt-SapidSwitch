#ifndef LOVELYHEARTSWITCH_H
#define LOVELYHEARTSWITCH_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "math.h"

class LovelyHeartSwitch : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double swtch READ getSwtchProg WRITE setSwtchProg NOTIFY swtchChanged)
public:
    LovelyHeartSwitch(QWidget *parent = nullptr);

    void setState(bool state);
    void setStateWithoutSignal(bool state);
    void switchState();
    void switchStateWithoutSignal();

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void calculateGeometry();
    void startSwitchAnimation();

    double getSwtchProg();
    void setSwtchProg(double p);

signals:
    void stateChanged(bool state);
    void swtchChanged(double prog);

public slots:

private:
    // 状态
    bool currentState = false; // 开关状态
    double aniProgess = 0;     // 当前动画值，趋向state，范围0~1

    // 几何
    const double PI = 3.1415926535;
    const double GenHao2 = sqrt(2.0);
    QPointF rightAnglePos; // 中间直角的点
    double diamondSide;    // 每个菱形单位的边长
    double circleRadius;
    double circleOutBorder = 2;
    double slideLeft, slideRight;

    // 交互
    QPoint pressPos;
    bool moved = false;
    bool dragging = false;
};

#endif // LOVELYHEARTSWITCH_H
