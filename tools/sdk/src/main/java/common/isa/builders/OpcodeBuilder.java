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
public class OpcodeBuilder extends AbstractBuilder {
    private String architectureId;
    private Map<String, Opcode> opcodes;

//-------------------------------------------------------------------------------------
    public OpcodeBuilder(Context _context, String _architectureId) {
        super(_context);
        architectureId = _architectureId;
        opcodes = new HashMap<String, Opcode>();
        init();
    }

//-------------------------------------------------------------------------------------
    public Opcode get(String _opcode) {
//        log.debug("InstructionBuilder: " + _opcode + ", " + _valueString + ", " + _valueNumber);
        Opcode _opcodeObj = opcodes.get(_opcode);
        if(_opcodeObj == null){
            log.error("Error: cannot find opcode: " + _opcode);
            System.exit(1);
        }
        return _opcodeObj;
    }   

//-------------------------------------------------------------------------------------
    public Opcode getOpcode(int _opcode) {
        return opcodes.get(_opcode);
    }

//-------------------------------------------------------------------------------------
    private void init() {
        addOpcode("add"); 
        addOpcode("addc");
        addOpcode("sub");
        addOpcode("rsub");
        addOpcode("subc");
        addOpcode("rsubc");
        addOpcode("mult");
        addOpcode("bwand");
        addOpcode("bwor");
        addOpcode("bwxor");
        addOpcode("load");
        addOpcode("store");
        addOpcode("compare");
        addOpcode("stack_store_pop");
        addOpcode("stack_push_load");
        addOpcode("swap_memacc");
        addOpcode("fadd");
        addOpcode("fsub");
        addOpcode("fmult");
        addOpcode("fdiv");
        addOpcode("fmult_first");
        addOpcode("fmult_second");
        addOpcode("fdiv_loop1");
        addOpcode("fdiv_loop2");
        addOpcode("addrinc");
        addOpcode("last_common_instr_val");
        addOpcode("srcall");
        addOpcode("search");
        addOpcode("csearch");
        addOpcode("insert");
        addOpcode("send");
        addOpcode("stack_operations");
        addOpcode("misc_store_load");
        addOpcode("rotate_local");
        addOpcode("global_SR");
        addOpcode("insval");
        addOpcode("float");
        addOpcode("right_fixed_shifting");
        addOpcode("last_common_instr_CTL");
        addOpcode("spatial_select");
        addOpcode("setscan");
        addOpcode("brshift");
        addOpcode("delete");
        addOpcode("jmp");
        addOpcode("sendint");
        addOpcode("misc_testing");
        addOpcode("param");
        addOpcode("prun");
        addOpcode("pload");
        addOpcode("trun");
        addOpcode("waitmatw");
        addOpcode("resready");
    }

//-------------------------------------------------------------------------------------
    private void addOpcode(String _opcodeName) {
        int _opcodeData = context.getArchitectureImplementations().getArchitecture(architectureId).get("ISA_" + _opcodeName);
        Opcode _opcodeObj = new Opcode(_opcodeName, _opcodeData);
        opcodes.put(_opcodeName, _opcodeObj);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
