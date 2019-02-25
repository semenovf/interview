#pragma once
#include <QMainWindow>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QCloseEvent;
QT_END_NAMESPACE

namespace Ui {
    class CalculatorForm;
} // namespace Ui

class TopWidget;
class BottomWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

//     Model *        _model;
// 
//     QMenu * _fileMenu;
//     QMenu * _viewMenu;
//     QMenu * _helpMenu;
// 
//     QAction * _exitAct;
//     QAction * _viewTopDiskListAct;
//     QAction * _viewTopVolumeListAct;
//     QAction * _viewTopGraphicalViewAct;
//     QAction * _viewBottomDiskListAct;
//     QAction * _viewBottomVolumeListAct;
//     QAction * _viewBottomGraphicalViewAct;
//     QAction * _viewBottomShowHideAct;
//     QAction * _aboutAct;
//     QAction * _aboutQtAct;

public:
    MainWindow ();
    ~MainWindow ();

protected:
    virtual void closeEvent (QCloseEvent *) override;

private:
//     void initActions ();
//     void initMenus ();
//     void initStatusBar ();
    void saveSettings ();
    void restoreSettings ();

private:
//     Q_SLOT void onExit ();
//     Q_SLOT void about ();
//     Q_SLOT void setDiskListAtTop ();
//     Q_SLOT void setVolumeListAtTop ();
//     Q_SLOT void setGraphicalViewAtTop ();
//     Q_SLOT void setDiskListAtBottom ();
//     Q_SLOT void setVolumeListAtBottom ();
//     Q_SLOT void setGraphicalViewAtBottom ();
//     Q_SLOT void showHideBottom ();
//     Q_SLOT void updateViewMenu ();
// 
//     Q_SIGNAL void emitDiskSelected (int diskIndex);
//     Q_SIGNAL void emitEntitySelected (int diskIndex, int volumeIndex);
//     //Q_SLOT void onDiskSelected (int diskIndex);
//     Q_SLOT void onEntitySelected (int diskIndex, int volumeIndex);

private:
    Q_SLOT void onButtonClicked (int id);
//     void setTopWidget (QWidget * w);
//     void setBottomWidget (QWidget * w);
// 
//     template <typename ViewType>
//     ViewType * createView (QWidget * parent);
private:
    Ui::CalculatorForm * _ui;
    QButtonGroup _buttonGroup;
};
