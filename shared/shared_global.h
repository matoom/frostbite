#ifndef SHARED_GLOBAL_H
#define SHARED_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATA_LIBRARY)
#  define SHAREDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHAREDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHARED_GLOBAL_H
