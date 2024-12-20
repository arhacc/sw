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
/* array memory is viewd as a matrix where
each line has "Ncores" length and each column has memorySize length
*/
//-------------------------------------------------------------------------------------
class Target {
  public:
    Target() = default;

    virtual ~Target() = default;

    virtual void reset() = 0;
    virtual void process(std::shared_ptr<Future> _future) = 0;
};

//-------------------------------------------------------------------------------------
