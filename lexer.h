#ifndef LEXER_H
#define LEXER_H

#include "ErrorReporter.h"
#include "token.h"
#include <istream>
#include <stdexcept>
#include <string>

class EnvironmentalLimitsException : public std::runtime_error
{
public:
    EnvironmentalLimitsException(const char* msg);
};

class Lexer
{
public:
    struct Options
    {
        bool rot13 = false;
    };

    explicit Lexer(std::istream& source,
                   ErrorReporter& errors,
                   const Options& options = Options());

    bool hasNext() const;

    Token next();

private:
    char getNextChar();
    void skipWhitespace();

    bool mHasNext;
    std::istream& mSource;
    ErrorReporter& mErrors;
    Options mOptions;
    FileLocation mLocation;
};
#endif
