#include <QApplication>
#include <QCloseEvent>
#include <QSettings>
#include <QTextCharFormat>
#include <QIntValidator>
#include "ui_CalculatorForm.h"
#include "Operation.hpp"
#include "MainWindow.hpp"

#include <QDebug>

static QChar DEC_POINT{','};
static QChar PLUS_SIGN{'+'};
static QChar MINUS_SIGN{'-'};
static QString const WINDOW_POSITION_KEY{"window-position"};
static QString const MAIN_AREA_SIZE_KEY{"main-area-size"};
static QString const GEOMETRY_KEY{"geometry"};
static QString const WINDOW_STATE_KEY{"window-state"};
static QString const EXTRA_AREA_VISIBLE_KEY{"extra-area-visible"};
static QString const TIMEOUT_KEY{"timeout"};

static int const DIGIT_BASE_ID    =   0;
static int const OPERATOR_BASE_ID =  10;
static int const BACKSPACE_ID     = 100;
static int const CLEAR_ID         = 101;
static int const DECPOINT_ID      = 102;
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

MainWindow::MainWindow (DoItFunc doIt)
    : QWidget{0}
    , _ui{new Ui::CalculatorForm}
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
    _buttonGroup.addButton(_ui->buttonDecPoint , DECPOINT_ID);
    _buttonGroup.addButton(_ui->buttonSign     , SIGN_ID);
    _buttonGroup.addButton(_ui->buttonEval     , EVAL_ID);

    _ui->timeoutEdit->setValidator(new QIntValidator(0, 99, this));

    connect(& _buttonGroup
            , static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked)
            , [this] (int id) { this->process(id); });

    connect(_ui->buttonMore
            , & QPushButton::clicked
            , [this] () { showHideExtraArea(!_ui->extraArea->isVisible()); });

    layout()->setContentsMargins(0,0,0,0);

    auto operationWorker = new OperationWorker(
              doIt
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

    connect(operationWorker, & OperationWorker::requestQueueSizeChanged
            , [this] (int sz) {
                _ui->requestQueueSizeLabel->setText(QString::number(sz));
            });

    connect(controlWorker, & ControlWorker::requestQueueSizeChanged
            , [this] (int sz) {
                _ui->requestQueueSizeLabel->setText(QString::number(sz));
            });

    connect(operationWorker, & OperationWorker::resultQueueSizeChanged
            , [this] (int sz) {
                _ui->resultQueueSizeLabel->setText(QString::number(sz));
            });

    connect(controlWorker, & ControlWorker::resultQueueSizeChanged
            , [this] (int sz) {
                _ui->resultQueueSizeLabel->setText(QString::number(sz));
            });

    connect(this, & MainWindow::requestCalculate
            , controlWorker, & ControlWorker::calculateRequested);

    connect(controlWorker, & ControlWorker::resultQueueSizeChanged
            , [this] (int sz) {
                _ui->resultQueueSizeLabel->setText(QString::number(sz));
            });

    connect(controlWorker, & ControlWorker::requestReady
            , operationWorker, & OperationWorker::calculate);

    connect(operationWorker, & OperationWorker::resultReady
            , controlWorker, & ControlWorker::processResult);

    connect(controlWorker, & ControlWorker::resultReady
            , this, & MainWindow::processResult);

    connect(_ui->timeoutEdit, & QLineEdit::editingFinished
            , [this] () {
                int timeout = _ui->timeoutEdit->text().toInt();
                qDebug() << "Set timeout" << timeout;
                emit setTimeout(timeout);
            });

    connect(this, & MainWindow::setTimeout
            , operationWorker, & OperationWorker::setTimeout);

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
    QVariant windowPosition = settings.value(WINDOW_POSITION_KEY, QPoint{0,0});
    QVariant mainAreaSize   = settings.value(MAIN_AREA_SIZE_KEY, QSize{640, 480});
    QVariant showExtraArea  = settings.value(EXTRA_AREA_VISIBLE_KEY, false);
    QVariant timeoutValue   = settings.value(TIMEOUT_KEY, QString{"0"});

    this->move(windowPosition.toPoint());
    _ui->mainArea->resize(mainAreaSize.toSize());
    _ui->extraArea->resize(QSize{mainAreaSize.toSize().width(), 100});
    _ui->timeoutEdit->setText(timeoutValue.toString());
    emit setTimeout(timeoutValue.toInt());
    //showHideExtraArea(showExtraArea.toBool());
    showHideExtraArea(true);
}

void MainWindow::saveSettings ()
{
    QSettings settings(qApp->organizationName(), qApp->applicationName());

    settings.setValue(GEOMETRY_KEY, saveGeometry());
    settings.setValue(WINDOW_POSITION_KEY, this->pos());
    settings.setValue(MAIN_AREA_SIZE_KEY, _ui->mainArea->size());
    settings.setValue(EXTRA_AREA_VISIBLE_KEY, _ui->extraArea->isVisible());
    settings.setValue(TIMEOUT_KEY, _ui->timeoutEdit->text());
}

