#ifndef QTSHADOWCACHE_H
#define QTSHADOWCACHE_H

#include "qtshadowcache_global.h"

#include <QPixmap>
#include <QWidget>

namespace QSDC {

struct Result {
    QPixmap pixmap; // 阴影图
    quint16 width;  // 图片宽高
    quint16 margin; // 阴影范围
    quint16 blur_radius;
    quint8 radius; // 圆角范围
};

class QTSHADOWCACHE_EXPORT Qtshadowcache {
  public:
    Qtshadowcache() = delete;

    static Result create(QWidget *parent = nullptr, quint8 window_radius = 6, quint8 blur_radius = 13, const QColor &shadow_color = Qt::black, const QColor &background_color = Qt::white);
};

} // namespace QSDC

#endif // QTSHADOWCACHE_H
