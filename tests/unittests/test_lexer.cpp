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

TEST_F(LexerTest, TestQuestionMarks)
{
    constructLexer("??");
    const auto lexemes = lexAll();

    const std::vector<Token> expected = {{TokenType::QUESTION, "?", {0, 1, 1}},
                                         {TokenType::QUESTION, "?", {1, 1, 2}},
                                         {TokenType::EOS, "", {2, 1, 3}}};
    EXPECT_EQ(expected.size(), lexemes.size());
    for (size_t ii = 0; ii < expected.size(); ++ii)
    {
        EXPECT_TRUE(expected[ii] == lexemes[ii]) << lexemes[ii];
    }
}

TEST_F(LexerTest, TestTrigraphs)
{
    constructLexer("??=??(??/??)??'??<??!??>??-??1");
    const auto lexemes = lexAll();

    const std::vector<Token> expected = {
            {TokenType::HASH, "#", {0, 1, 1}},
            {TokenType::LBRACKET, "[", {3, 1, 4}},
            {TokenType::ERROR, "\\", {6, 1, 7}},
            {TokenType::RBRACKET, "]", {9, 1, 10}},
            {TokenType::CARAT, "^", {12, 1, 13}},
            {TokenType::LBRACE, "{", {15, 1, 16}},
            {TokenType::PIPE, "|", {18, 1, 19}},
            {TokenType::RBRACE, "}", {21, 1, 22}},
            {TokenType::TILDE, "~", {24, 1, 25}},
            {TokenType::QUESTION, "?", {27, 1, 28}},
            {TokenType::QUESTION, "?", {28, 1, 29}},
            {TokenType::INTEGER, "1", {29, 1, 30}},
            {TokenType::EOS, "", {30, 1, 31}}};

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

TEST_F(LexerTest, CanHandleMaximumLineLength)
{
    // 2.2.4.1:
    // The implementation shall be able to translate and execute at least one
    // program that contains at least one instance of every one
    // of the following limits:
    // [...]
    // * 509 characters in a logical source line
    // We'll be generous and give them an extra character to play with
    std::vector<char> buffer(510);
    for (size_t vectorIndex = 0; vectorIndex < buffer.size(); ++vectorIndex)
    {
        if (vectorIndex % 2 == 0)
            buffer[vectorIndex] = ' ';
        else
            buffer[vectorIndex] = 'a';
    }
    buffer[509] = '\0';

    constructLexer(buffer.data());
    lexAll();

    ASSERT_FALSE(mErrorReporter->hasErrors());
}

TEST_F(LexerTest, ErrorOnLineTooLong)
{
    // 2.2.4.1:
    // The implementation shall be able to translate and execute at least one
    // program that contains at least one instance of every one
    // of the following limits:
    // [...]
    // * 509 characters in a logical source line
    // We'll be generous and give them an extra character to play with
    std::vector<char> buffer(513);
    for (size_t vectorIndex = 0; vectorIndex < buffer.size() - 3; ++vectorIndex)
    {
        if (vectorIndex % 2 == 0)
            buffer[vectorIndex] = ' ';
        else
            buffer[vectorIndex] = 'a';
    }
    // Invalid characters
    buffer[510] = 'b';
    buffer[511] = 'b';
    buffer[512] = '\0';

    constructLexer(buffer.data());

    try
    {
        lexAll();
        FAIL();
    }
    catch (const EnvironmentalLimitsException& e)
    {
        ASSERT_STREQ("Line 1 is too long", e.what());
    }
    catch (...)
    {
        FAIL() << "Unexpected exception\n";
    }

    ASSERT_FALSE(mErrorReporter->hasErrors());
}
