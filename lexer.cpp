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

EnvironmentalLimitsException::EnvironmentalLimitsException(const char* msg) :
    std::runtime_error(msg)
{
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
    if (column >= 512)
    {
        // Max line length is 510 (one more than 509 in 2.2.4.1)
        // One more for EOF character
        // And columns start at 1, rather than 0, so plus one for that
        // 512 total
        std::stringstream ss;
        ss << "Line " << line << " is too long";
        throw EnvironmentalLimitsException(ss.str().c_str());
    }
}

Lexer::Lexer(std::istream& source,
             ErrorReporter& errors,
             const Options& options) :
    mHasNext(true),
    mSource(source),
    mErrors(errors), mOptions(options), mLocation()
{
}

void Lexer::skipWhitespace()
{
    while (isspace(mSource.peek()))
        getNextChar();
}

char Lexer::getNextChar()
{
    char c = mSource.get();
    const auto originalPosition = mSource.tellg();
    bool isTrigraph = false;
    if (mOptions.rot13)
        c = rot13(c);

    // Handle trigraph sequences. See 2.2.1.1
    if (c == '?')
    {
        char trigraph[3] = {};
        trigraph[0] = c;
        trigraph[1] = mSource.get();
        trigraph[2] = mSource.get();

        // If we hit the end of the stream, not enough chars to make a trigraph, so go back and return original ?
        if (!mSource.good())
        {
            mSource.clear();
            mSource.seekg(originalPosition, std::ios::beg);
            mLocation.increment(c);
            return c;
        }

        if (trigraph[1] == '?')
        {
            isTrigraph = true;
            switch (trigraph[2])
            {
            case '=':
                c = '#';
                break;
            case '(':
                c = '[';
                break;
            case '/':
                c = '\\';
                break;
            case ')':
                c = ']';
                break;
            case '\'':
                c = '^';
                break;
            case '<':
                c = '{';
                break;
            case '!':
                c = '|';
                break;
            case '>':
                c = '}';
                break;
            case '-':
                c = '~';
                break;
            default:
                isTrigraph = false;
                break;
            }
            if (!isTrigraph)
            {
                mSource.seekg(-2, std::ios::cur);
            }
        }
    }

    mLocation.increment(c);
    if (isTrigraph)
    {
        // Advance twice more to eat up the other chars
        mLocation.increment(c);
        mLocation.increment(c);
    }
    return c;
}

bool Lexer::hasNext() const
{
    return mHasNext;
}

Token Lexer::next()
{
    std::string lexeme;
    skipWhitespace();
    const auto tokenLocation = mLocation;
    char c = getNextChar();
    if (isalpha(c))
    {
        lexeme.push_back(c);
        while (isalnum(mSource.peek()))
        {
            lexeme.push_back(getNextChar());
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
        while (isdigit(mSource.peek()))
        {
            lexeme.push_back(getNextChar());
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
    else if (c == '[')
        return {TokenType::LBRACKET, "[", tokenLocation};
    else if (c == ']')
        return {TokenType::RBRACKET, "]", tokenLocation};
    else if (c == '#')
        return {TokenType::HASH, "#", tokenLocation};
    else if (c == '^')
        return {TokenType::CARAT, "^", tokenLocation};
    else if (c == '|')
        return {TokenType::PIPE, "|", tokenLocation};
    else if (c == '~')
        return {TokenType::TILDE, "~", tokenLocation};
    else if (c == ';')
        return {TokenType::SEMICOLON, ";", tokenLocation};
    else if (c == '?')
        return {TokenType::QUESTION, "?", tokenLocation};
    else if (c == EOF)
    {
        mHasNext = false;
        return {TokenType::EOS, "", tokenLocation};
    }
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
