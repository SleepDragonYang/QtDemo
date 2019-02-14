#include "iconstyle.h"

IconStyle *IconStyle::self = nullptr;
IconStyle::AutoRelease IconStyle::m_auto;

IconStyle *IconStyle::Instance()
{
    if (!self) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new IconStyle;
        }
    }
    return self;
}

IconStyle::IconStyle(QObject *parent) : QObject(parent)
{
    int fontId = QFontDatabase::addApplicationFont(":/images/fontawesome-webfont.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (fontFamilies.size() > 0) {
        iconFont = QFont(fontFamilies.at(0));
    } else {
        qDebug() << "Load fontawesome-webfont.ttf error";
    }
}

void IconStyle::setIcon(QLabel *label, QChar c, int size)
{
    iconFont.setPixelSize(size);
    label->setFont(iconFont);
    label->setText(c);
}

void IconStyle::setIcon(QAbstractButton *btn, QChar c, int size)
{
    iconFont.setPixelSize(size);
    btn->setFont(iconFont);
    btn->setText(c);
}
