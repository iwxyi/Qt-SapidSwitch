#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QApplication>
#include "lovelyheartswitch.h"

LovelyHeartSwitch::LovelyHeartSwitch(QWidget *parent) : QWidget(parent)
{
    calculateGeometry();
}

void LovelyHeartSwitch::setState(bool state)
{
    setStateWithoutSignal(state);
    emit stateChanged(currentState);
}

void LovelyHeartSwitch::setStateWithoutSignal(bool state)
{
    this->currentState = state;
    startSwitchAnimation();
}

void LovelyHeartSwitch::switchState()
{
    switchStateWithoutSignal();
    emit stateChanged(currentState);
}

void LovelyHeartSwitch::switchStateWithoutSignal()
{
    setState(!currentState);
}

void LovelyHeartSwitch::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    const double& r = diamondSide;
    const double r_2 = r / 2;

    // 爱心外框路径
    QPainterPath path;
    path.moveTo(rightAnglePos);
    // 右上角耳朵
    const QPointF center1(rightAnglePos.x() + r_2/GenHao2, rightAnglePos.y() + r/2/GenHao2);
    const QRectF rect1(center1.x()-r_2, center1.y()-r_2, r, r);
    path.arcTo(rect1, 135, -180);
    // 中下部分
    const QPointF center2 = rightAnglePos;
    const double radius2 = r;
    const QRectF rect2(center2.x() - radius2, center2.y() - radius2, 2*radius2, 2*radius2);
    path.arcTo(rect2, -45, -90);
    // 左上角耳朵
    const QPointF center3(rightAnglePos.x() - r_2/GenHao2, rightAnglePos.y() + r/2/GenHao2);
    const QRectF rect3 = QRectF(center3.x()-r_2, center3.y()-r_2, r, r);
    path.arcTo(rect3, -135, -180);

    // 计算颜色
    QColor bg(
                colorOff.red() + (colorOn.red() - colorOff.red())*aniProgess,
                colorOff.green() + (colorOn.green() - colorOff.green())*aniProgess,
                colorOff.blue() + (colorOn.blue() - colorOff.blue())*aniProgess,
                colorOff.alpha() + (colorOn.alpha() - colorOff.alpha())*aniProgess
                );
    painter.fillPath(path, bg);

    // 绘制开关状态
    QPointF slideCenter(rightAnglePos.x(), rightAnglePos.y());
    const double slideRadius = circleRadius + circleOutBorder;
    QRectF slideRect(slideCenter.x()-slideRadius, slideCenter.y()-slideRadius, slideRadius*2, slideRadius*2);
    QPainterPath circleSlidePath;
    circleSlidePath.moveTo(rightAnglePos.x()-slideRadius/GenHao2, rightAnglePos.y()+slideRadius/GenHao2);
    circleSlidePath.arcTo(slideRect, -135, 90);
    QPointF circlePos = circleSlidePath.pointAtPercent(aniProgess);
    QPainterPath circlePath;
    circlePath.addEllipse(circlePos.x()-circleRadius, circlePos.y()-circleRadius, circleRadius*2, circleRadius*2);
    painter.fillPath(circlePath, QColor(255, 250, 250));
}

void LovelyHeartSwitch::resizeEvent(QResizeEvent *)
{
    calculateGeometry();
}

void LovelyHeartSwitch::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressPos = event->pos();
        moved = false;
        dragging = false;
        prevX = pressPos.x();
    }
    QWidget::mousePressEvent(event);
}

void LovelyHeartSwitch::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if (!moved && (event->pos() - pressPos).manhattanLength() > QApplication::startDragDistance())
        {
            moved = true;
        }
        if (moved)
        {
            // 设置圆圈的位置比例
            int x = event->pos().x();
            if (x <= slideLeft)
                aniProgess = 0;
            else if (x >= slideRight)
                aniProgess = 1;
            else
                aniProgess = static_cast<double>(x - slideLeft) / (slideRight - slideLeft);
            update();

            moveTargetState = (x > prevX ? true : false);
            prevX = x;
        }
    }
    QWidget::mouseMoveEvent(event);
}

void LovelyHeartSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!moved)
        {
            switchState();
        }
        else
        {
            bool oldState = currentState;
            const double totalDistance = slideRight - slideLeft;
            const double stickDistance = totalDistance * stickOnProp;
            int x = event->pos().x();
            if (x <= slideLeft + stickDistance)
            {
                currentState = false;
            }
            else if (x >= slideRight - stickDistance)
            {
                currentState = true;
            }
            else
            {
                currentState = moveTargetState;
            }
            if (currentState != oldState)
            {
                emit stateChanged(currentState);
            }
            update();
        }

        startSwitchAnimation();
    }
    QWidget::mouseReleaseEvent(event);
}

/**
 * 计算各个点的恰当坐标，以填充整个矩形
 */
void LovelyHeartSwitch::calculateGeometry()
{
    const double widthProp = GenHao2 + 1.0 - 1.0/GenHao2;
    const double heightProp = (GenHao2 - 1.0 + 2 * GenHao2) / 2 / GenHao2;
    diamondSide = qMin((width()-2)/widthProp, (height()-2)/heightProp);
    circleRadius = diamondSide/2-2;
    const double topProp = (GenHao2-1)/2/GenHao2/heightProp;
    rightAnglePos = QPointF(width()/2,
                           diamondSide*heightProp * topProp + (height()-diamondSide*heightProp)/2);

    const double slideRadius = circleRadius + circleOutBorder;
    slideLeft = rightAnglePos.x()-slideRadius/GenHao2;
    slideRight = rightAnglePos.x()+slideRadius/GenHao2;
}

void LovelyHeartSwitch::startSwitchAnimation()
{
    const double target = currentState ? 1 : 0;
    if (qAbs(target-aniProgess) < 1e-4)
        return ;
    int duration = static_cast<int>(qAbs((aniProgess - target) * (slideRight - slideLeft) * 10));
    duration = qMin(duration, 350);

    QPropertyAnimation* ani = new QPropertyAnimation(this, "swtch");
    ani->setStartValue(aniProgess);
    ani->setEndValue(target);
    if (duration > 50)
        ani->setEasingCurve(QEasingCurve::InOutCubic);
    ani->setDuration(duration);
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    connect(ani, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
    ani->start();
}

double LovelyHeartSwitch::getSwtchProg()
{
    return aniProgess;
}

void LovelyHeartSwitch::setSwtchProg(double p)
{
    this->aniProgess = p;
}
