#include "xmlparserthread.h"

#include <QXmlStreamReader>

#include "gamedatacontainer.h"
#include "textutils.h"
#include "hyperlinkutils.h"

XmlParserThread::XmlParserThread(QObject *parent, GameDataContainer* dataContainer) : Parent(parent), gameDataContainer(dataContainer) {
    rxDmg.setPattern("\\bat you\\..*\\blands\\b");
    
    bold = false;
    initRoundtime = false;
    initCastTime = false;
    prompt = false;

    charName = "";

    mono = false;

    pushStream = false;
}

void XmlParserThread::addData(QByteArray buffer) {
    Parent::addData(buffer);
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
void XmlParserThread::onProcess(const QByteArray& data) {
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
            processPushStream(aggregateXml("pushStream", lines, i));
        } else if(lines.at(i).startsWith("<dynaStream")) {
            processDynaStream(aggregateXml("dynaStream", lines, i));
        } else if(lines.at(i).startsWith("<component")) {
            processGameData(aggregateXml("component", lines, i));
        } else {
            processGameData(lines.at(i));
        }
    }
}

QString XmlParserThread::aggregateXml(QString tag, QList<QString> lines, int &i) {
    int count = 0;
    QString xml = lines.at(i) + "\n";
    while(i < lines.size() - 1) {
        count += lines.at(i).count("<" + tag) - lines.at(i).count("</" + tag + ">");
        if(count <= 0) break;
        xml += lines.at(++i) + "\n";
    }       
    return xml;
}

QString XmlParserThread::processMonoOutput(QString line) {
    if(mono) {
        line.replace("preset id=\"thought\"", "preset id=\"penalty\"");
        line.replace("preset id=\"speech\"", "preset id=\"bonus\"");        
        line.replace(QRegularExpression("(?<=>|^)(\\s{1,})(?=<)"), "<![CDATA[\\1]]>"); // preserve whitespaces in mono output
    }
    return line;
}

void XmlParserThread::processGameData(QString data) {
    data = processMonoOutput(data);
    data = fixCmdUnescapedTags(data);

    QDomDocument doc("gameData");
    if(!doc.setContent(this->wrapRoot(data))) {                
        // never logged into stormfront; send default settings
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
            emit gameModeIsCmgr(false);
            stormfrontSettings = toString(n.nextSiblingElement()).trimmed();
        } else if(e.attribute("id") == "CMGR") {
            emit gameModeIsCmgr(true);
            gameDataContainer->setRoomDesc("");
            emit updateRoomWindow();
        } else {
            emit gameModeIsCmgr(false);
        }
    } if(e.tagName() == "settingsInfo") {
        emit writeModeSettings();
        emit writeSettings();
    } if(e.tagName() == "app") {
        this->charName = e.attribute("char");
        gameDataContainer->setCharName(charName);
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
            gameText += "<span class=\"bold\">" + textData + "</span>";
        } else {
            gameText += textData;
        }
    /* Process game text between tags */
    } else if(e.tagName() == "d") {
        QString d = e.text().trimmed();
        QString cmd  = e.attribute("cmd", d);
        TextUtils::plainToHtml(d);
        HyperlinkUtils::createLink(d, cmd, 0, d);
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

            gameDataContainer->setCompassDirections(directions);

            QString text = gameDataContainer->getRoomName() +
                    TextUtils::stripMapSpecial(gameDataContainer->getRoomDesc())
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
        } else if(e.tagName() == "dialogData" && e.attribute("id") == "spellChoose") {
            QDomElement closeButton = e.firstChildElement("closeButton");
            gameText += closeButton.attribute("value") + ": [<span class=\"bold\">" + closeButton.attribute("cmd") + "</span>]";
        } else if(e.tagName() == "indicator") {
            /* filter player status indicator */
            //<indicator id="IconKNEELING" visible="n"/><indicator id="IconPRONE" visible="n"/>
            emit updateStatus(e.attribute("visible"), e.attribute("id"));
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
        } else if(e.tagName() == "streamWindow") {
            if(e.attribute("id") == "main") {
                /* filter main window title */
                QString title = e.attribute("subtitle");
                gameDataContainer->setRoomName(title.mid(3));
                emit setMainTitle(" (" + this->charName + ")" + title);
                //emit updateRoomWindowTitle(title);
            } else {
                emit registerStreamWindow(e.attribute("id"), e.attribute("title"));
            }
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
                    QString text = this->traverseXmlNode(e, QString("")).trimmed();
                    gameDataContainer->setRoomObjsData(text);
                    gameDataContainer->setRoomMonstersBold(text);

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
            } else if(e.attribute("id") == "group") {
                scheduled.insert(e.attribute("id"), QStringList());
                this->group.clear();
            } else if(e.attribute("id") == "experience") {
                foreach(QString key, gameDataContainer->getExp().keys()) {
                    emit updateExpWindow(key, "");
                }
            } else {
                emit clearStreamWindow(e.attribute("id"));
            }
        } else if (e.tagName() == "pushBold") {
            bold = true;
        } else if (e.tagName() == "popBold") {
            if(root.text() == "") gameText += "&nbsp;";
            bold = false;
        } else if (e.tagName() == "a") {
            gameText += "<a href=\"" + e.attribute("href") + "\">" + e.text() + "</a>";
        }
    }
    return gameText == "";
}

/*
 * in: <d cmd='urchin guide Leth Deriel, Sana'ati Dyaus Drui'tas'>Leth Deriel, Sana'ati Dyaus Drui'tas</d>
 * out: <d cmd='urchin guide Leth Deriel, Sana&apos;ati Dyaus Drui&apos;tas'>Leth Deriel, Sana'ati Dyaus Drui'tas</d>
 */
