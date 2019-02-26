#pragma once
#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
public:
    Console (QWidget * parent = nullptr);
    virtual ~Console () {}
};
