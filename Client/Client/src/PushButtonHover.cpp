#include "PushButtonHover.h"

PushButtonHover::PushButtonHover(QWidget *parent)
    : QPushButton(parent) {
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

void PushButtonHover::enterEvent(QEvent *)  {
    setCursor(Qt::PointingHandCursor);
}

void PushButtonHover::leaveEvent(QEvent *) {
    setCursor(Qt::ArrowCursor);
}

bool PushButtonHover::event(QEvent *e) {
    switch(e->type()) {
        case QEvent::HoverEnter:
            enterEvent(e);
        break;
        case QEvent::HoverLeave:
            leaveEvent(e);
        break;
        default:
        break;
    }
    return QWidget::event(e);
}
