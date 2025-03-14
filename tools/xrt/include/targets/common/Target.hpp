//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdio>
#include <memory>

class MatrixView;
class Future;

//-------------------------------------------------------------------------------------
class Target {
  public:
    Target() = default;

    virtual ~Target() = default;

    virtual void reset() = 0;

    virtual std::shared_ptr<Future> readRegisterAsync(std::uint32_t address, std::uint32_t* dataLocation) = 0;
    virtual std::shared_ptr<Future> writeRegisterAsync(std::uint32_t address, std::uint32_t data) = 0;
    virtual std::shared_ptr<Future> readMatrixArrayAsync(const std::shared_ptr<MatrixView>& view) = 0;
    virtual std::shared_ptr<Future> writeMatrixArrayAsync(const std::shared_ptr<const MatrixView>& view) = 0;
};

//-------------------------------------------------------------------------------------
