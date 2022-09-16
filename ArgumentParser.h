#ifndef EVILC_ARGUMENT_PARSER_H
#define EVILC_ARGUMENT_PARSER_H
#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class ArgumentParser
{
    struct Option
    {
        std::string name;
        bool takesArgument;
    };

public:
    class ParseError : public std::runtime_error
    {
    public:
        ParseError(const char* message);
    };
    class InvalidOption : public std::runtime_error
    {
    public:
        InvalidOption(const char* message);
    };

    explicit ArgumentParser(const std::string& programName);

    void addOption(const std::string& name, bool takesArgument = false);

    std::map<std::string, std::string> parse(int argc, char** argv);

private:
    std::string mProgramName;
    std::vector<Option> mOptions;
    std::vector<std::string> mPositionalArguments;
    std::map<std::string, std::string> mDefaultResults;
};
#endif
