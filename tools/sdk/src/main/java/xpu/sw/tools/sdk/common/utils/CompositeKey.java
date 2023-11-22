//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.apache.lucene.util.*;


import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class CompositeKey implements Serializable {
   public Object[] keys;

//-------------------------------------------------------------------------------------
   public CompositeKey(Object... _keys) {
        keys = _keys;
    }

//-------------------------------------------------------------------------------------
   @Override   
   public boolean equals(Object _obj) {
/*      if (!(_obj instanceof Key))
        return false;*/
      CompositeKey _ref = (CompositeKey) _obj;
        for (int i = 0; (i < _ref.keys.length) && (i < keys.length); i++) {
            if((_ref.keys[i] != null) && (keys[i] != null) && _ref.keys[i].equals(keys[i])){
                return true;
            }
        }
        return false;
   }

//-------------------------------------------------------------------------------------
    @Override
    public int hashCode() {
        int _hash = 0;
        for(int i = 0; i < keys.length; i++){
            if(keys[i] != null){
                _hash ^= keys[i].hashCode();
            }
        }
        return _hash;
    }

//-------------------------------------------------------------------------------------
   @Override   
   public String toString() {
        String _text = "";
        for (int i = 0; i < keys.length ; i++) {
            _text += keys[i].toString();
        }
        return _text;
   }
//-------------------------------------------------------------------------------------
}

//-------------------------------------------------------------------------------------
