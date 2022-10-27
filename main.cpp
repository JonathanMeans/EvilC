#include <filesystem>
#include <fstream>
#include <iostream>

#include "ArgumentParser.h"
#include "backend.h"
#include "lexer.h"
#include "platform.h"

int main(int argc, char** argv)
{
    ArgumentParser argparse(argv[0]);
    argparse.addOption("-o", "outputFilename");
    argparse.addOption("inputFile");

    std::map<std::string, std::string> args;
    try
    {
        args = argparse.parse(argc, argv);
    }
    catch (const ArgumentParser::ParseError& err)
    {
        std::cerr << err.what();
        return 1;
    }

    const std::string inputFile = args["inputFile"];
    std::ifstream inputStream(inputFile);
    if (!inputStream)
    {
        std::cerr << "Input file " << inputFile << " does not exist\n";
        return 2;
    }

    std::filesystem::path inputPath(inputFile);
    ErrorReporter errors(std::cerr);
    Lexer lexer(inputStream, errors, {false});
    while (lexer.hasNext())
        lexer.next();
    if (errors.hasErrors())
        return 1;
    std::filesystem::path outputPath;
    if (args.find("-o") != args.end())
    {
        outputPath = args["-o"];
    }
    else
    {
        outputPath = inputPath.stem();
        outputPath += ".out";
    }

    std::filesystem::path llvmIrPath(inputPath.stem());
    llvmIrPath += ".ll";

    // TODO: actually do the work
    {
        std::ofstream llvmIrStream(llvmIrPath);
        llvmIrStream << R"(
define i32 @main() {   
  ret i32 0
} )";
    }

    generateExecutableFromBytecode(Platform(), llvmIrPath, outputPath);
    return 0;
}
