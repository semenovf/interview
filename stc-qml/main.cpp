#include <QGuiApplication>
#include <QLibrary>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DoIt.hpp"
// #include "Operation.hpp"
// #include "Result.hpp"
// #include "MainWindow.hpp"

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
    QString libName{"doit"};

    if (argc > 1) {
        libName = QString{argv[1]};
    }

    QLibrary lib(libName);

    if (lib.isLoaded()) {
        qWarning() << "Failed to load library";
        return -1;
    }

    lib.setLoadHints(QLibrary::ResolveAllSymbolsHint);
    DoItFunc doIt = reinterpret_cast<DoItFunc>(lib.resolve("DoIt"));

    if (!doIt) {
        qWarning() << "Failed to resolve 'DoIt()'";
        return -1;
    }

    QGuiApplication app(argc, argv);
    app.setOrganizationName("stc-exam");
    app.setApplicationName("calculator");

    QQmlApplicationEngine engine;
    QObject::connect(& engine, & QQmlApplicationEngine::warnings
        , [] (QList<QQmlError> const & warnings) {
            for (auto const & w: warnings)
                qWarning() << "ERROR: " << w;
        }
    );
    engine.load(QUrl{"qrc:///main.qml"});
//     auto rootContext = engine.rootContext();
//
//     qDebug() << "rootContext=" << rootContext;
//
//     if (rootContext) {
//         qDebug() << "rootContext->isValid()" << rootContext->isValid();
//     }

//     qRegisterMetaType<Operation>("Operation");
//     qRegisterMetaType<Result>("Result");
//
//     MainWindow mainWin(doIt);
//     mainWin.show();
    return app.exec();
}
