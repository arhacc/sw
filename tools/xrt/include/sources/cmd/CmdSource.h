//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <stdio.h>
#include <Globals.h>
#include "readline/readline.h"
#include "readline/history.h"
#include "sources/mux/MuxSource.h"
#include "sources/cmd/terminal/Terminal.h"

//-------------------------------------------------------------------------------------
class CmdSource : public Source {

public:
	CmdSource(MuxSource* _muxSource);
 	~CmdSource();
	void initShell();
 	void progress();
 	void run();
 	void runCommand(std::string _name);
	std::string get_input(std::string _prompt);
 	
private:
	Terminal* terminal;
	MuxSource* muxSource;
};
//-------------------------------------------------------------------------------------


