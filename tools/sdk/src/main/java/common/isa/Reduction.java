package xpu.sw.tools.sdk.common.isa;

public abstract class Reduction {
    public static final int NETWORK_OP_NOP = 0;
    public static final int NETWORK_OP_ADD = 1;
    public static final int NETWORK_OP_SUM_PREFIX = 1;
    public static final int NETWORK_OP_SUB = 2;
    public static final int NETWORK_OP_MIN = 3;
    public static final int NETWORK_OP_MAX = 4;
    public static final int NETWORK_OP_SPLIT = 5;
    public static final int NETWORK_OP_PERMUTE = 6;
    public static final int NETWORK_OP_bitwise_OR = 7;
    public static final int NETWORK_OP_bitwise_AND = 8;
    public static final int NETWORK_OP_bitwise_XOR = 9;
    public static final int NETWORK_OP_bitwise_XOR_PREFIX = 9;
}
