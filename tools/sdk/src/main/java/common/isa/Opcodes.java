package xpu.sw.tools.sdk.common.isa;

public abstract class Opcodes {
    public static final int ADDRINC = 1;
    public static final int ADD     = 2;
    public static final int ADDC    = 3;
    public static final int SUB     = 4;
    public static final int RSUB    = 5;
    public static final int SUBC    = 6;
    public static final int RSUBC   = 7;

    public static final int MULT    = 8;
    public static final int BWAND   = 9;
    public static final int BWOR    = 10;
    public static final int BWXOR   = 11;

    public static final int LOAD    = 12;
    public static final int STORE   = 13;
    public static final int COMPARE = 14;

    public static final int STACK_STORE_POP = 15;
    public static final int STACK_PUSH_LOAD = 16;
    public static final int SWAP_MEMACC     = 17;

    public static final int FADD    = 18;
    public static final int FSUB    = 19;
    public static final int FMULT   = 20;
    public static final int FDIV    = 21;
    public static final int FMULT1  = 22;
    public static final int FMULT2  = 23;
    public static final int FDIV_LOOP1   = 24;
    public static final int FDIV_LOOP2   = 25;

    public static final int SRCALL  = 26;
    public static final int SEARCH  = 27;
    public static final int CSEARCH = 28;
    public static final int INSERT  = 29;

    public static final int MISC_STORE_LOAD = 3;
    public static final int ROTATE_LOCAL = 4;
    public static final int INSVAL = 5;
    public static final int FLOAT = 6;
    public static final int RIGHT_FIXED_SHIFTING = 7;
    public static final int SPATIAL_SELECT  = 8;
    public static final int SET_SCAN        = 9;
    public static final int BR_SHIFT        = 10;
    public static final int DELETE          = 11;

    public static final int JMP          = 8;
    public static final int SEND_INT     = 9;
    public static final int MISC_TESTING = 10;
    public static final int PARAM       = 11;
    public static final int P_RUN        = 12;
    public static final int P_LOAD       = 13;
    public static final int T_IN_RUN      = 14;
    public static final int T_OUT_RUN     = 15;
    public static final int WAIT_MAT_WRITE= 16;
    public static final int RES_READY     = 17;

    public static final int SEND    = 26;
}
