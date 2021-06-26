#pragma once
#include <QLocale>
#include <QTranslator>

class Translator
{
public:
    Translator () {}
    ~Translator () {}

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
