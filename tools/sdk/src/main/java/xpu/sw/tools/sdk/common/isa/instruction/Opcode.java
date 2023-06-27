//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.instruction;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.arch.*;

//-------------------------------------------------------------------------------------
public class Opcode extends Field {
//    private boolean isJump;
/*
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
*/

//-------------------------------------------------------------------------------------
    public Opcode(String _name, int _data) {
        super(_name.trim(), _data);
//        isJump = _isJump;
    }


//-------------------------------------------------------------------------------------
    public boolean equals(Opcode _opcode) {
        return (data == _opcode.data);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
