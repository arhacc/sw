//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;


//-------------------------------------------------------------------------------------
public class Operand extends Cell {

    public static final Operand VAL = new Operand("VAL", 0b000);
    public static final Operand MAB = new Operand("MAB", 0b001);
    public static final Operand MRL = new Operand("MRL", 0b010);
    public static final Operand MRI = new Operand("MRI", 0b011);
    public static final Operand COP = new Operand("COP", 0b100);
    public static final Operand MAC = new Operand("MAC", 0b101);
    public static final Operand MRC = new Operand("MRC", 0b110);
    public static final Operand CTL = new Operand("CTL", 0b111);

    public static final HashMap<Integer, Operand> ALL_OPERANDS = new HashMap<Integer, Operand>();

    static {
        ALL_OPERANDS.put(VAL.getIntData(), VAL);
        ALL_OPERANDS.put(MAB.getIntData(), MAB);
        ALL_OPERANDS.put(MRL.getIntData(), MRL);
        ALL_OPERANDS.put(MRI.getIntData(), MRI);
        ALL_OPERANDS.put(COP.getIntData(), COP);
        ALL_OPERANDS.put(MAC.getIntData(), MAC);
        ALL_OPERANDS.put(MRC.getIntData(), MRC);
        ALL_OPERANDS.put(CTL.getIntData(), CTL);
    }
//-------------------------------------------------------------------------------------
    public Operand(String _name, final int _data) {
        super(_name, 3, _data);
    }

//-------------------------------------------------------------------------------------
    public static Operand getOperand(String _name) {
        switch (_name.toUpperCase()) {
             case "VAL": return VAL;  // immediate value
             case "MAB": return MAB;  // absolute data memory
             case "MRL": return MRL;  // relative in data memory
             case "MRI": return MRI;  // relative and increment addr with scalar
             case "COP": return COP;  // co-operand
             case "MAC": return MAC;  // op[i] = vectMem[(contrOpCode==send) ? op : acc]
             case "MRC": return MRC;  // op[i] = vectMem[addr[i] + ((contrOpCode==send) ? op : acc)]
             case "CTL": return CTL;  // control instructions
             default: return null;
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
