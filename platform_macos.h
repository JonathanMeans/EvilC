
#ifndef EVILC_PLATFORM_MACOS_H
#define EVILC_PLATFORM_MACOS_H

#include <iostream>
#include <unistd.h>
#include <vector>

void runProgram(const std::string& programName, const std::vector<std::string>& arguments)
{
    pid_t child_process = fork();
    if (child_process == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_process > 0) {
        std::cout << "spawn child with pid - " << child_process << std::endl;
    } else {
        int n = arguments.size();
        char ** arg_list = new char *[n];
        for (int i = 0; i < n; i++) {
            arg_list[i] = strdup(arguments[i].c_str());
        }
        execve(programName.c_str(), arg_list, nullptr);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}
#endif  // EVILC_PLATFORM_MACOS_H
