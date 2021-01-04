#include "boundaryswitchbase.h"

BoundarySwitchBase::BoundarySwitchBase(QWidget *parent) : SapidSwitchBase(parent)
{
    setMinimumSize(128, 32);

    setBorder(QColor(30, 144, 255), 1);
    setAnimationDuration(600);
    setAnimationEasingCurve(QEasingCurve::OutCubic);
}

void BoundarySwitchBase::setType(int mode)
{
    this->mode = mode;
}

void BoundarySwitchBase::paintEvent(QPaintEvent *event)
{
    SapidSwitchBase::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 画边界
    drawBorder(painter);

    // 画开关
    drawFg(painter);

    // 画文字
    drawText(painter);
}

QSize BoundarySwitchBase::sizeHint() const
{
    return QSize(128, 32);
}

void BoundarySwitchBase::calculateGeometry()
{
    radius = (height() - 1) / 2;
}

QPainterPath BoundarySwitchBase::getBgPath() const
{
    QPainterPath path;
    if (mode == 3)
    {
        path.addRect(rect());
    }
    else if (mode == 1 || mode == 2)
    {
        path.addRoundedRect(rect(), radius, radius);
    }
    return path;
}

void BoundarySwitchBase::drawBorder(QPainter &painter)
{
    const double prop = aniProgess;

    if (mode == 1)
    {
        // 画OFF
        if (prop <= 0.75)
        {
            painter.save();
            // 右半部分整体rect
            QPainterPath path;
            QRect rect = this->rect();
            rect.setRight(rect.width() / 2);
            path.addRoundedRect(rect, radius, radius);
            double startAngle = 0;
            double arcAngle = 360 * (0.75 - prop) / 0.75;

            // 去掉的部分
            QPainterPath clipPath;
            const QPoint center = rect.center();
            const int r = rect.width()/2;
            clipPath.moveTo(center);
            clipPath.lineTo(this->rect().center());
            QRect circleRect(center.x()-r-1, center.y()-r, r*2+2, r*2);
            clipPath.arcTo(circleRect, startAngle, arcAngle);
            clipPath.lineTo(center);

            painter.setClipPath(clipPath);
            painter.setPen(colorOff);
            painter.drawPath(path);
            painter.restore();
//            painter.drawPath(clipPath);
        }

        // 画ON
        if (prop >= 0.25)
        {
            painter.save();
            // 左半部分整体rect
            QPainterPath path;
            QRect rect = this->rect();
            rect.setLeft(width() / 2);
            path.addRoundedRect(rect, radius, radius);
            double startAngle = 180;
            double arcAngle = 360 * (prop-0.25) / 0.75;

            // 去掉的部分
            QPainterPath clipPath;
            const QPoint center = rect.center();
            const int r = rect.width()/2;
            clipPath.moveTo(center);
            clipPath.lineTo(this->rect().center());
            QRect circleRect(center.x()-r-1, center.y()-r, r*2+2, r*2);
            clipPath.arcTo(circleRect, startAngle, -arcAngle);
            clipPath.lineTo(center);

            painter.setClipPath(clipPath);
            painter.setPen(colorOn);
            painter.drawPath(path);
//            painter.drawPath(clipPath);
            painter.restore();
        }
    }
}

void BoundarySwitchBase::drawFg(QPainter &painter)
{

}

void BoundarySwitchBase::drawText(QPainter &painter)
{
    // 画OFF
    QRect rect = this->rect();
    rect.setRight(width() / 2);
    painter.setPen(colorOff);
    painter.drawText(rect, Qt::AlignCenter, "OFF");

    // 画ON
    rect = this->rect();
    rect.setLeft(width() / 2);
    painter.setPen(isChecked() ? colorOn : colorOff);
    painter.drawText(rect, Qt::AlignCenter, "ON");

}
