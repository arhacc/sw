//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/cmd/CmdSource.h>
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
#include "ANSI-color-codes.h"
#include <sources/cmd/terminal/Terminal.cpp>

char* username;
const char* prompt = new char[256];

using namespace rxterm;

auto renderToTerm = [](auto const& vt, unsigned const w, Component const& c) {
  return vt.flip(c.render(w).toString());
};

std::vector<std::string> terminalCommands{"", "ll", "ls", "pwd", "cd", "exit", "quit", "q"};


//-------------------------------------------------------------------------------------
CmdSource::CmdSource(MuxSource* _muxSource){
    muxSource = _muxSource;
    terminal = new Terminal();
    initShell();
    progress();
    std::thread t(&CmdSource::run, this);
    t.detach();
}

//-------------------------------------------------------------------------------------
CmdSource::~CmdSource(){
  delete(terminal);
//  shutDownServer(_clientConnection);
}

//-------------------------------------------------------------------------------------
void CmdSource::initShell() {
//    clear();
    std::cout << RED << "************************************************************************************************************************" << std::endl;
    std::cout << RED << "******                                            " << YEL << XRT_LOGO << " Command Line" << RED << "                                           ******" << std::endl;
    std::cout << RED << "******                                          " << YEL << "Xpu RunTime © 2022-2023" << RED << "                                           ******" << std::endl;
    std::cout << RED << "************************************************************************************************************************" << COLOR_RESET << std::endl;
    username = getenv("USER");
}

//-------------------------------------------------------------------------------------
void CmdSource::progress(){
  using namespace std::chrono_literals;
  using namespace std::string_literals;

  VirtualTerminal vt;
  
//  std::string _message = "Loading "+ _name + "...";
  

  auto superProgressBar = [](auto x) -> FlowLayout<> {
    return {
//      Text(">"),
      FlowLayout<>{
        MaxWidth(120, Progress(x)),
      }
    };
  };

  auto w = VirtualTerminal::width();
  if (!w) w = 80;
  for (int i = 0; i < 101; ++i) {
    vt = renderToTerm(vt, w, superProgressBar(0.01 * i));
    std::this_thread::sleep_for(5ms);
  }  
}

//-------------------------------------------------------------------------------------
std::string CmdSource::get_input(std::string p) {
  
  std::string _line;
//  std::cout << ">2.["<<p<<"]" << std::endl;
  _line = readline(p.c_str());
//  std::cout << p;
//  std::getline(std::cin, _line);
//  std::cout << ">3.["<< _line << "]" << std::endl;
  if(_line!=""){
    add_history(_line.c_str());
  }
  return _line;
  
}

//-------------------------------------------------------------------------------------
void CmdSource::run(){
    std::string _homedir  = string(getenv("HOME"));
    std::string _user     = string(getenv("USER"));
    while(1) {
      std::string _pwd = string(getenv("PWD")).replace(0, _homedir.length(), "~");
      std::string _prompt = HGRN + _user + "@" + _pwd + ">" + CRESET;

      std::string _line = get_input(_prompt);
//      std::cout << ">0.[" << _line << "]" << std::endl;
      runCommand(_line);
//      std::cout << ">1.[" << _line << "]" << std::endl;
  }
}

//-------------------------------------------------------------------------------------
void CmdSource::runCommand(std::string _line){
//  std::cout << ">0.[" << _line << "]" << std::endl;
    strTokenizer(_line);
    if(terminal->isValidCommand(argv)) {
      std::string _result = terminal->runCommand(argv);
      std::cout << _result;
    } else {
      muxSource -> runCommand(argv);
    }
//    vector <string> commands;
//    CliTools::string_to_vect(commands, line.c_str(), " ");
}

//-------------------------------------------------------------------------------------
