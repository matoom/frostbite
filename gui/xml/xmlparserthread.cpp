#include "xmlparserthread.h"

XmlParserThread::XmlParserThread(QObject *parent) {
    mainWindow = (MainWindow*)parent;
    windowFacade = mainWindow->getWindowFacade();
    toolBar = mainWindow->getToolbar();
    vitalsBar = mainWindow->getVitalsBar();
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
    connect(this, SIGNAL(updateSpellWindow(QString)), windowFacade, SLOT(updateSpellWindow(QString)));

    connect(this, SIGNAL(updateVitals(QString, QString)), toolBar, SLOT(updateVitals(QString, QString)));
    connect(this, SIGNAL(updateVitals(QString, QString)), vitalsBar, SLOT(updateVitals(QString, QString)));

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

    connect(this, SIGNAL(registerStreamWindow(QString, QString)), windowFacade, SLOT(registerStreamWindow(QString, QString)));
    connect(this, SIGNAL(writeStreamWindow(QString, QString)), windowFacade, SLOT(writeStreamWindow(QString, QString)));
    connect(this, SIGNAL(clearStreamWindow(QString)), windowFacade, SLOT(clearStreamWindow(QString)));

    exit = false;
    bold = false;
    initRoundtime = false;
    initCastTime = false;
    prompt = false;

    charName = "";

    mono = false;
    cmgr = false;

    pushStream = false;
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
            cache(localData);
        }
        msleep(25);
    }
}

bool XmlParserThread::isCmgr() {
    QMutexLocker ml(&mMutex);
    return cmgr;
}

QString XmlParserThread::fixInputXml(QString data) {
    data.replace(QRegularExpression("&(?!#?[a-z0-9]+;)"), "&amp;");

    // pushStream
    data.replace(QRegularExpression("<pushStream(.+?)\\/>"), "<!---->\r\n<pushStream\\1>");
    data.replace(QRegularExpression("<popStream[^>]*\\/>"), "</pushStream>\r\n<!---->");

    return data;
}

void XmlParserThread::flushStream() {
    pushStream = false;
    streamCache.clear();
}

/* cache streams */
void XmlParserThread::cache(QByteArray data) {
    QString cache = QString::fromLocal8Bit(data);

    int lastPush = cache.lastIndexOf("<pushStream");
    int lastPop = cache.lastIndexOf("<popStream");

    if(lastPush != -1) pushStream = true;
    if(lastPop > lastPush) pushStream = false;

    streamCache.append(cache);
    if(!pushStream) {
        this->process(streamCache);
        streamCache.clear();
    }
}

void XmlParserThread::process(QString data) {    
    QString str = fixInputXml(TextUtils::rstrip(data));

    QList<QString> lines = str.split("\r\n");

    for(int i = 0; i < lines.size(); i++) {
        if(lines.at(i).contains(QRegularExpression("^<output.*mono.*>"))) {
            mono = true;
        } else if(lines.at(i).contains(QRegularExpression("^<output.*>"))) {
            mono = false;
        }

        if(lines.at(i).startsWith("<pushStream")) {
            int count = 0;
            QString pushStream = lines.at(i) + "\n";
            while(i < lines.size() - 1) {
                count += lines.at(i).count("<pushStream") - lines.at(i).count("</pushStream>");
                if(count <= 0) break;
                pushStream += lines.at(++i) + "\n";
            }
            this->processPushStream(pushStream);
        } else if(lines.at(i).startsWith("<dynaStream")) {
            int count = 0;
            QString dynaStream = lines.at(i) + "\n";
            while(i < lines.size() - 1) {
                count += lines.at(i).count("<dynaStream") - lines.at(i).count("</dynaStream>");
                if(count <= 0) break;
                dynaStream += lines.at(++i) + "\n";
            }
            processDynaStream(dynaStream);
        } else {
            processGameData(lines.at(i));
        }
    }
}

QString XmlParserThread::processMonoOutput(QString line) {
    if(mono) {
        line.replace("preset id=\"thought\"", "preset id=\"penalty\"");
        line.replace("preset id=\"speech\"", "preset id=\"bonus\"");
        line.replace(QRegularExpression("<d cmd=\"(.*)\">(.*)</d>(.*)"), "\\2 \\3 <![CDATA[<span class=\"bold\">[\\1]</span>]]>");
        line.replace(QRegularExpression("(?<=>|^)(\\s{1,})(?=<)"), "<![CDATA[\\1]]>"); // preserve whitespaces in mono output
    }
    return line;
}

