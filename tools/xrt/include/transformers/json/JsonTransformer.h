//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

//-------------------------------------------------------------------------------------

class JsonTransformer : public Transformer {

public:
  JsonTransformer(DirectTransformer* _directTransformer);

  ~JsonTransformer();

  void load(std::string _filename);
  void process();

private:
};
//-------------------------------------------------------------------------------------
