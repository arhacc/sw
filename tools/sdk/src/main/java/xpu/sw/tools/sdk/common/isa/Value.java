//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class Value extends Field {
    private Instruction instruction;
    private int[][] dimensions;
    private String[] argumentReferences;
    private String[] argumentValues;

    private int[] values;
    private Primitive primitive;


//-------------------------------------------------------------------------------------
    public Value() {
        this(null);
    }
/*
//-------------------------------------------------------------------------------------
    public Value(String _valueString, int _valueNumber) {
        super(_valueString, _valueNumber);
    }

//-------------------------------------------------------------------------------------
    public Value(int _value) {
        super("const", _value);
    }
*/
//-------------------------------------------------------------------------------------
    public Value(int[][] _dimensions) {
        super("const", 0);
        dimensions = _dimensions;
    }

//-------------------------------------------------------------------------------------
    public Value copyOf(){
        Value _value = new Value();
        if(dimensions != null){
            _value.dimensions = Arrays.stream(dimensions).map(int[]::clone).toArray(int[][]::new);
        }
        if(argumentReferences != null){
            _value.argumentReferences = Arrays.stream(argumentReferences).toArray(String[]::new);
        }
        return _value;
    }

//-------------------------------------------------------------------------------------
    public void setArgumentReferences(String[] _argumentReferences){
        argumentReferences = _argumentReferences;
    }

//-------------------------------------------------------------------------------------
    public void setArgumentValues(String[] _argumentValues, Primitive _primitive){
        argumentValues = _argumentValues;
        primitive = _primitive;
    }

//-------------------------------------------------------------------------------------
    public boolean link(Instruction _instruction) {
        instruction = _instruction;
        return true;
    }

//-------------------------------------------------------------------------------------
    public boolean resolve() {
//                System.out.println("search for:["+opcode.getName()+"][" + value.getName()+"]");
        values = new int[argumentReferences.length];
        for (int i = 0; i < argumentReferences.length; i++) {
            values[i] = resolve(argumentReferences[i], argumentValues);
//                System.out.println("i=" + i +", argumentReferences="+argumentReferences[i]+",value=[" + values[i]+"]");
        }
//        System.exit(0);
        return true;
    }

//-------------------------------------------------------------------------------------
    private int resolve(String _argumentReference, String _argumentValues[]) {
//        System.out.println(">>>>get [" + _argumentReference + "]");
        switch (_argumentReference) {
            case "ZERO" : {
                return 0;
            }    
            case "ARG0:LABEL" : {
                int _arg0 = getArgFromLabel(_argumentValues[0]);
                return _arg0;
            }
            case "ARG0:NUMBER" : {
                int _arg0 = getArgFromNumber(_argumentValues[0]);
                return _arg0;
            }    
            case "ARG1:LABEL" :  {
                int _arg1 = getArgFromLabel(_argumentValues[1]);
                return _arg1;
            }
            case "ARG1:NUMBER" : {
                int _arg1 = getArgFromNumber(_argumentValues[1]);
                return _arg1;
            }    
            case "ARG0:NUMBER - 1" : {
                int _arg0 = getArgFromNumber(_argumentValues[0]);
                return _arg0 - 1;
            }    
            case "DATA_SIZE – ARG0:NUMBER – 1" : {
                int _arg0 = getArgFromNumber(_argumentValues[0]);
                int _dataSize = primitive.getArchitectureImplementation().get("DATA_SIZE");
                return _dataSize - _arg0 - 1;
            }    
            default : {
                return primitive.getArchitectureImplementation().get(_argumentReference);
            }    
        }
    }

//-------------------------------------------------------------------------------------
    private int getArgFromNumber(String _argument) {
        if(_argument.startsWith("$")){
            _argument = _argument.substring(1);
            return primitive.getArchitectureImplementation().get(_argument);
        }
        try {
            return Integer.parseInt(_argument);
        } catch(NumberFormatException _e){
            System.out.println("Cannot convert argument [" + _argument + "] to integer!");
            _e.printStackTrace();
            return -1;
        }
    }
    
//-------------------------------------------------------------------------------------
    private int getArgFromLabel(String _argument) {
        int _labelAddress = primitive.getByLabel(_argument);
        int _currentAddress = instruction.getAddress();
        int _address = _labelAddress - _currentAddress;
//        System.out.println("Value. getArgFromLabel["+_argument+"]: _labelAddress= "+_labelAddress + ", _currentAddress="+_currentAddress +", _address="+_address);
        return _address;
    }
    
//-------------------------------------------------------------------------------------
    public void pack(int _width) {
        data = new FixedBitSet(_width);
//        int _data = intdata;
//        int _indexInData;
        for(int i = 0; i < dimensions.length; i++){
            int[] _dimensions = dimensions[i];
            int _value = values[i];
//            System.out.println("pack:_dimensions[0]"+_dimensions[0]+", _dimensions[1]="+_dimensions[1]+", _dimensions[2]="+_dimensions[2] +", _value="+_value);
            for(int j = _dimensions[1]; j <= _dimensions[2]; j++){
                pack(j, _value, j - _dimensions[1]);
            }
//            System.out.println("after pack:"+dump());
//            System.exit(0);
        }
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
