#include "genieutils.h"

GenieUtils::GenieUtils(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);

    connect(this, SIGNAL(reloadSettings()), mainWindow, SLOT(reloadSettings()));
}

void GenieUtils::importHighlights(QString imports) {
    QStringList entries = imports.split("#highlight");
    for(QString entry : entries) {
        QStringList values = entry.split("} {");
        if(values.size() > 3) {
            this->addRegexHighlight(TextUtils::stripBraces(values.at(2)), TextUtils::stripBraces(values.at(1)));
        }
    }
    this->commitSettings();
}

void GenieUtils::addRegexHighlight(QString pattern, QString color) {
    HighlightSettings* settings = HighlightSettings::getInstance();

    QBitArray opts = QBitArray(5);
    opts.setBit(1, 1);

    HighlightSettingsEntry entry = HighlightSettingsEntry(0, pattern, "General",
        QColor(color), QColor(), false, "", false, 0, "", opts);

    settings->addParameter("TextHighlight", entry);
}

void GenieUtils::commitSettings() {
    emit reloadSettings();
}
