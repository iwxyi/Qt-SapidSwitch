#ifndef BOUNDARYSWITCHBASE_H
#define BOUNDARYSWITCHBASE_H

#include "sapidswitchbase.h"

class BoundarySwitchBase : public SapidSwitchBase
{
public:
    BoundarySwitchBase(QWidget* parent = nullptr);
    void setType(int mode);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual QSize sizeHint() const override;

    virtual void calculateGeometry() override;
    virtual QPainterPath getBgPath() const override;
    virtual void drawBorder(QPainter &painter);
    virtual void drawFg(QPainter &painter);
    virtual void drawText(QPainter &painter);

private:
    int mode = 0;
    int radius = 0; // 圆角矩形的半径 = height()/2
};

#endif // BOUNDARYSWITCHBASE_H
