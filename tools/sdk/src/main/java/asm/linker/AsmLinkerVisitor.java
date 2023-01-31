//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.asm.linker;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------
public class AsmLinkerVisitor<T> extends AsmBaseVisitor<T> {
    private Logger log;
    private Context context;
    private AsmLinker linker;
    private HexFile hex;

//-------------------------------------------------------------------------------------
    public AsmLinkerVisitor(Context _context, AsmLinker _linker, HexFile _hex) {
        context = _context;
        log = _context.getLog();
        linker = _linker;
        hex = _hex;
    }

//-------------------------------------------------------------------------------------
    public T visit(ParseTree tree) {
//        log.debug("visit....");
        return super.visit(tree);
    }


/*
//-------------------------------------------------------------------------------------
    public void loadContext(AsmParser.StartContext _s, String _path) {
        List<AsmParser.IncludeContext> _li = _s.include();
        loadIncludeContext(_li, _path);
        List<AsmParser.FuncContext> _lm = _s.module();
        loadFuncContext(_lm);
        List<AsmParser.Func_declarationContext> _lmd = _s.module_declaration();
        vresolver.loadContext(_lmd);
        debug.outln("");        T _result = defaultResult();
        return _result;
    }

//-------------------------------------------------------------------------------------
    public void loadFuncContext(List<AsmParser.FuncContext> _l) {
        for(int i = 0; i < _l.size(); i++){
            AsmParser.FuncContext _m = _l.get(i);
            String _name = _m.name().NAME().getText();
            VFunc _vm = vresolver.getFuncByName(_name, null);
            if(_vm == null){
                debug.outln(Debug.ERROR, "Cannot find module:"+_name);
                System.exit(0);
                       T _result = defaultResult();
        return _result;
    }
            _vm.loadContext(debug, this, vresolver, _m, i);
            modules.add(_vm);
                   T _result = defaultResult();
        return _result;
    }        T _result = defaultResult();
        return _result;
    }
*/

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#parse}      
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitParse(AsmParser.ParseContext _ctx){
//        log.debug("parse....");

        return super.visitParse(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#line}       
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitLine(AsmParser.LineContext _ctx){
//        log.debug("line....");

        return super.visitLine(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#instruction}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitInstruction(AsmParser.InstructionContext _ctx){
        T _result = super.visitInstruction(_ctx);
        log.debug("instruction: " + _result);
        return _result;
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#instr}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitInstr(AsmParser.InstrContext _ctx){
        log.debug("instr: " + _ctx);

        return super.visitInstr(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#label}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitLabel(AsmParser.LabelContext _ctx){

        return super.visitLabel(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#directive}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitDirective(AsmParser.DirectiveContext _ctx){
//        log.debug("directive....");
        return super.visitDirective(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#define}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitDefine(AsmParser.DefineContext _ctx){

        return super.visitDefine(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#include}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitInclude(AsmParser.IncludeContext _ctx){
//        log.debug("include... " + _ctx);
/*        String _filename = _ctx.FILEPATH().getText();
        _filename = _filename.replace("\"","");
        linker.loadFile(".", _filename);       */ 
        return super.visitInclude(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#func}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitFunc(AsmParser.FuncContext _ctx){
        return super.visitFunc(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#endfunc}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitEndfunc(AsmParser.EndfuncContext _ctx){
        return super.visitEndfunc(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#lb}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitLb(AsmParser.LbContext _ctx){
        return super.visitLb(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#expression}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitExpression(AsmParser.ExpressionContext _ctx){
        return super.visitExpression(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#multiplyingExpression}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitMultiplyingExpression(AsmParser.MultiplyingExpressionContext _ctx){
        return super.visitMultiplyingExpression(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#value}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitOperand(AsmParser.ValueContext _ctx){
        return super.visitValue(_ctx);
    }


//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#name}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitName(AsmParser.NameContext _ctx){
        return super.visitName(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#number}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitNumber(AsmParser.NumberContext _ctx){
        return super.visitNumber(_ctx);
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#opcode}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitOpcode(AsmParser.OpcodeContext _ctx){
        return super.visitOpcode(_ctx);
//        switch (_ctx.OPCODE().getText()) {
            
//        }
    }

//-------------------------------------------------------------------------------------
    /**
     * Visit a parse tree produced by {@link AsmParser#comment}
     * @param _ctx the parse tree
     * @return the visitor result
     */
    public T visitComment(AsmParser.CommentContext _ctx){
        return super.visitComment(_ctx);
    }
/*
//-------------------------------------------------------------------------------------
    public T visitErrorNode(ErrorNode _errorNode){
        T _result = defaultResult();
        return _result;
    }

//-------------------------------------------------------------------------------------
    public T visitTerminal(TerminalNode _terminalNode){
        T _result = defaultResult();
        return _result;
    }

//-------------------------------------------------------------------------------------
    public T visitChildren(RuleNode _ruleNode){
        T _result = defaultResult();
        return _result;
    }

//-------------------------------------------------------------------------------------
    public T visitChildren(RuleNode _ruleNode){
        T _result = defaultResult();
        return _result;
    }
*/

//-------------------------------------------------------------------------------------

}