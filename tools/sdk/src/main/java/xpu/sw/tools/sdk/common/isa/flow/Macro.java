//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.flow;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

//import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;

import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class Macro extends Callable {
    private List<String> parameters;
    private Map<String, Expression> expressions;

//-------------------------------------------------------------------------------------
    public Macro(Context _context, String _name, Application _application, AsmParser.ParametersNamesContext _parametersNames) {
        super(_context, _name, _application);
        parameters = new ArrayList<String>();
        List<AsmParser.NameContext> _parameters = _parametersNames.name();
        for(int i = 0; i < _parameters.size(); i++){
            parameters.add(_parameters.get(i).NAME().getText());
        }
    }

//-------------------------------------------------------------------------------------
    public Macro(Context _context, String _name, Application _application, List<String> _parameters) {
        super(_context, _name, _application);
        parameters = _parameters;
    }

//-------------------------------------------------------------------------------------
    public Callable copyOf(){
        Macro _macro = (Macro)copyOf(null);
        _macro.expressions = new HashMap<String, Expression>();
        for (Map.Entry<String, Expression> _entry : expressions.entrySet()){
            _macro.expressions.put(_entry.getKey(), new Expression(_macro, _entry.getValue().getExpressionContext()));
        }
        return _macro;
    }

//-------------------------------------------------------------------------------------
    public Macro copyOf(List<AsmParser.ExpressionContext> _expressions){
        Macro _macro = new Macro(context, name, application, parameters);
        List<Callable> _macroInstructionLines = getAll();
        for(int i = 0; i < _macroInstructionLines.size(); i++){
            Callable _instructionLine = _macroInstructionLines.get(i);
            _instructionLine = _instructionLine.copyOf();
            _macro.addLine(_instructionLine);
        }

        _macro.labels = labels;
//        _macro.labelsByRelativeAddress = new HashMap<Integer, Integer>(labelsByRelativeAddress);
/*        for (Map.Entry<Integer, Integer> _entry : labelsByRelativeAddress.entrySet()){
            _macro.labelsByRelativeAddress.put(_entry.getKey(), _entry.getValue());
        }*/

//        _macro.index = index;
//        _macro.expressions = new HashMap<String, Expression>(expressions);
//        _macro.architectureImplementation = getArchitectureImplementation();
        if(_expressions != null){
            if(parameters.size() != _expressions.size()){
                log.error("Parameter list size differ from argumet list in macro: " + getName());
                return null;
            }
            _macro.expressions = new HashMap<String, Expression>();
            for(int i = 0; i < parameters.size(); i++){
                String _parameterName = parameters.get(i);
                _macro.expressions.put(_parameterName, new Expression(_macro, _expressions.get(i)));
    //            log.error("Load parameter: " + _parameterName);
            }
        }
//        log.debug("duplicate["+_macro.getName()+"]:" + _macro + ",from:"+this);
        return _macro;
    }


//-------------------------------------------------------------------------------------
    public int resolve(String _name) {
        Expression _expression = expressions.get(_name);
        if(_expression != null){
            if(parent != null){
                _expression.setCallable(parent);
            } else {
                log.error("instructionLine cannot be null in macro:" + this);
            }
            int _value = _expression.resolve();
//            log.error("In macro: " + this + ", resolve " + _name + ": " + _value);
            return _value;
        } else {
            log.error("Cannot resolve argument: " + _name + " in " + getName() + ", instance: " + this);
        }
        return -1;
    }

/*
//-------------------------------------------------------------------------------------
    public void replaceParametersWithExpressions(List<AsmParser.ExpressionContext> _expressions) {
        for(int i = 0; i < instructionLines.size(); i++){
            InstructionLine _instructionLine = instructionLines.get(i);
            _instructionLine.replaceParametersWithExpressions(parameters, _expressions);
        }
    }
*/
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
