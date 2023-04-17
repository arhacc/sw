//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils.switcher;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;

//-------------------------------------------------------------------------------------
public class Switcher extends XBasic {
    private Map<Integer, Command> caseCommands;
    private Command defaultCommand;

//-------------------------------------------------------------------------------------
    public Switcher(Context _context) {
        super(_context);
        caseCommands = new HashMap<Integer, Command>();

        setDefaultCaseCommand(new DoNothingCommand());
    }

//-------------------------------------------------------------------------------------
    private Command getCaseCommandByCaseId(Integer caseId) {
        if (caseCommands.containsKey(caseId)) {
            return caseCommands.get(caseId);
        } else {
            return defaultCommand;
        }
    }

//-------------------------------------------------------------------------------------
    public void addCaseCommand(Integer caseId, Command caseCommand) {
        caseCommands.put(caseId, caseCommand);
    }

//-------------------------------------------------------------------------------------
    public void setDefaultCaseCommand(Command defaultCommand) {
        if (defaultCommand != null) {
            this.defaultCommand = defaultCommand;
        }
    }

//-------------------------------------------------------------------------------------
    public void on(Integer caseId, int _operandValue) {
        Command command = getCaseCommandByCaseId(caseId);

        command.execute(_operandValue);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
