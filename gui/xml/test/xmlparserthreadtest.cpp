#include <QtTest/QtTest>
#include <map>
#include "xml/xmlparserthread.h"
#include "hyperlinkutils.h"

class GameTextCollector : public QObject {
    Q_OBJECT
public:
    GameTextCollector(XmlParserThread* xmlParser) : QObject(xmlParser) {
        connect(xmlParser, SIGNAL(writeText(QByteArray, bool)), this,
                SLOT(writeGameText(QByteArray, bool)));
    }
    QString text;
    bool prompt;
public slots:
    void writeGameText(QByteArray text, bool prompt) {
        this->text = text;
        this->prompt = prompt;
    }
};

class XmlParserThreadTest : public QObject {
    Q_OBJECT
public:
private slots:

    void pushBoldTestCase() {
        XmlParserThread* xmlParser = new XmlParserThread(this, NULL);
        GameTextCollector* textCollector = new GameTextCollector(xmlParser);

        static const QString input =
                "<pushBold/>******************************************************\
*** Please check <d>NEWS NEXT</d> for unread <d>NEWS</d> items. ***\
******************************************************\
<popBold/><prompt time=\"1665949176\">s&gt;</prompt>\
";
        static const QString expect =
                "<span class=\"bold\">********************************************************* "
                "Please check </span><a href=\"f://a/TkVXUyBORVhU\">NEWS NEXT</a><span "
                "class=\"bold\"> for unread </span><a href=\"f://a/TkVXUw==\">NEWS</a><span "
                "class=\"bold\"> items. "
                "*********************************************************</"
                "span>********************************************************* Please check NEWS "
                "NEXT for unread NEWS items. "
                "*********************************************************s>";
        xmlParser->onProcess(input.toLocal8Bit());
        QCOMPARE(textCollector->text, expect);

        delete xmlParser;
    }

    void d_cmd_TestCase() {
        XmlParserThread* xmlParser = new XmlParserThread(this, NULL);
        GameTextCollector* textCollector = new GameTextCollector(xmlParser);

        // a list of pairs: input data, expected result
        // clang-format off
        static std::pair<QString, QString> testData [] = {
            {"  <d cmd='inventory'>INV</d>               - Shows a description of all of the items you are currently wearing",
             "<a href=\"f://a/aW52ZW50b3J5\">INV</a>               - Shows a description of all of the items you are currently wearing"},
            {"<d cmd='inv slots'>INV SLOTS</d>         - Gives a summary of your inventory slots, showing amount worn in each.",
             "<a href=\"f://a/aW52IHNsb3Rz\">INV SLOTS</a>         - Gives a summary of your inventory slots, showing amount worn in each."},
            {" INV [<d cmd='inv armor'>ARMOR</d> | <d cmd='inv weapon'>WEAPON</d> | <d cmd='inv fluff'>FLUFF</d> | <d cmd='inv container'>CONTAINER</d> | <d cmd='inv combat'>COMBAT</d>] - Shows you the items of this type that you're wearing.",
             "INV [<a href=\"f://a/aW52IGFybW9y\">ARMOR</a> | <a href=\"f://a/aW52IHdlYXBvbg==\">WEAPON</a> | <a href=\"f://a/aW52IGZsdWZm\">FLUFF</a> | <a href=\"f://a/aW52IGNvbnRhaW5lcg==\">CONTAINER</a> | <a href=\"f://a/aW52IGNvbWJhdA==\">COMBAT</a>] - Shows you the items of this type that you're wearing."}
        };
        // clang-format on
        for (size_t i = 0; i < sizeof(testData) / sizeof(testData[0]); ++i) {
            xmlParser->onProcess(testData[i].first.toLocal8Bit());
            qDebug() << textCollector->text;
            QCOMPARE(textCollector->text, testData[i].second);
        }
        delete xmlParser;
    }
};

QTEST_MAIN(XmlParserThreadTest)
#include "xmlparserthreadtest.moc"
