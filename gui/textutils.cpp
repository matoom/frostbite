#include "textutils.h"

TextUtils* TextUtils::m_pInstance = NULL;

TextUtils* TextUtils::Instance() {
    if (!m_pInstance) {
        m_pInstance = new TextUtils;
    }

    return m_pInstance;
}

TextUtils::TextUtils(QObject *parent) : QObject(parent) {
    this->populateExpStates();

    rxNumber.setPattern("(\\d+)");
    rxRemoveTags.setPattern("<[^>]*>");
}

void TextUtils::populateExpStates() {
    mindStates << "clear" << "dabbling" << "perusing" << "learning" << "thoughtful"
        << "thinking" << "considering" << "pondering" << "ruminating" << "concentrating"
        << "attentive" << "deliberative" << "interested" << "examining" << "understanding" << "absorbing"
        << "intrigued" << "scrutinizing" << "analyzing" << "studious" << "focused" << "very focused"
        << "engaged" << "very engaged" << "cogitating" << "fascinated" << "captivated" << "engrossed" << "riveted"
        << "very riveted" << "rapt" << "very rapt" << "enthralled" << "nearly locked" << "mind lock";
}

QString TextUtils::addNumericStateToExp(QString exp) {
    int index = 0;
    for(int i = mindStates.length() - 1; i >= 0; i--) {
        index = exp.indexOf(mindStates.at(i));
        if(index != -1) {
            exp.insert(index, QString("%1 ").arg(i, 2, 10));
            break;
        }
    }
    return exp;
}

int TextUtils::expStateToNumeric(QString state) {
    return mindStates.indexOf(state);
}

int TextUtils::expBriefToNumeric(QString state) {
    rxNumber.indexIn(state, 0);
    return rxNumber.cap(1).toInt();
}

QString TextUtils::expNumericToState(int index) {
    return mindStates.at(index);
}

QString TextUtils::msToMMSS(int ms) {
    int ss = ms / 1000;
    int mm = ss / 60;
        ss = ss % 60;

    return QString().sprintf("%02d:%02d", mm, ss);
}

QString TextUtils::findLowestActiveValue(QStringList list) {
    int minVal = std::numeric_limits<int>::max();
    foreach(QString item, list) {
        rxNumber.indexIn(item, 0);
        int value = rxNumber.cap(1).toInt();

        if(value < minVal && value != 0) {
            minVal = value;
        }
    }
    if(minVal == std::numeric_limits<int>::max()) return "-";

    return QString::number(minVal);
}

void TextUtils::htmlToPlain(QString& data) {
    data.remove(rxRemoveTags);
    data.replace("&amp;", "&").replace("&quot;", "\"")
            .replace("&apos;", "\'").replace("&lt;", "<").replace("&gt;", ">");
}

void TextUtils::plainToHtml(QString& data) {
    data.replace("\"", "&quot;").replace("\'", "&apos;")
            .replace("<", "&lt;").replace(">", "&gt;");
}

QString TextUtils::toHash(QString text) {
    return QString(QCryptographicHash::hash(text.toLocal8Bit(), QCryptographicHash::Md5).toHex());
}

QString TextUtils::toBrief(QString direction) {
    if(direction == "north") {
        return "n";
    } else if (direction == "south"){
        return "s";
    } else if (direction == "east") {
        return "e";
    } else if (direction == "west") {
        return "w";
    } else if (direction == "northwest") {
        return "nw";
    } else if (direction == "southwest") {
        return "sw";
    } else if (direction == "northeast") {
        return "ne";
    } else if (direction == "southeast") {
        return "se";
    } else if (direction == "out") {
        return "out";
    } else if (direction == "down") {
        return "down";
    } else if (direction == "up") {
        return "up";
    }
    return "";
}

