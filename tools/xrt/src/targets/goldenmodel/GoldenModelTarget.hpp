//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <targets/common/Target.hpp>

#include <cstdint>

class MatrixView;

//-------------------------------------------------------------------------------------
class GoldenModelTarget : public Target {
  public:
    GoldenModelTarget() = default;

    ~GoldenModelTarget() override = default;

    void reset() override;

    // void process(std::shared_ptr<Future> _future) override;
};

//-------------------------------------------------------------------------------------
