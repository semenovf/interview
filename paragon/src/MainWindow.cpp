#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QSplitter>
#include <QLayoutItem>
#include <QVBoxLayout>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
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
static QString const TOP_WIDGET_KEY("top-widget");
static QString const BOTTOM_WIDGET_KEY("bottom-widget");
static QString const BOTTOM_WIDGET_VISIBLE_KEY("bottom-widget-visible");
static QString const DISK_LIST_VIEW_NAME("disk-list-view");
static QString const VOLUME_LIST_VIEW_NAME("volume-list-view");
static QString const GRAPHICAL_VIEW_NAME("graphical-view");

static QString const ABOUT = QT_TR_NOOP(
    "<center><b>Partition Manager</b></center>"
    "<br/>"
    "<center>Special for Paragon Software's Exam<center>");

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
    , _currentDisk(-1)
    , _currentVolume(-1)
    , _topWidget(new TopWidget)
    , _bottomWidget(new BottomWidget)
{
    _topWidget->setLayout(new QVBoxLayout);
    _topWidget->layout()->setContentsMargins(10,10,10,0);

    _bottomWidget->setLayout(new QVBoxLayout);
    _bottomWidget->layout()->setContentsMargins(10,0,10,10);

    QSplitter * splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(_topWidget);
    splitter->addWidget(_bottomWidget);

    setCentralWidget(splitter);

    initActions();
    initMenus();
    initStatusBar();
    restoreSettings();
}

void MainWindow::restoreSettings ()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());

    this->restoreGeometry(settings.value(GEOMETRY_KEY).toByteArray());
    this->restoreState(settings.value(WINDOW_STATE_KEY).toByteArray());

    setWindowTitle(tr("Partition Manager (special for Paragon Software's Exam)"));

    QString topWidgetName = settings.value(TOP_WIDGET_KEY).toString();
    QString bottomWidgetName = settings.value(BOTTOM_WIDGET_KEY).toString();

    if (topWidgetName == DISK_LIST_VIEW_NAME)
        setDiskListAtTop();
    else if (topWidgetName == GRAPHICAL_VIEW_NAME)
        setGraphicalViewAtTop();
    else
        setVolumeListAtTop();

    if (bottomWidgetName == DISK_LIST_VIEW_NAME)
        setDiskListAtBottom();
    else if (bottomWidgetName == VOLUME_LIST_VIEW_NAME)
        setVolumeListAtBottom();
    else
        setGraphicalViewAtBottom();

    if (settings.contains(BOTTOM_WIDGET_VISIBLE_KEY)) {
        bool bottomWidgetIsVisible = settings.value(BOTTOM_WIDGET_VISIBLE_KEY).toBool();

        if (!bottomWidgetIsVisible)
            _bottomWidget->setVisible(false);
    }
}

void MainWindow::saveSettings ()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    settings.setValue(GEOMETRY_KEY, saveGeometry());
    settings.setValue(WINDOW_STATE_KEY, saveState());
    settings.setValue(TOP_WIDGET_KEY, _topWidget->objectName());
    settings.setValue(BOTTOM_WIDGET_KEY, _bottomWidget->objectName());
    settings.setValue(BOTTOM_WIDGET_VISIBLE_KEY, _bottomWidget->isVisible());
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

    _aboutAct = new QAction(tr("About PartMan"), this);
    connect(_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    _aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
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

    _helpMenu = menuBar()->addMenu(tr("Help"));
    _helpMenu->addAction(_aboutAct);
    _helpMenu->addAction(_aboutQtAct);
}

void MainWindow::about ()
{
    QMessageBox::about(this, tr("About PartMan"), ABOUT);
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

void MainWindow::onExit ()
{
    qApp->closeAllWindows();
}

template <>
GraphicalView * MainWindow::createView<GraphicalView> (QWidget * parent)
{
    auto view = new GraphicalView(_model, parent);
    connect(view, SIGNAL(emitEntitySelected(int, int)), this, SIGNAL(emitEntitySelected(int, int)));
    connect(view, SIGNAL(emitEntitySelected(int, int)), this, SLOT(onEntitySelected(int, int)));
    connect(this, SIGNAL(emitEntitySelected(int, int)), view, SLOT(onEntitySelected(int, int)));
    return view;
}

template <>
DiskListView * MainWindow::createView<DiskListView> (QWidget * parent)
{
    auto view = new DiskListView(_model, parent);
    connect(view, SIGNAL(emitEntitySelected(int, int)), this, SIGNAL(emitEntitySelected(int, int)));
    connect(view, SIGNAL(emitEntitySelected(int, int)), this, SLOT(onEntitySelected(int, int)));
    connect(this, SIGNAL(emitEntitySelected(int, int)), view, SLOT(onEntitySelected(int, int)));
    return view;
}

template <>
VolumeListView * MainWindow::createView<VolumeListView> (QWidget * parent)
{
    auto view = new VolumeListView(_model, parent);
    connect(view, SIGNAL(emitEntitySelected(int, int)), this, SIGNAL(emitEntitySelected(int, int)));
    connect(view, SIGNAL(emitEntitySelected(int, int)), this, SLOT(onEntitySelected(int, int)));
    connect(this, SIGNAL(emitEntitySelected(int, int)), view, SLOT(onEntitySelected(int, int)));
    return view;
}

void MainWindow::setDiskListAtTop ()
{
    setTopWidget(createView<DiskListView>(_topWidget));
     emitEntitySelected(_currentDisk, _currentVolume);
     _topWidget->setObjectName(DISK_LIST_VIEW_NAME);
}

void MainWindow::setDiskListAtBottom ()
{
    setBottomWidget(createView<DiskListView>(_bottomWidget));
    emitEntitySelected(_currentDisk, _currentVolume);
    _bottomWidget->setObjectName(DISK_LIST_VIEW_NAME);
}

void MainWindow::setVolumeListAtTop ()
{
    setTopWidget(createView<VolumeListView>(_topWidget));
    emitEntitySelected(_currentDisk, _currentVolume);
    _topWidget->setObjectName(VOLUME_LIST_VIEW_NAME);
}

void MainWindow::setVolumeListAtBottom ()
{
    setBottomWidget(createView<VolumeListView>(_bottomWidget));
    emitEntitySelected(_currentDisk, _currentVolume);
    _bottomWidget->setObjectName(VOLUME_LIST_VIEW_NAME);
}

void MainWindow::setGraphicalViewAtTop ()
{
    setTopWidget(createView<GraphicalView>(_topWidget));
    emitEntitySelected(_currentDisk, _currentVolume);
    _topWidget->setObjectName(GRAPHICAL_VIEW_NAME);
}

void MainWindow::setGraphicalViewAtBottom ()
{
    setBottomWidget(createView<GraphicalView>(_bottomWidget));
    emitEntitySelected(_currentDisk, _currentVolume);
    _bottomWidget->setObjectName(GRAPHICAL_VIEW_NAME);
}

void MainWindow::showHideBottom ()
{
    if (_bottomWidget->isVisible())
        _bottomWidget->setVisible(false);
    else
        _bottomWidget->setVisible(true);
}

void MainWindow::onEntitySelected (int diskIndex, int volumeIndex)
{
    _currentDisk = diskIndex;
    _currentVolume = volumeIndex;
}
