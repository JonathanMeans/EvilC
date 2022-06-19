#include <filesystem>
#include <fstream>
#include <iostream>

#include "platform.h"

int main(int argc, char** argv)
{
    /*
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << "<input file>\n";
        return 1;
    }

    const std::string inputFile = argv[1];
    std::ifstream inputStream(inputFile);
    if (!inputStream)
    {
        std::cerr << "Input file " << argv << " does not exist\n";
        return 2;
    }
    */

    // std::filesystem::path inputPath(inputFile);
    std::filesystem::path inputPath("main.ll");
    std::filesystem::path outputPath(inputPath.stem());
    outputPath += ".out";

    std::filesystem::path llvmIrPath(inputPath.stem());
    llvmIrPath += ".ll";

    {
        std::ofstream llvmIrStream(llvmIrPath);
        llvmIrStream << R"(
define i32 @main() {   
  ret i32 0
} )";
    }
    try
    {
        Platform().runProgram("clang", {"-m32", llvmIrPath.string()});
    }
    catch (...)
    {
        std::cerr << "Exception occurred calling hello\n";
    }

    std::filesystem::path compiledExePath(Platform().defaultOutputFilename);
    if (!std::filesystem::exists(compiledExePath))
    {
        throw std::runtime_error(
                std::string("Clang failed to create expected file ") +
                compiledExePath.string());
    }
    if (std::filesystem::exists(outputPath))
    {
        std::filesystem::remove(outputPath);
    }

    std::filesystem::copy(compiledExePath, outputPath);
    std::filesystem::remove(compiledExePath);

    // TODO: actually do the work

    return 0;
}
