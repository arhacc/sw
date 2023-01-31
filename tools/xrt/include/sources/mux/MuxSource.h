//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <Globals.h>

//-------------------------------------------------------------------------------------
class MuxSource : public Source {

public:
	MuxSource(Transformers* _transformers);
 	~MuxSource();
 	void runCommand(std::string _name);
 	void runCommand(char** _argv);
 	
private:
	Transformers* transformers;
};
//-------------------------------------------------------------------------------------


