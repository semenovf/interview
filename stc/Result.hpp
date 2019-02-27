#pragma once
#include <QString>

class Result
{
private:
    Result (bool isError)
        : _isError{isError}
    {}

public:
    Result () : _isError{true} {} // Need by Qt's Meta subsystem
    Result (Result const &) = default;
    Result (Result &&) = default;
    Result & operator = (Result const &) = default;
    Result & operator = (Result &&) = default;
    ~Result () = default;

    static Result makeOk (double value)
    {
        Result r{false};
        r._result.value = value;
        return r;
    }

    static Result makeError (int errorCode)
    {
        Result r{true};
        r._result.errorCode = errorCode;
        return r;
    }

    bool isError () const { return _isError; }
    double result () const { return _result.value; }
    int error () const { return _result.errorCode; }

    static QString toErrorString (int errorCode);

private:
    bool _isError;
    union { double value; int errorCode; } _result;
};
