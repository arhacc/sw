//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#define XRT_LOGO "XRT v0.1"

#define XPU_FILE_HEX  1
#define XPU_FILE_JSON 2
#define XPU_FILE_OBJ  3
#define XPU_FILE_ONNX 4
#define XPU_FILE_C    5
#define XPU_FILE_CPP  6
#define XPU_FILE_SO   7

//-------------------------------------------------------------------------------------

#ifdef _WIN32
#define FS_DELIMITERS     "/\\"
#define FS_DELIMITER_CHAR '\\'
#else
#define FS_DELIMITERS     "/"
#define FS_DELIMITER_CHAR '/'
#endif

//-------------------------------------------------------------------------------------