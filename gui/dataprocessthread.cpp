#include "dataprocessthread.h"

DataProcessThread::DataProcessThread(QObject *parent) {
    mainWindow = (MainWindow*)parent;
    windowManager = mainWindow->getWindowManager();
    toolbarManager = mainWindow->getToolbarManager();
    commandLine = mainWindow->getCommandLine();
    gameDataContainer = GameDataContainer::Instance();
    highlighter = new Highlighter(parent);

    rxAmp.setPattern("&(?!#?[a-z0-9]+;)");
    rxDmg.setPattern("fail to\\b|attempt to\\b|counter little of\\b|You evade, barely\\b");

    connect(this, SIGNAL(updateConversationsWindow(QString)), windowManager, SLOT(updateConversationsWindow(QString)));
    connect(this, SIGNAL(updateNavigationDisplay(DirectionsList)), windowManager, SLOT(updateNavigationDisplay(DirectionsList)));
    connect(this, SIGNAL(updateRoomWindowTitle(QString)), windowManager, SLOT(updateRoomWindowTitle(QString)));

    connect(this, SIGNAL(updateExpWindow()), windowManager, SLOT(updateExpWindow()));
    connect(this, SIGNAL(updateRoomWindow()), windowManager, SLOT(updateRoomWindow()));

    connect(this, SIGNAL(updateDeathsWindow(QString)), windowManager, SLOT(updateDeathsWindow(QString)));
    connect(this, SIGNAL(updateThoughtsWindow(QString)), windowManager, SLOT(updateThoughtsWindow(QString)));
    connect(this, SIGNAL(updateArrivalsWindow(QString)), windowManager, SLOT(updateArrivalsWindow(QString)));
    connect(this, SIGNAL(updateFamiliarWindow(QString)), windowManager, SLOT(updateFamiliarWindow(QString)));

    connect(this, SIGNAL(updateVitals(QString, QString)), toolbarManager, SLOT(updateVitals(QString, QString)));
    connect(this, SIGNAL(updateStatus(QString, QString)), toolbarManager, SLOT(updateStatus(QString, QString)));
    connect(this, SIGNAL(updateWieldLeft(QString)), toolbarManager, SLOT(updateWieldLeft(QString)));
    connect(this, SIGNAL(updateWieldRight(QString)), toolbarManager, SLOT(updateWieldRight(QString)));
    connect(this, SIGNAL(updateSpell(QString)), toolbarManager, SLOT(updateSpell(QString)));

    connect(this, SIGNAL(setTimer(int)), commandLine->getRoundtimeDisplay(), SLOT(setTimer(int)));
    connect(this, SIGNAL(writeScriptMessage(QByteArray)), mainWindow->getScriptService(), SLOT(writeOutgoingMessage(QByteArray)));
    connect(this, SIGNAL(setMainTitle(QString)), mainWindow, SLOT(setMainTitle(QString)));
    connect(this, SIGNAL(writeText(QByteArray, bool)), windowManager, SLOT(writeGameText(QByteArray, bool)));

    pushStream = false;
    inv = false;
    mono = false;
    bold = false;
    initRoundtime = false;
    prompt = false;

    familiarElementCount = 0;
}

void DataProcessThread::updateHighlighterSettings() {
    highlighter->reloadSettings();
}

void DataProcessThread::addData(QByteArray buffer) {
    mMutex.lock();
    dataQueue.enqueue(buffer);
    mMutex.unlock();
}

void DataProcessThread::run() {
    while(!dataQueue.isEmpty()) {
        mMutex.lock();
        localData = dataQueue.dequeue();
        mMutex.unlock();
        process(localData);
    }
}

void DataProcessThread::process(QByteArray data) {
    QList<QByteArray> lines = data.split('\n');

    for(int i = 0; i < lines.size(); i++) {
        processGameData(lines.at(i));
    }
}

