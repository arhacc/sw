//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.lucene.util.*;
import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class Expression {
    private Application application;
    private Callable callable;
    private AsmParser.ExpressionContext expressionContext;
    private boolean isResolved;
    private int value;

//-------------------------------------------------------------------------------------
    public Expression(Application _application, AsmParser.ExpressionContext _expression) {
//        super(_context);
        application = _application;
        expressionContext = _expression;
        isResolved = false;
        value = -1;
    }

//-------------------------------------------------------------------------------------
    public Expression(Callable _callable, AsmParser.ExpressionContext _expression) {
//        super(_context);
        application = _callable.getApplication();
        callable = _callable;
        expressionContext = _expression;
        isResolved = false;
        value = -1;
    }

//-------------------------------------------------------------------------------------
    public Application getApplication() {
        return application;
    }

//-------------------------------------------------------------------------------------
    public int resolve() {
        if(isResolved){
            return value;
        } else {
            return resolve(expressionContext);
        }
    }
/*
   | value
   | SIGN expression
   | OP_UNARY '(' expression ')'
   | expression OP_DUAL expression

*/
//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.ExpressionContext _expression) {
//   | value
        AsmParser.ValueContext _valueContext = _expression.value();
        if(_valueContext != null){
            return resolve(_valueContext);
        }

//   : SIGN expression
/*        TerminalNode _sign = _expression.SIGN();
        int _result = resolve(_expression.expression(0));
        if(_sign != null){            
            return _sign.getText().equals("-")? -_result : _result;
        }
*/
        TerminalNode _opUnary = _expression.OP_UNARY();
        if(_opUnary != null){
            return resolve(null, _opUnary.getText(), _expression.expression(0));
        }

        TerminalNode _opDual = _expression.OP_DUAL();
        if(_opDual != null){
            return resolve(_expression.expression(0), _opDual.getText(), _expression.expression(1));
        }
//        log.error("Unknown expression type:" + _expression);
        return -1;
    }

/*

value
   : number
   | name
   |'(' expression ')'
   ;

*/
//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.ValueContext _valueContext) {
        AsmParser.NumberContext _numberContext = _valueContext.number();
        if(_numberContext != null){
            return resolve(_numberContext);
        }

        AsmParser.NameContext _nameContext = _valueContext.name();
        if(_nameContext != null){
            return resolve(_nameContext);
        }

        AsmParser.ExpressionContext _expression = _valueContext.expression();
        if(_expression != null){
            return resolve(_expression);
        }
        return -1;
    }

//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.NumberContext _numberContext) {
        TerminalNode _sign = _numberContext.SIGN();
        TerminalNode _number = _numberContext.NUMBER();
        int _numberInt = Integer.parseInt(_number.getText());
        if(_sign != null){
            return _sign.getText().equals("-") ? -_numberInt : _numberInt; 
        }
        return _numberInt;
    }

//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.NameContext _nameContext) {
        TerminalNode _name = _nameContext.NAME();
        if(_name != null){
            return resolve(_name.getText());
        }
        return -1;
    }

//-------------------------------------------------------------------------------------
    private int resolve(String _name) {
        if(_name.startsWith("$")){
            _name = _name.substring(1);
            Expression _expression = getApplication().getDefine(_name);
            if(_expression != null){
                return _expression.resolve();
            }
            return callable.getArchitectureImplementation().get(_name);
        }
        return -1;
    }

//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.ExpressionContext _expression0, String _operation, AsmParser.ExpressionContext _expression1) {
        switch(_operation){
            case "+": {
                return resolve(_expression0) + resolve(_expression1);
            }
            case "-": {
                return resolve(_expression0) - resolve(_expression1);                
            }
            case "*": {
                return resolve(_expression0) * resolve(_expression1);
            }
            case "/": {
                return resolve(_expression0) / resolve(_expression1);                
            }
            case "log": {
                return (int) (Math.log(resolve(_expression0)) / Math.log(2));
            }
            default: {
                return 0;
            }
        }
    }

/*//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.ExpressionContext _expression) {
        if(_argument.startsWith("$")){
            _argument = _argument.substring(1);
            return callable.getArchitectureImplementation().get(_argument);
        }
        try {
            return Integer.parseInt(_argument);
        } catch(NumberFormatException _e){
            System.out.println("Cannot convert argument [" + _argument + "] to integer!");
            _e.printStackTrace();
            return -1;
        }
    }
    
//-------------------------------------------------------------------------------------
    private String getStringFromExpression(AsmParser.ExpressionContext _expression) {
        AsmParser.NumberContext _numberContext = _expression.multiplyingExpression(0).value(0).number();
        String _tmp = "";
        if(_numberContext.SIGN() != null){
            _tmp += _numberContext.SIGN().getText();
        }
        _tmp += _numberContext.NUMBER().getText();
        return _tmp;
    }
*/
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
