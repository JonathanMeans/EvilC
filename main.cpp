#include <filesystem>
#include <fstream>
#include <iostream>

#include "backend.h"
#include "platform.h"

int main(int argc, char** argv)
{
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

    std::filesystem::path inputPath(inputFile);
    std::filesystem::path outputPath(inputPath.stem());
    outputPath += ".out";

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
