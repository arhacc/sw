//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;

import xpu.sw.tools.sdk.asm.parser.*;


//-------------------------------------------------------------------------------------
public class Macro extends Callable {
    private List<String> parameters;

//-------------------------------------------------------------------------------------
    public Macro(Context _context, String _architectureId, String _name, Application _application, AsmParser.ParametersNamesContext _parametersNames) {
        super(_context, _architectureId, _name, _application);
        parameters = new ArrayList<String>();
        List<AsmParser.NameContext> _parameters = _parametersNames.name();
        for(int i = 0; i < _parameters.size(); i++){
            parameters.add(_parameters.get(i).NAME().getText());
        }
    }

//-------------------------------------------------------------------------------------
    public Macro(Context _context, String _architectureId, String _name, Application _application, List<String> _parameters) {
        super(_context, _architectureId, _name, _application);
        parameters = _parameters;
    }

//-------------------------------------------------------------------------------------
    public Macro copyOf(){
        Macro _macro = new Macro(context, architectureId, name, application, parameters);
        List<InstructionLine> _macroInstructionLines = getAll();
        for(int i = 0; i < _macroInstructionLines.size(); i++){
            InstructionLine _instructionLine = _macroInstructionLines.get(i);
            _macro.addInstruction(_instructionLine.copyOf(), instructionLinesText.get(i));
        }

        _macro.labelsByName = labelsByName;
        _macro.labelsByRelativeAddress = new HashMap<Integer, Location>(labelsByRelativeAddress);

        _macro.index = index;
        _macro.architectureImplementation = getArchitectureImplementation();
        return _macro;
    }

//-------------------------------------------------------------------------------------
    public void replaceParametersWithExpressions(List<AsmParser.ExpressionContext> _expressions) {
        for(int i = 0; i < instructionLines.size(); i++){
            InstructionLine _instructionLine = instructionLines.get(i);
            _instructionLine.replaceParametersWithExpressions(parameters, _expressions);
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
