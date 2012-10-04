#include "commandparser.h"

CommandParser::CommandParser(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowManager = mainWindow->getWindowManager();
    toolbarManager = mainWindow->getToolbarManager();
    commandLine = mainWindow->getCommandLine();
    gameDataContainer = GameDataContainer::Instance();

    pushStream = false;
    mono = false;
    bold = false;
    initRoundtime = false;
}

void CommandParser::processGameData(QByteArray data) {
    QDomDocument doc("GameData");
    QByteArray rawData(data);

    data.prepend("<root>");
    data.append("</root>");

    if(!doc.setContent(data)) {
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();

    gameText = "";
    while(!n.isNull()) {
        /* Process game text */
        if(!filterPlainText(root, n)) {
            break;
        }
        /* Process game data tags */
        filterDataTags(root, n);

        n = n.nextSibling();
    }

    /* Write game text */
    writeGameText(rawData);
}

bool CommandParser::filterPlainText(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    if(!mono && !pushStream) {
        /* Process game text with start tag only */
        if(e.tagName() == "style" && e.attribute("id") == "roomName") {
            gameText += "<span id=\"roomName\">" + root.text() + "</span>";
            return false;
        /* All plain text without tags */
        } else if(n.isText()) {
            if(n.toText().data().startsWith("  You also see")) {
                gameText += "<span id=\"youAlsoSee\">" + n.toText().data() + "</span>";
                return false;
            } else if (n.toText().data().startsWith("Also here: ")) {
                gameText += "<span id=\"alsoHere\">" + n.toText().data() + "</span>";
                return false;
            } else {
                /* Compensates for QDomText discarding HTML entities  */
                QString textData = n.toText().data();
                textData.replace("<", "&lt;");

                if(bold) {
                    gameText += "<span id=\"bold\">" + textData + "</span>";
                } else {
                    gameText += textData;
                }
            }
        /* Process game text between tags */
        } else if(e.tagName() == "d") {
            gameText += e.text();
        } else if(e.tagName() == "preset" && e.attribute("id") == "roomDesc") {
            gameText += e.text();
        } else if(e.tagName() == "preset" && e.attribute("id") == "speech") {
            gameText += "<span id=\"speech\">" + e.text() + "</span>";
        } else if(e.tagName() == "preset" && e.attribute("id") == "thought") {
            gameText += "<span id=\"thought\">" + e.text() + "</span>";
        }
    }

    return true;
}

void CommandParser::filterDataTags(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    if(!e.isNull()) {
        if(e.tagName() == "prompt") {
            /* filter prompt */
            if(initRoundtime) {
                time.setTime_t(e.attribute("time").toInt());
                commandLine->getRoundtimeDisplay()->setTimer(time.secsTo(roundTime));
                initRoundtime = false;
            }
            windowManager->writePromptGameWindow(root.text().trimmed().toUtf8());
        } else if(e.tagName() == "compass") {
            /* filter compass */
            QList<QString> directions;
            QDomNode compassNode  = root.firstChildElement("compass").firstChild();
            while(!compassNode.isNull()) {
                directions << compassNode.toElement().attribute("value");
                compassNode = compassNode.nextSibling();
            }
            windowManager->updateNavigationDisplay(directions);
        } else if(e.tagName() == "roundTime") {
            roundTime.setTime_t(e.attribute("value").toInt() - 1);
            initRoundtime = true;
            //commandLine->getRoundtimeDisplay()->setTimer(time.secsTo(roundTime));
        } else if(e.tagName() == "dialogData" && e.attribute("id") == "minivitals") {
            /* filter vitals */
            QDomElement vitalsElement = root.firstChildElement("dialogData").firstChildElement("progressBar");
            toolbarManager->updateVitals(vitalsElement.attribute("id"), vitalsElement.attribute("value"));
        }  else if(e.tagName() == "indicator") {
            /* filter player status indicator */
            //<indicator id="IconKNEELING" visible="n"/><indicator id="IconPRONE" visible="n"/>
            toolbarManager->updateStatus(e.attribute("visible"), e.attribute("id"));
        } else if(e.tagName() == "left") {
            /* filter player wielding in left hand */
            //<right exist="162134941" noun="sharks">fuzzy sharks</right>
            toolbarManager->updateWieldLeft(e.text());
            /* persist wield data in memory */
            WieldModel* wield = gameDataContainer->getWield();
            wield->setLeft(e.text());
            wield->setLeftNoun(e.attribute("noun"));
        } else if(e.tagName() == "right") {
            /* filter player wielding in right hand */
            toolbarManager->updateWieldRight(e.text());
            /* persist wield data in memory */
            WieldModel* wield = gameDataContainer->getWield();
            wield->setRight(e.text());
            wield->setRightNoun(e.attribute("noun"));
        } else if(e.tagName() == "spell") {
            //qDebug() << "############### SPELL ###############";
        } else if(e.tagName() == "streamWindow" && e.attribute("id") == "main") {
            /* filter main window title */
            QString title = e.attribute("subtitle");
            mainWindow->setMainTitle(title);

            RoomModel* room = gameDataContainer->getRoom();
            room->setName(title.mid(3));
            windowManager->updateRoomWindowTitle(title);
        } else if(e.tagName() == "component") {
            if(e.attribute("id").startsWith("exp")) {
                QString text = e.text();
                if(!text.isEmpty()) {
                    gameDataContainer->setExpField(new ExpModel(text));
                } else {
                    QString id = e.attribute("id").mid(4);
                    gameDataContainer->removeExpField(id);
                }
                windowManager->updateExpWindow();
            } else if(e.attribute("id").startsWith("room")) {
                RoomModel* room = gameDataContainer->getRoom();

                QString id = e.attribute("id");
                if(id.endsWith("desc")) {
                    room->setDesc(e.text());
                } else if (id.endsWith("objs")) {
                    room->setObjs(e.text());
                } else if (id.endsWith("players")) {
                    room->setPlayers(e.text());
                } else if (id.endsWith("exits")) {
                    room->setExits(e.text());
                } else if (id.endsWith("extra")) {
                    room->setExtra(e.text());
                }
                windowManager->updateRoomWindow();
            }
        } else if(e.tagName() == "pushStream") {
            pushStream = true;
            if(e.attribute("id") == "assess") {
                pushStream = false;
            } else {
                pushStream = true;
            }

            if(e.attribute("id") == "logons") {
                windowManager->updateArrivalsWindow(root.text());
            } else if(e.attribute("id") == "thoughts") {
                windowManager->updateThoughtsWindow(root.text().trimmed() +
                    " [" + QTime::currentTime().toString() + "]\n");
            } else if(e.attribute("id") == "death") {
                windowManager->updateDeathsWindow(root.text());
            }
        } else if (e.tagName() == "popStream") {
            pushStream = false;
        } else if(e.tagName() == "output") {
            if(e.attribute("class") == "mono") {
                mono = true;
            } else {
                mono = false;
            }
        } else if (e.tagName() == "pushBold") {
            bold = true;
        } else if (e.tagName() == "popBold") {
            bold = false;
        }
    }
}

void CommandParser::writeGameText(QByteArray rawData) {
    if (rawData.size() == 1) {
        windowManager->writeGameWindow("");
    } else if (mono && !pushStream) {
        /* mono handled separately otherwise white spaces
        before tags are not recognized as text */
        QString line = rawData;

        if(bold) {
            line.prepend("<span style=\"white-space:pre;\" id=\"bold\">");
        } else {
            line.prepend("<span style=\"white-space:pre;\" id=\"body\">");
        }
        line.append("</span>");

        if(!rawData.startsWith("<output class=\"mono\"/>")) {
            windowManager->writeGameWindow(line.toLocal8Bit());
        }

    } else if(gameText != "") {
        gameText.prepend("<span style=\"white-space:pre;\" id=\"body\">");
        gameText.append("</span>");

        windowManager->writeGameWindow(gameText.toLocal8Bit());
    }
}

void CommandParser::processMock() {
    QFile file("C:/Projects/FrostBite/data/mock.xml");

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug("Unable to open mock data file!");
        return;
    }
    QByteArray data = file.readAll();

    process(data);

    file.close();
}

void CommandParser::process(QByteArray data) {
    QList<QByteArray> lines = data.split('\n');

    for(int i = 0; i < lines.size(); i++ ) {
        //qDebug() << lines.at(i);
        processGameData(lines.at(i));
    }
}
