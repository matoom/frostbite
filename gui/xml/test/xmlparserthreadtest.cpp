#include "xml/xmlparserthread.h"
#include <QtTest/QtTest>

class XmlParserThreadTest : public QObject {
    Q_OBJECT
private slots:

    void toUpper() {
        XmlParserThread* xmlparser = new XmlParserThread(this, NULL);
        
        connect(xmlparser, SIGNAL(writeText(QByteArray, bool)), this,
                SLOT(writeGameText(QByteArray, bool)));
        
        xmlparser->onProcess("<pushBold/>******************************************************\
*** Please check <d>NEWS NEXT</d> for unread <d>NEWS</d> items. ***\
******************************************************\
<popBold/><prompt time=\"1665949176\">s&gt;</prompt>\
");
        QCOMPARE(10, 10);
    }

    void writeGameText(QByteArray text, bool prompt) { qDebug() << text; }
};

QTEST_MAIN(XmlParserThreadTest)
#include "xmlparserthreadtest.moc"
