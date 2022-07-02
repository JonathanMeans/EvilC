#include "backend.h"

#include "file_utils.h"
#include "platform.h"

void
generateExecutableFromBytecode(const Platform& platform,
                               std::filesystem::path llvmIrPath,
                               std::filesystem::path outputPath)
{
  try {
    Platform().runProgram("clang", { "-m32", llvmIrPath.string() });
  } catch (...) {
    std::cerr << "Exception occurred calling hello\n";
  }

  std::filesystem::path compiledExePath(Platform().defaultOutputFilename);
  if (!std::filesystem::exists(compiledExePath)) {
    throw std::runtime_error(
      std::string("Clang failed to create expected file ") +
      compiledExePath.string());
  }
  deleteFile(llvmIrPath);
  deleteFile(outputPath);

  std::filesystem::copy(compiledExePath, outputPath);
  deleteFile(compiledExePath);
}
