#include <QGuiApplication>
#include <QLibrary>
#include <QDebug>
#include <QQmlApplicationEngine>
#include "DoIt.hpp"
// #include "Operation.hpp"
// #include "Result.hpp"
// #include "MainWindow.hpp"

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
    engine.load(QString{":main.qml"});

//     qRegisterMetaType<Operation>("Operation");
//     qRegisterMetaType<Result>("Result");
//
//     MainWindow mainWin(doIt);
//     mainWin.show();
    return app.exec();
}
