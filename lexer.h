#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <string>

enum class TokenType
{
    // Punctuators
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    SEMICOLON,

    // Constants
    INTEGER,

    // Keywords and identifiers
    INT,
    RETURN,
    IDENTIFIER,

    // Special
    EOS,
    ERROR
};

class Lexer
{
public:
    struct Token
    {
        TokenType type;
        std::string lexeme;

        bool operator==(const Token&) const;
    };

    struct Options
    {
        bool rot13 = false;
    };

    explicit Lexer(std::istream& source, const Options& options = Options());

    bool hasNext() const;

    Token next();

private:
    char peek() const;
    char get();

    std::istream& mSource;
    Options mOptions;
};
#endif
