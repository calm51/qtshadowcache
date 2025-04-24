#pragma execution_character_set("utf-8")

#ifndef QTSHADOWCACHE_H
#define QTSHADOWCACHE_H

#include "qtshadowcache_global.h"

#include <QPixmap>
#include <QString>
#include <QWidget>

#include <tuple>

namespace QSDC {


struct QTSHADOWCACHE_EXPORT Request {
    QWidget *parent = nullptr;
    quint8 window_radius_left_top = 0;
    quint8 window_radius_left_bottom = 0;
    quint8 window_radius_right_top = 0;
    quint8 window_radius_right_bottom = 0;
    quint8 blur_radius = 13;
    QColor shadow_color = Qt::black;
    QColor background_color = Qt::white;

    friend bool operator==(const Request &lhs, const Request &rhs);
    friend bool operator!=(const Request &lhs, const Request &rhs);

    // bool operator==(const Request& other) const {
    //     return this->parent == other.parent;
    // }
};

struct QTSHADOWCACHE_EXPORT Result {
    Request request;
    bool cached = false;
    QPixmap pixmap; // 阴影图
    QString name;
    QString save2;
    quint16 width;
    quint16 margin; // 阴影范围
};

class QTSHADOWCACHE_EXPORT Qtshadowcache {
  public:
    Qtshadowcache() = delete;

    static Result create(const Request &request);
    static void clear();
};

} // namespace QSDC

#endif // QTSHADOWCACHE_H
