//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
public class Parameters {

    public static final int LEVEL_LOW           = 0;
    public static final int LEVEL_MID           = 1;
    public static final int LEVEL_APP           = 2;

//-------------------------------------------------------------------------------------
    public static String toString(int _commandCode) {
        switch (_commandCode) {
            case COMMAND_RESERVED:                      return "COMMAND_RESERVED";
            case COMMAND_HALT:                          return "COMMAND_HALT";
            case COMMAND_RESET:                         return "COMMAND_RESET";
            case COMMAND_IDLE:                          return "COMMAND_IDLE";
            case COMMAND_RUN:                           return "COMMAND_RUN";

            case COMMAND_DEBUG_MODE:                    return "COMMAND_DEBUG_MODE";
            case COMMAND_LOAD_CODE_MEMORY:              return "COMMAND_LOAD_CODE_MEMORY";
            case COMMAND_LOAD_DATA_MEMORY:              return "COMMAND_LOAD_DATA_MEMORY";
            case COMMAND_UNLOAD_DATA_MEMORY:            return "COMMAND_UNLOAD_DATA_MEMORY";
            case COMMAND_LOAD_FEATURE_MEMORY:           return "COMMAND_LOAD_FEATURE_MEMORY";
            case COMMAND_UNLOAD_FEATURE_MEMORY:         return "COMMAND_UNLOAD_FEATURE_MEMORY";

            case COMMAND_DONE:                          return "COMMAND_DONE";
            case COMMAND_ERROR:                         return "COMMAND_ERROR";
            case COMMAND_RETRY:                         return "COMMAND_RETRY";

            case COMMAND_RUN_FILE_ONNX:                 return "COMMAND_RUN_FILE_ONNX";

            case COMMAND_PING:                          return "COMMAND_PING";
            case COMMAND_ACK:                           return "COMMAND_ACK";

            case COMMAND_OPEN_CONNECTION:               return "COMMAND_OPEN_CONNECTION";
            case COMMAND_CLOSE_CONNECTION:              return "COMMAND_CLOSE_CONNECTION";
            default:                                    return "COMMAND_UNKNOWN";
        }
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
