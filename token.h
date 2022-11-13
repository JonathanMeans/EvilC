#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>
#include <string>

struct FileLocation
{
    FileLocation();
    FileLocation(unsigned int, unsigned int, unsigned int);

    unsigned int offset;
    unsigned int line;
    unsigned int column;

    bool operator==(const FileLocation&) const;
    void increment(char c);
};

enum class TokenType
{
    // Punctuators
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    LPAREN,
    RPAREN,
    SEMICOLON,
    HASH,
    CARAT,
    PIPE,
    TILDE,
    QUESTION,

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

struct Token
{
    TokenType type;
    std::string lexeme;
    FileLocation location;

    bool operator==(const Token&) const;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
