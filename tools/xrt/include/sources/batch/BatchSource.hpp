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
class BatchSource : public Source {
  public:
    BatchSource(MuxSource* _muxSource, const std::vector<std::string>& _batchFiles);

    ~BatchSource() override = default;
};
//-------------------------------------------------------------------------------------
