

#include "KeyboardResponsiveTextEdit.h"

KeyboardResponsiveTextEdit::KeyboardResponsiveTextEdit(QWidget *parent)
    : QPlainTextEdit{parent} {

}

void KeyboardResponsiveTextEdit::keyPressEvent(QKeyEvent *event) {
    QPlainTextEdit::keyPressEvent(event);

    if (event->key() == Qt::Key_Return) {
        auto textWithoutNewLine{this->toPlainText()};
        textWithoutNewLine.chop(1);

        this->setPlainText(textWithoutNewLine);
        emit enterPressed();
    }
}
