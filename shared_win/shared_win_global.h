#ifndef SHARED_WIN_GLOBAL_H
#define SHARED_WIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SHARED_WIN_LIBRARY)
#  define DATASHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SARED_WIN_GLOBAL_H
