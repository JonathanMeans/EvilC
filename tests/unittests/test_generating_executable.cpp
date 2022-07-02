#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "backend.h"
#include "file_utils.h"
#include "platform.h"

namespace {
class TestPlatform : public Platform
{
  void runProgram(const std::string&,
                  const std::vector<std::string>&) const override
  {
  }
};

class GenerateExeTest : public ::testing::Test
{
protected:
  std::filesystem::path mOutputPath;
  std::vector<std::filesystem::path> createdFiles;

  void SetUp() { mOutputPath = "main.out"; }
  void TearDown()
  {
    deleteFile(mOutputPath);
    for (const auto& path : createdFiles)
      deleteFile(path);
  }

  std::filesystem::path createFile(const std::string& filename,
                                   const std::string& contents)
  {
    std::filesystem::path result(filename);
    {
      std::ofstream stream(result);
      stream << contents;
    }
    createdFiles.push_back(result);
    return result;
  }
};

TEST_F(GenerateExeTest, IrFileShouldGetDeleted)
{
  const auto llvmIrPath = createFile("testinput.ll", "abcdef");
  const auto compiledExePath =
    createFile(Platform().defaultOutputFilename, "abcdef");

  const TestPlatform platform;
  generateExecutableFromBytecode(platform, llvmIrPath, mOutputPath);

  EXPECT_FALSE(std::filesystem::exists(llvmIrPath));
}

TEST_F(GenerateExeTest, OutputFileShouldBeCreatedFromClangOutput)
{
  const auto llvmIrPath = createFile("testinput.ll", "abcdef");
  const auto compiledExePath =
    createFile(Platform().defaultOutputFilename, "abcdef");

  const TestPlatform platform;
  generateExecutableFromBytecode(platform, llvmIrPath, mOutputPath);

  EXPECT_TRUE(std::filesystem::exists(mOutputPath));
}
} // namespace
