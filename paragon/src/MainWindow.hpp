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
    TopWidget * _topWidget;
    BottomWidget * _bottomWidget;

    QMenu * _fileMenu;
    QMenu * _viewMenu;
    QMenu * _toolsMenu;
    QMenu * _windowMenu;
    QMenu * _helpMenu;

public:
    MainWindow ();

protected:
    virtual void closeEvent (QCloseEvent *) override;

private:
    void initActions ();
    void initMenus ();
    void initStatusBar ();
    void saveSettings ();
    void restoreSettings ();

    void setTopWidget (QWidget * w);
    void setBottomWidget (QWidget * w);
};
