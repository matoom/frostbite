#ifndef HYPERLINKUTILS_H
#define HYPERLINKUTILS_H

#include <QString>
#include <QUrl>

namespace HyperlinkUtils {

void addLink(QString& text, const QString& pattern, const QString& command);
int createLink(QString& text, const QString& command, int indexStart, QString match);
QUrl createSearchElanthipediaUrl(const QString& text);

}

#endif // HYPERLINKUTILS_H
