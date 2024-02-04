//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa.flow;
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
public class Expression implements Serializable {
    private Application application;
    private Callable callable;
    private transient AsmParser.ExpressionContext expressionContext;
    private boolean isResolved;
    private int value;

//-------------------------------------------------------------------------------------
    public Expression(AsmParser.ExpressionContext _expressionContext) {
        this(null, null, _expressionContext);
    }

//-------------------------------------------------------------------------------------
    public Expression(Application _application, AsmParser.ExpressionContext _expressionContext) {
        this(_application, null, _expressionContext);
    }

//-------------------------------------------------------------------------------------
    public Expression(Callable _callable, AsmParser.ExpressionContext _expressionContext) {
        this(null, _callable, _expressionContext);
    }

//-------------------------------------------------------------------------------------
    public Expression(Application _application, Callable _callable, AsmParser.ExpressionContext _expressionContext) {
        application = _application;
        setCallable(_callable);
        expressionContext = _expressionContext;
        isResolved = false;
        value = -1;
    }

//-------------------------------------------------------------------------------------
    public Application getApplication() {
        return application;
    }

//-------------------------------------------------------------------------------------
    public Callable getCallable() {
        return callable;
    }

//-------------------------------------------------------------------------------------
    public void setCallable(Callable _callable) {
        callable = _callable;
        if(_callable != null){
            application = _callable.getApplication();
        }      
        isResolved = false;  
    }

//-------------------------------------------------------------------------------------
    public AsmParser.ExpressionContext getExpressionContext() {
        return expressionContext;
    }

//-------------------------------------------------------------------------------------
    public int resolve() {
        if(isResolved){
            return value;
        } else {
            value = resolve(expressionContext);
            isResolved = true;
            return value;
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
/*
//   | value
        AsmParser.ValueContext _valueContext = _expression.value();
        if(_valueContext != null){
            return resolve(_valueContext);
        }


//   : SIGN expression
        TerminalNode _sign = _expression.SIGN();
        int _result = resolve(_expression.expression(0));
        if(_sign != null){            
            return _sign.getText().equals("-")? -_result : _result;
        }

        TerminalNode _opUnary = _expression.OP_UNARY();
        if(_opUnary != null){
            return resolve(null, _opUnary.getText(), _expression.expression(0));
        }

        TerminalNode _opDual = _expression.OP_DUAL();
        if(_opDual != null){
            return resolve(_expression.expression(0), _opDual.getText(), _expression.expression(1));
        }
//        log.out("Unknown expression type:" + _expression);
        return -1;
*/
        boolean _adding = true;
        int _value = 0;
        for(int i = 0; i < _expression.getChildCount(); i++){
            ParseTree _child = _expression.getChild(i);
            if(_child instanceof TerminalNode){
                _adding = _child.getText().equals("+");
            } else {
                int _multiplyingValue = resolve((AsmParser.MultiplyingExpressionContext)_child);
                if(_adding){
                    _value += _multiplyingValue;
                } else {
                    _value -= _multiplyingValue;
                }
            }
        }
//        out(">resolve1.:" + _value);
        return _value;
    }

/*

value
   : number
   | name
   |'(' expression ')'
   ;

*/
//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.MultiplyingExpressionContext _multiplyingExpressionContext) {
        boolean _multiplying = true;
        int _value = 1;
        for(int i = 0; i < _multiplyingExpressionContext.getChildCount(); i++){
            ParseTree _child = _multiplyingExpressionContext.getChild(i);
            if(_child instanceof TerminalNode){
                _multiplying = _child.getText().equals("*");
            } else {
                int _signedAtomValue = resolve((AsmParser.SignedAtomContext)_child);
                if(_multiplying){
                    _value *= _signedAtomValue;
                } else {
                    _value /= _signedAtomValue;
                }
            }
        }
//        out(">resolve2.:" + _value);
        return _value;
    }
/*
//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.PowExpressionContext _powExpressionContext) {
        boolean _pow = true;
        int _value = 0;
        for(int i = 0; i < _powExpressionContext.getChildCount(); i++){
            ParseTree _child = _powExpressionContext.getChild(i);
            if(_child instanceof TerminalNode){
                _pow = _child.getText().equals("^");
            } else {
                int _signedAtomValue = resolve((AsmParser.SignedAtomContext)_child);
                if(_pow){
                    _value = _value ^ _signedAtomValue;
                } else {
                    _value = _signedAtomValue;
                }
            }
        }
        return _value;
    }
*/
//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.SignedAtomContext _signedAtomContext) {
        TerminalNode _plus = _signedAtomContext.PLUS();
        if(_plus != null){            
            return resolve(_signedAtomContext.signedAtom());
        }
        TerminalNode _minus = _signedAtomContext.MINUS();
        if(_minus != null){            
            return -resolve(_signedAtomContext.signedAtom());
        }
        AsmParser.FunctionContext _functionContext = _signedAtomContext.function();
        if(_functionContext != null){            
            return resolve(_functionContext);
        }
        AsmParser.AtomContext _atomContext = _signedAtomContext.atom();
        if(_atomContext != null){            
            return resolve(_atomContext);
        }
        out("Cannot resolve SignedAtomContext:" + _signedAtomContext);
        return -1;
    }


