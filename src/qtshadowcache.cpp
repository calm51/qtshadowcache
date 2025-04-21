#include "qtshadowcache.h"


#include <QApplication>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>


namespace QSDC {

Result Qtshadowcache::create(QWidget *parent, quint8 window_radius, quint8 blur_radius, const QColor &shadow_color, const QColor &background_color) {
    Result result;

    // qDebug() << qApp;

    quint16 _width = 256;

    // _width *= 2;
    // window_radius *= 2;
    // blur_radius *= 2;

    // quint16 _margin = blur_radius;
    quint16 _margin = qMax(2, blur_radius - 2);
    if (blur_radius > 6) {
        _margin = qRound(static_cast<double>(blur_radius) * 0.7);
    }

    QWidget *background = new QWidget(parent);
    background->setAttribute(Qt::WA_TranslucentBackground);
    background->resize(_width, _width);
    background->setFixedSize(_width, _width);
    QHBoxLayout *layout = new QHBoxLayout(background);
    layout->setContentsMargins(_margin, _margin, _margin, _margin);
    QLabel *child = new QLabel(background);
    layout->addWidget(child);
    background->setLayout(layout);

    {
        // QPalette palette = background->palette();
        // palette.setColor(QPalette::Window, Qt::green);
        // background->setAutoFillBackground(true); // 调色板,必须启用这个才能看到背景色变化
        // background->setPalette(palette);
    }
    {
        // QPalette palette = child->palette();
        // palette.setColor(QPalette::Window, background_color);
        // child->setAutoFillBackground(true);
        // child->setPalette(palette);
        child->setStyleSheet(QString("background-color: rgba(%1,%2,%3,%4);"
                                     "border: 0px solid black;"
                                     "border-radius: %5")
                                 .arg(QString::number(background_color.red()),
                                      QString::number(background_color.green()),
                                      QString::number(background_color.blue()),
                                      QString::number(background_color.alpha()),
                                      QString::number(window_radius)));
    }


    QGraphicsDropShadowEffect *sd = new QGraphicsDropShadowEffect{child};
    sd->setColor(shadow_color);
    sd->setOffset(QPointF(0, 0));
    sd->setBlurRadius(blur_radius);
    child->setGraphicsEffect(sd);

    background->setVisible(false);

    QPixmap pixmap(background->size());
    pixmap.fill(Qt::transparent);
    background->render(&pixmap);

    background->deleteLater();

    result.pixmap = pixmap;
    result.width = _width;
    result.margin = _margin;
    result.blur_radius = blur_radius;
    result.radius = window_radius;

    return result;
}

} // namespace QSDC
