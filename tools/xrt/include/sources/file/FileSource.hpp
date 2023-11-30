//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/common/Source.hpp>
#include <sources/mux/MuxSource.hpp>

//-------------------------------------------------------------------------------------
class FileSource : public Source {
  public:
    FileSource(MuxSource* _muxSource, const std::vector<std::string>& _files);

    ~FileSource() override = default;
};
//-------------------------------------------------------------------------------------
