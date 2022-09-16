#include "ArgumentParser.h"
#include <gtest/gtest.h>

namespace
{

class CommandLine
{
public:
    explicit CommandLine(const std::vector<std::string>& args) :
        argc(static_cast<int>(args.size()))
    {
        argv = new char*[argc];
        for (size_t ii = 0; ii < argc; ++ii)
            argv[ii] = _strdup(args[ii].c_str());
    }

    ~CommandLine()
    {
        for (size_t ii = 0; ii < argc; ++ii)
            free(argv[ii]);
        delete[] argv;
    }

    int argc;
    char** argv;
};

std::map<std::string, std::string> parse(
        ArgumentParser& parser, const std::vector<std::string>& commandlineArgs)
{
    const CommandLine commandLine(commandlineArgs);
    try
    {
        return parser.parse(commandLine.argc, commandLine.argv);
    }
    catch (ArgumentParser::ParseError)
    {
        throw;
    }
}

TEST(ArgumentParserTest, UnrecognizedCommandLineOption)
{
    ArgumentParser parser("exe");
    try
    {
        parse(parser, {"exe", "-foo"});
        FAIL();
    }
    catch (const ArgumentParser::ParseError& err)
    {
        ASSERT_STREQ("exe: error: unrecognized command line option '-foo'\n",
                     err.what());
    }
}

TEST(ArgumentParserTest, MissingArgument)
{
    ArgumentParser parser("exe");
    parser.addOption("-o", "filename");
    try
    {
        parse(parser, {"exe", "-o"});
        FAIL();
    }
    catch (const ArgumentParser::ParseError& err)
    {
        ASSERT_STREQ("exe: error: missing argument after '-o'\n", err.what());
    }
}

TEST(ArgumentParserTest, ParseSingleOption)
{
    ArgumentParser parser("exe");
    parser.addOption("-foo");
    const auto result = parse(parser, {"exe", "-foo"});
    ASSERT_NE(result.find("-foo"), result.end());
    ASSERT_EQ(result.at("-foo"), "true");
}

TEST(ArgumentParserTest, ParseMissingOption)
{
    ArgumentParser parser("exe");
    parser.addOption("-foo");
    const auto result = parse(parser, {"exe"});
    ASSERT_NE(result.find("-foo"), result.end());
    ASSERT_EQ(result.at("-foo"), "");
}

TEST(ArgumentParserTest, ParseNoNamedOption)
{
    ArgumentParser parser("exe");
    parser.addOption("-o", "outputName");
    const auto result = parse(parser, {"exe"});
    ASSERT_EQ(result.find("-o"), result.end());
}

TEST(ArgumentParserTest, ParseArgument)
{
    ArgumentParser parser("exe");
    parser.addOption("-o", "filename");
    const auto result = parse(parser, {"exe", "-o", "a.out"});
    ASSERT_NE(result.find("-o"), result.end());
    ASSERT_EQ(result.at("-o"), "a.out");
}

TEST(ArgumentParserTest, ErrorOnMultipleArguments)
{
    ArgumentParser parser("exe");
    parser.addOption("-foo");
    try
    {
        parse(parser, {"exe", "-foo", "-foo"});
        FAIL();
    }
    catch (const ArgumentParser::ParseError& err)
    {
        ASSERT_STREQ("exe: error: multiple '-foo' arguments\n", err.what());
    }
}

TEST(ArgumentParserTest, ParseMultipleArguments)
{
    ArgumentParser parser("exe");
    parser.addOption("-o", "filename");
    parser.addOption("--foo");
    const auto result = parse(parser, {"exe", "-o", "a.out", "--foo"});
    ASSERT_NE(result.find("-o"), result.end());
    ASSERT_EQ(result.at("-o"), "a.out");

    ASSERT_NE(result.find("--foo"), result.end());
    ASSERT_EQ(result.at("--foo"), "true");
}

TEST(ArgumentParserTest, ParsePositionalArgument)
{
    ArgumentParser parser("exe");
    parser.addOption("filename");
    const auto result = parse(parser, {"exe", "a.out"});
    ASSERT_NE(result.find("filename"), result.end());
    ASSERT_EQ(result.at("filename"), "a.out");
}

TEST(ArgumentParserTest, MissingPositionalArguments)
{
    ArgumentParser parser("exe");
    parser.addOption("filename");
    parser.addOption("extra");
    try
    {
        const auto result = parse(parser, {"exe", "a.out"});
        FAIL();
    }
    catch (const ArgumentParser::ParseError& err)
    {
        ASSERT_STREQ("exe: error: missing positional arguments: 'extra',\n",
                     err.what());
    }
}

TEST(ArgumentParserTest, ParseFullCommandLine)
{
    ArgumentParser parser("exe");
    parser.addOption("-o", "filename");
    parser.addOption("--foo");
    parser.addOption("inputFile");
    const auto result = parse(parser, {"exe", "-o", "a.out", "filename.cpp"});
    ASSERT_NE(result.find("-o"), result.end());
    ASSERT_EQ(result.at("-o"), "a.out");

    ASSERT_NE(result.find("--foo"), result.end());
    ASSERT_EQ(result.at("--foo"), "");

    ASSERT_NE(result.find("inputFile"), result.end());
    ASSERT_EQ(result.at("inputFile"), "filename.cpp");
}

TEST(ArgumentParserTest, PositionalArgumentCantTakeArgument)
{
    ArgumentParser parser("exe");
    try
    {
        parser.addOption("filename", true);
        FAIL();
    }
    catch (const ArgumentParser::InvalidOption& err)
    {
        ASSERT_STREQ("positional argument cannot take an argument", err.what());
    }
}
}
