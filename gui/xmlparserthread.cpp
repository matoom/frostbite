#include "xmlparserthread.h"

XmlParserThread::XmlParserThread(QObject *parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = mainWindow->getWindowFacade();
    toolBar = mainWindow->getToolbar();
    commandLine = mainWindow->getCommandLine();
    gameDataContainer = GameDataContainer::Instance();
    highlighter = new Highlighter(parent);

    rxAmp.setPattern("&(?!#?[a-z0-9]+;)");
    rxDmg.setPattern("fail to\\b|attempt to\\b|counter little of\\b|You evade, barely\\b");

    connect(this, SIGNAL(updateConversationsWindow(QString)), windowFacade, SLOT(updateConversationsWindow(QString)));
    connect(this, SIGNAL(updateNavigationDisplay(DirectionsList)), windowFacade, SLOT(updateNavigationDisplay(DirectionsList)));
    connect(this, SIGNAL(updateRoomWindowTitle(QString)), windowFacade, SLOT(updateRoomWindowTitle(QString)));

    connect(this, SIGNAL(updateExpWindow()), windowFacade, SLOT(updateExpWindow()));
    connect(this, SIGNAL(updateRoomWindow()), windowFacade, SLOT(updateRoomWindow()));

    connect(this, SIGNAL(updateDeathsWindow(QString)), windowFacade, SLOT(updateDeathsWindow(QString)));
    connect(this, SIGNAL(updateThoughtsWindow(QString)), windowFacade, SLOT(updateThoughtsWindow(QString)));
    connect(this, SIGNAL(updateArrivalsWindow(QString)), windowFacade, SLOT(updateArrivalsWindow(QString)));
    connect(this, SIGNAL(updateFamiliarWindow(QString)), windowFacade, SLOT(updateFamiliarWindow(QString)));

    connect(this, SIGNAL(updateVitals(QString, QString)), toolBar, SLOT(updateVitals(QString, QString)));
    connect(this, SIGNAL(updateStatus(QString, QString)), toolBar, SLOT(updateStatus(QString, QString)));
    connect(this, SIGNAL(updateWieldLeft(QString)), toolBar, SLOT(updateWieldLeft(QString)));
    connect(this, SIGNAL(updateWieldRight(QString)), toolBar, SLOT(updateWieldRight(QString)));
    connect(this, SIGNAL(updateSpell(QString)), toolBar, SLOT(updateSpell(QString)));
    connect(this, SIGNAL(updateActiveSpells()), toolBar, SLOT(updateActiveSpells()));
    connect(this, SIGNAL(clearActiveSpells()), toolBar, SLOT(clearActiveSpells()));

    connect(this, SIGNAL(setTimer(int)), commandLine->getRoundtimeDisplay(), SLOT(setTimer(int)));
    connect(this, SIGNAL(writeScriptMessage(QByteArray)), mainWindow->getScriptService(), SLOT(writeScriptText(QByteArray)));
    connect(this, SIGNAL(setMainTitle(QString)), mainWindow, SLOT(setMainTitle(QString)));
    connect(this, SIGNAL(writeText(QByteArray, bool)), windowFacade, SLOT(writeGameText(QByteArray, bool)));

    exit = false;
    pushStream = false;
    inv = false;
    mono = false;
    bold = false;
    initRoundtime = false;
    prompt = false;

    familiarElementCount = 0;
}

void XmlParserThread::updateHighlighterSettings() {
    highlighter->reloadSettings();
}

void XmlParserThread::addData(QByteArray buffer) {
    mMutex.lock();
    dataQueue.enqueue(buffer);
    mMutex.unlock();
}

void XmlParserThread::run() {
    while(!exit) {
        while(!dataQueue.isEmpty()) {
            mMutex.lock();
            localData = dataQueue.dequeue();
            mMutex.unlock();
            process(localData);
        }
        msleep(100);
    }
}

void XmlParserThread::process(QByteArray data) {
    QList<QByteArray> lines = data.split('\n');

    for(int i = 0; i < lines.size(); i++) {
        processGameData(lines.at(i));
    }
}

void XmlParserThread::processGameData(QByteArray data) {
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
        filterDataTags(root, n);

        n = n.nextSibling();
    }

    // process pushstream data over multiple lines
    processPushStream(rawData);

    // Write game text and raw data
    writeGameText(rawData);
}

bool XmlParserThread::filterPlainText(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    if(!mono && !pushStream) {
        /* Process game text with start tag only */
        if(e.tagName() == "style" && e.attribute("id") == "roomName") {
            gameText += "<span class=\"room-name\">" + root.text().trimmed() + "</span>";
            return false;
        /* All plain text without tags */
        } else if(n.isText()) {
            // compensate for qdomnode discarding &lt
            QString textData = n.toText().data();

            // remove CRs
            if(textData.endsWith("\r")) {
                textData.chop(1);
            }

            textData.replace("<", "&lt;");

            if(bold) {
                if(root.text().contains(rxDmg)) {
                    gameText += "<span class=\"damage\">" + textData + "</span>";
                } else {
                    gameText += "<span class=\"bold\">" + textData + "</span>";
                }
            } else {
                gameText += textData;
            }
        /* Process game text between tags */
        } else if(e.tagName() == "d") {
            gameText += e.text().trimmed();
        } else if(e.tagName() == "preset" && e.attribute("id") == "roomDesc") {
            gameText += e.text().trimmed();
        } else if(e.tagName() == "preset" && e.attribute("id") == "speech") {
            gameText += "<span class=\"speech\">" + e.text().trimmed() + "</span>";

            QString speech = root.text().trimmed();
            speech.insert(speech.indexOf("\""), "</span>");
            speech.prepend("<span class=\"speech\">");

            emit updateConversationsWindow(speech +
            " [" + QTime::currentTime().toString("h:mm ap") + "]");
        } else if(e.tagName() == "preset" && e.attribute("id") == "whisper") {
            gameText += "<span class=\"whisper\">" + e.text().trimmed() + "</span>";

            QString whisper = root.text().trimmed();
            whisper.insert(whisper.indexOf("\""), "</span>");
            whisper.prepend("<span class=\"whisper\">");

            emit updateConversationsWindow(whisper +
            " [" + QTime::currentTime().toString("h:mm ap") + "]");
        } else if(e.tagName() == "b") {
            gameText += "<span class=\"speech\">" + e.text().trimmed() + "</span>";

            QString yell = root.text().trimmed();
            yell.insert(yell.indexOf("\""), "</span>");
            yell.prepend("<span class=\"speech\">");

            emit updateConversationsWindow(yell +
            " [" + QTime::currentTime().toString("h:mm ap") + "]");
        }
    }
    return true;
}

