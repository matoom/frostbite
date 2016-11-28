#include "xmlparserthread.h"

XmlParserThread::XmlParserThread(QObject *parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = mainWindow->getWindowFacade();
    toolBar = mainWindow->getToolbar();
    commandLine = mainWindow->getCommandLine();
    gameDataContainer = GameDataContainer::Instance();
    highlighter = new Highlighter(parent);

    rxDmg.setPattern("\\bat you\\..*\\blands\\b");

    connect(this, SIGNAL(updateConversationsWindow(QString)), windowFacade, SLOT(updateConversationsWindow(QString)));
    connect(this, SIGNAL(updateNavigationDisplay(DirectionsList)), windowFacade, SLOT(updateNavigationDisplay(DirectionsList)));
    connect(this, SIGNAL(updateRoomWindowTitle(QString)), windowFacade, SLOT(updateRoomWindowTitle(QString)));

    connect(this, SIGNAL(updateRoomWindow()), windowFacade, SLOT(updateRoomWindow()));
    connect(this, SIGNAL(updateMapWindow(QString)), windowFacade, SLOT(updateMapWindow(QString)));
    connect(this, SIGNAL(updateExpWindow(QString, QString)), windowFacade, SLOT(updateExpWindow(QString, QString)));
    connect(this, SIGNAL(updateDeathsWindow(QString)), windowFacade, SLOT(updateDeathsWindow(QString)));
    connect(this, SIGNAL(updateThoughtsWindow(QString)), windowFacade, SLOT(updateThoughtsWindow(QString)));
    connect(this, SIGNAL(updateArrivalsWindow(QString)), windowFacade, SLOT(updateArrivalsWindow(QString)));
    connect(this, SIGNAL(updateFamiliarWindow(QString)), windowFacade, SLOT(updateFamiliarWindow(QString)));

    connect(this, SIGNAL(updateVitals(QString, QString)), toolBar, SLOT(updateVitals(QString, QString)));
    connect(this, SIGNAL(updateStatus(QString, QString)), toolBar, SLOT(updateStatus(QString, QString)));
    connect(this, SIGNAL(updateWieldLeft(QString)), toolBar, SLOT(updateWieldLeft(QString)));
    connect(this, SIGNAL(updateWieldRight(QString)), toolBar, SLOT(updateWieldRight(QString)));
    connect(this, SIGNAL(updateSpell(QString)), toolBar, SLOT(updateSpell(QString)));
    connect(this, SIGNAL(updateActiveSpells(QStringList)), toolBar, SLOT(updateActiveSpells(QStringList)));
    connect(this, SIGNAL(clearActiveSpells()), toolBar, SLOT(clearActiveSpells()));

    connect(this, SIGNAL(setTimer(int)), commandLine->getRoundtimeDisplay(), SLOT(setTimer(int)));
    connect(this, SIGNAL(setCastTimer(int)), commandLine->getRoundtimeDisplay(), SLOT(setCastTimer(int)));

    connect(this, SIGNAL(writeScriptMessage(QByteArray)), mainWindow->getScriptService(), SLOT(writeScriptText(QByteArray)));
    connect(this, SIGNAL(setMainTitle(QString)), mainWindow, SLOT(setMainTitle(QString)));
    connect(this, SIGNAL(writeText(QByteArray, bool)), windowFacade, SLOT(writeGameText(QByteArray, bool)));

    exit = false;
    bold = false;
    initRoundtime = false;
    initCastTime = false;
    prompt = false;
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
        msleep(25);
    }
}

QString XmlParserThread::fixInputXml(QString data) {
    data.replace(QRegularExpression("&(?!#?[a-z0-9]+;)"), "&amp;");

    // pushStream
    data.replace(QRegularExpression("<pushStream(.+?)\\/>"), "<!---->\r\n<pushStream\\1>");
    data.replace(QRegularExpression("<popStream[^>]*\\/>"), "</pushStream>\r\n<!---->");

    // mono
    data.replace(QRegularExpression("<output.*\"mono\"[^>]*\\/>"), "<!---->\r\n<output class=\"mono\"><![CDATA[");
    data.replace(QRegularExpression("<output.*\"\"[^>]*\\/>"), "]]></output>\r\n<!---->");

    return data;
}

void XmlParserThread::process(QByteArray data) {
    QString str = fixInputXml(QString::fromLocal8Bit(data.trimmed()));

    QList<QString> lines = str.split("\r\n");

    for(int i = 0; i < lines.size(); i++) {
        if(lines.at(i).startsWith("<pushStream")) {
            int count = 0;
            QString pushStream = lines.at(i) + "\n";
            while(i < lines.size() - 1) {
                count += lines.at(i).count("<pushStream") - lines.at(i).count("</pushStream>");
                if(count <= 0) break;
                pushStream += lines.at(++i) + "\n";
            }
            processPushStream(pushStream);
        } else if(lines.at(i).startsWith("<output")) {
            QString output = lines.at(i) + "\n";
            while(i < lines.size() - 1) {
                output += lines.at(++i) + "\n";
                if(lines.at(i).endsWith("</output>")) break;
            }
            processOutput(output);
        } else {
            processGameData(lines.at(i).toLocal8Bit());
        }
    }
}

