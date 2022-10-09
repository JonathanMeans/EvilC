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

    explicit Lexer(std::istream& source);

    bool hasNext() const;

    Token next();

private:
    std::istream& mSource;
};
#endif
