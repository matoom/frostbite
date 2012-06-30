#include "statusindicator.h"

StatusIndicator::StatusIndicator(QObject *parent) : QObject(parent) {
    stateDead = false;
    stateStunned = false;
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
    statusLabel->setStyleSheet("border: 1px solid rgb(190, 190, 190);"
                               "margin-left: -1px;"
                               "margin-right: -1px;"
                               "margin-top: -1;");

    return statusLabel;
}

QWidget *StatusIndicator::create() {
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

void StatusIndicator::updateStatus(QString visible, QString icon) {
    if(icon == "IconKNEELING") {
        this->setPosture(visibleToBool(visible), icon);
    } else if(icon == "IconPRONE") {
        this->setPosture(visibleToBool(visible), icon);
    } else if(icon == "IconSITTING") {
        this->setPosture(visibleToBool(visible), icon);
    } else if(icon == "IconSTANDING") {
        this->setPosture(visibleToBool(visible), icon);
    } else if(icon == "IconSTUNNED") {
        this->setCondition(visibleToBool(visible), icon);
    } else if(icon == "IconDEAD") {
        this->setCondition(visibleToBool(visible), icon);
    } else if(icon == "IconHIDDEN") {
        this->setHidden(visibleToBool(visible));
    } else if(icon == "IconINVISIBLE") {
        this->setInvisible(visibleToBool(visible));
    } else if(icon == "IconWEBBED") {
        this->setImmobile(visibleToBool(visible));
    } else if(icon == "IconJOINED") {
        this->setJoined(visibleToBool(visible));
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
        invisible->setPixmap(NULL);
    }
}

void StatusIndicator::setImmobile(bool visible) {
    if(visible) {
        immobile->setPixmap(QPixmap(IMMOBILE_ICO));
        immobile->setToolTip(tr("Immobile"));
    } else {
        immobile->setToolTip("");
        immobile->setPixmap(NULL);
    }
}

void StatusIndicator::setJoined(bool visible) {
    if(visible) {
        joined->setPixmap(QPixmap(GROUP_ICO));
        joined->setToolTip(tr("Grouped"));
    } else {
        joined->setToolTip("");
        joined->setPixmap(NULL);
    }
}

void StatusIndicator::setHidden(bool visible) {
    if(visible) {
        hidden->setPixmap(QPixmap(HIDDEN_ICO));
        hidden->setToolTip(tr("Hidden"));
    } else {
        hidden->setToolTip("");
        hidden->setPixmap(NULL);
    }
}

void StatusIndicator::setCondition(bool visible, QString icon) {
    if(visible) {
        if(icon == "IconSTUNNED") {
            stateStunned = true;
        } else if(icon == "IconDEAD") {
            stateDead = true;
        }
    } else if(!visible) {
        if(icon == "IconSTUNNED") {
            stateStunned = false;
        } else if (icon == "IconDEAD") {
            stateDead = false;
        }
    }

    if (stateStunned && stateDead) {
        updateCondition("Dead", DEAD_ICO);
    } else if (stateStunned) {
        updateCondition("Stunned", STUNNED_ICO);
    } else if(stateDead) {
        updateCondition("Dead", DEAD_ICO);
    }  else {
        updateCondition("", NULL);
    }
}

void StatusIndicator::updateCondition(const char* title, QString icon) {
    if(icon.isNull()) {
        condition->setPixmap(NULL);
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
