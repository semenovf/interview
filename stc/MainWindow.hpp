#pragma once
#include <QMainWindow>
#include <QButtonGroup>
#include <QThread>
#include <QColor>
#include "Worker.hpp"
#include "Operation.hpp"

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
    MainWindow (DoItFunc doIt);
    ~MainWindow ();

protected:
    virtual void closeEvent (QCloseEvent *) override;
    virtual void keyReleaseEvent (QKeyEvent *) override;
    virtual void resizeEvent (QResizeEvent *) override;

private:
    void saveSettings ();
    void restoreSettings ();

    void process (int id);
    void clearText ();
    void transformText (QString (* transformFunc) (QString const &));
    void transformText (QString (* transformFunc) (QString const &, QString const &)
            , QString const &);

    void logColored (QString const & s, QColor const & fgColor);
    void logRequest (QString const & s);
    void logResult (QString const & s);
    void logError (QString const & s);

    Q_SLOT void processResult (Result const & res);
    Q_SIGNAL void requestCalculate (Operation const & op);
    Q_SIGNAL void setTimeout (int t);

private:
    Ui::CalculatorForm * _ui;
    QButtonGroup _buttonGroup;
    State        _state = INITIAL;
    Operator     _operator = Operator::UNKNOWN;
    QString      _operandA;
    QString      _operandB;

    QThread      _operationThread;
    QThread      _controlThread;
    RequestQueue _requestQueue;
    ResultQueue  _resultQueue;
};
