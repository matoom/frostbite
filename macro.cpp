#include "macro.h"

Macro::Macro(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
}

bool Macro::execute(QString command) {
    if(!command.isEmpty()) {
        qDebug() << command;

        QString cmd = "put kurk in my back$nput my kurk in my cloak$sput my kala in my kala$spoint kurk$n";

        qDebug() << cmd.split(QRegExp("\\$n|\\$s"), QString::SkipEmptyParts);

        QByteArray x = cmd.toLocal8Bit();
        QStringList commands;
        for(int i = 0; i < x.length(); i++){
            if(x.at(i) == '$') {
                if(!(i >= x.length())) {
                    commands << QString(x.at(i + 1));
                }
            }
        }

        qDebug() << commands;

        return true;
    }
    return false;
}
