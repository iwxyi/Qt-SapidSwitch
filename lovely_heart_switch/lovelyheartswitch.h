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
    Q_PROPERTY(double swtch READ getSwtchProg WRITE setSwtchProg)
    Q_PROPERTY(double press READ getPressProg WRITE setPressProg)
public:
    LovelyHeartSwitch(QWidget *parent = nullptr);

public slots:
    void setState(bool state);
    void setStateWithoutSignal(bool state);
    void switchState();
    void switchStateWithoutSignal();
    void setColors(QColor on, QColor off);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void calculateGeometry();
    QPainterPath getBgPath();
    void startSwitchAnimation();

    double getSwtchProg();
    void setSwtchProg(double p);
    double getPressProg();
    void setPressProg(double p);

signals:
    void stateChanged(bool state);

private:
    // 状态
    bool currentState = false; // 开关状态
    double aniProgess = 0;     // 当前动画值，趋向state，范围0~1
    QColor colorOn = QColor(236, 97, 139);
    QColor colorOff = Qt::lightGray;

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
    double pressAniProg;
    bool moved = false;
    bool dragging = false;
    bool moveTargetState = false; // 滑动的目标状态，等待松手
    const double stickOnProp = 0.15; // 在两侧贴靠，不收左右滑动手势影响
    int prevX = 0;

    // 按压逻辑
    const double pressScale = 0.9; // 按压缩小动画
    double pressScaleProgress = 1;
};

#endif // LOVELYHEARTSWITCH_H
