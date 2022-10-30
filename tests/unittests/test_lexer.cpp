#include "lexer.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(LexerTest, LexerWithRot13Encoding)
{
    std::stringstream ss;
    ss << "vag znva() {\nerghea 0; }\n";
    ss.seekg(0);

    Lexer::Options options;
    options.rot13 = true;
    ErrorReporter errors(std::cout);
    Lexer lexer(ss, errors, options);
    std::vector<Token> lexemes;
    while (lexer.hasNext())
    {
        lexemes.push_back(lexer.next());
    }
    const std::vector<Token> expected = {
            {TokenType::INT, "int", {0, 1, 1}},
            {TokenType::IDENTIFIER, "main", {4, 1, 5}},
            {TokenType::LPAREN, "(", {8, 1, 9}},
            {TokenType::RPAREN, ")", {9, 1, 10}},
            {TokenType::LBRACE, "{", {11, 1, 12}},
            {TokenType::RETURN, "return", {13, 2, 1}},
            {TokenType::INTEGER, "0", {20, 2, 8}},
            {TokenType::SEMICOLON, ";", {21, 2, 9}},
            {TokenType::RBRACE, "}", {23, 2, 11}},
            {TokenType::EOS, "", {25, 3, 1}}};
    EXPECT_EQ(expected.size(), lexemes.size());
    for (size_t ii = 0; ii < expected.size(); ++ii)
    {
        EXPECT_TRUE(expected[ii] == lexemes[ii]) << lexemes[ii];
    }
}

TEST(LexerTest, LexErrors)
{
    std::stringstream ss;
    ss << "int@";
    ss.seekg(0);

    std::stringstream errorStream;
    ErrorReporter errors(errorStream);
    Lexer lexer(ss, errors);

    while (lexer.hasNext())
        lexer.next();

    ASSERT_TRUE(errors.hasErrors());
    const auto errorMessage = errorStream.str();
    ASSERT_STREQ("<source>:1:4: error: stray '@' in program\n",
                 errorMessage.c_str());
}

TEST(LexerTest, TestExpectedLexemes)
{
    std::stringstream ss;
    ss << "int main() {\nreturn 0; }\n";
    ss.seekg(0);

    ErrorReporter errors(std::cout);
    Lexer lexer(ss, errors);
    std::vector<Token> lexemes;
    while (lexer.hasNext())
    {
        lexemes.push_back(lexer.next());
    }
    const std::vector<Token> expected = {
            {TokenType::INT, "int", {0, 1, 1}},
            {TokenType::IDENTIFIER, "main", {4, 1, 5}},
            {TokenType::LPAREN, "(", {8, 1, 9}},
            {TokenType::RPAREN, ")", {9, 1, 10}},
            {TokenType::LBRACE, "{", {11, 1, 12}},
            {TokenType::RETURN, "return", {13, 2, 1}},
            {TokenType::INTEGER, "0", {20, 2, 8}},
            {TokenType::SEMICOLON, ";", {21, 2, 9}},
            {TokenType::RBRACE, "}", {23, 2, 11}},
            {TokenType::EOS, "", {25, 3, 1}}};

    EXPECT_EQ(expected.size(), lexemes.size());
    for (size_t ii = 0; ii < expected.size(); ++ii)
    {
        EXPECT_TRUE(expected[ii] == lexemes[ii]) << lexemes[ii];
    }
}
