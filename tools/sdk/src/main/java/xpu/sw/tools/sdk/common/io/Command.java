//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
public class Command {

    public static final int COMMAND_RESERVED                    = 0;
    public static final int COMMAND_HALT                        = 1;
    public static final int COMMAND_RESET                       = 2;
    public static final int COMMAND_IDLE                        = 3;
    public static final int COMMAND_RUN                         = 4;

    public static final int COMMAND_GET_ARCHITECTURE_ID         = 40000;

    public static final int COMMAND_DEBUG_MODE                  = 5;

    public static final int COMMAND_LOAD_CODE_MEMORY            = 6;
    public static final int COMMAND_LOAD_DATA_MEMORY            = 7;
    public static final int COMMAND_UNLOAD_DATA_MEMORY          = 8;
    public static final int COMMAND_LOAD_FEATURE_MEMORY         = 9;
    public static final int COMMAND_UNLOAD_FEATURE_MEMORY       = 10;

    public static final int COMMAND_DONE                        = 100;
    public static final int COMMAND_ERROR                       = 101;
    public static final int COMMAND_RETRY                       = 102;
    public static final int COMMAND_BREAKPOINT_HIT              = 103;

    public static final int COMMAND_GET_RESOURCE                = 200;

    public static final int COMMAND_RUN_GRAPH                   = 400;

    public static final int COMMAND_DEBUG_ADD_BREAKPOINT                         = 401;
    public static final int COMMAND_DEBUG_REMOVE_BREAKPOINT                      = 402;
    public static final int COMMAND_DEBUG_REMOVE_ALL_BREAKPOINTS                 = 403;

    public static final int COMMAND_DEBUG_READ_CONTROLLER_REGISTRY              = 500;
    public static final int COMMAND_DEBUG_READ_CONTROLLER_MEMORY_INSTRUCTIONS   = 501;
    public static final int COMMAND_DEBUG_READ_CONTROLLER_MEMORY_DATA           = 502;
    public static final int COMMAND_DEBUG_READ_ARRAY_REGISTRY                    = 503;
    public static final int COMMAND_DEBUG_READ_ARRAY_MEMORY_INSTRUCTIONS         = 504;
    public static final int COMMAND_DEBUG_READ_ARRAY_MEMORY_DATA                 = 505;

    public static final int COMMAND_DEBUG_WRITE_CONTROLLER_REGISTRY             = 600;
    public static final int COMMAND_DEBUG_WRITE_CONTROLLER_MEMORY_INSTRUCTIONS  = 601;
    public static final int COMMAND_DEBUG_WRITE_CONTROLLER_MEMORY_DATA          = 602;
    public static final int COMMAND_DEBUG_WRITE_ARRAY_REGISTRY                   = 603;
    public static final int COMMAND_DEBUG_WRITE_ARRAY_MEMORY_INSTRUCTIONS        = 604;
    public static final int COMMAND_DEBUG_WRITE_ARRAY_MEMORY_DATA                = 605;


    public static final int COMMAND_PING                        = 1000;
    public static final int COMMAND_ACK                         = 1001;

    public static final int COMMAND_OPEN_CONNECTION             = 10000;
    public static final int COMMAND_CLOSE_CONNECTION            = 10001;

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

            case COMMAND_GET_RESOURCE:                  return "COMMAND_GET_RESOURCE";
            case COMMAND_RUN_GRAPH:                     return "COMMAND_RUN_GRAPH";

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
