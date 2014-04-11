#include "snapshot.h"

Snapshot::Snapshot(WindowInterface* window) : QObject() {
    this->window = window;
}

void Snapshot::save() {
    window->getMainWindow()->appendHtml("<span class=\"echo\">Saving snapshot..</span>");

    QString path = QApplication::applicationDirPath() + "/snapshots/" +
            window->getObjectName() + "_" +
            QDate::currentDate().toString("yyyy-MM-dd") + "_" +
            QTime::currentTime().toString("hh-mm-ss") + ".html";

    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    QString html = "";

    QTextCursor cursor = window->getMainWindow()->textCursor();
    if(cursor.hasSelection()) {
        QTextDocument* doc = window->getDocument()->clone();
        doc->setHtml(cursor.selection().toHtml());

        html = doc->toHtml();
    } else {
        html = window->getDocument()->toHtml();
    }

    // set style for background and font colors
    QString bodyStyle = "<body style=\"";
    out << html.insert(html.indexOf(bodyStyle) + bodyStyle.size(),
                "background: " + window->getBgColor().name() + ";" +
                "color: " + window->getTextColor().name() + ";");

    file.close();

    window->getMainWindow()->appendHtml("<span class=\"echo\">Output saved to - " +
                     path + "</span><br/>");
}
