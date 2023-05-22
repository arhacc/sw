//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
public class Parameters {

    public static final int LEVEL_LOW           = 0;
    public static final int LEVEL_MID           = 1;
    public static final int LEVEL_APP           = 2;

//-------------------------------------------------------------------------------------
    public static String toString(int _parameterCode) {
        switch (_parameterCode) {
            case LEVEL_LOW:                 return "LEVEL_LOW";
            case LEVEL_MID:                 return "LEVEL_MID";
            case LEVEL_APP:                 return "LEVEL_APP";
            default       :                 return "UNKNOWN";
        }
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
