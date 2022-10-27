#ifndef ERROR_REPORTER_H
#define ERROR_REPORTER_H

#include "token.h"
#include <ostream>
#include <vector>

enum class DiagnosticType
{
    ERROR
};

struct Diagnostic
{
    DiagnosticType type;
    Token token;
    std::string message;
};

std::ostream& operator<<(std::ostream& os, const Diagnostic& diagnostic);

class ErrorReporter
{
public:
    explicit ErrorReporter(std::ostream& outputStream);
    void reportDiagnostic(const Diagnostic&);
    bool hasErrors() const;

private:
    std::ostream& mOutputStream;
    bool mHasErrors;
};

#endif
