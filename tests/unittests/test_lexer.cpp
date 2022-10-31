#include "lexer.h"
#include <gtest/gtest.h>
#include <sstream>

class LexerTest : public ::testing::Test
{
protected:
    void constructLexer(const char* input, const Lexer::Options& options = {})
    {
        mInputStream << input;
        mInputStream.seekg(0);

        mErrorReporter =
                std::make_unique<ErrorReporter>(mErrorStream, mInputStream);
        mLexer =
                std::make_unique<Lexer>(mInputStream, *mErrorReporter, options);
    }

    std::vector<Token> lexAll()
    {
        std::vector<Token> result;
        while (mLexer->hasNext())
            result.push_back(mLexer->next());
        return result;
    }

    void TearDown() override
    {
        mLexer.release();
        mErrorReporter.release();
    }

    std::unique_ptr<Lexer> mLexer;
    std::stringstream mInputStream;
    std::stringstream mErrorStream;
    std::unique_ptr<ErrorReporter> mErrorReporter;
};

TEST_F(LexerTest, LexerWithRot13Encoding)
{
    Lexer::Options options;
    options.rot13 = true;
    constructLexer("vag znva() {\nerghea 0; }\n", options);
    ASSERT_TRUE(mLexer);

    const auto lexemes = lexAll();

    ASSERT_FALSE(mErrorReporter->hasErrors());
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
TEST_F(LexerTest, TestExpectedLexemes)
{
    constructLexer("int main() {\nreturn 0; }\n");
    const auto lexemes = lexAll();

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

TEST_F(LexerTest, LexErrors)
{
    constructLexer(R"(int main()
{
    return @ 0;
})");
    ASSERT_TRUE(mLexer);

    const auto tokens = lexAll();

    EXPECT_TRUE(mErrorReporter->hasErrors());
    const auto errorMessage = mErrorStream.str();
    const auto expected =
            R"(<source>:3:12: error: stray '@' in program
3 |     return @ 0;
  |            ^
)";
    EXPECT_STREQ(expected, errorMessage.c_str());

    // Make sure reporting the error didn't mess up future lexing
    ASSERT_EQ(11, tokens.size());
    ASSERT_STREQ("}", tokens[9].lexeme.c_str());
}
