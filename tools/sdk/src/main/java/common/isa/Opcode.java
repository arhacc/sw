//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class Opcode extends Field {
    private boolean isJump;

    public static final Opcode ADDRINC   = new Opcode ("addrinc");

    public static final Opcode ADD       = new Opcode ("add");
    public static final Opcode ADDC      = new Opcode ("addc");
    public static final Opcode SUB       = new Opcode ("sub");
    public static final Opcode RSUB      = new Opcode ("rsub");
    public static final Opcode SUBC      = new Opcode ("subc");
    public static final Opcode RSUBC     = new Opcode ("rsubc");
    public static final Opcode MULT      = new Opcode ("mult");

    public static final Opcode BWAND     = new Opcode ("bwand");
    public static final Opcode BWOR      = new Opcode ("bwor");
    public static final Opcode BWXOR     = new Opcode ("bwxor");

    public static final Opcode LOAD      = new Opcode ("load");
    public static final Opcode STORE     = new Opcode ("store");
    public static final Opcode COMPARE   = new Opcode ("compare");

//    public static final Opcode STKSP     = new Opcode ("stkstorepop");
//    public static final Opcode STKPL     = new Opcode ("stkpushload");

//    public static final Opcode SWAPMEMACC= new Opcode ("swapmemacc");

    public static final Opcode FADD       = new Opcode ("fadd");
    public static final Opcode FSUB       = new Opcode ("fsub");
    public static final Opcode FMULT      = new Opcode ("fmult");
    public static final Opcode FDIV       = new Opcode ("fdiv");
//    public static final Opcode FMULTF     = new Opcode ("fmultf");
//    public static final Opcode FMULTS     = new Opcode ("fmults");
//    public static final Opcode FDIVL1     = new Opcode ("fdivl1");
//    public static final Opcode FDIVL2     = new Opcode ("fdivl2");

    public static final Opcode SRCALL    = new Opcode ("srcall");
    public static final Opcode SEARCH    = new Opcode ("search");
    public static final Opcode CSEARCH   = new Opcode ("csearch");
    public static final Opcode INSERT    = new Opcode ("insert");
    public static final Opcode SEND      = new Opcode ("send");

    public static final Opcode JMP       = new Opcode ("jmp     ", true);
/*    public static final Opcode BRZ       = new Opcode ("brz     ", true);
    public static final Opcode BRNZ      = new Opcode ("brnz    ", true);
    public static final Opcode BRZDEC    = new Opcode ("brzdec  ", true);
    public static final Opcode BRNZDEC   = new Opcode ("brnzdec ", true);
    public static final Opcode BRCR      = new Opcode ("brcr    ", true);
    public static final Opcode BRNCR     = new Opcode ("brncr   ", true);
    public static final Opcode BRBOOL    = new Opcode ("brbool  ");
    public static final Opcode BRNBOOL   = new Opcode ("brnbool ");
    public static final Opcode BRSGN     = new Opcode ("brsgn   ");
    public static final Opcode BRNSGN    = new Opcode ("brnsgn  ");
*/
//    public static final Opcode REDINS    = new Opcode ("redins  ");
//    public static final Opcode POPLOOP   = new Opcode ("poploop ");
//    public static final Opcode PUSHLOOP  = new Opcode ("pushloop");

//    public static final Opcode ACTIVATE  = new Opcode ("activate");
//    public static final Opcode WHERE     = new Opcode ("where   ");
//    public static final Opcode ELSEW     = new Opcode ("elsew   ");
//    public static final Opcode ENDWHERE  = new Opcode ("endwhere");

    public static final Opcode SPATIAL_SELECT  = new Opcode ("spatial_select");



    public static final Opcode SETSCAN   = new Opcode ("setscan ");
    public static final Opcode MISC_STORE_LOAD  = new Opcode ("misc_store_load");
//    public static final Opcode IXLOAD    = new Opcode ("ixload  ");
//    public static final Opcode GLSH      = new Opcode ("glsh    ");
    public static final Opcode BRSHIFT   = new Opcode ("brshift ");
    public static final Opcode DELETE    = new Opcode ("delete  ");
//    public static final Opcode SETRED    = new Opcode ("setred  ");
//    public static final Opcode SRLOAD    = new Opcode ("srload  ");
//    public static final Opcode SRSTORE   = new Opcode ("srstore ");
    public static final Opcode INSVAL    = new Opcode ("insval  ");

    public static final Opcode RIGHT_FIXED_SHIFTING  = new Opcode ("right_fixed_shifting");
//    public static final Opcode SHRIGHT   = new Opcode ("shright ");
//    public static final Opcode SHARIGHT  = new Opcode ("sharight");
    public static final Opcode ROTATE_LOCAL    = new Opcode ("rotate_local");

//    public static final Opcode ADDRLD    = new Opcode ("addrld  ");

//    public static final Opcode IOLOAD    = new Opcode ("ioload  ");
//    public static final Opcode IOSTORE   = new Opcode ("iostore ");
//    public static final Opcode INSERTIO  = new Opcode ("insertio");
    public static final Opcode SENDINT   = new Opcode ("sendint ");

    public static final Opcode MISC_TESTING     = new Opcode ("misc_testing");
//    public static final Opcode STOP      = new Opcode ("stop    ");
    public static final Opcode PARAM     = new Opcode ("param   ");
    public static final Opcode PRUN      = new Opcode ("prun    ");
    //public static final Opcode PLOAD     = new Opcode ("pload   ", 0b11111);
//    public static final Opcode PCHECK     = new Opcode ("pcheck15 ");
    public static final Opcode STACK_STORE_POP = new Opcode ("stack_store_pop");
    public static final Opcode STACK_PUSH_LOAD = new Opcode ("stack_push_load");
    public static final Opcode SWAP_MEMACC     = new Opcode ("swap_memacc    ");

    public static final Opcode WAITMATW     = new Opcode ("waitmatw");
    public static final Opcode RESREADY     = new Opcode ("resready");

//-------------------------------------------------------------------------------------
    public Opcode(String _name) {
        this(_name, false);
    }

//-------------------------------------------------------------------------------------
    public Opcode(String _name, boolean _isJump) {
        super(_name.trim(), ArchitectureDefinitions.get("ISA_" + _name.trim()));
        isJump = _isJump;
    }

//-------------------------------------------------------------------------------------
    public boolean isJump() {
        return isJump;
    }

//-------------------------------------------------------------------------------------
    public boolean equals(Opcode _opcode) {
        return (data == _opcode.data);
    }
/*
//-------------------------------------------------------------------------------------
    public static Opcode getOperation(String _name) {
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
            case "SRCALL"    : return SRCALL  ;//      # search in all fields (ARRAY ONLY!)
            case "SEARCH"    : return SEARCH  ;//      # search in selected fields (ARRAY ONLY!)
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
            case "PRUN"      : return PRUN    ;//        # primitive run:  cPRUN(startAddr) => {16'b0, 8'b1111_0111, startAddr[7:0]} (CONTROLLER ONLY!)
            //case "PLOAD"     : return PLOAD   ;//       # primitive load:     cPLOAD => {16'b0, 8'b1111_1111, 7'b0} (CONTROLLER ONLY!)
            case "PCHECK"    : return PCHECK;
            default          : return null;
        }
    }*/
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
