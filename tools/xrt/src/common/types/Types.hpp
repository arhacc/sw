//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

enum class RuntimeType {
    Matrix,     // argument is std::shared_ptr<Matrix>*
    MatrixView, // argument is std::shared_ptr<MatrixView>*
    UInt32      // argument is uint32_t*
};
