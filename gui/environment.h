#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#ifdef Q_OS_WIN
#define MOCK_DATA_PATH "C:/Projects/FrostBite/support/mock.xml"
#else
#define MOCK_DATA_PATH "~/projects/FrostBite/support/mock.xml"
#endif

#endif // ENVIRONMENT_H
