#include "wordcompleter.h"

WordCompleter::WordCompleter(QObject *parent) : QObject(parent) {
    words << "arrange" << "attack" << "advance" << "accept" << "ambush" << "aim"
          << "appraise" << "ask" << "awaken"
          << "block" << "braid"  << "break" << "bundle" << "buy"
          << "cast" << "carve" << "chant" << "charge" << "clean" << "collect" << "close"
          << "combine" << "compare" << "concentrate" << "contact" << "crush" << "cut" << "climb"
          << "decline" << "depart" << "deposit" << "disarm" << "disband" << "dismantle"
          << "dodge" << "drag" << "drop" << "dump"
          << "exit"
          << "face" << "fire" << "fix" << "fletch" << "focus" << "forage"
          << "get" << "guard"
          << "harness" << "health" << "hide" << "hold"
          << "info" << "inventory" << "invoke"
          << "join"
          << "khri"
          << "load" << "light" << "listen"
          << "make" << "mana" << "mark"
          << "observe" << "open" << "offer"
          << "point" << "parry" << "perceive" << "pick" << "play" << "poach" << "practice"
          << "pull" << "put"
          << "quit"
          << "retreat" << "read" << "recall" << "release" << "remove" << "roar" << "rummage"
          << "say" << "sign" << "stalk" << "stance" << "scrape" << "search" << "shiver"
          << "show" << "signal" << "skin" << "skills" << "slip" << "sneak" << "spells"
          << "steal" << "stomp" << "stow" << "study" << "swap"
          << "think" << "target" << "teach" << "tell" << "thump" << "touch"
          << "unhide" << "unload" << "unlock"
          << "whisper" << "watch" << "wealth";
}

void WordCompleter::findMatches(QStringList commandList) {
    if(match) {
        matchesFound = words.filter(QRegExp("^" + commandList.last()));
        commandCount = commandList.size();
        order = 0;
        match = false;
    }
}

QString WordCompleter::complete(QString commands) {
    QStringList commandList = commands.split(" ");

    this->findMatches(commandList);

    if(matchesFound.size() > 0) {
        commandList.removeLast();
        commandList.append(matchesFound.at(order));

        order = order >= matchesFound.size() - 1 ? 0 : order + 1;

        return commandList.join(" ");
    } else {
        return commands;
    }
}

