#include <utility>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLocale>
#include <QTranslator>
#include <QFileInfo>
#include <QDebug>
//#include <QQmlContext>

//
//==============================================================================
// Problem:
//------------------------------------------------------------------------------
// On Ubuntu:
// appmenu-qt: handleReparent 138 The given QWindow has no QMenuBar assigned
//
// Workaround:
//------------------------------------------------------------------------------
// Setting the UBUNTU_MENUPROXY environment variable to the empty string
//

template <typename CodeT>
class ResultCode {};

class ResultStatus
{
public:
    ResultStatus ()
        : _ok(true)
    {}

    ResultStatus (QString const & errstr)
        : _ok(false)
        , _errstr(errstr)
    {}

    ResultStatus (QString && errstr)
        : _ok(false)
        , _errstr(std::forward<QString>(errstr))
    {}

    ResultStatus (ResultStatus const &) = default;
    ResultStatus (ResultStatus &&) = default;
    ResultStatus & operator = (ResultStatus const &) = default;
    ResultStatus & operator = (ResultStatus &&) = default;
    ~ResultStatus () = default;

    operator bool () const
    {
        return _ok;
    }

    QString const & what () const
    {
        return _errstr;
    }

private:
    bool    _ok;
    QString _errstr;
};

class Translator
{
public:
    Translator () {}
    ~Translator () { qDebug() << "~Translator";}

    ResultStatus enable ()
    {
        QLocale currentLocale{};
        QString basename{QCoreApplication::applicationName()};

        bool success = _translator.load(currentLocale
                , basename
                , "."     // prefix
                , "./"    // directory
                , ".qm"); // suffix

        if (!success)
            return ResultStatus{QString{"failed to load translation for %1"}
                    .arg(currentLocale.name())};

        success = QCoreApplication::installTranslator(& _translator);

        if (!success)
            return ResultStatus{QString{"failed to install translator for %1"}
                    .arg(currentLocale.name())};

        return ResultStatus{};
    }

private:
    QTranslator _translator;
};

int main (int argc, char * argv[])
{
    QGuiApplication app(argc, argv);
    QString programName{QFileInfo{argv[0]}.baseName()};

    app.setOrganizationName("stc-exam");
    app.setApplicationName(programName);

    Translator translator{};
    ResultStatus rs = translator.enable();

    if (!rs) {
        qWarning() << "ERROR:" << rs.what();
        return -1;
    }

    QQmlApplicationEngine engine;

    QObject::connect(& engine, & QQmlApplicationEngine::warnings
        , [] (QList<QQmlError> const & warnings) {
            for (auto const & w: warnings)
                qWarning() << "ERROR: " << w;
        }
    );

    QPM_INIT(engine);
    engine.load(QUrl{"qrc:///main.qml"});

//     auto rootContext = engine.rootContext();
//
//     qDebug() << "rootContext=" << rootContext;
//
//     if (rootContext) {
//         qDebug() << "rootContext->isValid()" << rootContext->isValid();
//     }

//     MainWindow mainWin(doIt);
//     mainWin.show();

    return app.exec();
}
