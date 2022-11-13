#include "ErrorReporter.h"
#include <cassert>
#include <fstream>

void Diagnostic::print(std::ostream& os,
                       std::istream& filestream,
                       const std::string& filename) const
{
    os << filename << ":" << token.location.line << ":" << token.location.column
       << ": ";
    switch (type)
    {
    case DiagnosticType::ERROR:
        os << "error: ";
        break;
    default:
        throw std::runtime_error("Unhandled switch statement");
    }
    os << message << "\n";

    const auto originalStreamPosition = filestream.tellg();
    filestream.seekg(0, std::ios::beg);
    os << token.location.line << " | ";
    const size_t linesToSkip = token.location.line - 1;
    char buffer[1024]; // Max line length of 510 is already enforced, so this is
                       // safe
    for (size_t skippedLines = 0; skippedLines < linesToSkip; ++skippedLines)
        filestream.getline(buffer, 4096);
    if (!filestream.good())
        int x = 0;
    assert(filestream.good());
    filestream.getline(buffer, 4096);
    os << buffer << "\n";

    os << "  | ";
    const auto spaces = std::string(token.location.column - 1, ' ');
    os << spaces;
    os << "^\n";

    filestream.seekg(originalStreamPosition, std::ios::beg);
}

ErrorReporter::ErrorReporter(std::ostream& outputStream,
                             std::istream& filestream,
                             const std::string& filename) :
    mOutputStream(outputStream),
    mFileStream(filestream),
    mFileName(filename.empty() ? "<source>" : filename), mHasErrors(false)
{
}

void ErrorReporter::reportDiagnostic(const Diagnostic& diagnostic)
{
    if (diagnostic.type == DiagnosticType::ERROR)
        mHasErrors = true;
    diagnostic.print(mOutputStream, mFileStream, mFileName);
}

bool ErrorReporter::hasErrors() const
{
    return mHasErrors;
}
