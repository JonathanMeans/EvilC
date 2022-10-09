#include "lexer.h"
#include <gtest/gtest.h>
#include <sstream>

TEST(LexerTest, TestExpectedLexemes)
{
    std::stringstream ss;
    ss << "int main() { return 0; }\n@";
    ss.seekg(0);

    Lexer lexer(ss);
    std::vector<Lexer::Token> lexemes;
    while (lexer.hasNext())
    {
        lexemes.push_back(lexer.next());
    }
    std::vector<Lexer::Token> expected = {{TokenType::INT, "int"},
                                          {TokenType::IDENTIFIER, "main"},
                                          {TokenType::LPAREN, "("},
                                          {TokenType::RPAREN, ")"},
                                          {TokenType::LBRACE, "{"},
                                          {TokenType::RETURN, "return"},
                                          {TokenType::INTEGER, "0"},
                                          {TokenType::SEMICOLON, ";"},
                                          {TokenType::RBRACE, "}"},
                                          {TokenType::ERROR, "@"},
                                          {TokenType::EOS, ""}};
    EXPECT_EQ(expected.size(), lexemes.size());
    for (size_t ii = 0; ii < expected.size(); ++ii)
    {
        EXPECT_TRUE(expected[ii] == lexemes[ii]) << lexemes[ii].lexeme;
    }
}
