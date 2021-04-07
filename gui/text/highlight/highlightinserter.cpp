#include "highlightinserter.h"

#include "mainwindow.h"
#include "text/highlight/highlightsettings.h"
#include "text/highlight/highlightsettingsentry.h"

HighlightInserter::HighlightInserter(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);

    connect(this, SIGNAL(reloadTextHighlights()), mainWindow, SLOT(reloadTextHighlights()));
}

void HighlightInserter::regex(QString pattern, QString color) {
    HighlightSettings* settings = HighlightSettings::getInstance();

    QBitArray opts = QBitArray(5);
    opts.setBit(1, 1);

    HighlightSettingsEntry entry = HighlightSettingsEntry(0, pattern, "General",
        QColor(color), false, "", false, 0, "", opts);

    settings->addParameter("TextHighlight", entry);
}

void HighlightInserter::commit() {
    emit reloadTextHighlights();
}
