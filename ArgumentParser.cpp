#include "ArgumentParser.h"

namespace
{
std::string unrecognizedOption(const std::string& programName,
                               const char* option)
{
    std::stringstream ss;
    ss << programName << ": error: unrecognized command line option '" << option
       << "'\n";
    return ss.str();
}

std::string missingArgument(const std::string& programName,
                            const std::string& name)
{
    std::stringstream ss;
    ss << programName << ": error: missing argument after '" << name << "'\n";
    return ss.str();
}

std::string multipleArguments(const std::string& programName,
                              const std::string& name)
{
    std::stringstream ss;
    ss << programName << ": error: multiple '" << name << "' arguments\n";
    return ss.str();
}

std::string missingPositionalArguments(
        const std::string& programName,
        const std::vector<std::string>& positionalArguments,
        size_t index)
{
    std::stringstream ss;
    ss << programName << ": error: missing positional arguments:";
    for (size_t ii = index; ii < positionalArguments.size(); ++ii)
        ss << " '" << positionalArguments[ii] << "',";
    ss << "\n";
    return ss.str();
}

void verifyPositionalArgumentsWereConsumed(
        const std::string& programName,
        const std::vector<std::string>& positionalArguments,
        size_t index)
{
    if (index < positionalArguments.size())
    {
        const auto message = missingPositionalArguments(programName,
                                                        positionalArguments,
                                                        index);
        throw ArgumentParser::ParseError(message.c_str());
    }
}

bool isOption(const char* text)
{
    if (text == NULL)
        return false;
    return text[0] == '-';
}
}

ArgumentParser::ParseError::ParseError(const char* message) :
    std::runtime_error(message)
{
}

ArgumentParser::InvalidOption::InvalidOption(const char* message) :
    std::runtime_error(message)
{
}

ArgumentParser::ArgumentParser(const std::string& programName) :
    mProgramName(programName)
{
}

void ArgumentParser::addOption(const std::string& name, bool takesArgument)
{
    if (isOption(name.c_str()))
    {
        mOptions.push_back(Option(name, takesArgument));
        if (!takesArgument)
            mDefaultResults[name] = "";
    }
    else
    {
        if (takesArgument)
            throw InvalidOption("positional argument cannot take an argument");
        mPositionalArguments.push_back(name);
    }
}

std::map<std::string, std::string> ArgumentParser::parse(int argc, char** argv)
{
    auto result = mDefaultResults;
    size_t positionalArgumentIndex = 0;
    // TODO: This is a bit of a mess, but it works...
    for (int i = 1; i < argc; ++i)
    {
        char* currentArg = argv[i];
        if (isOption(currentArg))
        {
            for (const auto& option : mOptions)
            {
                if (option.name == currentArg)
                {
                    if (!result[option.name].empty())
                        throw ParseError(
                                multipleArguments(mProgramName, option.name)
                                        .c_str());
                    if (option.takesArgument)
                    {
                        i++;
                        if (i == argc || isOption(argv[i]))
                            throw ParseError(
                                    missingArgument(mProgramName, option.name)
                                            .c_str());
                        result[option.name] = argv[i];
                    }
                    else
                    {
                        result[option.name] = "true";
                    }
                    break;
                }
            }

            if (result.find(currentArg) == result.end())
            {
                throw ParseError(
                        unrecognizedOption(mProgramName, argv[i]).c_str());
            }
        }
        else
        {
            result[mPositionalArguments[positionalArgumentIndex++]] =
                    currentArg;
        }
    }

    verifyPositionalArgumentsWereConsumed(mProgramName,
                                          mPositionalArguments,
                                          positionalArgumentIndex);
    return result;
}
