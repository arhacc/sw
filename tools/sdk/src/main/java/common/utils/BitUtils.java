//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class BitUtils {

//-------------------------------------------------------------------------------------
public static byte[] toByteArray(FixedBitSet bits) {
    byte[] bytes = new byte[(bits.length() + 7) / 8];
    for (int i=0; i < bits.length(); i++) {
        if (bits.get(i)) {
            bytes[bytes.length - (i/8) - 1] |= 1<<(i%8);
        }
    }
    return bytes;
}

//-------------------------------------------------------------------------------------
public static FixedBitSet concatenate(FixedBitSet _in1, FixedBitSet _in2) {
/*  FixedBitSet vector_1_in_clone = (FixedBitSet)vector_1_in.clone();
  FixedBitSet vector_2_in_clone = (FixedBitSet)vector_2_in.clone();
  int n = 5;//_desired length of the first (leading) vector
  int index = -1;
  while (index < (vector_2_in_clone.length() - 1)) {
    index = vector_2_in_clone.nextSetBit((index + 1));
    vector_1_in_clone.set((index + n));
  }
  return vector_1_in_clone;*/
  int _length = _in1.length() + _in2.length();
  FixedBitSet _out = new FixedBitSet(_length);
  int _start = _length - _in1.length();
  for(int i = 0; i < _in1.length(); i++){
    if(_in1.get(i)){
        _out.set(_start + i);
    }
  }
//  int _start = 0;
  for(int i = 0; i < _in2.length(); i++){
    if(_in2.get(i)){
        _out.set(i);
    }
  }
  return _out;

}
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
