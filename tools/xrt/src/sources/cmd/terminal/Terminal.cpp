//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdio>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include "sources/cmd/terminal/Terminal.h"

std::vector<std::string> Terminal::TERMINAL_COMMANDS{"ll", "ls", "pwd", "cd", "w"};

//-------------------------------------------------------------------------------------
bool Terminal::isValidCommand(char **_argv) {
    std::string firstArg = _argv[0];
    return std::ranges::any_of(TERMINAL_COMMANDS.begin(), TERMINAL_COMMANDS.end(),
            [&firstArg](const auto &s) { return s == firstArg; });
}

//-------------------------------------------------------------------------------------
std::string Terminal::runCommand(char **_argv) {
    /*
    //   int _exitCode = std::system(_line.c_str());
      char buffer[128];
      std::string result = "";

       // Open pipe to file
       FILE* pipe = popen(_argv[0], "r");
       if (!pipe) {
          return "popen failed!";
       }

       // read till end of process:
       while (!feof(pipe)) {

          // use buffer to read and add to result
          if (fgets(buffer, 128, pipe) != NULL)
             result += buffer;
       }

       pclose(pipe);
       return result;
    */
    //    system("DATE");
    int pid = fork();
    if (pid == 0) {
        /*
         * A return value of 0 means this is the child process that we will use
         * to execute the shell command.
         */
        char buffer[128];
        std::string result;

        // Open pipe to file
        FILE *pipe = popen(_argv[0], "r");
        if (!pipe) {
            return "popen failed!";
        }

        // read till end of process:
        while (!feof(pipe)) {

            // use buffer to read and add to result
            if (fgets(buffer, 128, pipe) != nullptr)
                result += buffer;
        }

        pclose(pipe);
        return result;
    }
    return "";
}
//-------------------------------------------------------------------------------------
