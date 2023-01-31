//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#define TOKENCOUNT 	 10
#define TOKENSIZE 	100
//-------------------------------------------------------------------------------------
class Source {

public:
	Source();
 	~Source();
	void strTokenizer(std::string _input);
 	void run(std::string _name);

protected:
	char** argv;
};
//-------------------------------------------------------------------------------------