QString XmlParserThread::fixCmdUnescapedTags(QString text) {
    if (text.contains("<d cmd='")) {
        QStringList cmds = text.split("</d>");
        for(int i = 0; i < cmds.length(); i++) {
            QRegularExpression rx("<d cmd='(.*)'>");
            QRegularExpressionMatchIterator matchIterator = rx.globalMatch(cmds[i]);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                if (match.hasMatch()) {
                    int start = match.capturedStart(1);
                    int end = match.capturedEnd(1);
                    cmds[i].remove(start, end - start);
                    QString captured = match.captured(1);
                    TextUtils::escapeSingleQuotes(captured);
                    cmds[i].insert(start, captured);
                }
            }
        }
        text = cmds.join("</d>");
    }
    return text;
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
        QString text = this->traverseXmlNode(e, QString("")).trimmed();
        if(!text.isEmpty()) {
            QDomElement element = e.firstChild().toElement();
            if(element.attribute("id") == "thought") {
                emit updateThoughtsWindow(addTime(text));
            } else {
                emit updateConversationsWindow(addTime(text));
            }
        }
    } else if(e.attribute("id") == "logons") {
        emit updateArrivalsWindow(addTime(root.text().trimmed()));
    } else if(e.attribute("id") == "inv") {
        gameDataContainer->setInventory(root.text().split("\n"));
    } else if(e.attribute("id") == "room") {
        // ignored
        // <compDef id='room desc'/> ..
    } else if(e.attribute("id") == "combat") {
        QString text = this->traverseXmlNode(e, QString("")).trimmed();
        if(text.contains(rxDmg)) text.replace("class=\"bold\"", "class=\"damage\"");
        emit updateCombatWindow(text);
    } else if(e.attribute("id") == "assess") {
        QString ass = root.text().trimmed();
        if(!ass.isEmpty()) this->writeTextLines(ass);
    } else if(e.attribute("id") == "thoughts") {                        
        QString text = this->traverseXmlNode(e, QString("")).trimmed();
        emit updateThoughtsWindow(addTime(text));
    } else if(e.attribute("id") == "death") {
        emit updateDeathsWindow(addTime(root.text().trimmed()));
    } else if(e.attribute("id") == "atmospherics") {
        QString atmo = root.text().trimmed();
        if(!atmo.isEmpty()) emit updateAtmosphericsWindow(atmo);
    } else if(e.attribute("id") == "whispers") {
        emit updateConversationsWindow(addTime(this->traverseXmlNode(e, QString("")).trimmed()));
    } else if(e.attribute("id") == "familiar") {
        QDomElement next = e.firstChild().nextSibling().toElement();
        if(next.tagName() == "pushStream") {
            emit updateFamiliarWindow(this->traverseXmlNode(next, QString("")));
        } else {
            emit updateFamiliarWindow(this->traverseXmlNode(e, QString("")));
        }
    } else if(e.attribute("id") == "ooc") {
        QString text = this->traverseXmlNode(e, QString("")).trimmed();
        QDomElement element = e.firstChild().toElement();
        if(element.tagName() == "preset") {
            // ignore speech in ooc stream; duplicated from whisper stream
            // emit updateConversationsWindow(addTime(text));
        } else {            
            this->writeTextLines(text);
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
    } else if(e.attribute("id") == "shopWindow") {
        this->writeTextLines(toString(e));
    } else if(e.attribute("id") == "group") {
        group += root.text();
        QStringList list = group.split("\n", QString::SkipEmptyParts);
        scheduled.insert(e.attribute("id"), list);
    } else if(e.attribute("id") == "chatter") {
        QString text = this->traverseXmlNode(e, QString("")).trimmed();
        emit updateThoughtsWindow(addTime(text));
    } else if(e.tagName() == "pushStream") {
        emit writeStreamWindow(e.attribute("id"), e.text());
    } else {
        this->warnUnknownEntity("push-stream", data);
    }
}

QString XmlParserThread::traverseXmlNode(QDomElement element, QString text) {
    for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
        //qDebug() << node.nodeName();
        if(node.isText()) {
            QDomText t = node.toText();
            if (!t.isNull()) {
                QString plain = t.data();
                TextUtils::plainToHtml(plain);
                text += plain;
            }
        } else if (node.isElement()) {
            QDomElement el = node.toElement();
            if(el.tagName() == "style") {
                if(el.attribute("id") == "roomName") {
                    text += "<span class=\"room-name\">";
                } else if (el.attribute("id") == "") {
                    text += "</span>";
                }
            } else if(el.tagName() == "preset") {
                if(el.attribute("id") == "speech") {
                    text += "<span class=\"speech\">";
                } else if(el.attribute("id") == "whisper") {
                    text += "<span class=\"whisper\">";
                } else if(el.attribute("id") == "thought") {
                    text += "<span class=\"thinking\">";
                }
                text = this->traverseXmlNode(node.toElement(), text);
                text += "</span>";
            } else if(el.tagName() == "b") {
                text += "<span class=\"speech\">";
                text = this->traverseXmlNode(node.toElement(), text);
                text += "</span>";
            } else if(el.tagName() == "pushBold") {
                text += "<span class=\"bold\">";
            } else if(el.tagName() == "popBold") {
                text += "</span>";
            } else {
                text = this->traverseXmlNode(node.toElement(), text);
            }
        }
    }
    return text;
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
    } else if(event == "group") {
        emit updateGroupWindow(data.toStringList().join("\n") + "\n");
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
