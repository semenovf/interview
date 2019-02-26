#pragma once

class Result
{
private:
    Result (bool isError)
        : _isError{isError}
    {}

public:
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

private:
    bool _isError;
    union { double value; int errorCode; } _result;
};
