//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io;
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
public class RemoteRunResponse {
    private int commandCode;
    private int[] parameters;
    private String[] outputs;

//-------------------------------------------------------------------------------------
    public RemoteRunResponse(int _commandCode) {
        this(_commandCode, 0);
    }

//-------------------------------------------------------------------------------------
    public RemoteRunResponse(int _commandCode, int _parameter) {
        this(_commandCode, new int[]{_parameter});
    }

//-------------------------------------------------------------------------------------
    public RemoteRunResponse(int _commandCode, int[] _parameters) {
        commandCode = _commandCode;
        parameters = _parameters;
    }

//-------------------------------------------------------------------------------------
    public RemoteRunResponse(int _commandCode, String[] _outputs) {
        commandCode = _commandCode;
        parameters = null;
        outputs = _outputs;
    }

//-------------------------------------------------------------------------------------
    public int getCommandCode() {
        return commandCode;
    }

//-------------------------------------------------------------------------------------
    public int getParameter() {
        return getParameter(0);
    }

//-------------------------------------------------------------------------------------
    public int getParameter(int _index) {
        return parameters[_index];
    }

//-------------------------------------------------------------------------------------
    public String[] getOutputs() {
        return outputs;
    }

//-------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