void XmlParserThread::processGameData(QByteArray data) {
    data.prepend("<root>");
    data.append("</root>");

    QDomDocument doc("gameData");
    if(!doc.setContent(data)) {
        this->warnUnknownEntity("game-data-doc", data);
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();

    gameText = "";
    bool empty = false;
    while(!n.isNull()) {
        // plain text
        if(!filterPlainText(root, n)) {
            empty = false;
            break;
        }
        // data tags
        empty = filterDataTags(root, n);

        n = n.nextSibling();
    }

    if(!empty) emit writeText(gameText.toLocal8Bit(), prompt);
}

bool XmlParserThread::filterPlainText(QDomElement root, QDomNode n) {
    QDomElement e = n.toElement();

    /* Process game text with start tag only */
    if(e.tagName() == "settingsInfo") {
        emit writeSettings();
    } else if(e.tagName() == "settings") {
        emit writeModeSettings();
    } else if(e.tagName() == "style" && e.attribute("id") == "roomName") {
        QString roomName = root.text().trimmed();
        TextUtils::plainToHtml(roomName);
        gameText += "<span class=\"room-name\">" + roomName + "</span>";
        return false;
    /* All plain text without tags */
    } else if(n.isText()) {
        // compensate for qdomnode discarding &lt
        QString textData = n.toText().data();

        TextUtils::plainToHtml(textData);

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
        QString d = e.text().trimmed();
        TextUtils::plainToHtml(d);
        gameText += d;
    } else if(e.tagName() == "preset" && e.attribute("id") == "roomDesc") {
        QString preset = e.text().trimmed();
        TextUtils::plainToHtml(preset);
        gameText += preset;
    } else if(e.tagName() == "preset" && e.attribute("id") == "thought") {
        gameText += this->parseTalk(e);
    } else if(e.tagName() == "preset" && e.attribute("id") == "speech") {
        gameText += this->parseTalk(e);
    } else if(e.tagName() == "preset" && e.attribute("id") == "whisper") {
        gameText += this->parseTalk(e);
    } else if(e.tagName() == "b") {
        gameText += this->parseTalk(e);
    }
    return true;
}

bool XmlParserThread::filterDataTags(QDomElement root, QDomNode n) {
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

            if(initCastTime) {
                time.setTime_t(e.attribute("time").toInt());

                int t_to = time.secsTo(castTime);
                emit setCastTimer(t_to > 300 ? 300 : t_to);

                initCastTime = false;
            }
            prompt = true;
            gameText += root.text().trimmed().toUtf8();
            this->runScheduledEvents();
        } else if(e.tagName() == "compass") {
            /* filter compass */
            QList<QString> directions;
            QDomNode compassNode  = root.firstChildElement("compass").firstChild();
            while(!compassNode.isNull()) {
                directions << compassNode.toElement().attribute("value");
                compassNode = compassNode.nextSibling();
            }
            qSort(directions);

            GameDataContainer::Instance()->setCompassDirections(directions);

            QString text = GameDataContainer::Instance()->getRoomName() +
                    TextUtils::stripMapSpecial(GameDataContainer::Instance()->getRoomDesc())
                    + directions.join("");

            QString hash = TextUtils::toHash(text);

            /*qDebug() << "====";
            qDebug() << text;
            qDebug() << hash;
            qDebug() << "====";*/

            emit updateNavigationDisplay(directions);
            emit updateMapWindow(hash);
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
        } else if(e.tagName() == "castTime") {
            castTime.setTime_t(e.attribute("value").toInt());
            initCastTime = true;
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
            //emit updateRoomWindowTitle(title);
        } else if(e.tagName() == "nav") {
            emit writeScriptMessage("{nav}");
        } else if(e.tagName() == "component") {
            if(e.attribute("id").startsWith("exp")) {
                QString text = e.text();
                QString id = e.attribute("id").mid(4);

                if(id != "tdp") {
                    if(!text.isEmpty()) {
                        if(e.elementsByTagName("d").count() == 0) {
                            gameDataContainer->setExpField(false, id, text);
                            emit updateExpWindow(id, TextUtils::addNumericStateToExp(text));
                        } else {
                            gameDataContainer->setExpField(true, id, text);
                            emit updateExpWindow(id, text);
                        }
                    } else {
                        gameDataContainer->removeExpField(id);
                        emit updateExpWindow(id, text);
                    }
                }
            } else if(e.attribute("id").startsWith("room")) {
                QString id = e.attribute("id");
                if(id.endsWith("desc")) {
                    QString roomDesc = e.text();
                    TextUtils::plainToHtml(roomDesc);
                    gameDataContainer->setRoomDesc(roomDesc);
                } else if (id.endsWith("objs")) {
                    QString str;
                    QTextStream stream(&str);
                    e.save(stream, QDomNode::ElementNode);
                    gameDataContainer->setRoomObjsData(str);
                    QString roomObjs = e.text();
                    TextUtils::plainToHtml(roomObjs);
                    gameDataContainer->setRoomObjs(roomObjs);
                } else if (id.endsWith("players")) {
                    QString roomPlayers = e.text();
                    TextUtils::plainToHtml(roomPlayers);
                    gameDataContainer->setRoomPlayers(roomPlayers);
                } else if (id.endsWith("exits")) {
                    QString roomExits = e.text();
                    TextUtils::plainToHtml(roomExits);
                    gameDataContainer->setRoomExits(roomExits);
                } else if (id.endsWith("extra")) {
                    QString roomExtra = e.text();
                    TextUtils::plainToHtml(roomExtra);
                    gameDataContainer->setRoomExtra(roomExtra);
                }
                emit updateRoomWindow();
            }
        } else if(e.tagName() == "clearStream") {
            if(e.attribute("id") == "percWindow") {
                gameDataContainer->clearActiveSpells();
                emit clearActiveSpells();
                this->activeSpells.clear();
            }
        } else if (e.tagName() == "pushBold") {
            bold = true;
        } else if (e.tagName() == "popBold") {
            bold = false;
        }
    }
    return gameText == "";
}

