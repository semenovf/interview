#include <QApplication>
#include <QMessageBox>
// #include "Exception.hpp"
// #include "Model.hpp"
#include "MainWindow.hpp"

int main (int argc, char * argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("stc-exam");
    app.setApplicationName("calculator");

    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
