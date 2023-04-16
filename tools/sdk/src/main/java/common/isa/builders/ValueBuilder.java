//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.builders;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import com.opencsv.*;
import com.opencsv.exceptions.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;

//-------------------------------------------------------------------------------------
public class ValueBuilder extends AbstractBuilder {
    private String architectureId;
    private Map<String, Value> values;
    private Value EMPTY = new Value("", 0);

//-------------------------------------------------------------------------------------
    public ValueBuilder(Context _context, String _architectureId) {
        super(_context);
        architectureId = _architectureId;
        values = new HashMap<String, Value>();
        init();        
    }

//-------------------------------------------------------------------------------------
    public Value get(ValueFormat _valueFormat, String[] _values) {
        if(_values.length == 0){
            return EMPTY;
        }
        Value _valueObj = values.get(_values[0]);
        if(_valueObj == null){
            log.error("Error: cannot find value: [" + _values[0] + "]");
            System.exit(1);
        }
        return _valueObj;
    }

//-------------------------------------------------------------------------------------
    private void init() {
        addValue("ZERO");
        

        addValue("INSTR_VALUE_NR_BITS");
        addValue("INSTR_VALUE_LOC");

        addValue("INSTR_FIXED_SHIFTING_NR_BITS");
        addValue("INSTR_FIXED_SHIFTING_LOC");

        addValue("INSTR_SH_ROTATE_LOCAL_CTRL_BITS_NR_BITS");
        addValue("INSTR_SH_ROTATE_LOCAL_CTRL_LOC");
        addValue("INSTR_SH_ROTATE_LOCAL_SPLIT_POINT_NR_BITS");
        addValue("INSTR_SH_ROTATE_LOCAL_SPLIT_POINT_LOC");
        addValue("INSTR_SH_ROTATE_LOCAL_AMOUNT_NR_BITS");
        addValue("INSTR_SH_ROTATE_LOCAL_SHIFT_AMOUNT_LOC");
        addValue("INSTR_GLOBAL_NR_BITS");
        addValue("INSTR_GLOBAL_LOC");
        addValue("INSTR_JMP_FUNCTION_NR_BITS");
        addValue("INSTR_JMP_FUNCTION_LOC");
        addValue("INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS");
        addValue("INSTR_JMP_FUNCTION_BR_w_VAL_LOC");
        addValue("INSTR_JMP_VALUE_NR_BITS");
        addValue("INSTR_JMP_VALUE_LOC");
        addValue("INSTR_SPATIAL_SELECT_FUNCTION_NR_BITS");
        addValue("INSTR_SPATIAL_SELECT_FUNCTION_LOC");
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_NR_BITS");
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_LOC");
        addValue("NETWORK_NR_OPCODE_BITS");
        addValue("INSTR_SETSCAN_OPCODE_LOC");
        addValue("NETWORK_SCAN_MODES_NR_BITS");
        addValue("INSTR_SETSCAN_ADDR_MODE_LOC");
        addValue("INSTR_MISC_STORE_LOAD_NR_BITS");
        addValue("INSTR_MISC_STORE_LOAD_LOC");
        addValue("CONTROLLER_ADDR_REG_SELECTOR_NR_BITS");
        addValue("INSTR_MISC_STORE_LOAD_VALUE_LOC");
        addValue("INSTR_MISC_TESTING_SEL_NR_BITS");
        addValue("INSTR_MISC_TESTING_SEL_LOC");
        addValue("INSTR_MISC_STORE_LOAD_NR_BITS");
        addValue("INSTR_MISC_STORE_LOAD_LOC");
        addValue("INSTR_JMP_FUNCTION_BR_w_VAL_NR_BITS");
        addValue("INSTR_MISC_STORE_LOAD_BR_VAL_REG_SEL_LOC");
        addValue("INSTR_MISC_TESTING_SEL_NR_BITS");
        addValue("INSTR_MISC_TESTING_SEL_LOC");
        addValue("ISA_stack_operations_CTL_val_nr_bits");

        addValue("INSTR_SHIFT1_RIGHT");
        addValue("INSTR_SHIFT1_RIGHT_CARRY");
        addValue("INSTR_SHIFT1_RIGHT_ARITHMETIC");
        addValue("INSTR_SHIFT_RIGHT_FIXED_AMOUNT");
        addValue("INSTR_SHIFT_RIGHT_FIXED_AMOUNT_ARITHMETIC");

        addValue("INSTR_SH_ROT_CTRL_bits_RIGHT_ROTATE");
        addValue("INSTR_SH_ROT_CTRL_bits_LEFT_ROTATE");
        addValue("INSTR_SH_ROT_CTRL_bits_RIGHT_SHIFT");

        addValue("INSTR_GLOBAL_SHIFT_right_WOB");
        addValue("INSTR_GLOBAL_SHIFT_right_WBZ");
        addValue("INSTR_GLOBAL_SHIFT_right_WBH");
        
        addValue("INSTR_GLOBAL_RIGHT_INS");        
        addValue("INSTR_GLOBAL_LEFT_INS");

        addValue("INSTR_GLOBAL_RIGHT_RED_INS");
        addValue("INSTR_GLOBAL_LEFT_RED_INS");
        addValue("INSTR_GLOBAL_SHIFT_left_WOB");
        addValue("INSTR_GLOBAL_SHIFT_left_WBZ");
        addValue("INSTR_GLOBAL_SHIFT_left_WBH");

        addValue("INSTR_GLOBAL_ROTATE_right");
        addValue("INSTR_GLOBAL_ROTATE_left");

        addValue("INSTR_JMP_FUNCTION_JMP");
        addValue("INSTR_JMP_FUNCTION_BRZ");
        addValue("INSTR_JMP_FUNCTION_BRNZ");
        addValue("INSTR_JMP_FUNCTION_BRSGN");
        addValue("INSTR_JMP_FUNCTION_BRNSGN");
        addValue("INSTR_JMP_FUNCTION_BRZDEC");
        addValue("INSTR_JMP_FUNCTION_BRNZDEC");
        addValue("INSTR_JMP_FUNCTION_BRBOOL");
        addValue("INSTR_JMP_FUNCTION_BRNBOOL");
        addValue("INSTR_JMP_FUNCTION_BRCR");
        addValue("INSTR_JMP_FUNCTION_BRNCR");
        addValue("INSTR_JMP_FUNCTION_BRCR_delayed");
        addValue("INSTR_JMP_FUNCTION_BRNCR_delayed");
        addValue("INSTR_JMP_FUNCTION_BRCMPVal");
        addValue("INSTR_JMP_FUNCTION_BRCMPNVal");
        addValue("INSTR_JMP_FUNCTION_BRCMPValDEC");
        addValue("INSTR_JMP_FUNCTION_BRCMPNValDEC");
        addValue("INSTR_JMP_FUNCTION_BRValZ");
        addValue("INSTR_JMP_FUNCTION_BRValNZ");
        addValue("INSTR_JMP_FUNCTION_BRValSGN");
        addValue("INSTR_JMP_FUNCTION_BRValNSGN");
        addValue("INSTR_JMP_FUNCTION_BRValZDEC");
        addValue("INSTR_JMP_FUNCTION_BRValNZDEC");

        addValue("INSTR_MISC_STORE_LOAD_set_addrreg_selector");

        addValue("INSTR_MISC_TESTING_SEL_CC_START_W_HALT");
        addValue("INSTR_MISC_TESTING_SEL_CC_START_WO_HALT");
        addValue("INSTR_MISC_TESTING_SEL_CC_STOP");
        addValue("INSTR_MISC_TESTING_SEL_CC_RESET");

        addValue("INSTR_MISC_STORE_LOAD_srstore");
        addValue("INSTR_MISC_STORE_LOAD_addrstore");
        addValue("INSTR_MISC_STORE_LOAD_setdec");
        addValue("INSTR_MISC_STORE_LOAD_set_loop_counter_value");

        addValue("INSTR_MISC_STORE_LOAD_addrload");

        addValue("ISA_stack_operations_CTL_val_POP");
        addValue("ISA_stack_operations_CTL_val_DUPLICATE");
        addValue("ISA_stack_operations_CTL_val_OVER");
        addValue("ISA_stack_operations_CTL_val_SWAP");
        addValue("ISA_stack_operations_CTL_val_LOAD_LAYRER1");        

        addValue("INSTR_SPATIAL_SELECT_FUNCTION_WHERE");
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHEREZERO"  );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENSGN"  );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERECARRY" );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHEREFIRST" );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENEXT"  );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHEREPREV"  );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENZERO" );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERESGN"   );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENCARRY");
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENFIRST");
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENNEXT" );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_WHERENPREV" );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  );
        addValue("INSTR_SPATIAL_SELECT_WHERE_COND_DONT_CARE"  );

        addValue("NETWORK_OP_NOP");
        addValue("NETWORK_OP_SPLIT");
        addValue("NETWORK_OP_PERMUTE");
        addValue("NETWORK_OP_ADD");
        addValue("NETWORK_OP_MIN");
        addValue("NETWORK_OP_MAX");
        addValue("NETWORK_OP_SUM_PREFIX");
        addValue("NETWORK_OP_bitwise_XOR_PREFIX");
        addValue("NETWORK_OP_SUB");
        addValue("NETWORK_OP_bitwise_OR");
        addValue("NETWORK_OP_bitwise_XOR");
        addValue("NETWORK_OP_bitwise_AND");

        addValue("INSTR_MISC_STORE_LOAD_array_addr_reg_stack_duplicate");
        addValue("INSTR_MISC_STORE_LOAD_array_addr_reg_stack_pop");

        addValue("INSTR_MISC_STORE_LOAD_addrstore");
        addValue("INSTR_MISC_STORE_LOAD_addrstore_COP");
        addValue("INSTR_MISC_STORE_LOAD_insertio");

        addValue("ISA_stack_operations_CTL_val_POP");
        addValue("ISA_stack_operations_CTL_val_DUPLICATE");
        addValue("ISA_stack_operations_CTL_val_OVER");
        addValue("ISA_stack_operations_CTL_val_SWAP");
        addValue("ISA_stack_operations_CTL_val_LOAD_LAYRER1");

    }

//-------------------------------------------------------------------------------------
    private void addValue(String _valueName) {
        if(_valueName.equals("ZERO")){
            values.put("ZERO", new Value());
        } else if(_valueName.endsWith("_LOC")){
            addValueRaw(_valueName + "_UPPER");
            addValueRaw(_valueName + "_LOWER");
        } else {
            addValueRaw(_valueName);
        }
    }

//-------------------------------------------------------------------------------------
    private void addValueRaw(String _valueName) {
        int _valueData = context.getArchitectureImplementations().getArchitecture(architectureId).get(_valueName);
        Value _valueObj = new Value(_valueName, _valueData);
        values.put(_valueName, _valueObj);            
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
