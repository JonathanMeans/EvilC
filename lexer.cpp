#include "lexer.h"
#include <cassert>
#include <map>

namespace
{
void skipWhitespace(std::istream& source)
{
    while (isspace(source.peek()))
        source.get();
}

const std::map<std::string, TokenType> KEYWORDS = {{"int", TokenType::INT},
                                                   {"return",
                                                    TokenType::RETURN}};
}

bool Lexer::Token::operator==(const Token& rhs) const
{
    return this->type == rhs.type && this->lexeme == rhs.lexeme;
}

Lexer::Lexer(std::istream& source) : mSource(source)
{
}

bool Lexer::hasNext() const
{
    return mSource.good();
}

Lexer::Token Lexer::next()
{
    std::string lexeme;
    skipWhitespace(mSource);
    char c = mSource.get();
    if (isalpha(c))
    {
        lexeme.push_back(c);
        while (isalnum(mSource.peek()))
        {
            lexeme.push_back(mSource.get());
        }

        const auto keywordIt = KEYWORDS.find(lexeme);
        const auto tokenType = keywordIt == KEYWORDS.end()
                ? TokenType::IDENTIFIER
                : keywordIt->second;
        return {tokenType, lexeme};
    }
    else if (isdigit(c))
    {
        lexeme.push_back(c);
        while (isdigit(mSource.peek()))
        {
            lexeme.push_back(mSource.get());
        }
        return {TokenType::INTEGER, lexeme};
    }
    else if (c == '{')
        return {TokenType::LBRACE, "{"};
    else if (c == '}')
        return {TokenType::RBRACE, "}"};
    else if (c == '(')
        return {TokenType::LPAREN, "("};
    else if (c == ')')
        return {TokenType::RPAREN, ")"};
    else if (c == ';')
        return {TokenType::SEMICOLON, ";"};
    else if (c == EOF)
        return {TokenType::EOS, ""};
    else
        return {TokenType::ERROR, std::string(1, c)};
}
