#include <utility>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFileInfo>
#include <QDebug>
#include "ResultStatus.hpp"
#include "Translator.hpp"
//#include <QQmlContext>

//
//==============================================================================
// Problem:
//------------------------------------------------------------------------------
// On Ubuntu:
// appmenu-qt: handleReparent 138 The given QWindow has no QMenuBar assigned
//
// Workaround:
//------------------------------------------------------------------------------
// Setting the UBUNTU_MENUPROXY environment variable to the empty string
//


int main (int argc, char * argv[])
{
    QGuiApplication app(argc, argv);
    QString programName{QFileInfo{argv[0]}.baseName()};

    app.setOrganizationName("qml-demo");
    app.setApplicationName(programName);

    Translator translator{};
    ResultStatus rs = translator.enable();

    if (!rs) {
        qWarning() << "WARNING:" << rs.what();
    }

    QQmlApplicationEngine engine;

    QObject::connect(& engine, & QQmlApplicationEngine::warnings
        , [] (QList<QQmlError> const & warnings) {
            for (auto const & w: warnings)
                qWarning() << "ERROR: " << w;
        }
    );

    QPM_INIT(engine);
    engine.load(QUrl{"qrc:///main.qml"});
    //engine.load(QUrl{"qrc:///Demo001.qml"});

    return app.exec();
}
