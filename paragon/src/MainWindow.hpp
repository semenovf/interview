#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QCloseEvent;
QT_END_NAMESPACE

class TopWidget;
class BottomWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Model *        _model;
    TopWidget *    _topWidget;
    BottomWidget * _bottomWidget;

    QMenu * _fileMenu;
    QMenu * _viewMenu;
    QMenu * _helpMenu;

    QAction * _exitAct;
    QAction * _viewTopDiskListAct;
    QAction * _viewTopVolumeListAct;
    QAction * _viewTopGraphicalViewAct;
    QAction * _viewBottomDiskListAct;
    QAction * _viewBottomVolumeListAct;
    QAction * _viewBottomGraphicalViewAct;
    QAction * _viewBottomShowHideAct;

public:
    MainWindow (Model * model);

protected:
    virtual void closeEvent (QCloseEvent *) override;

private:
    void initActions ();
    void initMenus ();
    void initStatusBar ();
    void saveSettings ();
    void restoreSettings ();

private:
    Q_SLOT void onExit ();
    Q_SLOT void setDiskListAtTop ();
    Q_SLOT void setVolumeListAtTop ();
    Q_SLOT void setGraphicalViewAtTop ();
    Q_SLOT void setDiskListAtBottom ();
    Q_SLOT void setVolumeListAtBottom ();
    Q_SLOT void setGraphicalViewAtBottom ();
    Q_SLOT void showHideBottom ();
    Q_SLOT void updateViewMenu ();

    Q_SIGNAL void emitDiskSelected (QString const & diskName);
    Q_SIGNAL void emitVolumeSelected (QString const & diskName, QString const & volumeName);

    Q_SLOT void onVolumeSelected (QString const & diskName, QString const & volumeName);

private:
    void setTopWidget (QWidget * w);
    void setBottomWidget (QWidget * w);
};
