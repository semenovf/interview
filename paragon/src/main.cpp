#include <QApplication>
#include <QMessageBox>
#include "Exception.hpp"
#include "Model.hpp"
#include "MainWindow.hpp"

int main (int argc, char * argv[])
{
    QCoreApplication::addLibraryPath("./");

    QApplication app(argc, argv);
    app.setOrganizationName("paragon-exam");
    app.setApplicationName("partman");

    QString errmsg("No errors");
    Model * model = requestModel(& errmsg);

    if (!model) {
        //qCritical() << errmsg;
        QMessageBox::critical(0, QT_TR_NOOP("Error"), errmsg, QMessageBox::Ok);
        return -1;
    }

    int r = -1;

    try {
        MainWindow mainWin(model);
        mainWin.show();
        int r = app.exec();
    } catch (Exception const & ex) {
        QMessageBox::critical(0, QT_TR_NOOP("Error"), ex.what(), QMessageBox::Ok);
    }

    releaseModel(model);
    return r;
}
