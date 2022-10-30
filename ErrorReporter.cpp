#include "ErrorReporter.h"

void Diagnostic::print(std::ostream& os, const std::string& filename) const
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
}

ErrorReporter::ErrorReporter(std::ostream& outputStream,
                             const std::string& filename) :
    mOutputStream(outputStream),
    mFileName(filename), mHasErrors(false)
{
}

void ErrorReporter::reportDiagnostic(const Diagnostic& diagnostic)
{
    if (diagnostic.type == DiagnosticType::ERROR)
        mHasErrors = true;
    diagnostic.print(mOutputStream, mFileName);
}

bool ErrorReporter::hasErrors() const
{
    return mHasErrors;
}
