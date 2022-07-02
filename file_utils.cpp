#include "file_utils.h"

void
deleteFile(const std::filesystem::path& path)
{
  if (std::filesystem::exists(path))
    std::filesystem::remove(path);
}