//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.FunctionContext _functionContext) {
        AsmParser.FuncnameContext _funcnameContext = _functionContext.funcname();
        if(_funcnameContext != null){
            int _expressionValue = resolve(_functionContext.expression());
            if(_funcnameContext.LOG2() != null){
                return (int) (Math.log(_expressionValue) / Math.log(2));
            } else {
                return (int)Math.sqrt(_expressionValue);
            }
        } else {
            out("Cannot find funcname in functionContext:" + _functionContext);
        }
        return -1;
    }

//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.AtomContext _atomContext) {
        AsmParser.NumberContext _numberContext = _atomContext.number();
        if(_numberContext != null){
            return resolve(_numberContext);
        }

        AsmParser.NameContext _nameContext = _atomContext.name();
        if(_nameContext != null){
            return resolve(_nameContext);
        }

        AsmParser.ExpressionContext _expression = _atomContext.expression();
        if(_expression != null){
            return resolve(_expression);
        }
        return -1;
    }

//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.NumberContext _numberContext) {
//        TerminalNode _sign = _numberContext.SIGN();
        TerminalNode _number = _numberContext.NUMBER();
        int _numberInt = Integer.parseInt(_number.getText());
//        out(">resolve3.:" + _numberInt);

/*        if(_sign != null){
            return _sign.getText().equals("-") ? -_numberInt : _numberInt; 
        }*/
        return _numberInt;
    }

//-------------------------------------------------------------------------------------
    private int resolve(AsmParser.NameContext _nameContext) {
        TerminalNode _name = _nameContext.NAME();
        if(_name != null){
            return resolve(_name.getText());
        }
        out("Unresolved NameContext: " + _nameContext);
        return -1;
    }

//-------------------------------------------------------------------------------------
    private int resolve(String _name) {
        if(_name.startsWith("$")){
            return resolveDefine(_name.substring(1));
        } else {
//            out("Expression.resolve["+_name+"] @ macro: " + callable);
            return ((Macro)callable).resolve(_name);
        }
    }

//-------------------------------------------------------------------------------------
    private int resolveDefine(String _name) {
        Expression _expression = null;
        Callable _callable = getCallable();
        if(_callable instanceof Primitive){
            _expression = ((Primitive)_callable).getConst(_name);
        }
        if(_expression != null){
            return _expression.resolve();
        }
        _expression = getApplication().getDefine(_name);
        if(_expression != null){
            return _expression.resolve();
        }
        return getApplication().getArchitectureImplementation().get(_name);
    }

/*
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
*/
//-------------------------------------------------------------------------------------
    private void out(String _message) {
        System.out.println(_message);
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
