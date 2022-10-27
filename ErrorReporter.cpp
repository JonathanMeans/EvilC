#include "ErrorReporter.h"

ErrorReporter::ErrorReporter(std::ostream& outputStream) :
    mOutputStream(outputStream), mHasErrors(false)
{
}

void ErrorReporter::reportDiagnostic(const Diagnostic& diagnostic)
{
    if (diagnostic.type == DiagnosticType::ERROR)
        mHasErrors = true;
    mOutputStream << diagnostic << "\n";
}

bool ErrorReporter::hasErrors() const
{
    return mHasErrors;
}
