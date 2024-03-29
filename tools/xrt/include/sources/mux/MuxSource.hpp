//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/common/Source.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <span>
#include <vector>

#include <unistd.h>

// forward declaration
class Source;
class Transformers;

//-------------------------------------------------------------------------------------
enum class MuxCommandReturnType {
    VOID,

    WORD_VECTOR,
};

//-------------------------------------------------------------------------------------
struct MuxCommandReturnValue {
    MuxCommandReturnType type;
    union {
        std::vector<uint32_t> words;
    };

    inline MuxCommandReturnValue() : type(MuxCommandReturnType::VOID) {}

    inline MuxCommandReturnValue(const std::vector<uint32_t>& _words)
        : type(MuxCommandReturnType::WORD_VECTOR), words(_words) {}

    inline MuxCommandReturnValue(std::vector<uint32_t>&& _words)
        : type(MuxCommandReturnType::WORD_VECTOR), words(std::move(_words)) {}

    inline MuxCommandReturnValue& operator=(MuxCommandReturnValue&& _value) {
        switch (type) {
            case MuxCommandReturnType::VOID: {
                break;
            }

            case MuxCommandReturnType::WORD_VECTOR: {
                words = std::move(_value.words);

                break;
            }
        }

        return *this;
    }

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