void XmlParserThread::processGameData(QString data) {
    data = processMonoOutput(data);

    QDomDocument doc("gameData");
    if(!doc.setContent(this->wrapRoot(data))) {                
        // never loged into stormfront; send default settings
        if(data.contains("space not found")) {
            emit writeDefaultSettings(stormfrontSettings);
            return;
        }
        TextUtils::plainToHtml(data);
        if(!doc.setContent(this->wrapRoot(this->wrapCdata(data)))) {
            this->warnInvalidXml("game-data-doc", data);
            return;
        }
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
    if(e.tagName() == "mode") {
        if(e.attribute("id") == "GAME") {
            cmgr = false;
            stormfrontSettings = toString(n.nextSiblingElement()).trimmed();
        } else if(e.attribute("id") == "CMGR") {
            cmgr = true;
            gameDataContainer->setRoomDesc("");
            emit updateRoomWindow();
        } else {
            cmgr = false;
        }
    } if(e.tagName() == "settingsInfo") {
        emit writeModeSettings();
        emit writeSettings();
    } if(e.tagName() == "app") {
        this->charName = e.attribute("char");
        emit setMainTitle(" - " + this->charName);
    } else if(e.tagName() == "style" && e.attribute("id") == "roomName") {
        QString roomName = root.text().trimmed();
        TextUtils::plainToHtml(roomName);
        gameText += "<span class=\"room-name\">" + roomName + "</span>";
        return false;
    /* All plain text without tags */
    } else if(n.isText()) {
        // compensate for qdomnode discarding &lt
        QString textData = n.toText().data();

        if(!mono) TextUtils::plainToHtml(textData);

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
    } else if(e.tagName() == "preset" && e.attribute("id") == "penalty") {
        gameText += tr("<span class=\"penalty\">%1</span>").arg(e.text());
    } else if(e.tagName() == "preset" && e.attribute("id") == "bonus") {
        gameText += tr("<span class=\"bonus\">%1</span>").arg(e.text());
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

                int t_to = time.msecsTo(roundTime);
                emit setTimer(t_to > 300000 ? 300000 : t_to);

                initRoundtime = false;
            }

            if(initCastTime) {
                time.setTime_t(e.attribute("time").toInt());

                int t_to = time.msecsTo(castTime);
                emit setCastTimer(t_to > 300000 ? 300000 : t_to);

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
        } else if(e.tagName() == "dialogData" && e.attribute("id") == "spellChoose") {
            QDomElement closeButton = e.firstChildElement("closeButton");
            gameText += closeButton.attribute("value") + ": [<span class=\"bold\">" + closeButton.attribute("cmd") + "</span>]";
        } else if(e.tagName() == "indicator") {
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
            //emit setMainTitle(" - " + this->charName + title);
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
                        } else {
                            gameDataContainer->setExpField(true, id, text);                            
                        }
                        emit updateExpWindow(id, text);
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
                scheduled.insert(e.attribute("id"), QStringList());
                this->activeSpells.clear();
            } else {
                if(!WindowFacade::staticWindows.contains(e.attribute("id"))) {
                    emit clearStreamWindow(e.attribute("id"));
                }
            }
        } else if (e.tagName() == "pushBold") {
            bold = true;
        } else if (e.tagName() == "popBold") {
            if(root.text() == "") gameText += "&nbsp;";
            bold = false;
        } else if (e.tagName() == "a") {
            gameText += "<a href=\"" + e.attribute("href") + "\">" + e.text() + "</a>";
        } else if (e.tagName() == "streamWindow") {
            if(!WindowFacade::staticWindows.contains(e.attribute("id"))) {
                emit registerStreamWindow(e.attribute("id"), e.attribute("title"));
            }
        }
    }
    return gameText == "";
}

QString XmlParserThread::fixUnclosedStreamTags(QString data) {
    int tagCount = data.count("<pushStream") - data.count("</pushStream>");
    int index = data.indexOf("</pushStream>");
    for(int i = 0; i < tagCount; i++) {
        data.insert(index, "</pushStream>");
    }
    return data;
}

