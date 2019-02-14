#ifndef ICONSTYLE_H
#define ICONSTYLE_H

#include <QtCore>
#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif
#include <QObject>

class IconStyle : public QObject
{
    Q_OBJECT
public:
    static IconStyle *Instance();
    explicit IconStyle(QObject *parent = nullptr);

    virtual void setIcon(QLabel *label, QChar c, int size = 9);
    virtual void setIcon(QAbstractButton *btn, QChar c, int size = 9);

    class AutoRelease
    {
    public:
        ~AutoRelease()
        {
            if(IconStyle::self != nullptr)
            {
                delete IconStyle::self;
                IconStyle::self = nullptr;
            }
        }
    };
private:
    QFont iconFont;
    static IconStyle *self;    //对象自身
    static AutoRelease m_auto;
};

#endif // ICONSTYLE_H
