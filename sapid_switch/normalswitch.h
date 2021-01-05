#ifndef NORMALSWITCH_H
#define NORMALSWITCH_H

#include "boundaryswitchbase.h"

class NormalSwitch : public BoundarySwitchBase
{
public:
    NormalSwitch(QWidget* parent = nullptr);

protected:
    virtual void calculateGeometry() override;
    virtual void paintEvent(QPaintEvent*) override;
    virtual QPainterPath getBgPath() const override;

private:
    QRectF bgRect;
};

#endif // NORMALSWITCH_H
