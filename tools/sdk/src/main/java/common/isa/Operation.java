//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class Operation extends Cell {
    private boolean isJump;

    private static int ISA_global_SR = 0;
    public static final Operation ADDRINC   = new Operation ("ADDRINC", ISA_global_SR + 1);
    private static int ISA_last_common_instr_operand = 1;

    public static final Operation ADD       = new Operation ("ADD", ADDRINC.getIntData() + 1);
    public static final Operation ADDC      = new Operation ("ADDC", ADD.getIntData() + 1);
    public static final Operation SUB       = new Operation ("SUB", ADDC.getIntData() + 1);
    public static final Operation RSUB      = new Operation ("RSUB", SUB.getIntData() + 1);
    public static final Operation SUBC      = new Operation ("SUBC", RSUB.getIntData() + 1);
    public static final Operation RSUBC     = new Operation ("RSUBC", SUBC.getIntData() + 1);
    public static final Operation MULT      = new Operation ("MULT", RSUBC.getIntData() + 1);

    public static final Operation BWAND     = new Operation ("BWAND", MULT.getIntData() + 1);
    public static final Operation BWOR      = new Operation ("BWOR", BWAND.getIntData() + 1);
    public static final Operation BWXOR     = new Operation ("BWXOR", BWOR.getIntData() + 1);

    public static final Operation LOAD      = new Operation ("LOAD", BWXOR.getIntData() + 1);
    public static final Operation STORE     = new Operation ("STORE", LOAD.getIntData() + 1);
    public static final Operation COMPARE   = new Operation ("COMPARE", STORE.getIntData() + 1);

    public static final Operation STKSP     = new Operation ("STKSTOREPOP", COMPARE.getIntData() + 1);
    public static final Operation STKPL     = new Operation ("STKPUSHLOAD", STKSP.getIntData() + 1);

    public static final Operation SWAPMEMACC= new Operation ("SWAPMEMACC", STKPL.getIntData() + 1);

    public static final Operation FADD       = new Operation ("FADD", SWAPMEMACC.getIntData() + 1);
    public static final Operation FSUB       = new Operation ("FSUB", FADD.getIntData() + 1);
    public static final Operation FMULT      = new Operation ("FMULT", FSUB.getIntData() + 1);
    public static final Operation FDIV       = new Operation ("FDIV", FMULT.getIntData() + 1);
    public static final Operation FMULTF     = new Operation ("FMULTF", FDIV.getIntData() + 1);
    public static final Operation FMULTS     = new Operation ("FMULTS", FMULTF.getIntData() + 1);
    public static final Operation FDIVL1     = new Operation ("FDIVL1", FMULTS.getIntData() + 1);
    public static final Operation FDIVL2     = new Operation ("FDIVL2", FDIVL1.getIntData() + 1);

    public static final Operation SRCALL    = new Operation ("SRCALL  ", FDIVL2.getIntData() + 1);
    public static final Operation SEARCH    = new Operation ("SEARCH  ", SRCALL.getIntData() + 1);
    public static final Operation CSEARCH   = new Operation ("CSEARCH ", SEARCH.getIntData() + 1);
    public static final Operation INSERT    = new Operation ("INSERT  ", CSEARCH.getIntData() + 1);
    public static final Operation SEND      = new Operation ("SEND", INSERT.getIntData() + 1);

    public static final Operation JMP       = new Operation ("JMP     ", 0b00000, true);
    public static final Operation BRZ       = new Operation ("BRZ     ", 0b00010, true);
    public static final Operation BRNZ      = new Operation ("BRNZ    ", 0b00011, true);
    public static final Operation BRZDEC    = new Operation ("BRZDEC  ", 0b00100, true);
    public static final Operation BRNZDEC   = new Operation ("BRNZDEC ", 0b00101, true);
    public static final Operation BRCR      = new Operation ("BRCR    ", 0b00110, true);
    public static final Operation BRNCR     = new Operation ("BRNCR   ", 0b00111, true);
    public static final Operation BRBOOL    = new Operation ("BRBOOL  ", 0b01000);
    public static final Operation BRNBOOL   = new Operation ("BRNBOOL ", 0b01001);
    public static final Operation BRSGN     = new Operation ("BRSGN   ", 0b01010);
    public static final Operation BRNSGN    = new Operation ("BRNSGN  ", 0b01011);

    public static final Operation REDINS    = new Operation ("REDINS  ", 0b01101);
    public static final Operation POPLOOP   = new Operation ("POPLOOP ", 0b01110);
    public static final Operation PUSHLOOP  = new Operation ("PUSHLOOP", 0b01111);

    public static final Operation ACTIVATE  = new Operation ("ACTIVATE", 0b00000);
    public static final Operation WHERE     = new Operation ("WHERE   ", 0b00001);
    public static final Operation ELSEW     = new Operation ("ELSEW   ", 0b00010);
    public static final Operation ENDWHERE  = new Operation ("ENDWHERE", 0b00011);

    public static final Operation SETSCAN   = new Operation ("SETSCAN ", 0b00111);
    public static final Operation SCANLOAD  = new Operation ("SCANLOAD", 0b01000);
    public static final Operation IXLOAD    = new Operation ("IXLOAD  ", 0b01001);
    public static final Operation GLSH      = new Operation ("GLSH    ", 0b01010);
    public static final Operation BRSHIFT   = new Operation ("BRSHIFT ", 0b01011);
    public static final Operation DELETE    = new Operation ("DELETE  ", 0b01100);
    public static final Operation SETRED    = new Operation ("SETRED  ", 0b01101);
    public static final Operation SRLOAD    = new Operation ("SRLOAD  ", 0b01110);
    public static final Operation SRSTORE   = new Operation ("SRSTORE ", 0b01111);
    public static final Operation INSVAL    = new Operation ("INSVAL  ", 0b10000);

    public static final Operation SHRIGHTC  = new Operation ("SHRIGHTC", 0b10001);
    public static final Operation SHRIGHT   = new Operation ("SHRIGHT ", 0b10010);
    public static final Operation SHARIGHT  = new Operation ("SHARIGHT", 0b10011);
    public static final Operation ROTATE    = new Operation ("ROTATE  ", 0b10100);

    public static final Operation ADDRLD    = new Operation ("ADDRLD  ", 0b10101);

    public static final Operation IOLOAD    = new Operation ("IOLOAD  ", 0b10110);
    public static final Operation IOSTORE   = new Operation ("IOSTORE ", 0b10111);
    public static final Operation INSERTIO  = new Operation ("INSERTIO", 0b11000);
    public static final Operation SENDINT   = new Operation ("SENDINT ", 0b10111);

    public static final Operation START     = new Operation ("START   ", 0b11000);
    public static final Operation STOP      = new Operation ("STOP    ", 0b11001);
    public static final Operation PARAM     = new Operation ("PARAM   ", 0b11101);
    public static final Operation PRUN      = new Operation ("PRUN    ", 0b11110);
    //public static final Operation PLOAD     = new Operation ("PLOAD   ", 0b11111);
    public static final Operation PCHECK     = new Operation ("PCHECK15 ", 0b11111);

    public static final HashMap<Integer, Operation> ALL_OPERATIONS = new HashMap<Integer, Operation>();
    static {
        ALL_OPERATIONS.put(SEND.getIntData(), SEND);
        ALL_OPERATIONS.put(COMPARE.getIntData(), COMPARE);
        ALL_OPERATIONS.put(SRCALL.getIntData(), SRCALL);
        ALL_OPERATIONS.put(SEARCH.getIntData(), SEARCH);
        ALL_OPERATIONS.put(CSEARCH.getIntData(), CSEARCH);
        ALL_OPERATIONS.put(INSERT.getIntData(), INSERT);
        ALL_OPERATIONS.put(JMP.getIntData(), JMP);
        ALL_OPERATIONS.put(BRZ.getIntData(), BRZ);
        ALL_OPERATIONS.put(BRNZ.getIntData(), BRNZ);
        ALL_OPERATIONS.put(BRZDEC.getIntData(), BRZDEC);
        ALL_OPERATIONS.put(BRNZDEC.getIntData(), BRNZDEC);
        ALL_OPERATIONS.put(BRCR.getIntData(), BRCR);
        ALL_OPERATIONS.put(BRNCR.getIntData(), BRNCR);
        ALL_OPERATIONS.put(BRBOOL.getIntData(), BRBOOL);
        ALL_OPERATIONS.put(BRNBOOL.getIntData(), BRNBOOL);
        ALL_OPERATIONS.put(BRSGN.getIntData(), BRSGN);
        ALL_OPERATIONS.put(BRNSGN.getIntData(), BRNSGN);
        ALL_OPERATIONS.put(REDINS.getIntData(), REDINS);
        ALL_OPERATIONS.put(POPLOOP.getIntData(), POPLOOP);
        ALL_OPERATIONS.put(PUSHLOOP.getIntData(), PUSHLOOP);
        ALL_OPERATIONS.put(ACTIVATE.getIntData(), ACTIVATE);
        ALL_OPERATIONS.put(WHERE.getIntData(), WHERE);
        ALL_OPERATIONS.put(ELSEW.getIntData(), ELSEW);
        ALL_OPERATIONS.put(ENDWHERE.getIntData(), ENDWHERE);
        ALL_OPERATIONS.put(SETSCAN.getIntData(), SETSCAN);
        ALL_OPERATIONS.put(SCANLOAD.getIntData(), SCANLOAD);
        ALL_OPERATIONS.put(IXLOAD.getIntData(), IXLOAD);
        ALL_OPERATIONS.put(GLSH.getIntData(), GLSH);
        ALL_OPERATIONS.put(BRSHIFT.getIntData(), BRSHIFT);
        ALL_OPERATIONS.put(DELETE.getIntData(), DELETE);
        ALL_OPERATIONS.put(SETRED.getIntData(), SETRED);
        ALL_OPERATIONS.put(SRLOAD.getIntData(), SRLOAD);
        ALL_OPERATIONS.put(SRSTORE.getIntData(), SRSTORE);
        ALL_OPERATIONS.put(INSVAL.getIntData(), INSVAL);
        ALL_OPERATIONS.put(SHRIGHTC.getIntData(), SHRIGHTC);
        ALL_OPERATIONS.put(SHRIGHT.getIntData(), SHRIGHT);
        ALL_OPERATIONS.put(SHARIGHT.getIntData(), SHARIGHT);
        ALL_OPERATIONS.put(ROTATE.getIntData(), ROTATE);
        ALL_OPERATIONS.put(ADDRLD.getIntData(), ADDRLD);
        ALL_OPERATIONS.put(IOLOAD.getIntData(), IOLOAD);
        ALL_OPERATIONS.put(IOSTORE.getIntData(), IOSTORE);
        ALL_OPERATIONS.put(INSERTIO.getIntData(), INSERTIO);
        ALL_OPERATIONS.put(SENDINT.getIntData(), SENDINT);
        ALL_OPERATIONS.put(START.getIntData(), START);
        ALL_OPERATIONS.put(STOP.getIntData(), STOP);
        ALL_OPERATIONS.put(PARAM.getIntData(), PARAM);
        ALL_OPERATIONS.put(PRUN.getIntData(), PRUN);
        //ALL_OPERATIONS.put(PLOAD.getIntData(), PLOAD);
        ALL_OPERATIONS.put(PCHECK.getIntData(), PCHECK);
    }
