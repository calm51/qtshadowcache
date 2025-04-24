#pragma execution_character_set("utf-8")

#include "qtshadowcache.h"


#include <QApplication>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QTemporaryDir>

#if defined Q_OS_ANDROID
#include "QtAndroid"
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroidExtras>
#endif


namespace QSDC {

QWidget *background = nullptr;
QHBoxLayout *layout = nullptr;
QWidget *child = nullptr;
QGraphicsDropShadowEffect *sd = nullptr;
QTemporaryDir *tempfolder = nullptr;

QList<Result> caches;


QString qcolor2qss(const QColor &c) {
    int r, g, b, a;
    c.getRgb(&r, &g, &b, &a);
    return QString(R"(rgba(%1,%2,%3,%4))").arg(r).arg(g).arg(b).arg(a);
}

QString colorToArgbString(const QColor &color) {
    return QString("#%1%2%3%4").arg(color.alpha(), 2, 16, QChar('0')).arg(color.red(), 2, 16, QChar('0')).arg(color.green(), 2, 16, QChar('0')).arg(color.blue(), 2, 16, QChar('0')).toLower();
}

Result Qtshadowcache::create(const Request &request) {
    if (!tempfolder) {
        tempfolder = new QTemporaryDir(QDir(
#ifdef Q_OS_ANDROID
                                           QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
#else
                                           QDir::tempPath()
#endif
                                               )
                                           .absoluteFilePath("qsdc-XXXXXX"));
    }

    foreach (auto i, caches) {
        if (i.request == request) {
            i.cached = true;
            return i;
        }
    };
    while (caches.length() > 32) {
        caches.removeFirst();
    }

    Result result;
    result.request = request;

    // qDebug() << qApp;

    quint16 _width = 128;

    // _width *= 2;
    // window_radius *= 2;
    // blur_radius *= 2;

    // quint16 _margin = request.blur_radius;
    quint16 _margin = qMax(2, request.blur_radius - 2);
    if (request.blur_radius > 6) {
        _margin = qRound(static_cast<double>(request.blur_radius) * 0.7);
    } else if (request.blur_radius == 0) {
        _margin = 0;
    }

    if (!background) {
        background = new QWidget();
    }
    background->setParent(request.parent);
    background->setAttribute(Qt::WA_TranslucentBackground);
    background->resize(_width, _width);
    background->setFixedSize(_width, _width);
    if (!layout) {
        layout = new QHBoxLayout(background);
    }
    layout->setContentsMargins(_margin, _margin, _margin, _margin);
    if (!child) {
        child = new QLabel(background);
    }
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
        child->setStyleSheet(QString("background-color: %1;"
                                     "border: 0px solid black;"
                                     "border-top-left-radius: %2;"
                                     "border-top-right-radius: %3;"
                                     "border-bottom-left-radius: %4;"
                                     "border-bottom-right-radius: %5;")
                                 .arg(qcolor2qss(request.background_color),
                                      QString::number(request.window_radius_left_top),
                                      QString::number(request.window_radius_right_top),
                                      QString::number(request.window_radius_left_bottom),
                                      QString::number(request.window_radius_right_bottom)));
    }


    if (!sd) {
        sd = new QGraphicsDropShadowEffect{child};
    }
    sd->setColor(request.shadow_color);
    sd->setOffset(QPointF(0, 0));
    sd->setBlurRadius(request.blur_radius);
    child->setGraphicsEffect(sd);

    background->setVisible(false);

    QPixmap pixmap(background->size());
    pixmap.fill(Qt::transparent);
    background->render(&pixmap);

    // background->deleteLater();

    result.pixmap = pixmap;
    result.width = _width;
    result.margin = _margin;

    result.name = QString("qsdc-%1x%1-%2_%3-%4_%5_%6_%7-%8-%9.png")
                      .arg(QString::number(result.width),
                           QString::number(request.blur_radius),
                           QString::number(result.margin),
                           QString::number(request.window_radius_left_top),
                           QString::number(request.window_radius_right_top),
                           QString::number(request.window_radius_left_bottom),
                           QString::number(request.window_radius_right_bottom),
                           colorToArgbString(request.shadow_color).mid(1),
                           colorToArgbString(request.background_color).mid(1));

    result.save2 = tempfolder->filePath(result.name);

    caches.append(result);

    return result;
}

void Qtshadowcache::clear() {
    caches.clear();
    if (tempfolder) {
        delete tempfolder;
        tempfolder = nullptr;
    }
}


bool operator==(const Request &lhs, const Request &rhs) {
    return std::tie(lhs.parent,
                    lhs.window_radius_left_top,
                    lhs.window_radius_left_bottom,
                    lhs.window_radius_right_top,
                    lhs.window_radius_right_bottom,
                    lhs.blur_radius,
                    lhs.shadow_color,
                    lhs.background_color)
           == std::tie(rhs.parent,
                       rhs.window_radius_left_top,
                       rhs.window_radius_left_bottom,
                       rhs.window_radius_right_top,
                       rhs.window_radius_right_bottom,
                       rhs.blur_radius,
                       rhs.shadow_color,
                       rhs.background_color);
}

bool operator!=(const Request &lhs, const Request &rhs) {
    return !(lhs == rhs);
}

} // namespace QSDC
