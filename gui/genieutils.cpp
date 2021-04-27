#include "genieutils.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QColor>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "mainwindow.h"
#include "text/highlight/highlightsettingsentry.h"
#include "text/highlight/highlightsettings.h"
#include "textutils.h"

#include <QtDebug>

GenieUtils::GenieUtils(QObject *parent) : QObject(parent) {
}

void GenieUtils::importHighlights(QString imports) {
    QStringList entries = imports.split("#highlight");
    for(QString entry : entries) {
        // restore prefix
        entry.prepend("highlight ");
        auto hl = parseHighlight(entry);
        // parse at least until regexp
        if (!hl.regexp.isEmpty()) {
            this->addRegexHighlight(hl);
        }
    }
    this->commitSettings();
}

bool GenieUtils::importFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Unable to find file");
        return false;
    }
    QTextStream in(&file);
    for (QString line = in.readLine(); !line.isEmpty(); line = in.readLine()) {
        auto hl = parseHighlight(line);
        // parse at least until regexp
        if (!hl.regexp.isEmpty()) {
            this->addRegexHighlight(hl);
        }
    }
    return true;
}

void GenieUtils::addRegexHighlight(const GenieUtils::GenieHighlight& highlight) {
    HighlightSettings* settings = HighlightSettings::getInstance();

    QBitArray opts(5);
    opts.setBit(1, 1);
    if (highlight.type == "beginswith") {
        opts.setBit(0, 1);
        opts.setBit(2, 1);
    }

    HighlightSettingsEntry entry
        = HighlightSettingsEntry(0,
                                 highlight.regexp,
                                 highlight.category.isEmpty() ? "General" : highlight.category,
                                 highlight.color,
                                 QColor(),
                                 !highlight.sound.isEmpty(),
                                 highlight.sound,
                                 false, 0, "", opts);

    settings->addParameter("TextHighlight", entry);
}

void GenieUtils::commitSettings() {
    emit reloadSettings();
}

GenieUtils::GenieHighlight GenieUtils::parseHighlight(const QString &line) {
    GenieHighlight highlight;
    if (line.startsWith("#highlight")) {
        static const QRegularExpression re("{(.*?)}");
        auto matchIterator = re.globalMatch(line);
        if (matchIterator.hasNext()) {
            int pos = 0;
            while (matchIterator.hasNext()) {
                highlight.fieldFromPosition(pos, matchIterator.next().captured(1));
                ++pos;
            }
        }
    }
    return highlight;
}
