#include "lexer.h"
#include "ErrorReporter.h"
#include <cassert>
#include <map>
#include <sstream>

namespace
{
char rot13(char c)
{
    if (isupper(c))
    {
        if (c + 13 <= 'Z')
            return c + 13;
        return c - 13;
    }
    if (islower(c))
    {
        if (c + 13 <= 'z')
            return c + 13;
        return c - 13;
    }
    return c;
}

const std::map<std::string, TokenType> KEYWORDS = {{"int", TokenType::INT},
                                                   {"return",
                                                    TokenType::RETURN}};
}

bool Token::operator==(const Token& rhs) const
{
    return this->type == rhs.type && this->lexeme == rhs.lexeme &&
            this->location == rhs.location;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    os << token.location.line << ":" << token.location.column << ":"
       << token.lexeme;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Diagnostic& diagnostic)
{
    switch (diagnostic.type)
    {
    case DiagnosticType::ERROR:
        os << "error:";
        break;
    }
    os << diagnostic.token.location.line << ":"
       << diagnostic.token.location.column << ": " << diagnostic.message
       << "\n";
    return os;
}

bool FileLocation::operator==(const FileLocation& rhs) const
{
    return this->offset == rhs.offset && this->line == rhs.line &&
            this->column == rhs.column;
}

FileLocation::FileLocation(unsigned int offset,
                           unsigned int line,
                           unsigned int column) :
    offset(offset),
    line(line), column(column)
{
}

FileLocation::FileLocation() : offset(0), line(1), column(1)
{
}

void FileLocation::increment(char c)
{
    offset++;
    if (c == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }
}

Lexer::Lexer(std::istream& source,
             ErrorReporter& errors,
             const Options& options) :
    mSource(source),
    mErrors(errors), mOptions(options), mLocation()
{
}

void Lexer::skipWhitespace()
{
    while (isspace(mSource.peek()))
        get();
}

char Lexer::peek() const
{
    char c = mSource.peek();
    if (mOptions.rot13)
        c = rot13(c);
    return c;
}

char Lexer::get()
{
    char c = mSource.get();
    if (mOptions.rot13)
        c = rot13(c);
    mLocation.increment(c);
    return c;
}

bool Lexer::hasNext() const
{
    return mSource.good();
}

Token Lexer::next()
{
    std::string lexeme;
    skipWhitespace();
    const auto tokenLocation = mLocation;
    char c = get();
    if (isalpha(c))
    {
        lexeme.push_back(c);
        while (isalnum(peek()))
        {
            lexeme.push_back(get());
        }

        const auto keywordIt = KEYWORDS.find(lexeme);
        const auto tokenType = keywordIt == KEYWORDS.end()
                ? TokenType::IDENTIFIER
                : keywordIt->second;
        return {tokenType, lexeme, tokenLocation};
    }
    else if (isdigit(c))
    {
        lexeme.push_back(c);
        while (isdigit(peek()))
        {
            lexeme.push_back(get());
        }
        return {TokenType::INTEGER, lexeme, tokenLocation};
    }
    else if (c == '{')
        return {TokenType::LBRACE, "{", tokenLocation};
    else if (c == '}')
        return {TokenType::RBRACE, "}", tokenLocation};
    else if (c == '(')
        return {TokenType::LPAREN, "(", tokenLocation};
    else if (c == ')')
        return {TokenType::RPAREN, ")", tokenLocation};
    else if (c == ';')
        return {TokenType::SEMICOLON, ";", tokenLocation};
    else if (c == EOF)
        return {TokenType::EOS, "", tokenLocation};
    else
    {
        const Token result{TokenType::ERROR, std::string(1, c), tokenLocation};
        std::stringstream errorMessage;
        errorMessage << "stray '" << result.lexeme << "' in program";
        mErrors.reportDiagnostic(
                {DiagnosticType::ERROR, result, errorMessage.str()});
        return result;
    }
}
