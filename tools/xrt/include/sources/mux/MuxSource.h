//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/Globals.h>
#include <sources/common/Source.h>
#include <transformers/Transformers.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <span>
#include <vector>

#include <unistd.h>

//-------------------------------------------------------------------------------------
enum class MuxCommandReturnType {
    VOID,

    WORD_VECTOR,
};

//-------------------------------------------------------------------------------------
struct MuxCommandReturnValue {
    const MuxCommandReturnType type;
    union {
        std::vector<uint32_t> words;
    };

    inline MuxCommandReturnValue() : type(MuxCommandReturnType::VOID) {}

    inline MuxCommandReturnValue(const std::vector<uint32_t>& _words)
        : type(MuxCommandReturnType::WORD_VECTOR), words(_words) {}

    inline MuxCommandReturnValue(std::vector<uint32_t>&& _words)
        : type(MuxCommandReturnType::WORD_VECTOR), words(std::move(_words)) {}

    inline ~MuxCommandReturnValue() {
        switch (type) {
            case MuxCommandReturnType::VOID: {
                break;
            }

            case MuxCommandReturnType::WORD_VECTOR: {
                words.~vector();
                break;
            }
        }
    }
};

//-------------------------------------------------------------------------------------
class MuxSource : public Source {
    Transformers* transformers;
    std::mutex mux;

  public:
    MuxSource(Transformers* _transformers);
    ~MuxSource() override = default;

    MuxCommandReturnValue runCommand(std::string _name);
    MuxCommandReturnValue runCommand(std::span<const std::string> _argv);
};
//-------------------------------------------------------------------------------------
