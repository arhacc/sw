//-------------------------------------------------------------------------------------
//
//                         HIgh libraries for XPU
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
void matrixMultiply(int _m, int _n) {
    std::cout << "[matrixMultiply]: " << std::endl;
    int* _fMultiplyMatrix = lowLevel("matrix_multiply");

    for(int i = 0; i < _m; i++){
        for(int j = 0; j < _n; j++){
            
            // pentru Mihai! :)
            runRuntime(_fMultiplyMatrix);
        }
    }
}

//-------------------------------------------------------------------------------------
