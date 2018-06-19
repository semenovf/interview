#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QSplitter>
#include <QLayoutItem>
#include <QVBoxLayout>
#include "BottomWidget.hpp"
#include "TopWidget.hpp"
#include "DiskListView.hpp"
#include "VolumeListView.hpp"
#include "MainWindow.hpp"

static QString const GEOMETRY_KEY("geometry");
static QString const WINDOW_STATE_KEY("window-state");

static void __deleteAllWidgetsFromLayout (QLayout * layout)
{
    QLayoutItem * child;

    while ((child = layout->takeAt(0)) != 0) {
        delete child;
    }
}

MainWindow::MainWindow ()
    : QMainWindow(0)
    , _topWidget(new TopWidget)
    , _bottomWidget(new BottomWidget)
{
    _topWidget->setLayout(new QVBoxLayout);
    _bottomWidget->setLayout(new QVBoxLayout);

    QSplitter * splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(_topWidget);
    splitter->addWidget(_bottomWidget);

    setCentralWidget(splitter);
//     splitter->setContentsMargins(0, 0, 0, 0);
//     setContentsMargins(0, 0, 0, 0);

    setTopWidget(new DiskListView(_topWidget));
    setBottomWidget(new VolumeListView(_bottomWidget));

    initActions();
    initMenus();
    initStatusBar();
    restoreSettings();
}

void MainWindow::closeEvent (QCloseEvent * event)
{
    saveSettings();
    event->accept();
}

void MainWindow::initActions ()
{
    // TODO
}

void MainWindow::initMenus ()
{
    // TODO
}

void MainWindow::initStatusBar ()
{
    // TODO
}

void MainWindow::setTopWidget (QWidget * w)
{
    auto layout = qobject_cast<QVBoxLayout *>(_topWidget->layout());
    __deleteAllWidgetsFromLayout(layout);
    layout->addWidget(w);
}

void MainWindow::setBottomWidget (QWidget * w)
{
    auto layout = qobject_cast<QVBoxLayout *>(_bottomWidget->layout());
    __deleteAllWidgetsFromLayout(layout);
    layout->addWidget(w);
}

void MainWindow::saveSettings ()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    settings.setValue(GEOMETRY_KEY, saveGeometry());
    settings.setValue(WINDOW_STATE_KEY, saveState());
}

void MainWindow::restoreSettings ()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());

    this->restoreGeometry(settings.value(GEOMETRY_KEY).toByteArray());
    this->restoreState(settings.value(WINDOW_STATE_KEY).toByteArray());
}