void XmlParserThread::processPushStream(QString data) {
    data.prepend("<root>");
    data.append("</root>");

    QDomDocument doc("pushStream");
    if(!doc.setContent(data)) {
        this->warnUnknownEntity("push-stream-doc", data);
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();

    QDomElement e = n.toElement();

    if(e.attribute("id") == "talk") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            if(element.attribute("id") == "speech") {
                QString elementText = element.nextSibling().toText().data().trimmed();
                TextUtils::plainToHtml(elementText);
                QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
                emit updateConversationsWindow(addTime(text));
            } else if(element.attribute("id") == "thought") {
                QString elementText = element.nextSibling().toText().data().trimmed();
                TextUtils::plainToHtml(elementText);
                QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
                emit updateThoughtsWindow(addTime(text));
            } else {
                this->warnUnknownEntity("talk-preset", data);
            }
        } else if(element.tagName() == "b") {
            QString elementText = element.nextSibling().toText().data().trimmed();
            TextUtils::plainToHtml(elementText);
            QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
            emit updateConversationsWindow(addTime(text));
        } else {
            this->warnUnknownEntity("talk", data);
        }
    } else if(e.attribute("id") == "logons") {
        emit updateArrivalsWindow(addTime(root.text().trimmed()));
    } else if(e.attribute("id") == "inv") {
        gameDataContainer->setInventory(root.text().split("\n"));
    } else if(e.attribute("id") == "experience") {
        foreach(QString key, gameDataContainer->getExp().keys()) {
            emit updateExpWindow(key, "");
        }
    } else if(e.attribute("id") == "room") {
        // ignored
        // <compDef id='room desc'/> ..
    } else if(e.attribute("id") == "assess") {
        // ignored
    } else if(e.attribute("id") == "thoughts") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            QString elementText = element.nextSibling().toText().data().trimmed();
            TextUtils::plainToHtml(elementText);
            QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
            emit updateThoughtsWindow(addTime(text));
        } else {
            this->warnUnknownEntity("thoughts", data);
        }
    } else if(e.attribute("id") == "death") {
        emit updateDeathsWindow(addTime(root.text().trimmed()));
    } else if(e.attribute("id") == "atmospherics") {
        emit writeText(root.text().trimmed().toLocal8Bit(), false);
    } else if(e.attribute("id") == "whispers") {
        QDomElement element = e.firstChild().toElement();
        if(element.attribute("id") == "whisper") {
            QString elementText = element.nextSibling().toText().data().trimmed();
            TextUtils::plainToHtml(elementText);
            QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
            emit updateConversationsWindow(addTime(text));
        } else {
            this->warnUnknownEntity("whispers", data);
        }
    } else if(e.attribute("id") == "familiar") {
        QDomElement element = e.firstChild().toElement();
        QString id = element.attribute("id");
        if(id == "talk") {
            QDomElement first = element.firstChild().toElement();
            if(first.attribute("id") == "speech" || first.attribute("id") == "whisper" || first.tagName() == "b") {
                QString elementText = first.nextSibling().toText().data().trimmed();
                TextUtils::plainToHtml(elementText);
                QString text = tr("%1%2").arg(this->parseTalk(first), elementText);
                emit updateFamiliarWindow(text);
            } else {
                this->warnUnknownEntity("familiar-talk", data);
            }
        } else if(id == "speech" || id == "whisper") {
            QString elementText = element.nextSibling().toText().data().trimmed();
            TextUtils::plainToHtml(elementText);
            QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
            emit updateFamiliarWindow(text);
        } else if (id == "familiar") {
            QDomElement element = e.firstChild().toElement();
            emit updateFamiliarWindow(element.text().replace("\r\n", "\n"));
        } else {
            emit updateFamiliarWindow(e.text().replace("\r\n", "\n"));
        }
    } else if(e.attribute("id") == "ooc") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            if(element.attribute("id") == "whisper") {
                QString elementText = element.nextSibling().toText().data().trimmed();
                TextUtils::plainToHtml(elementText);
                QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
                emit updateConversationsWindow(addTime(text));
            } else {
                this->warnUnknownEntity("ooc-preset", data);
            }
        } else {
            QString ooc = root.text().trimmed();
            TextUtils::plainToHtml(ooc);
            emit writeText(ooc.toLocal8Bit(), false);
        }
    } else if(e.attribute("id") == "percWindow") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "b") {
            activeSpells += toString(element);
        } else {
            activeSpells += root.text();
        }
        QStringList list = activeSpells.split("\n", QString::SkipEmptyParts);
        gameDataContainer->setActiveSpells(list);
        scheduled.insert(e.attribute("id"), list);
    } else if(e.attribute("id") == "group") {
        // ignored
    } else if(e.attribute("id") == "chatter") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            if(element.attribute("id") == "thought") {
                QString elementText = element.nextSibling().toText().data();
                QString trimTrailing = elementText.remove(QRegularExpression("\r\n$"));
                TextUtils::plainToHtml(trimTrailing);
                QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
                emit updateThoughtsWindow(addTime(text));
            } else {
                this->warnUnknownEntity("chatter-preset", data);
            }
        } else {
            this->warnUnknownEntity("chatter", data);
        }
    } else {
        this->warnUnknownEntity("push-stream", data);
    }
}

