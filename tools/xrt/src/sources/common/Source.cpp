//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/common/Source.h>
#include <cstring>
#include <iostream>

//-------------------------------------------------------------------------------------
Source::Source() {
    argv = new char *[TOKENCOUNT];
    for (unsigned int i = 0; i < TOKENCOUNT; i++) {
        argv[i] = new char[TOKENSIZE];
    }
}

//-------------------------------------------------------------------------------------
Source::~Source() {
    delete[] argv;
}

//-------------------------------------------------------------------------------------
void Source::strTokenizer(std::string _input) {
    char *stringTokenized;
    char _buff[100];
    strcpy(_buff, _input.c_str());
    //  printf("_buff= %s\n", _buff);
    stringTokenized = strtok(_buff, " ");
    int _index = 0;
    while (stringTokenized != NULL) {
        //    argv[_index]  = stringTokenized;
        strcpy(argv[_index], stringTokenized);
        //    printf("stringTokenized= %s, _index=%d, aaa=%s\n", stringTokenized, _index, argv[_index]);
        stringTokenized = strtok(NULL, " ");
        _index++;
    }
    for (; _index < TOKENCOUNT; _index++) {
        argv[_index][0] = 0;
    }
}

//-------------------------------------------------------------------------------------
void Source::run(std::string _name) {
    std::cout << "Source.... " << std::endl;
}

//-------------------------------------------------------------------------------------
