#include "hyperlinkutils.h"

#include <QRegExp>
#include <QRegularExpression>

#include "globaldefines.h"

namespace {

QString createCommand(QString text, QString command) {
    command.replace(QRegExp("\\$1"), text);
    return FROSTBITE_SCHEMA + QString("://a/") + command.toLocal8Bit().toBase64();
}

}

namespace HyperlinkUtils
{
void addLink(QString &text, const QString &pattern, const QString &command) {
    QRegularExpression re(pattern + "(?=[^>]*(<|$))");
    QRegularExpressionMatchIterator matchIterator = re.globalMatch(text);
    int global = 0;
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        if(match.hasMatch()) {
            int count = re.captureCount();
            if(count == 1) {
                global = createLink(text, command, match.capturedStart(0) + global, match.captured(0));
            } else {
                int inserted = 0;
                for(int i = 1; i < count; i++) {
                    inserted += createLink(text, command, match.capturedStart(i) + inserted, match.captured(i));
                }
            }
        }
    }
}

int createLink(QString &text, const QString &command, int indexStart, QString match) {
    QString startTag = "<a href=\"" + createCommand(match, command) + "\">";
    QString endTag = "</a>";

    int startTagLength = startTag.length();
    int indexEnd = indexStart + startTagLength + match.length();

    text.insert(indexStart, startTag);
    text.insert(indexEnd, endTag);

    return startTagLength + endTag.length();
}


QUrl createSearchElanthipediaUrl(const QString& text) {
    return QUrl(QString("https://elanthipedia.play.net/index.php?search=") +
                QUrl::toPercentEncoding(text, " ") + QString("&title=Special%3ASearch&go=Go"));
}

}

