#include "sapidswitchbase.h"

SapidSwitchBase::SapidSwitchBase(QWidget *parent) : QWidget(parent)
{
    calculateGeometry();
}

bool SapidSwitchBase::getState() const
{
    return currentState;
}

bool SapidSwitchBase::isChecked() const
{
    return currentState;
}

void SapidSwitchBase::setState(bool state)
{
    setStateWithoutSignal(state);
    emit stateChanged(currentState);
}

void SapidSwitchBase::setStateWithoutSignal(bool state)
{
    this->currentState = state;
    startSwitchAnimation();
}

void SapidSwitchBase::switchState()
{
    switchStateWithoutSignal();
    emit stateChanged(currentState);
}

void SapidSwitchBase::switchStateWithoutSignal()
{
    setState(!currentState);
}

void SapidSwitchBase::setForeground(QColor color)
{
    this->colorFg = color;
}

void SapidSwitchBase::setBackground(QColor on, QColor off)
{
    this->colorOn = on;
    this->colorOff = off;
    update();
}

void SapidSwitchBase::setBorder(QColor color, int size)
{
    this->colorBd = color;
    this->borderSize = size;
}

void SapidSwitchBase::setAnimationDuration(int dur)
{
    this->switchDuration = dur;
}

void SapidSwitchBase::setAnimationEasingCurve(QEasingCurve curve)
{
    this->curve = curve;
}

void SapidSwitchBase::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    calculateGeometry();
}

void SapidSwitchBase::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressPos = event->pos();
        moved = false;
        dragging = false;
        prevX = pressPos.x();

        QPropertyAnimation* ani = new QPropertyAnimation(this, "press");
        ani->setStartValue(pressScaleProgress);
        ani->setEndValue(pressScale);
        ani->setDuration(100);
        ani->setEasingCurve(QEasingCurve::InOutCubic);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        connect(ani, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
        ani->start();
    }
    QWidget::mousePressEvent(event);
}

void SapidSwitchBase::mouseMoveEvent(QMouseEvent *event)
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
                setSwtchProg(0);
            else if (x >= slideRight)
                setSwtchProg(1);
            else
                setSwtchProg(static_cast<double>(x - slideLeft) / (slideRight - slideLeft));
            update();

            moveTargetState = (x > prevX ? true : false);
            prevX = x;
        }
        event->accept();
    }
    QWidget::mouseMoveEvent(event);
}

void SapidSwitchBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!moved)
        {
            QPainterPath path = getBgPath();
            if (path.contains(event->pos())) // 外面允许直接穿透过去（默认穿透吧）
            {
                switchState();
            }
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
        event->accept();

        startSwitchAnimation();

        QPropertyAnimation* ani = new QPropertyAnimation(this, "press");
        ani->setStartValue(pressScaleProgress);
        ani->setEndValue(1);
        ani->setDuration(100);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        connect(ani, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
        ani->start();
    }
    QWidget::mouseReleaseEvent(event);
}

/**
 * 计算各个点的恰当坐标，以尽量填充整个矩形
 */
void SapidSwitchBase::calculateGeometry()
{
    // 计算手势拖拽的位置
    slideLeft = 0;
    slideRight = width();
}

/**
 * 获取整个背景的路径（支持不规则图形）
 */
QPainterPath SapidSwitchBase::getBgPath() const
{
    QPainterPath path;
    path.addRect(rect());
    return path;
}

/**
 * 根据动画进度返回对应颜色
 */
QColor SapidSwitchBase::getBgColor() const
{
    return QColor(
                static_cast<int>(colorOff.red() + (colorOn.red() - colorOff.red())*aniProgess),
                static_cast<int>(colorOff.green() + (colorOn.green() - colorOff.green())*aniProgess),
                static_cast<int>(colorOff.blue() + (colorOn.blue() - colorOff.blue())*aniProgess),
                static_cast<int>(colorOff.alpha() + (colorOn.alpha() - colorOff.alpha())*aniProgess)
            );
}

void SapidSwitchBase::startSwitchAnimation()
{
    const double target = currentState ? 1 : 0;
    if (qAbs(target-aniProgess) < 1e-4)
        return ;
    int duration = static_cast<int>(qAbs((aniProgess - target) * (slideRight - slideLeft) * 10));
    duration = qMin(duration, switchDuration);

    startSwitchAnimation(target, duration);
}

/**
 * 开启切换动画
 * @param target   开关进度，0.0 ~ 1.0
 * @param duration 动画时长。如果距离短的话不宜太长
 */
void SapidSwitchBase::startSwitchAnimation(double target, int duration)
{
    QPropertyAnimation* ani = new QPropertyAnimation(this, "swtch");
    ani->setStartValue(aniProgess);
    ani->setEndValue(target);
    if (duration > 50) // 太小的话就不计算非线性动画了
        ani->setEasingCurve(curve);
    ani->setDuration(duration);
    connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    connect(ani, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
    ani->start();
}

void SapidSwitchBase::setSwtchProgManual(double p)
{
    setSwtchProg(p);
}

double SapidSwitchBase::getSwtchProg()
{
    return aniProgess;
}

void SapidSwitchBase::setSwtchProg(double p)
{
    this->aniProgess = p;
}

double SapidSwitchBase::getPressProg()
{
    return pressScaleProgress;
}

void SapidSwitchBase::setPressProg(double p)
{
    this->pressScaleProgress = p;
}