void MainWindow::closeEvent (QCloseEvent * event)
{
    saveSettings();
    event->accept();
    _operationThread.quit();
    _controlThread.quit();
}

void MainWindow::keyReleaseEvent (QKeyEvent * event)
{
    switch (event->key()) {
        case Qt::Key_0: process(DIGIT_BASE_ID + 0); break;
        case Qt::Key_1: process(DIGIT_BASE_ID + 1); break;
        case Qt::Key_2: process(DIGIT_BASE_ID + 2); break;
        case Qt::Key_3: process(DIGIT_BASE_ID + 3); break;
        case Qt::Key_4: process(DIGIT_BASE_ID + 4); break;
        case Qt::Key_5: process(DIGIT_BASE_ID + 5); break;
        case Qt::Key_6: process(DIGIT_BASE_ID + 6); break;
        case Qt::Key_7: process(DIGIT_BASE_ID + 7); break;
        case Qt::Key_8: process(DIGIT_BASE_ID + 8); break;
        case Qt::Key_9: process(DIGIT_BASE_ID + 9); break;

        case Qt::Key_Comma:
        case Qt::Key_Period:
            process(DECPOINT_ID);
            break;

        case Qt::Key_Backspace:
            process(BACKSPACE_ID);
            break;

        case Qt::Key_Plus:
            process(OPERATOR_BASE_ID + static_cast<int>(Operator::PLUS));
            break;

        case Qt::Key_Minus:
            process(OPERATOR_BASE_ID + static_cast<int>(Operator::MINUS));
            break;

        case Qt::Key_Asterisk:
            process(OPERATOR_BASE_ID + static_cast<int>(Operator::MULTIPLY));
            break;

        case Qt::Key_Slash:
            process(OPERATOR_BASE_ID + static_cast<int>(Operator::DIVIDE));
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Equal:
            process(EVAL_ID);
            break;

        default:
            //qDebug() << "Key" << event->key();
            QWidget::keyReleaseEvent(event);
    }
}

void MainWindow::resizeEvent (QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    //_mainAreaSize = _ui->mainArea->size();
    _ui->extraArea->resize(
          _ui->mainArea->size().width()
        , _ui->extraArea->height());
    qDebug() << "================";
    qDebug() << "Window Size:" << this->size();
    qDebug() << "Main Area Size:" << _ui->mainArea->size();
    qDebug() << "Extra Area Size:" << _ui->extraArea->size();
}

void MainWindow::showHideExtraArea (bool show)
{
    if (show) {
        _mainAreaSize = _ui->mainArea->size();
        //this->setGeometry(this->geometry() + QMargins{0,0,0,100});
        _ui->extraArea->show();
        _ui->buttonMore->setText("less...");
        this->resize(QSize{_mainAreaSize.width()
                , _mainAreaSize.height() + _ui->extraArea->height()});
    } else {
        this->resize(_mainAreaSize);
        _ui->extraArea->hide();
        _ui->buttonMore->setText("more...");
        //this->resize(_mainAreaSize);
        _ui->mainArea->resize(_mainAreaSize);
        //this->resize(this->size() - QSize(0, _ui->extraArea->height()));
    }

//     _ui->mainArea->resize(_mainAreaSize);
//     _ui->mainArea->update();
    qDebug() << "+++++++";
    qDebug() << "New size:" << this->size();
    qDebug() << "_mainAreaSize:" << _mainAreaSize;
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

        case DECPOINT_ID:
            if (_state == OPERAND_A || _state == OPERAND_B)
                transformText(appendDecPoint);
            break;

        case SIGN_ID:
            if (_state == OPERAND_A || _state == OPERAND_B)
                transformText(changeSign);
            break;

        case BACKSPACE_ID:
            if (_state == INITIAL || _state == OPERAND_A || _state == OPERAND_B)
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
            _state = INITIAL;
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

void MainWindow::processResult (Result const & res)
{
    if (res.isError()) {
        logError(Result::toErrorString(res.error()));
        _ui->lineEdit->setText("#error#");
    } else {
        QString resultString = QString::number(res.result());
        resultString.replace(QChar('.'), DEC_POINT);
        _ui->lineEdit->setText(resultString);
        logResult(resultString);
    }
}

void MainWindow::logColored (QString const & s, QColor const & fgColor)
{
    QTextCharFormat tf;
    tf = _ui->console->currentCharFormat();
    tf.setForeground(fgColor);
    _ui->console->setCurrentCharFormat(tf);
    _ui->console->appendPlainText(s);
}

void MainWindow::logRequest (QString const & s)
{
    logColored("Expression: " + s, Qt::darkGreen);
}

void MainWindow::logResult (QString const & s)
{
    logColored("Result: " + s, Qt::blue);
}

void MainWindow::logError (QString const & s)
{
    logColored("Error: " + s, Qt::red);
}
