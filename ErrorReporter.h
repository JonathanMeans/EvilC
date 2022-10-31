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

    void print(std::ostream& os,
               std::istream& filestream,
               const std::string& filename) const;
};

class ErrorReporter
{
public:
    ErrorReporter(std::ostream& outputStream,
                  std::istream& filestream,
                  const std::string& filename = "");

    void reportDiagnostic(const Diagnostic&);
    bool hasErrors() const;

private:
    std::ostream& mOutputStream;
    std::istream& mFileStream;
    const std::string mFileName;
    bool mHasErrors;
};

#endif
