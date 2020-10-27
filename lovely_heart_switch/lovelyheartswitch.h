#ifndef LOVELYHEARTSWITCH_H
#define LOVELYHEARTSWITCH_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "math.h"

class LovelyHeartSwitch : public QWidget
{
    Q_OBJECT
public:
    LovelyHeartSwitch(QWidget *parent = nullptr);

    void setState(bool state);
    void setStateWithoutSignal(bool state);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    void calculateGeometry();
    void startSwitchAnimation();

signals:
    void stateChanged(bool state);

public slots:

private:
    // 状态
    bool currentState = false; // 开关状态
    double aniProgess = 0;     // 当前动画值，趋向state，范围0~1

    // 几何
    const double PI = 3.1415926535;
    const double GenHao2 = sqrt(2.0);
    QPointF rightAnglePos; // 中间直角的点
    double diamondSide;      // 每个菱形单位的边长
    double circleRadius;
};

#endif // LOVELYHEARTSWITCH_H
