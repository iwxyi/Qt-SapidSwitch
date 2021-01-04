#include "boundaryswitchbase.h"

BoundarySwitchBase::BoundarySwitchBase(QWidget *parent) : SapidSwitchBase(parent)
{
    setMinimumSize(128, 32);

    setBackground(QColor(30, 144, 255), Qt::gray);
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
    radius = height() / 2;
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
            painter.restore();
        }
    }
    else if (mode == 2)
    {
        painter.save();
        QRect rect = this->rect();
        painter.setPen(colorOff);
        QPainterPath path;
        path.addRoundedRect(rect, radius, radius);
        painter.drawPath(path);

        // 画动画背景
        const double totalLen = 2 * PI * radius + 2 * (rect.width() - 2 * radius);
        double currLen = totalLen * prop; // 应当动画的时长
        painter.setPen(colorOn);
        path.clear();
        path.moveTo(0, radius);
        if (currLen > 0) // 左下角四分之一个圆
        {
            QPoint o(rect.left() + radius, radius);
            double len = radius * PI / 2; // 90°周长
            double angle;
            if (len <= currLen) // 够长
            {
                angle = 90;
                currLen -= len;
            }
            else // len > currLen，不够这一个圆的
            {
                angle = 90 * currLen / len;
                currLen = 0;
            }
            path.arcTo(QRect(0, 0, radius*2, radius*2), 180, angle);
        }
        if (currLen > 0) // 下面直线
        {
            double len = rect.width() - 2 * radius;
            double moveLen;
            if (currLen > len) // 够长
            {
                moveLen = len;
                currLen -= moveLen;
            }
            else
            {
                moveLen = currLen;
                currLen = 0;
            }
            path.lineTo(radius + moveLen, radius*2);
        }
        if (currLen > 0) // 右边半圆
        {
            QPoint o(rect.right() - radius, radius);
            double len = radius * PI; // 半圆周长
            double angle;
            if (len <= currLen) // 够长
            {
                angle = 180;
                currLen -= len;
            }
            else // len > currLen，不够这一个圆的
            {
                angle = 180 * currLen / len;
                currLen = 0;
            }
            path.arcTo(QRect(width()-2*radius, 0, radius*2, radius*2), 270, angle);
        }
        if (currLen > 0) // 上面直线
        {
            double len = rect.width() - 2 * radius;
            double moveLen;
            if (currLen > len) // 够长
            {
                moveLen = len;
                currLen -= moveLen;
            }
            else
            {
                moveLen = currLen;
                currLen = 0;
            }
            path.lineTo(width()-radius-moveLen, 0);
        }
        if (currLen > 0) // 左上角圆
        {
            QPoint o(rect.left() + radius, radius);
            double len = radius * PI / 2; // 90°周长
            double angle;
            if (len <= currLen) // 够长
            {
                angle = 90;
                currLen -= len;
            }
            else // len > currLen，不够这一个圆的
            {
                angle = 90 * currLen / len;
                currLen = 0;
            }
            path.arcTo(QRect(0, 0, radius*2, radius*2), 90, angle);
        }
        painter.setPen(colorOn);
        painter.drawPath(path);
        painter.restore();

        /*painter.save();
        // 右半部分整体rect
        QPainterPath path;
        QRect rect = this->rect();
        path.addRoundedRect(rect, radius, radius);
        double startAngle = 180;
        double arcAngle = 360 * prop / 0.75;

        // 去掉的部分
        QPainterPath clipPath;
        const QPoint center = rect.center();
        const int r = rect.width()/2;
        clipPath.moveTo(center);
        clipPath.lineTo(this->rect().center());
        QRect circleRect(center.x()-r-1, center.y()-r, r*2+2, r*2);
        clipPath.arcTo(circleRect, startAngle, arcAngle);
        clipPath.lineTo(center);

        painter.setPen(colorOff);
        painter.drawPath(path);
        painter.setClipPath(clipPath);
        painter.setPen(colorOn);
        painter.drawPath(path);
        painter.restore();*/
    }
    else if (mode == 3)
    {

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
