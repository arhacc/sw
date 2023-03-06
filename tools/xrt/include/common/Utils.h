//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <string>
#include <vector>
#include <sstream>

//-------------------------------------------------------------------------------------
void printUsage();
void signalHandler(int _signal);
int getFileTypeFromGeneralPath(std::string _path);
int getFileTypeFromPath(std::string _path);
std::vector<std::string> split(std::string _value, std::string _separator);
inline bool endsWith(std::string const & value, std::string const & ending);
//-------------------------------------------------------------------------------------
