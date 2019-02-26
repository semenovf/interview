#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
// #include <QSplitter>
// #include <QLayoutItem>
//#include <QVBoxLayout>
// #include <QAction>
// #include <QMenu>
// #include <QMenuBar>
// #include <QMessageBox>
// #include "Model.hpp"
// #include "BottomWidget.hpp"
// #include "TopWidget.hpp"
// #include "DiskListView.hpp"
// #include "VolumeListView.hpp"
// #include "GraphicalView.hpp"
#include "ui_CalculatorForm.h"
#include "Operation.hpp"
#include "MainWindow.hpp"

#include <QDebug>

static QChar DEC_POINT{','};
static QChar PLUS_SIGN{'+'};
static QChar MINUS_SIGN{'-'};
static QString const GEOMETRY_KEY{"geometry"};
static QString const WINDOW_STATE_KEY{"window-state"};
static QString const EXTRA_AREA_VISIBLE_KEY{"extra-area-visible"};
// static QString const BOTTOM_WIDGET_KEY("bottom-widget");
// static QString const BOTTOM_WIDGET_VISIBLE_KEY("bottom-widget-visible");
// static QString const DISK_LIST_VIEW_NAME("disk-list-view");
// static QString const VOLUME_LIST_VIEW_NAME("volume-list-view");
// static QString const GRAPHICAL_VIEW_NAME("graphical-view");

// static QString const ABOUT = QT_TR_NOOP(
//     "<center><b>Partition Manager</b></center>"
//     "<br/>"
//     "<center>Special for Paragon Software's Exam<center>");

// static void __deleteAllWidgetsFromLayout (QLayout * layout)
// {
//     QLayoutItem * child;
//
//     while ((child = layout->takeAt(0)) != 0) {
//         child->widget()->deleteLater();
//         delete child;
//     }
// }
static int const DIGIT_BASE_ID    =   0;
static int const OPERATOR_BASE_ID =  10;
static int const BACKSPACE_ID     = 100;
static int const CLEAR_ID         = 101;
static int const DEC_POINT_ID     = 102;
static int const SIGN_ID          = 103;
static int const EVAL_ID          = 104;

inline QString backspaced (QString const & s)
{
    return s.isEmpty()
        ? s
        : s.left(s.size() - 1);
}

QString changeSign (QString const & s)
{
    QString result;

    if (!s.isEmpty()) {
        if (s.startsWith(MINUS_SIGN)) {
            result = s.right(s.size() - 1);
        } else {
            result = MINUS_SIGN + s;
        }
    }

    return result;
}

QString appendDecPoint (QString const & s)
{
    QString result{s};

    if (!s.isEmpty() && !s.contains(DEC_POINT)) {
        result += DEC_POINT;
    }

    return result;
}

QString appendString (QString const & s, QString const & s1)
{
    return s + s1;
}

