#include <QApplication>
#include <QDebug>
#include "Model.hpp"
#include "MainWindow.hpp"

int main (int argc, char * argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("paragon-exam");
    app.setApplicationName("partman");

    Model * model = requestModel();

    if (!model) {
        qWarning() << "ERROR: Failed to request model";
        return -1;
    }

    MainWindow mainWin(model);
    mainWin.show();

    int r = app.exec();
    releaseModel(model);
    return r;
}
