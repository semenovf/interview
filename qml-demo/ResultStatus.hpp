#pragma once
#include <QString>

class ResultStatus
{
public:
    ResultStatus ()
        : _status(true)
    {}

    ResultStatus (QString const & errstr)
        : _status(false)
        , _errstr(errstr)
    {}

    ResultStatus (QString && errstr)
        : _status(false)
        , _errstr(std::forward<QString>(errstr))
    {}

    ResultStatus (ResultStatus const &) = default;
    ResultStatus (ResultStatus &&) = default;
    ResultStatus & operator = (ResultStatus const &) = default;
    ResultStatus & operator = (ResultStatus &&) = default;
    ~ResultStatus () = default;

    operator bool () const
    {
        return _status;
    }

    QString const & what () const
    {
        return _errstr;
    }

private:
    bool    _status;
    QString _errstr;
};
