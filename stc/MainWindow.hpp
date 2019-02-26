#pragma once
#include <QMainWindow>
#include <QButtonGroup>
#include <QThread>
#include "Worker.hpp"
#include "Operation.hpp"

QT_BEGIN_NAMESPACE
class QCloseEvent;
QT_END_NAMESPACE

namespace Ui {
    class CalculatorForm;
} // namespace Ui

class MainWindow : public QWidget
{
    Q_OBJECT

    enum State {
          INITIAL
        , OPERAND_A
        , OPERATOR
        , OPERAND_B
    };

public:
    MainWindow ();
    ~MainWindow ();

protected:
    virtual void closeEvent (QCloseEvent *) override;

private:
    void saveSettings ();
    void restoreSettings ();

//     Q_SLOT void onButtonClicked (int id);
//     Q_SLOT void onMoreClicked ();
//     Q_SLOT void onRequestQueueSize (int sz);
//     Q_SLOT void onResultQueueSize (int sz);
    Q_SIGNAL void requestCalculate (Operation op);

    void process (int id);
    void showHideExtraArea (bool show);
    void clearText ();
    void transformText (QString (* transformFunc) (QString const &));
    void transformText (QString (* transformFunc) (QString const &, QString const &)
            , QString const &);

    void logRequest (QString const & s);
    void logResult (QString const & s);
    void logError (QString const & s);

private:
    Ui::CalculatorForm * _ui;
    QButtonGroup         _buttonGroup;
    bool                 _extraAreaVisible;
    State                _state = INITIAL;
    Operator             _operator = Operator::UNKNOWN;
    QString              _operandA;
    QString              _operandB;

    QThread      _operationThread;
    QThread      _controlThread;
    RequestQueue _requestQueue;
    ResultQueue  _resultQueue;
};