//-------------------------------------------------------------------------------------
    public Operation(String _name, final int _data) {
        this(_name, _data, false);
    }

//-------------------------------------------------------------------------------------
    public Operation(String _name, final int _data, boolean _isJump) {
        super(_name, 5, _data);
        isJump = _isJump;
    }

//-------------------------------------------------------------------------------------
    public boolean isJump() {
        return isJump;
    }

//-------------------------------------------------------------------------------------
    public static Operation getOperation(String _name) {
        switch (_name.toUpperCase().trim()) {
            case "ADD"       : return ADD     ;//         # {cr, acc} <= acc + op
            case "ADDC"      : return ADDC    ;//        # {cr, acc} <= acc + op + cr
            case "SUB"       : return SUB     ;//         # {cr, acc} <= acc - op
            case "RSUB"      : return RSUB    ;//        # {cr, acc} <= op - acc
            case "SUBC"      : return SUBC    ;//        # {cr, acc} <= acc - op - cr
            case "RSUBC"     : return RSUBC   ;//       # {cr, acc} <= op - acc - cr
            case "MULT"      : return MULT    ;//        # acc <= acc * op
            case "BWAND"     : return BWAND   ;//       # acc <= acc & op
            case "BWOR"      : return BWOR    ;//        # acc <= acc | op
            case "BWXOR"     : return BWXOR   ;//       # acc <= acc ^ op;
            case "LOAD"      : return LOAD    ;//        # acc <= op
            case "STORE"     : return STORE   ;//       # op <= acc
            case "SEND"      : return SEND    ;//        # send op as coOperand to array (NF2) (CONTROLLER ONLY!)
            case "COMPARE"   : return COMPARE ;//     # cr <= (acc - op)[n]
            case "SRCALL"    : return SRCALL  ;//      # search in all cells (ARRAY ONLY!)
            case "SEARCH"    : return SEARCH  ;//      # search in selected cells (ARRAY ONLY!)
            case "CSEARCH"   : return CSEARCH ;//     # conditional search (ARRAY ONLY!)
            case "INSERT"    : return INSERT  ;//      # insert op at first (ARRAY ONLY!)
            case "JMP"       : return JMP     ;//         # pc <= pc + scalar (CONTROLLER ONLY!)
            case "BRZ"       : return BRZ     ;//         # pc <= acc=0 ? pc + scalar : pc + 1 (CONTROLLER ONLY!)
            case "BRNZ"      : return BRNZ    ;//        # pc <= acc=0 ? pc + 1 : pc + scalar (CONTROLLER ONLY!)
            case "BRZDEC"    : return BRZDEC  ;//      # pc <= acc=0 ? pc + scalar : pc + 1; acc <= acc - 1 (CONTROLLER ONLY!)
            case "BRNZDEC"   : return BRNZDEC ;//     # pc <= acc=0 ? pc + 1 : pc + scalar; acc <= acc - 1 (CONTROLLER ONLY!)
            case "BRCR"      : return BRCR    ;//        # pc <= cr ? pc + scalar : pc + 1 (CONTROLLER ONLY!)
            case "BRNCR"     : return BRNCR   ;//       # pc <= cr ? pc + 1 : pc + scalar (CONTROLLER ONLY!)
            case "BRBOOL"    : return BRBOOL  ;//      # pc <= bool=0 ? pc + scalar : pc + 1 (CONTROLLER ONLY!)
            case "BRNBOOL"   : return BRNBOOL ;//     # pc <= bool=0 ? pc + 1 : pc + scalar (CONTROLLER ONLY!)
            case "BRSGN"     : return BRSGN   ;//       # pc <= bool=0 ? pc + scalar : pc + 1 (CONTROLLER ONLY!)
            case "BRNSGN"    : return BRNSGN  ;//      # pc <= bool=0 ? pc + 1 : pc + scalar (CONTROLLER ONLY!)
            case "REDINS"    : return REDINS  ;//      # insert the output of reduction in shift register (CONTROLLER ONLY!)
            case "POPLOOP"   : return POPLOOP ;//     # pc <= acc=0 ? pc + 1 : pc;  acc <= (recEmpty | ca[0]) ? acc : {acc-1 & popData } (CONTROLLER ONLY!)
            case "PUSHLOOP"  : return PUSHLOOP;//    # pc <= acc=0 ? pc + 1 : pc;  acc <= (sentFull | !ca[p-1]) ? acc : {acc-1 & pushData} (CONTROLLER ONLY!)
            case "ACTIVATE"  : return ACTIVATE;//    # actVect[i] = 0 (ARRAY ONLY!)
            case "WHERE"     : return WHERE   ;//       # boolVect <= (boolVect & cond[scalar[1:0]]) ? 1 : 0 (ARRAY ONLY!)
            case "ELSEW"     : return ELSEW   ;//       # boolVect <= ~boolVect (ARRAY ONLY!)
            case "ENDWHERE"  : return ENDWHERE;//,   # boolVect <= 1 (ARRAY ONLY!)
            case "SETSCAN"   : return SETSCAN ;//      # set scan function to value[2:0]
            case "SCANLOAD"  : return SCANLOAD;//     # acc <= scanOut
            case "IXLOAD"    : return IXLOAD  ;//      # index load: acc[i] <= i (ARRAY ONLY!)
            case "GLSH"      : return GLSH    ;//        # global left shift (ARRAY ONLY!)
            case "BRSHIFT"   : return BRSHIFT ;//     # Boolean vector right shift; for read from array (ARRAY ONLY!)
            case "DELETE"    : return DELETE  ;//      # delete first (ARRAY ONLY!)
            case "SETRED"    : return SETRED  ;//      # set reduction function to value[1:0]: 00= nop 01=add, 10=min, 11=max (ARRAY ONLY!)
            case "SRLOAD"    : return SRLOAD  ;//      # serial register load: acc[i] <= serialReg[i] (ARRAY ONLY!)
            case "SRSTORE"   : return SRSTORE ;//     # serial register store: serialReg[i] <= acc[i] (ARRAY ONLY!)
            case "INSVAL"    : return INSVAL  ;//      # acc <= {acc[23:0], scalar}
            case "SHRIGHTC"  : return SHRIGHTC;//    # {cr, acc} <= {acc[0], cr, acc[n-1:1]}
            case "SHRIGHT"   : return SHRIGHT ;//     # {cr, acc} <= {acc[0], 1'b0, acc[n-1:1]}
            case "SHARIGHT"  : return SHARIGHT;//    # {cr, acc} <= {acc[0], acc[n-1], acc[n-1:1]}
            case "ROTATE"    : return ROTATE  ;//      # {cr, acc} <= {cr, acc rotate right with value[4:0] positions}
            case "ADDRLD"    : return ADDRLD  ;//      # addr[i] <= acc[i] (ARRAY ONLY!)
            case "IOLOAD"    : return IOLOAD  ;//      # input-output register load: acc[i] <= ioReg[i]; ioRst  (NF1) (ARRAY ONLY!)
            case "IOSTORE"   : return IOSTORE ;//     # input-output register store: ioReg[i] <= acc[i]; ioSet (NF1) (ARRAY ONLY!)
            case "INSERTIO"  : return INSERTIO;//    # insert in ioReg: ioReg <= <acc, ioReg[0],...,ioReg[2^x-2] (ARRAY ONLY!)
            case "SENDINT"   : return SENDINT ;//     # send interrupt (CONTROLLER ONLY!)
            case "START"     : return START   ;//       # start cycle counter (CONTROLLER ONLY!)
            case "STOP"      : return STOP    ;//        # stop cycle counter (CONTROLLER ONLY!)
            case "PARAM"     : return PARAM   ;//       # acc <= prog (CONTROLLER ONLY!)
            case "PRUN"      : return PRUN    ;//        # program run:  cPRUN(startAddr) => {16'b0, 8'b1111_0111, startAddr[7:0]} (CONTROLLER ONLY!)
            //case "PLOAD"     : return PLOAD   ;//       # program load:     cPLOAD => {16'b0, 8'b1111_1111, 7'b0} (CONTROLLER ONLY!)
            case "PCHECK"    : return PCHECK;
            default          : return null;
        }
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
