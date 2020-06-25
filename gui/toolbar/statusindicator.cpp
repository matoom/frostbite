#include "statusindicator.h"

StatusIndicator::StatusIndicator(QObject *parent) : QObject(parent) {
    gameDataContainer = GameDataContainer::Instance();
}

QLabel *StatusIndicator::playerStatusLabel(const char* oName, const char* img, bool show) {
    QImage statusImage(img);

    QLabel *statusLabel = new QLabel();
    statusLabel->setObjectName(oName);

    if(show) {
        statusLabel->setPixmap(QPixmap::fromImage(statusImage));
    }

    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFixedWidth(32);

    statusLabel->setStyleSheet("QLabel {"
                               "border: 1px solid rgb(190, 190, 190);"
                               "background: #383533;"
                               "border-radius: 5px; }"
                               "QToolTip {"
                               "color: #F8F8F8;"
                               "font: 11pt \"" TOOLBAR_FONT "\";"
                               "background-color: #383533;"
                               "border: 2px outset #2a82da;"
                               "padding: 2px; "
                               "}");

    return statusLabel;
}

QHash<QString, bool> StatusIndicator::getFullStatus() {
    return fullStatus;
}

QWidget* StatusIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(25, 10, 25, 10);

    invisible = this->playerStatusLabel("invisible", INVISIBLE_ICO, false);
    hLayout->addWidget(invisible);

    immobile = this->playerStatusLabel("immobile", IMMOBILE_ICO, false);
    hLayout->addWidget(immobile);

    joined = this->playerStatusLabel("joined", GROUP_ICO, false);
    hLayout->addWidget(joined);

    hidden = this->playerStatusLabel("hidden", HIDDEN_ICO, false);
    hLayout->addWidget(hidden);

    condition = this->playerStatusLabel("condition", STUNNED_ICO, false);
    hLayout->addWidget(condition);

    posture = this->playerStatusLabel("posture", STANDING_ICO, false);
    hLayout->addWidget(posture);

    widget->setLayout(hLayout);

    return widget;
}

void StatusIndicator::updateStatus(QString visibleString, QString icon) {
    bool visible = visibleToBool(visibleString);

    if(icon == "IconKNEELING") {
        fullStatus.insert("kneeling", visible);
        this->setPosture(visible, icon);
        gameDataContainer->setKneeling(visible);
    } else if(icon == "IconPRONE") {
        fullStatus.insert("prone", visible);
        this->setPosture(visible, icon);
        gameDataContainer->setProne(visible);
    } else if(icon == "IconSITTING") {
        fullStatus.insert("sitting", visible);
        this->setPosture(visible, icon);
        gameDataContainer->setSitting(visible);
    } else if(icon == "IconSTANDING") {
        fullStatus.insert("standing", visible);
        this->setPosture(visible, icon);
        gameDataContainer->setStanding(visible);
    } else if(icon == "IconSTUNNED") {
        fullStatus.insert("stunned", visible);
        this->setCondition(visible, icon);
        gameDataContainer->setStunned(visible);
    } else if(icon == "IconDEAD") {
        fullStatus.insert("dead", visible);
        this->setCondition(visible, icon);
        gameDataContainer->setDead(visible);
    }else if(icon == "IconBLEEDING") {
        fullStatus.insert("bleeding", visible);
        this->setCondition(visible, icon);
        gameDataContainer->setBleeding(visible);
    } else if(icon == "IconHIDDEN") {
        fullStatus.insert("hidden", visible);
        this->setHidden(visible);
        gameDataContainer->setHidden(visible);
    } else if(icon == "IconINVISIBLE") {
        fullStatus.insert("invisible", visible);
        this->setInvisible(visible);
        gameDataContainer->setInvisible(visible);
    } else if(icon == "IconWEBBED") {
        fullStatus.insert("webbed", visible);
        this->setImmobile(visible);        
        gameDataContainer->setWebbed(visible);
    } else if(icon == "IconJOINED") {
        fullStatus.insert("joined", visible);
        this->setJoined(visible);
        gameDataContainer->setJoined(visible);
    }
}

bool StatusIndicator::visibleToBool(QString visible) {
    if(visible == "y") {
        return true;
    } else {
        return false;
    }
}

void StatusIndicator::setInvisible(bool visible) {
    if(visible) {
        invisible->setPixmap(QPixmap(INVISIBLE_ICO));
        invisible->setToolTip(tr("Invisible"));
    } else {
        invisible->setToolTip("");
        invisible->setPixmap(QPixmap());
    }
}

void StatusIndicator::setImmobile(bool visible) {
    if(visible) {
        immobile->setPixmap(QPixmap(IMMOBILE_ICO));
        immobile->setToolTip(tr("Immobile"));
    } else {
        immobile->setToolTip("");
        immobile->setPixmap(QPixmap());
    }
}

void StatusIndicator::setJoined(bool visible) {
    if(visible) {
        joined->setPixmap(QPixmap(GROUP_ICO));
        joined->setToolTip(tr("Grouped"));
    } else {
        joined->setToolTip("");
        joined->setPixmap(QPixmap());
    }
}

void StatusIndicator::setHidden(bool visible) {
    if(visible) {
        hidden->setPixmap(QPixmap(HIDDEN_ICO));
        hidden->setToolTip(tr("Hidden"));
    } else {
        hidden->setToolTip("");
        hidden->setPixmap(QPixmap());
    }
}

void StatusIndicator::setCondition(bool visible, QString icon) {
    conditionState.insert(icon, visible);

    if(conditionState.value("IconDEAD")) {
        updateCondition("Dead", DEAD_ICO);
    } else if(conditionState.value("IconSTUNNED")) {
        updateCondition("Stunned", STUNNED_ICO);
    } else if(conditionState.value("IconBLEEDING")) {
        updateCondition("Bleeding", BLEEDING_ICO);
    } else {
        updateCondition("", NULL);
    }
}

void StatusIndicator::updateCondition(const char* title, QString icon) {
    if(icon.isNull()) {
        condition->setPixmap(QPixmap());
    } else {
        condition->setPixmap(QPixmap(icon));
    }

    condition->setToolTip(tr(title));
}

void StatusIndicator::setPosture(bool visible, QString icon) {
    if(visible) {
        if(icon == "IconSTANDING") {
            updatePosture("Standing", STANDING_ICO);
        } else if(icon == "IconKNEELING") {
            updatePosture("Kneeling", KNEELING_ICO);
        } else if(icon == "IconSITTING") {
            updatePosture("Sitting",SITTING_ICO);
        } else if(icon == "IconPRONE") {
            updatePosture("Prone", PRONE_ICO);
        }
    }
}

void StatusIndicator::updatePosture(const char* title, QString icon) {
    posture->setToolTip(tr(title));
    posture->setPixmap(QPixmap(icon));
}

StatusIndicator::~StatusIndicator() {
    delete invisible;
    delete immobile;
    delete joined;
    delete hidden;
    delete condition;
    delete posture;
}
