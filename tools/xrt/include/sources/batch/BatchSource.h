//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include "sources/common/Source.h"
#include "sources/mux/MuxSource.h"

//-------------------------------------------------------------------------------------
class BatchSource : public Source {

public:
	BatchSource(MuxSource* _muxSource, std::string _batch);
 	~BatchSource();

private:
};
//-------------------------------------------------------------------------------------