MainWindow::MainWindow ()
    : QWidget{0}
    , _ui{new Ui::CalculatorForm}
    , _extraAreaVisible{false}
{
    _ui->setupUi(this);
    _ui->extraArea->hide();

    _buttonGroup.addButton(_ui->button0, DIGIT_BASE_ID + 0);
    _buttonGroup.addButton(_ui->button1, DIGIT_BASE_ID + 1);
    _buttonGroup.addButton(_ui->button2, DIGIT_BASE_ID + 2);
    _buttonGroup.addButton(_ui->button3, DIGIT_BASE_ID + 3);
    _buttonGroup.addButton(_ui->button4, DIGIT_BASE_ID + 4);
    _buttonGroup.addButton(_ui->button5, DIGIT_BASE_ID + 5);
    _buttonGroup.addButton(_ui->button6, DIGIT_BASE_ID + 6);
    _buttonGroup.addButton(_ui->button7, DIGIT_BASE_ID + 7);
    _buttonGroup.addButton(_ui->button8, DIGIT_BASE_ID + 8);
    _buttonGroup.addButton(_ui->button9, DIGIT_BASE_ID + 9);
    _buttonGroup.addButton(_ui->buttonPlus , OPERATOR_BASE_ID + static_cast<int>(Operator::PLUS));
    _buttonGroup.addButton(_ui->buttonMinus, OPERATOR_BASE_ID + static_cast<int>(Operator::MINUS));
    _buttonGroup.addButton(_ui->buttonMul  , OPERATOR_BASE_ID + static_cast<int>(Operator::MULTIPLY));
    _buttonGroup.addButton(_ui->buttonDiv  , OPERATOR_BASE_ID + static_cast<int>(Operator::DIVIDE));
    _buttonGroup.addButton(_ui->buttonBackspace, BACKSPACE_ID);
    _buttonGroup.addButton(_ui->buttonClear    , CLEAR_ID);
    _buttonGroup.addButton(_ui->buttonDecPoint , DEC_POINT_ID);
    _buttonGroup.addButton(_ui->buttonSign     , SIGN_ID);
    _buttonGroup.addButton(_ui->buttonEval     , EVAL_ID);

    connect(& _buttonGroup
            , static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            , [this] (int id) { this->process(id); });
//             , this
//             , & MainWindow::onButtonClicked);

    connect(_ui->buttonMore
            , & QPushButton::clicked
            , [this] (int id) { showHideExtraArea(!_extraAreaVisible); });
    //             , this
//             , & MainWindow::onMoreClicked);


    layout()->setContentsMargins(0,0,0,0);

    auto operationWorker = new OperationWorker(
              DoIt
            , _requestQueue
            , _resultQueue);

    auto controlWorker = new ControlWorker(
              _requestQueue
            , _resultQueue);

    operationWorker->moveToThread(& _operationThread);
    controlWorker->moveToThread(& _controlThread);

    connect(& _operationThread, & QThread::finished
            , operationWorker, & QObject::deleteLater);
    connect(& _controlThread, & QThread::finished
            , controlWorker, & QObject::deleteLater);

    connect(this, & MainWindow::requestCalculate
            , controlWorker, & ControlWorker::requested);

    connect(operationWorker, & OperationWorker::requestQueueSizeChanged
            , [this] (int sz) { _ui->requestQueueSizeLabel->setText(QString::number(sz)); });
            //, this, & MainWindow::onRequestQueueSize);

    connect(controlWorker, & ControlWorker::requestQueueSizeChanged
            , [this] (int sz) { _ui->requestQueueSizeLabel->setText(QString::number(sz)); });
//            , this, & MainWindow::onRequestQueueSize);

    connect(operationWorker, & OperationWorker::resultQueueSizeChanged
            , [this] (int sz) { _ui->resultQueueSizeLabel->setText(QString::number(sz)); });
//            , this, & MainWindow::onResultQueueSize);

    connect(controlWorker, & ControlWorker::resultQueueSizeChanged
            , [this] (int sz) { _ui->resultQueueSizeLabel->setText(QString::number(sz)); });
//            , this, & MainWindow::onResultQueueSize);

    _operationThread.start();
    _controlThread.start();

    restoreSettings();
}

MainWindow::~MainWindow ()
{
    _operationThread.quit();
    _controlThread.quit();
    _operationThread.wait();
    _controlThread.wait();

    delete _ui;
}

void MainWindow::restoreSettings ()
{
    setWindowTitle(tr("Calculator (special for STC's Exam)"));

    QSettings settings(qApp->organizationName(), qApp->applicationName());

    this->restoreGeometry(settings.value(GEOMETRY_KEY).toByteArray());
    bool showExtraArea = settings.value(EXTRA_AREA_VISIBLE_KEY, false).toBool();

    showHideExtraArea(showExtraArea);

    if (!settings.contains(GEOMETRY_KEY))
        resize(800, 600);
}

void MainWindow::saveSettings ()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());
    settings.setValue(GEOMETRY_KEY, saveGeometry());
    settings.setValue(EXTRA_AREA_VISIBLE_KEY, _extraAreaVisible);
}

void MainWindow::closeEvent (QCloseEvent * event)
{
    saveSettings();
    event->accept();
}

void MainWindow::showHideExtraArea (bool show)
{
    if (show) {
        _ui->extraArea->show();
        _ui->buttonMore->setText("less...");
    } else {
        _ui->extraArea->hide();
        _ui->buttonMore->setText("more...");
    }
    _extraAreaVisible = show;
}