void XmlParserThread::filterDataTags(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    prompt = false;

    if(!e.isNull()) {
        if(e.tagName() == "prompt") {
            /* filter prompt */
            if(initRoundtime) {
                time.setTime_t(e.attribute("time").toInt());

                int t_to = time.secsTo(roundTime);
                emit setTimer(t_to > 300 ? 300 : t_to);

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
            gameDataContainer->setLeft(e.text());
            gameDataContainer->setLeftNoun(e.attribute("noun"));
        } else if(e.tagName() == "right") {
            /* filter player wielding in right hand */
            emit updateWieldRight(e.text());
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
        } else if(e.tagName() == "clearStream") {
            if(e.attribute("id") == "percWindow") {
                gameDataContainer->clearActiveSpells();
                emit clearActiveSpells();
            }
        } else if(e.tagName() == "pushStream") {
            pushStream = true;
            if(e.attribute("id") == "assess") {
                pushStream = false;
            }

            if(e.attribute("id") == "logons") {
                emit updateArrivalsWindow(root.text().trimmed() +
                    " [" + QTime::currentTime().toString("h:mm ap") + "]");
            } else if(e.attribute("id") == "thoughts") {
                QString thought = root.text().trimmed();
                thought.insert(thought.indexOf("\""), "</span>");
                thought.prepend("<span class=\"thinking\">");
                emit updateThoughtsWindow(thought +
                    " [" + QTime::currentTime().toString("h:mm ap") + "]");
            } else if(e.attribute("id") == "death") {
                emit updateDeathsWindow(root.text().trimmed() +
                    " [" + QTime::currentTime().toString("h:mm ap") + "]");
            } else if(e.attribute("id") == "atmospherics") {
                gameText += root.text().trimmed();
            } else if(e.attribute("id") == "inv") {
                inv = true;
            } else if(e.attribute("id") == "familiar") {
                familiarElementCount++;
                if(familiarElementCount == 1) {
                    QString text = root.text().trimmed();

                    if(root.firstChildElement("preset").attribute("id") == "speech" ||
                       root.firstChildElement("b").tagName() == "b") {
                        text.insert(text.indexOf("\""), "</span>");
                        text.prepend("<span class=\"speech\">");
                    } else if(root.firstChildElement("preset").attribute("id") == "whisper") {
                        text.insert(text.indexOf("\""), "</span>");
                        text.prepend("<span class=\"whisper\">");
                    } else {
                        gameText += text;
                    }

                    emit updateFamiliarWindow(text);
                }
            } else if(e.attribute("id") == "ooc") {
                gameText += root.text().trimmed();
            } else if(e.attribute("id") == "percWindow") {
              gameDataContainer->addActiveSpells(root.text().trimmed());
              emit updateActiveSpells();
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

void XmlParserThread::processPushStream(QByteArray rawData) {
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

void XmlParserThread::writeGameText(QByteArray rawData) {
    if (rawData.size() == 1) {
        emit writeText("", false);
    } else if (mono && !pushStream) {
        /* mono handled separately otherwise white spaces
        before tags are not recognized as text */
        QString line = rawData;

        // remove CRs
        if(line.endsWith("\r")) {
            line.chop(1);
        }

        this->fixMonoTags(line);

        if(!rawData.startsWith("<output class=\"mono\"/>")) {
            emit writeText(line.toLocal8Bit(), false);
        }
    } else if(gameText != "") {
        emit writeText(gameText.toLocal8Bit(), prompt);
    }
}

// custom parsing for mono tags
void XmlParserThread::fixMonoTags(QString& line) {
    // strip tags from cmd=flag
    if(line.startsWith("  <d cmd=\"flag")) {
        line = stripTags("<temp>" + line + "</temp>");
        if(line.endsWith('\n')) {
            line.chop(1);
        }
    } else {
        // fix ids
        line.replace("preset id=\"thought\"", "preset class=\"penalty\"");
        line.replace("preset id=\"speech\"", "preset class=\"bonus\"");
        // fix bold
        if(line.contains("pushBold")) {
            line.replace("pushBold/", "span class=\"bold\"");
            line.append("</span>");
        }
        line.remove("<popBold/>");
    }
}

QString XmlParserThread::stripTags(QString line) {
    QXmlStreamReader xml(line);
    QString textString;
    while (!xml.atEnd()) {
        if (xml.readNext() == QXmlStreamReader::Characters) {
            textString += xml.text();
        }
    }
    return textString;
}

XmlParserThread::~XmlParserThread() {
    this->exit = true;
    if(!this->wait(1000)) {
        qWarning("Thread deadlock detected, terminating thread.");
        this->terminate();
        this->wait();
    }
    delete highlighter;
}
