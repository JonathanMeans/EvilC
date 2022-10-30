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

    void print(std::ostream& os, const std::string& filename) const;
};

class ErrorReporter
{
public:
    explicit ErrorReporter(std::ostream& outputStream,
                           const std::string& filename = "");
    void reportDiagnostic(const Diagnostic&);
    bool hasErrors() const;

private:
    std::ostream& mOutputStream;
    const std::string mFileName;
    bool mHasErrors;
};

#endif
