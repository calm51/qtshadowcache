#pragma execution_character_set("utf-8")

#ifndef QTSHADOWCACHE_GLOBAL_H
#define QTSHADOWCACHE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTSHADOWCACHE_LIBRARY)
#define QTSHADOWCACHE_EXPORT Q_DECL_EXPORT
#else
#define QTSHADOWCACHE_EXPORT Q_DECL_IMPORT
#endif

#endif // QTSHADOWCACHE_GLOBAL_H
