#ifndef LEXER_H
#define LEXER_H

#include "ErrorReporter.h"
#include "token.h"
#include <istream>
#include <string>

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
    char peek() const;
    char get();
    void skipWhitespace();

    std::istream& mSource;
    ErrorReporter& mErrors;
    Options mOptions;
    FileLocation mLocation;
};
#endif
