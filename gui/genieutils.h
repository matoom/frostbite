#ifndef GENIEUTILS_H
#define GENIEUTILS_H

#include <QObject>
#include <QColor>

class MainWindow;

class GenieUtils : public QObject {
    Q_OBJECT

public:
    explicit GenieUtils(QObject *parent = 0);

    void importHighlights(QString imports);
    bool importFile(QString fileName);

private:
    struct GenieHighlight {
        QString type;
        QColor color;
        QString regexp;
        QString category;
        QString sound;

        void fieldFromPosition(int pos, const QString &text) {
            switch (pos) {
            case 0:
                type = text;
                break;
            case 1:
                color.setNamedColor(text);
                break;
            case 2:
                regexp = text;
                break;
            case 3:
                category = text;
                break;
            case 4:
                sound = text;
                break;
            default:
                break;
            }
        }
    };
    
    static GenieHighlight parseHighlight(const QString &line);
    void addRegexHighlight(const GenieHighlight& highlight);
    void commitSettings();

    MainWindow *mainWindow;

signals:
    void reloadSettings();
};

#endif // GENIEUTILS_H
