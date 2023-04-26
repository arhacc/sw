//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <sources/mux/MuxSource.h>
#include <cstring>
#include <common/Utils.h>
#include <sources/cmd/rxterm/terminal.hpp>
#include <sources/cmd/rxterm/style.hpp>
#include <stdexcept>
#include <utility>

#include <cinttypes>


//-------------------------------------------------------------------------------------
MuxSource::MuxSource(Transformers *_transformers) {
    transformers = _transformers;
    //    argv = new char[TOKENCOUNT][TOKENSIZE];
}

//-------------------------------------------------------------------------------------
void MuxSource::runCommand(std::string _command) {

    //  std::cout << ">MuxSource[" << _command << "]" << std::endl;

    //  char input[250];
    //  std::cin.getline(input,250);
    //  char *_argv[TOKENSIZE];
    strTokenizer(std::move(_command));
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
void MuxSource::runCommand(char **_argv) {

    //  std::cout << ">MuxSource[" << _command << "]" << std::endl;
    //  std::cout << "_cmdString=" << _cmdString << "..." << std::endl;
    //  std::cout << "_argString=" << _argString << "..." << std::endl;

    if (strcmp(_argv[0], "") == 0) {

    } else if (strcmp(_argv[0], "prun") == 0) {
        std::string _path(_argv[1]);
        transformers->run(_path);
    } else if (strcmp(_argv[0], "source") == 0) {
        std::string _path(_argv[1]);
        transformers->load(_path);
    } else if (strcmp(_argv[0], "pload") == 0) {

        std::string _name(_argv[1]);
        uint32_t _address;
        
        if (sscanf(_argv[2], "%" SCNx32, &_address) != 1) {
            throw std::runtime_error("invalid address");
        }

        transformers->uploadFunction(_name, _address);
    } else if ((strcmp(_argv[0], "exit") == 0) || (strcmp(_argv[0], "quit") == 0) || (strcmp(_argv[0], "q") == 0)) {
        std::cout << "Exiting..." << std::endl;
        signalHandler(0);
        //    exit(0);
    } else if ((strcmp(_argv[0], "dump") == 0) || (strcmp(_argv[0], "d") == 0)) {
        std::cout << "Dumping..." << std::endl;
        transformers->dump(_argv[1]);
        //    exit(0);
    }
}


//-------------------------------------------------------------------------------------
