#ifndef SHARED_H
#define SHARED_H

#include <QtCore>
#include <QDebug>
#include <QMap>

#include "shared_global.h"
#include "defines.h"

#ifdef Q_OS_WIN
#define EXPORT_FUNCTION __declspec(dllexport)
#else
#define EXPORT_FUNCTION
#endif

class SHAREDSHARED_EXPORT Shared {
public:
    Shared();
};

#endif // SHARED_H
