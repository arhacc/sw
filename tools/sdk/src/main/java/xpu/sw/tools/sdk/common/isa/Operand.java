//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;


//-------------------------------------------------------------------------------------
public class Operand extends Field {
/*
    public static final int OP_VAL = 0b000;
    public static final int OP_MAB = 0b001;
    public static final int OP_MRL = 0b010;
    public static final int OP_MRI = 0b011;
    public static final int OP_COP = 0b100;
    public static final int OP_MAC = 0b101;
    public static final int OP_MRC = 0b110;
    public static final int OP_CTL = 0b111;

    public static final Operand VAL = new Operand("val", OP_VAL);
    public static final Operand MAB = new Operand("mab", OP_MAB);
    public static final Operand MRL = new Operand("mrl", OP_MRL);
    public static final Operand MRI = new Operand("mri", OP_MRI);
    public static final Operand COP = new Operand("cop", OP_COP);
    public static final Operand MAC = new Operand("mac", OP_MAC);
    public static final Operand MRC = new Operand("mrc", OP_MRC);
    public static final Operand CTL = new Operand("ctl", OP_CTL);
*/
//-------------------------------------------------------------------------------------
    public Operand(String _name, int _data) {
        super(_name, _data);
    }
/*
//-------------------------------------------------------------------------------------
    public static Operand getOperand(String _operand) {
        switch (_operand) {
             case "val": return VAL;  // immediate value
             case "mab": return MAB;  // absolute data memory
             case "mrl": return MRL;  // relative in data memory
             case "mri": return MRI;  // relative and increment addr with scalar
             case "cop": return COP;  // co-operand
             case "mac": return MAC;  // op[i] = vectMem[(contrOpCode==send) ? op : acc]
             case "mrc": return MRC;  // op[i] = vectMem[addr[i] + ((contrOpCode==send) ? op : acc)]
             case "ctl": return CTL;  // control instructions
             default: return null;
        }
    }
*/
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
