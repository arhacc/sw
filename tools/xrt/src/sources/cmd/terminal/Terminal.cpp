//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <chrono>

#include "sources/cmd/terminal/Terminal.h"

std::vector<std::string> TERMINAL_COMMANDS {
                    "ll", 
                    "ls", 
                    "pwd", 
                    "cd",
                    "w"
};


//-------------------------------------------------------------------------------------
Terminal::Terminal(){
}

//-------------------------------------------------------------------------------------
Terminal::~Terminal(){
}

//-------------------------------------------------------------------------------------
bool Terminal::isValidCommand(char** _argv){
//  std::cout << ">1.[" << _line << "]" << std::endl;
  for(auto _terminalCommand : TERMINAL_COMMANDS){
//    std::cout << ">2.[" << _terminalCommand << "]" << std::endl;
    if(strcmp(_terminalCommand.c_str(), _argv[0]) == 0){
//      std::cout << ">3.[" << _line << "]" << std::endl;
      return 1;
    }
  }
  return 0;
}

//-------------------------------------------------------------------------------------
std::string Terminal::runCommand(char** _argv){
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
    }
    return "";
}
//-------------------------------------------------------------------------------------
