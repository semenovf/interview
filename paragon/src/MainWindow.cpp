#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QSplitter>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include "Model.hpp"
#include "BottomWidget.hpp"
#include "TopWidget.hpp"
#include "DiskListView.hpp"
#include "VolumeListView.hpp"
#include "GraphicalView.hpp"
#include "MainWindow.hpp"

#include <QDebug>

static QString const GEOMETRY_KEY("geometry");
static QString const WINDOW_STATE_KEY("window-state");

static void __deleteAllWidgetsFromLayout (QLayout * layout)
{
    QLayoutItem * child;

    while ((child = layout->takeAt(0)) != 0) {
        child->widget()->deleteLater();
        delete child;
    }
}

MainWindow::MainWindow (Model * model)
    : QMainWindow(0)
    , _model(model)
    , _topWidget(new TopWidget)
    , _bottomWidget(new BottomWidget)
{
    _topWidget->setLayout(new QVBoxLayout);
    _topWidget->layout()->setContentsMargins(10,10,10,0);

    _bottomWidget->setLayout(new QVBoxLayout);
    _bottomWidget->layout()->setContentsMargins(10,0,10,10);

//     auto topWidgetSA = new QScrollArea(this);
//     auto bottomWidgetSA = new QScrollArea(this);
//
//     topWidgetSA->setWidget(_topWidget);
//     bottomWidgetSA->setWidget(_bottomWidget);

    QSplitter * splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(_topWidget);
    splitter->addWidget(_bottomWidget);

    setCentralWidget(splitter);

    setVolumeListAtTop();
    setGraphicalViewAtBottom();

    initActions();
    initMenus();
    initStatusBar();
    restoreSettings();

    connect(this, SIGNAL(volumeSelected (QString const &, QString const &))
            , SLOT(onVolumeSelected (QString const &, QString const &)));
}

void MainWindow::closeEvent (QCloseEvent * event)
{
    saveSettings();
    event->accept();
}

void MainWindow::initActions ()
{
    _exitAct                    = new QAction(tr("Exit"), this);
    _viewTopDiskListAct         = new QAction(tr("Disk List"), this);
    _viewTopVolumeListAct       = new QAction(tr("Volume List"), this);
    _viewTopGraphicalViewAct    = new QAction(tr("Graphical View"), this);
    _viewBottomDiskListAct      = new QAction(tr("Disk List"), this);
    _viewBottomVolumeListAct    = new QAction(tr("Volume List"), this);
    _viewBottomGraphicalViewAct = new QAction(tr("Graphical View"), this);
    _viewBottomShowHideAct      = new QAction(tr("Hide"), this);

    connect(_exitAct, SIGNAL(triggered()), this, SLOT(onExit()));
    connect(_viewTopDiskListAct, SIGNAL(triggered()), this, SLOT(setDiskListAtTop()));
    connect(_viewTopVolumeListAct, SIGNAL(triggered()), this, SLOT(setVolumeListAtTop()));
    connect(_viewTopGraphicalViewAct, SIGNAL(triggered()), this, SLOT(setGraphicalViewAtTop()));
    connect(_viewBottomDiskListAct, SIGNAL(triggered()), this, SLOT(setDiskListAtBottom()));
    connect(_viewBottomVolumeListAct, SIGNAL(triggered()), this, SLOT(setVolumeListAtBottom()));
    connect(_viewBottomGraphicalViewAct, SIGNAL(triggered()), this, SLOT(setGraphicalViewAtBottom()));
    connect(_viewBottomShowHideAct, SIGNAL(triggered()), this, SLOT(showHideBottom()));

    _exitAct->setShortcuts(QKeySequence::Quit);
    _exitAct->setStatusTip(tr("Exit from application"));
}

void MainWindow::initMenus ()
{
    _fileMenu = menuBar()->addMenu(tr("File"));
    _fileMenu->addAction(_exitAct);

    _viewMenu = menuBar()->addMenu(tr("View"));
    connect(_viewMenu, SIGNAL(aboutToShow()), this, SLOT(updateViewMenu()));
    QMenu * viewTopMenu    = _viewMenu->addMenu(tr("Top"));
    QMenu * viewBottomMenu = _viewMenu->addMenu(tr("Bottom"));

    viewTopMenu->addAction(_viewTopDiskListAct);
    viewTopMenu->addAction(_viewTopVolumeListAct);
    viewTopMenu->addAction(_viewTopGraphicalViewAct);

    viewBottomMenu->addAction(_viewBottomDiskListAct);
    viewBottomMenu->addAction(_viewBottomVolumeListAct);
    viewBottomMenu->addAction(_viewBottomGraphicalViewAct);
    viewBottomMenu->addAction(_viewBottomShowHideAct);
    updateViewMenu();

//     _windowMenu = menuBar()->addMenu(tr("Окно"));
//     updateWindowMenu();
//     connect(_windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

//     _helpMenu = menuBar()->addMenu(tr("Help"));
//     _helpMenu->addAction(_aboutAct);
//     _helpMenu->addAction(_aboutQtAct);
}

void MainWindow::updateViewMenu ()
{
    if (_bottomWidget->isVisible())
        _viewBottomShowHideAct->setText(tr("Hide"));
    else
        _viewBottomShowHideAct->setText(tr("Show"));
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

void MainWindow::onExit ()
{
    qApp->closeAllWindows();
}

void MainWindow::setDiskListAtTop ()
{
    setTopWidget(new DiskListView(_model, _topWidget));
}

void MainWindow::setVolumeListAtTop ()
{
    auto view = new VolumeListView(_model, _topWidget);
    connect(view
            , SIGNAL(emitSelected(QString const &, QString const &))
            , this
            , SIGNAL(emitVolumeSelected(QString const &, QString const &)));
    connect(this
            , SIGNAL(emitVolumeSelected(QString const &, QString const &))
            , this
            , SIGNAL(onVolumeSelected(QString const &, QString const &)));
    setTopWidget(view);
}

void MainWindow::setGraphicalViewAtTop ()
{
    setTopWidget(new GraphicalView(_model, _topWidget));
}

void MainWindow::setDiskListAtBottom ()
{
    setBottomWidget(new DiskListView(_model, _bottomWidget));
}

void MainWindow::setVolumeListAtBottom ()
{
    setBottomWidget(new VolumeListView(_model, _bottomWidget));
}

void MainWindow::setGraphicalViewAtBottom ()
{
    setBottomWidget(new GraphicalView(_model, _bottomWidget));
}

void MainWindow::showHideBottom ()
{
    if (_bottomWidget->isVisible())
        _bottomWidget->setVisible(false);
    else
        _bottomWidget->setVisible(true);

}

void MainWindow::onVolumeSelected (QString const & diskName, QString const & volumeName)
{
    qDebug() << diskName;
    qDebug() << volumeName;
}
