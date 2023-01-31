//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/mux/MuxSource.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <chrono>

#include <sources/cmd/rxterm/terminal.hpp>
#include <sources/cmd/rxterm/style.hpp>
#include <sources/cmd/rxterm/image.hpp>
#include <sources/cmd/rxterm/components/text.hpp>
#include <sources/cmd/rxterm/components/stacklayout.hpp>
#include <sources/cmd/rxterm/components/flowlayout.hpp>
#include <sources/cmd/rxterm/components/progress.hpp>
#include <sources/cmd/rxterm/components/maxwidth.hpp>

//-------------------------------------------------------------------------------------
MuxSource::MuxSource(Transformers* _transformers){
    transformers = _transformers;
//    argv = new char[TOKENCOUNT][TOKENSIZE];
}

//-------------------------------------------------------------------------------------
MuxSource::~MuxSource(){
//  delete[] argv;
}

//-------------------------------------------------------------------------------------
void MuxSource::runCommand(std::string _command){

//  std::cout << ">MuxSource[" << _command << "]" << std::endl;

//  char input[250];
//  std::cin.getline(input,250);
//  char *_argv[TOKENSIZE];
  strTokenizer(_command);
//  printf( "xxx._command= %s\n", _command.c_str() );
//  printf( "xxx.argv0= %s\n", argv[0]);
//  printf( "xxx.argv1= %s\n", argv[1]);


//  std::string _cmdString(argv[0]);
//  std::string _argString(argv[1]);

//  std::cout << "_cmdString=" << _cmdString << "..." << std::endl;
//  std::cout << "_argString=" << _argString << "..." << std::endl;
  runCommand(argv);
}

//-------------------------------------------------------------------------------------
void MuxSource::runCommand(char** _argv){

//  std::cout << ">MuxSource[" << _command << "]" << std::endl;
//  std::cout << "_cmdString=" << _cmdString << "..." << std::endl;
//  std::cout << "_argString=" << _argString << "..." << std::endl;

  if(strcmp(_argv[0], "") == 0){

  } else if(strcmp(_argv[0], "run") == 0){
    std::string _path(_argv[1]);
    transformers -> runFile(_path);
  } else if((strcmp(_argv[0], "exit") == 0) || (strcmp(_argv[0], "quit") == 0) || (strcmp(_argv[0], "q") == 0)){
    exit(0);
  }
}


//-------------------------------------------------------------------------------------