void XmlParserThread::processOutput(QString data) {
    data.prepend("<root>");
    data.append("</root>");

    data.replace("pushBold/", "span class=\"bold\"");
    data.replace("popBold/", "/span");

    data.replace("preset id=\"thought\"", "preset class=\"penalty\"");
    data.replace("preset id=\"speech\"", "preset class=\"bonus\"");

    QDomDocument doc("output");
    if(!doc.setContent(data)) {
        this->warnUnknownEntity("output-doc", data);
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();

    QDomElement e = n.toElement();

    if(e.attribute("class") == "mono") {
        QString text = e.text();
        if(text.startsWith('\n')) text = text.right(text.size() - 1);
        if(text.endsWith('\n')) text.chop(1);
        emit writeText(text.toLocal8Bit(), false);
    }
}

void XmlParserThread::warnUnknownEntity(QString ref, QString xml) {
    qDebug() << tr("unknown xml event (%1): %2").arg(ref, xml);
}

QString XmlParserThread::parseTalk(QDomElement element) {
    if(element.attribute("id") == "speech") {
        return tr("<span class=\"speech\">%1</span>").arg(element.text());
    } else if(element.attribute("id") == "thought") {
        return tr("<span class=\"thinking\">%1</span>").arg(element.text());
    } else if(element.attribute("id") == "whisper") {
        return tr("<span class=\"whisper\">%1</span>").arg(element.text());
    } else if(element.tagName() == "b") {
        return tr("<span class=\"speech\">%1</span>").arg(element.text());
    } else {
        this->warnUnknownEntity("parse-preset", toString(element));
    }
    return "";
}

void XmlParserThread::runScheduledEvents() {
    if(scheduled.empty()) return;

    QHashIterator<QString, QVariant> i(scheduled);
    while (i.hasNext()) {
        i.next();
        this->runEvent(i.key(), i.value());
        scheduled.remove(i.key());
    }
}

void XmlParserThread::runEvent(QString event, QVariant data) {
    if(event == "percWindow") {
        emit updateActiveSpells(data.toStringList());
    } else {
        qDebug() << tr("Event \"%1\" not found").arg(event);
    }
}

QString XmlParserThread::toString(QDomElement element) {
    QString str;
    QTextStream stream(&str);
    element.save(stream, QDomNode::ElementNode);
    return str;
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

QString XmlParserThread::addTime(QString data) {
    return data + " <span class=\"body\">[" + QTime::currentTime().toString("h:mm ap") + "]</span>";
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