inline void MainWindow::clearText ()
{
    _ui->lineEdit->setText(QString{});
}

inline void MainWindow::transformText (QString (* transformFunc) (QString const &))
{
    _ui->lineEdit->setText(transformFunc(_ui->lineEdit->text()));
}

inline void MainWindow::transformText (QString (* transformFunc) (QString const &, QString const &)
        , QString const & s)
{
    _ui->lineEdit->setText(transformFunc(_ui->lineEdit->text(), s));
}

void MainWindow::process (int id)
{
    int errorCode = 0;

    switch (id) {
        case DIGIT_BASE_ID + 0:
        case DIGIT_BASE_ID + 1:
        case DIGIT_BASE_ID + 2:
        case DIGIT_BASE_ID + 3:
        case DIGIT_BASE_ID + 4:
        case DIGIT_BASE_ID + 5:
        case DIGIT_BASE_ID + 6:
        case DIGIT_BASE_ID + 7:
        case DIGIT_BASE_ID + 8:
        case DIGIT_BASE_ID + 9:
            if (_state == INITIAL) {
                clearText();
                _state = OPERAND_A;
            } else if (_state == OPERATOR) {
                clearText();
                _state = OPERAND_B;
            }

            if (_state == OPERAND_A || _state == OPERAND_B) {
                transformText(appendString, QString::number(id, 10));
            }

            break;

        case DEC_POINT_ID:
            if (_state == OPERAND_A || _state == OPERAND_B)
                transformText(appendDecPoint);
            break;

        case SIGN_ID:
            if (_state == OPERAND_A || _state == OPERAND_B)
                transformText(changeSign);
            break;

        case BACKSPACE_ID:
            if (_state == OPERAND_A || _state == OPERAND_B)
                transformText(backspaced);
            break;

        case OPERATOR_BASE_ID + static_cast<int>(Operator::PLUS):
        case OPERATOR_BASE_ID + static_cast<int>(Operator::MINUS):
        case OPERATOR_BASE_ID + static_cast<int>(Operator::MULTIPLY):
        case OPERATOR_BASE_ID + static_cast<int>(Operator::DIVIDE):
            if (_state == OPERAND_A) {
                _state = OPERATOR;
                _operandA = _ui->lineEdit->text();
                _operator = static_cast<Operator>(id - OPERATOR_BASE_ID);
            }
            break;

        case CLEAR_ID:
            _ui->lineEdit->setText(QString{});
            _state == INITIAL;
            break;

        case EVAL_ID:
            if (_state == OPERAND_B) {
                _state = INITIAL;
                _operandB = _ui->lineEdit->text();
                logRequest(QString{"%1 %2 %3"}
                        .arg(_operandA)
                        .arg(toString(_operator))
                        .arg(_operandB));

                if (DEC_POINT != QChar('.')) {
                    _operandA.replace(DEC_POINT, QChar('.'));
                    _operandB.replace(DEC_POINT, QChar('.'));
                }

                bool ok = true;
                double op1 = _operandA.toDouble(& ok);

                if (! ok) {
                    logError("operand A is invalid");
                    break;
                }

                double op2 = _operandB.toDouble(& ok);

                if (! ok) {
                    logError("operand B is invalid");
                    break;
                }

                emit requestCalculate(Operation{_operator, op1, op2});
            }
            break;
    }
}

// void MainWindow::onButtonClicked (int id)
// {
//     process(id);
// }

// void MainWindow::onMoreClicked ()
// {
//     showHideExtraArea(!_extraAreaVisible);
// }

// void MainWindow::onRequestQueueSize (int sz)
// {
//     _ui->requestQueueSizeLabel->setText(QString::number(sz));
// }

// void MainWindow::onResultQueueSize (int sz)
// {
//     _ui->resultQueueSizeLabel->setText(QString::number(sz));
// }

void MainWindow::logRequest (QString const & s)
{
    _ui->console->appendPlainText(s);
}

void MainWindow::logResult (QString const & s)
{
    _ui->console->appendPlainText(s);
}

void MainWindow::logError (QString const & s)
{
    _ui->console->appendPlainText(s);
}
