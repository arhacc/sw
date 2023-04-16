//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.utils.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------

public class ValueFormat {
    private int numberOfSubFields;
    private String[][] fieldsFormat;

//-------------------------------------------------------------------------------------
    public ValueFormat(String[] _fieldsFormat) {
        numberOfSubFields = 1;
        fieldsFormat = new String[1][];
        fieldsFormat[0] = _fieldsFormat;
    }

//-------------------------------------------------------------------------------------
    public ValueFormat(String[] _fieldsFormat0, String[] _fieldsFormat1) {
        numberOfSubFields = 1;
        fieldsFormat = new String[2][];
        fieldsFormat[0] = _fieldsFormat0;
        fieldsFormat[1] = _fieldsFormat1;
    }

//-------------------------------------------------------------------------------------
    public ValueFormat(String[] _fieldsFormat0, String[] _fieldsFormat1, String[] _fieldsFormat2) {
        numberOfSubFields = 1;
        fieldsFormat = new String[3][];
        fieldsFormat[0] = _fieldsFormat0;
        fieldsFormat[1] = _fieldsFormat1;
        fieldsFormat[2] = _fieldsFormat2;
    }


//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
