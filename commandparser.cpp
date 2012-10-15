#include "commandparser.h"

CommandParser::CommandParser(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    windowManager = mainWindow->getWindowManager();
    toolbarManager = mainWindow->getToolbarManager();    
    commandLine = mainWindow->getCommandLine();
    gameDataContainer = GameDataContainer::Instance();
    highlighter = new Highlighter(parent);

    pushStream = false;
    inv = false;
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

    /* process pushstream data over multiple lines */
    processPushStream(rawData);

    /* Write game text and raw data */
    writeGameText(rawData);
}

bool CommandParser::filterPlainText(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    if(!mono && !pushStream) {
        /* Process game text with start tag only */
        if(e.tagName() == "style" && e.attribute("id") == "roomName") {
            gameText += "<SPAN ID=\"_ROOM_NAME\">" + root.text() + "</SPAN>";
            return false;
        /* All plain text without tags */
        } else if(n.isText()) {
            // Compensates for QDomText discarding HTML entities
            QString textData = n.toText().data();
            textData.replace("<", "&lt;");

            if(bold) {
                gameText += "<SPAN ID=\"_BOLD\">" + textData + "</SPAN>";
            } else {
                gameText += textData;
            }
        /* Process game text between tags */
        } else if(e.tagName() == "d") {
            gameText += e.text();
        } else if(e.tagName() == "preset" && e.attribute("id") == "roomDesc") {
            gameText += e.text();
        } else if(e.tagName() == "preset" && e.attribute("id") == "speech") {
            gameText += "<SPAN ID=\"_SPEECH\">" + e.text() + "</SPAN>";
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
            /* write prompt */
            windowManager->writePromptGameWindow(root.text().trimmed().toUtf8());            
            mainWindow->getScriptService()->writeOutgoingMessage(root.text().trimmed().toUtf8());
        } else if(e.tagName() == "compass") {
            /* filter compass */
            QList<QString> directions;
            QDomNode compassNode  = root.firstChildElement("compass").firstChild();
            while(!compassNode.isNull()) {
                directions << compassNode.toElement().attribute("value");
                compassNode = compassNode.nextSibling();
            }
            windowManager->updateNavigationDisplay(directions);
        } else if (e.tagName() == "clearContainer") {
            QStringList container;
            QDomElement invElem = root.firstChildElement("inv");
            while(!invElem.isNull()) {
                container << invElem.text().trimmed();
                invElem = invElem.nextSiblingElement("inv");
            }
            gameDataContainer->setContainer(container);
        } else if(e.tagName() == "roundTime") {
            roundTime.setTime_t(e.attribute("value").toInt() - 1);
            initRoundtime = true;
            //commandLine->getRoundtimeDisplay()->setTimer(time.secsTo(roundTime));
        } else if(e.tagName() == "dialogData" && e.attribute("id") == "minivitals") {
            /* filter vitals */
            QDomElement vitalsElement = root.firstChildElement("dialogData").firstChildElement("progressBar");
            toolbarManager->updateVitals(vitalsElement.attribute("id"), vitalsElement.attribute("value"));
            highlighter->alert(vitalsElement.attribute("id"), vitalsElement.attribute("value").toInt());
        }  else if(e.tagName() == "indicator") {
            /* filter player status indicator */
            //<indicator id="IconKNEELING" visible="n"/><indicator id="IconPRONE" visible="n"/>
            toolbarManager->updateStatus(e.attribute("visible"), e.attribute("id"));
            if(e.attribute("visible") == "y") {
                highlighter->alert(e.attribute("id"));
            }
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
            toolbarManager->updateSpell(e.text());
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
                QString id = e.attribute("id").mid(4);
                if(!text.isEmpty()) {
                    if(e.firstChildElement("d").isNull()) {
                        gameDataContainer->setExpField(id, new ExpModel(false, text));
                    } else {
                        gameDataContainer->setExpField(id, new ExpModel(true, text));
                    }
                } else {                    
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
            }

            if(e.attribute("id") == "logons") {
                windowManager->updateArrivalsWindow(highlighter->highlight(root.text()));
            } else if(e.attribute("id") == "thoughts") {
                QString thought = root.text().trimmed();
                thought.insert(thought.indexOf("\""), "</SPAN>");
                thought.prepend("<SPAN ID=\"_THINKING\">");
                windowManager->updateThoughtsWindow(highlighter->highlight(thought +
                    " [" + QTime::currentTime().toString() + "]\n"));
            } else if(e.attribute("id") == "death") {
                windowManager->updateDeathsWindow(highlighter->highlight(root.text()));
            } else if(e.attribute("id") == "atmospherics") {
                gameText += root.text();
            } else if(e.attribute("id") == "inv") {
                inv = true;
            }
        } else if (e.tagName() == "popStream") {
            pushStream = false;
            if(inv) {
                gameDataContainer->setInventory(inventory);
                inventory.clear();
                inv = false;
            }
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

void CommandParser::processPushStream(QByteArray rawData) {
    QString tag = "";
    if(pushStream) {
        if(inv) {
            tag = "<pushStream id='inv'/>";
            int index = rawData.indexOf(tag);
            if(index != -1) {
                inventory << rawData.mid(index + tag.length()).trimmed();
            } else {
                inventory << rawData.trimmed();
            }
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

        this->fixMonoTags(line);

        if(!rawData.startsWith("<output class=\"mono\"/>")) {
            line = highlighter->highlight(line);

            if(bold) {
                line.prepend("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BOLD\">");
            } else {
                line.prepend("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">");
            }
            line.append("</SPAN>");

            windowManager->writeGameWindow(line.toLocal8Bit());
            this->writeScript(line.toLocal8Bit());
        }
    } else if(gameText != "") {
        gameText = highlighter->highlight(gameText);
        gameText.prepend("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">");
        gameText.append("</SPAN>");

        windowManager->writeGameWindow(gameText.toLocal8Bit());
        this->writeScript(gameText.toLocal8Bit());
    }
}

// custom parsing for mono tags
void CommandParser::fixMonoTags(QString& line) {
    // fix style ids
    if(line.contains("preset id=\"thought\"")) {
        line.replace("preset id=\"thought\"", "PRESET ID=\"_PENALTY\"");
    } else if(line.contains("preset id=\"speech\"")) {
        line.replace("preset id=\"speech\"", "PRESET ID=\"_BONUS\"");
    } else if(line.startsWith("  <d cmd=\"flag")) {
        line = stripTags("<temp>" + line + "</temp>");
        if(line.endsWith('\n')) {
            line.chop(1);
        }
    }
}

QString CommandParser::stripTags(QString line) {
    QXmlStreamReader xml(line);
    QString textString;
    while (!xml.atEnd()) {
        if ( xml.readNext() == QXmlStreamReader::Characters ) {
            textString += xml.text();
        }
    }
    return textString;
}

void CommandParser::writeScript(QByteArray rawData) {
    QXmlStreamReader xml(rawData);
    QString textString;
    while (!xml.atEnd()) {
        if ( xml.readNext() == QXmlStreamReader::Characters ) {
            textString += xml.text();
        }
    }
    mainWindow->getScriptService()->writeOutgoingMessage("game_text#" + textString.toLocal8Bit());
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