void DataProcessThread::processGameData(QByteArray data) {
    /*The ampersand character (&) and the left angle bracket (<) MUST NOT
    appear in their literal form, except when used as markup delimiters, or
    within a comment, a processing instruction, or a CDATA section. If they
    are needed elsewhere, they MUST be escaped using either numeric
    character references or the strings "&amp;" and "&lt;" respectively.*/

    QString d = data;    
    d.replace(rxAmp, "&amp;");
    data = d.toLocal8Bit();

    QDomDocument doc("GameData");
    QByteArray rawData(data);

    data.prepend("<root>");
    data.append("</root>");

    if(!doc.setContent(data)) {
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();

    familiarElementCount = 0;

    gameText = "";
    while(!n.isNull()) {
        // Process game text
        if(!filterPlainText(root, n)) {
            break;
        }
        // Process game data tags
        filterDataTags(root, n, data);

        n = n.nextSibling();
    }

    // process pushstream data over multiple lines
    processPushStream(rawData);

    // Write game text and raw data
    writeGameText(rawData);
}

bool DataProcessThread::filterPlainText(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    if(!mono && !pushStream) {
        /* Process game text with start tag only */
        if(e.tagName() == "style" && e.attribute("id") == "roomName") {
            gameText += "<SPAN ID=\"_ROOM_NAME\">" + root.text() + "</SPAN>";
            return false;
        /* All plain text without tags */
        } else if(n.isText()) {
            // compensate for qdomnode discarding &lt
            QString textData = n.toText().data();
            textData.replace("<", "&lt;");

            if(bold) {
                if(root.text().contains(rxDmg)) {
                    gameText += "<SPAN ID=\"_DAMAGE\">" + textData + "</SPAN>";
                } else {
                    gameText += "<SPAN ID=\"_BOLD\">" + textData + "</SPAN>";
                }
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

            QString speech = root.text().trimmed();
            speech.insert(speech.indexOf("\""), "</SPAN>");
            speech.prepend("<SPAN ID=\"_SPEECH\">");

            emit updateConversationsWindow(speech +
            " [" + QTime::currentTime().toString("h:mm ap") + "]");
        } else if(e.tagName() == "preset" && e.attribute("id") == "whisper") {
            gameText += "<SPAN ID=\"_WHISPER\">" + e.text() + "</SPAN>";

            QString whisper = root.text().trimmed();
            whisper.insert(whisper.indexOf("\""), "</SPAN>");
            whisper.prepend("<SPAN ID=\"_WHISPER\">");

            emit updateConversationsWindow(whisper +
            " [" + QTime::currentTime().toString("h:mm ap") + "]");
        } else if(e.tagName() == "b") {
            gameText += "<SPAN ID=\"_SPEECH\">" + e.text() + "</SPAN>";

            QString yell = root.text().trimmed();
            yell.insert(yell.indexOf("\""), "</SPAN>");
            yell.prepend("<SPAN ID=\"_SPEECH\">");

            emit updateConversationsWindow(yell +
            " [" + QTime::currentTime().toString("h:mm ap") + "]");
        }
    }
    return true;
}

void DataProcessThread::filterDataTags(QDomElement root, QDomNode n, QByteArray rawData) {
    QDomElement e = n.toElement();

    prompt = false;

    if(!e.isNull()) {
        if(e.tagName() == "prompt") {
            /* filter prompt */
            if(initRoundtime) {
                time.setTime_t(e.attribute("time").toInt());

                int t_to = time.secsTo(roundTime);
                emit setTimer(t_to > 100 ? 100 : t_to);

                initRoundtime = false;
            }

            prompt = true;
            gameText += root.text().trimmed().toUtf8();
        } else if(e.tagName() == "compass") {
            /* filter compass */
            QList<QString> directions;
            QDomNode compassNode  = root.firstChildElement("compass").firstChild();
            while(!compassNode.isNull()) {
                directions << compassNode.toElement().attribute("value");
                compassNode = compassNode.nextSibling();
            }
            emit updateNavigationDisplay(directions);
        } else if (e.tagName() == "clearContainer") {
            QStringList container;
            QDomElement invElem = root.firstChildElement("inv");
            while(!invElem.isNull()) {
                container << invElem.text().trimmed();
                invElem = invElem.nextSiblingElement("inv");
            }
            gameDataContainer->setContainer(container);
        } else if(e.tagName() == "roundTime") {
            roundTime.setTime_t(e.attribute("value").toInt());
            initRoundtime = true;

        } else if(e.tagName() == "dialogData" && e.attribute("id") == "minivitals") {
            /* filter vitals */
            QDomElement vitalsElement = root.firstChildElement("dialogData").firstChildElement("progressBar");
            emit updateVitals(vitalsElement.attribute("id"), vitalsElement.attribute("value"));
            highlighter->alert(vitalsElement.attribute("id"), vitalsElement.attribute("value").toInt());
        }  else if(e.tagName() == "indicator") {
            /* filter player status indicator */
            //<indicator id="IconKNEELING" visible="n"/><indicator id="IconPRONE" visible="n"/>
            emit updateStatus(e.attribute("visible"), e.attribute("id"));
            if(e.attribute("visible") == "y") {
                highlighter->alert(e.attribute("id"));
            }
        } else if(e.tagName() == "left") {
            /* filter player wielding in left hand */
            //<right exist="162134941" noun="sharks">fuzzy sharks</right>
            emit updateWieldLeft(e.text());
            /* persist wield data in memory */
            gameDataContainer->setLeft(e.text());
            gameDataContainer->setLeftNoun(e.attribute("noun"));
        } else if(e.tagName() == "right") {
            /* filter player wielding in right hand */
            emit updateWieldRight(e.text());
            /* persist wield data in memory */
            gameDataContainer->setRight(e.text());
            gameDataContainer->setRightNoun(e.attribute("noun"));
        } else if(e.tagName() == "spell") {
            emit updateSpell(e.text());
        } else if(e.tagName() == "streamWindow" && e.attribute("id") == "main") {
            /* filter main window title */
            QString title = e.attribute("subtitle");
            gameDataContainer->setRoomName(title.mid(3));            
            emit setMainTitle(title);
            emit updateRoomWindowTitle(title);
        } else if(e.tagName() == "nav") {
            emit writeScriptMessage("{nav}");
        } else if(e.tagName() == "component") {
            if(e.attribute("id").startsWith("exp")) {
                QString text = e.text();
                QString id = e.attribute("id").mid(4);

                if(id != "tdp") {
                    if(!text.isEmpty()) {
                        if(e.firstChildElement("d").isNull()) {
                            gameDataContainer->setExpField(id, text);
                        } else {
                            gameDataContainer->setExpFieldBrief(id, text);
                        }
                    } else {
                        gameDataContainer->removeExpField(id);
                    }
                    emit updateExpWindow();
                }
            } else if(e.attribute("id").startsWith("room")) {
                QString id = e.attribute("id");
                if(id.endsWith("desc")) {
                    gameDataContainer->setRoomDesc(e.text());
                } else if (id.endsWith("objs")) {
                    gameDataContainer->setRoomObjs(e.text());
                } else if (id.endsWith("players")) {
                    gameDataContainer->setRoomPlayers(e.text());
                } else if (id.endsWith("exits")) {
                    gameDataContainer->setRoomExits(e.text());
                } else if (id.endsWith("extra")) {
                    gameDataContainer->setRoomExtra(e.text());
                }                                
                emit updateRoomWindow();
            }
        } else if(e.tagName() == "pushStream") {
            pushStream = true;
            if(e.attribute("id") == "assess") {
                pushStream = false;
            }

            if(e.attribute("id") == "logons") {
                emit updateArrivalsWindow(root.text() +
                    " [" + QTime::currentTime().toString("h:mm ap") + "]");
            } else if(e.attribute("id") == "thoughts") {
                QString thought = root.text().trimmed();
                thought.insert(thought.indexOf("\""), "</SPAN>");
                thought.prepend("<SPAN ID=\"_THINKING\">");
                emit updateThoughtsWindow(thought +
                    " [" + QTime::currentTime().toString("h:mm ap") + "]");
            } else if(e.attribute("id") == "death") {
                emit updateDeathsWindow(root.text() +
                    " [" + QTime::currentTime().toString("hh:mm ap") + "]");
            } else if(e.attribute("id") == "atmospherics") {
                gameText += root.text();
            } else if(e.attribute("id") == "inv") {
                inv = true;
            } else if(e.attribute("id") == "familiar") {
                familiarElementCount++;
                if(familiarElementCount == 1) {
                    QString text = root.text().trimmed();

                    if(root.firstChildElement("preset").attribute("id") == "speech" ||
                       root.firstChildElement("b").tagName() == "b") {
                        text.insert(text.indexOf("\""), "</SPAN>");
                        text.prepend("<SPAN ID=\"_SPEECH\">");
                    } else if(root.firstChildElement("preset").attribute("id") == "whisper") {
                        text.insert(text.indexOf("\""), "</SPAN>");
                        text.prepend("<SPAN ID=\"_WHISPER\">");
                    } else {
                        gameText += text;
                    }

                    emit updateFamiliarWindow(text);
                }
            } else if(e.attribute("id") == "ooc") {
                gameText += root.text();
            }
        } else if(e.tagName() == "popStream") {
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

void DataProcessThread::processPushStream(QByteArray rawData) {
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

void DataProcessThread::writeGameText(QByteArray rawData) {
    if (rawData.size() == 1) {
        emit writeText("", false);
    } else if (mono && !pushStream) {
        /* mono handled separately otherwise white spaces
        before tags are not recognized as text */
        QString line = rawData;

        this->fixMonoTags(line);

        if(!rawData.startsWith("<output class=\"mono\"/>")) {
            if(bold) {
                line.prepend("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BOLD\">");
                line.append("</SPAN>");
            }

            emit writeText(line.toLocal8Bit(), false);
        }
    } else if(gameText != "") {
        emit writeText(gameText.toLocal8Bit(), prompt);
    }
}

// custom parsing for mono tags
void DataProcessThread::fixMonoTags(QString& line) {
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

QString DataProcessThread::stripTags(QString line) {
    QXmlStreamReader xml(line);
    QString textString;
    while (!xml.atEnd()) {
        if (xml.readNext() == QXmlStreamReader::Characters) {
            textString += xml.text();
        }
    }
    return textString;
}

DataProcessThread::~DataProcessThread() {
    delete highlighter;
}
