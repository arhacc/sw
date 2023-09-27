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
void Source::strTokenizer(std::string _input) {
    argv.erase(argv.begin(), argv.end());

    size_t _lastPos = 0;
    size_t _pos;

    do {
        _pos = _input.find(" ", _lastPos);

        size_t _endOfTokenPos = (_pos != std::string::npos) ? _pos : _input.length();

        if (_endOfTokenPos - _lastPos > 0)
            argv.push_back(_input.substr(_lastPos, _endOfTokenPos - _lastPos));

        _lastPos = _pos + 1;
    } while (_pos != std::string::npos);
}

//-------------------------------------------------------------------------------------
void Source::run(std::string _name) {
    std::cout << "Source.... " << std::endl;
}

//-------------------------------------------------------------------------------------