void XmlParserThread::processPushStream(QString data) {
    data = this->wrapRoot(data);

    QDomDocument doc("pushStream");
    if(!doc.setContent(data)) {
        if(!doc.setContent(this->fixUnclosedStreamTags(data))) {
            this->warnInvalidXml("push-stream-doc", data);
            return;
        }
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
        QString ass = root.text().trimmed();
        if(!ass.isEmpty()) this->writeTextLines(ass);
    } else if(e.attribute("id") == "thoughts") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            QString elementText = element.nextSibling().toText().data().trimmed();
            TextUtils::plainToHtml(elementText);
            QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
            emit updateThoughtsWindow(addTime(text));
        } else {            
            emit updateThoughtsWindow(addTime(e.text().trimmed()));
        }
    } else if(e.attribute("id") == "death") {
        emit updateDeathsWindow(addTime(root.text().trimmed()));
    } else if(e.attribute("id") == "atmospherics") {
        QString atmo = root.text().trimmed();
        if(!atmo.isEmpty()) this->writeTextLines(atmo);
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
                QString elementText = first.nextSibling().toText().data();
                TextUtils::plainToHtml(elementText);
                QString text = tr("%1%2").arg(this->parseTalk(first), elementText);
                emit updateFamiliarWindow(text);
            } else {
                this->warnUnknownEntity("familiar-talk", data);
            }
        } else if(id == "speech" || id == "whisper") {
            QString elementText = element.nextSibling().toText().data();
            TextUtils::plainToHtml(elementText);
            QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
            emit updateFamiliarWindow(text);
        } else if (id == "familiar") {
            QDomElement element = e.firstChild().toElement();
            emit updateFamiliarWindow(element.text().replace("\r\n", "\n"));
        } else {
            QDomElement next = e.firstChild().nextSibling().toElement();
            if(next.tagName() == "pushStream") {
                if(next.attribute("id") == "talk") {
                    QDomElement first = next.firstChild().toElement();
                    if(first.attribute("id") == "speech" || first.attribute("id") == "whisper" || first.tagName() == "b") {
                        QString elementText = first.nextSibling().toText().data();
                        TextUtils::plainToHtml(elementText);
                        QString text = tr("%1%2").arg(this->parseTalk(first), elementText);
                        emit updateFamiliarWindow(text);
                    } else {
                        this->warnUnknownEntity("familiar-talk", data);
                    }
                } else {
                    emit updateFamiliarWindow(e.text());
                }
            } else {
                emit updateFamiliarWindow(e.text());
            }
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
            this->writeTextLines(ooc);
        }
    } else if(e.attribute("id") == "percWindow") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "b") {
            activeSpells += toString(element);
        } else {
            activeSpells += root.text();
        }
        QStringList list = activeSpells.split("\n", QString::SkipEmptyParts);        
        scheduled.insert(e.attribute("id"), list);
    } else if(e.attribute("id") == "group") {
        // ignored
    } else if(e.attribute("id") == "chatter") {
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            if(element.attribute("id") == "thought") {
                QString elementText = element.nextSibling().toText().data();
                QString trimTrailing = elementText.remove(QRegularExpression("\n$"));
                TextUtils::plainToHtml(trimTrailing);
                QString text = tr("%1%2").arg(this->parseTalk(element), elementText);
                emit updateThoughtsWindow(addTime(text));
            } else {
                this->warnUnknownEntity("chatter-preset", data);
            }
        } else {
            this->warnUnknownEntity("chatter", data);
        }
    } else if(e.tagName() == "pushStream") {
        emit writeStreamWindow(e.attribute("id"), e.text());
    } else {
        this->warnUnknownEntity("push-stream", data);
    }
}

void XmlParserThread::processDynaStream(QString data) {    
    data = this->processMonoOutput(data);
    data = this->wrapRoot(data);

    QDomDocument doc("dynaStream");
    if(!doc.setContent(data)) {
        this->warnInvalidXml("dyna-stream-doc", data);
        return;
    }

    QDomElement root = doc.documentElement();
    QDomNode n = root.firstChild();

    QDomElement e = n.toElement();
    if(e.attribute("id") == "spellInfo") {
        this->writeTextLines(e.text());
    } else if(e.attribute("id") == "spells") {
        this->writeTextLines(root.text());
    } else {
        this->warnUnknownEntity("dynaStream", data);
    }
}

void XmlParserThread::warnUnknownEntity(QString ref, QString xml) {
    qDebug() << tr("unknown xml event (%1): %2").arg(ref, xml);
}

void XmlParserThread::warnInvalidXml(QString ref, QString xml) {
    qDebug() << tr("invalid xml (%1): %2").arg(ref, xml);
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
        gameDataContainer->setActiveSpells(data.toStringList());
        emit updateActiveSpells(data.toStringList());
        emit updateSpellWindow(data.toStringList().join("\n"));        
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

void XmlParserThread::writeTextLines(QString text) {
    for(QString line: text.split('\n')) {
        emit writeText(line.toLocal8Bit(), false);
    }
}

QString XmlParserThread::addTime(QString data) {
    return data + " <span class=\"body\">[" + QTime::currentTime().toString("h:mm ap") + "]</span>";
}

QString XmlParserThread::wrapRoot(QString data) {
    return "<root>" + data + "</root>";
}

QString XmlParserThread::wrapCdata(QString data) {
    return "<![CDATA[" + data + "]]>";
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
