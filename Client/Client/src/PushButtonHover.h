#ifndef PUSHBUTTONHOVER_H
#define PUSHBUTTONHOVER_H



#include <QPushButton>
#include <QtWidgets>
#include <QEvent>

class PushButtonHover final : public QPushButton {
    Q_OBJECT
public:
    explicit                PushButtonHover(QWidget *parent = nullptr);

protected:
    virtual void            enterEvent(QEvent *e);
    virtual void            leaveEvent(QEvent *e) override;
    virtual bool            event(QEvent *e) override;
};

#endif // PUSHBUTTONHOVER_H
