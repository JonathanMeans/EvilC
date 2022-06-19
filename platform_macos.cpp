#if defined(__APPLE__)
#include <unistd.h>

#include <vector>

#include "platform.h"

PlatformMacOS::PlatformMacOS() : PlatformBase("a.out")
{
}

void PlatformMacOS::runProgram(const std::string& programName,
                               const std::vector<std::string>& arguments) const
{
    int fds[2];  // an array that will hold two file descriptors
    pipe(fds);  // populates fds with two file descriptors
    pid_t child_process = fork();
    if (child_process == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (child_process > 0)
    {
        // parent process
        std::cout << "spawn child with pid - " << child_process << std::endl;
        close(fds[1]);
        int status;
        if (wait(&status) == -1)
        {
            perror("wait");
        }
        char buff[1000] = {};
        ssize_t returnCode = read(fds[0], buff, 1000);
        if (returnCode == -1)
        {
            perror("read");
        }
        std::cout << "buffer - " << buff << std::endl;
    }
    else
    {
        // child process
        dup2(fds[1], STDERR_FILENO);
        close(fds[0]);  // file descriptor no longer needed in child since stdin
                        // is a copy
        close(fds[1]);  // file descriptor unused in child
        int n = arguments.size();
        // TODO: Free arg_list
        char** arg_list = new char*[n + 1];
        for (int i = 0; i < n; i++)
        {
            char* arg = strdup(arguments[i].c_str());
            arg_list[i] = arg;
        }
        arg_list[n] = nullptr;
        execvp(programName.c_str(), arg_list);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
}
#endif
