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
import xpu.sw.tools.sdk.common.isa.builders.*;
import xpu.sw.tools.sdk.common.isa.flow.*;
import xpu.sw.tools.sdk.common.isa.instruction.*;
//import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------

public class AsmLinkerListener extends AsmBaseListener {
    private Context context;
    private Logger log;
    private AsmLinker linker;
    private Application application;

    //    private String currentArchCode;
    private Primitive currentPrimitive;
    private Macro currentMacro;
    private Callable currentCallable;

    private InstructionLine currentInstructionLine;
//    private Macro currentMacroCall;

    private ArchitectureBuilder architectureBuilder;

    private boolean success;

    //-------------------------------------------------------------------------------------
    public AsmLinkerListener (Context _context, AsmLinker _linker, Application _application) {
        context = _context;
        log = _context.getLog();
        linker = _linker;
        application = _application;
        architectureBuilder = new ArchitectureBuilder(_context);
        success = true;
    }

    //-------------------------------------------------------------------------------------
    public boolean getSuccess () {
        return success;
    }

//-------------------------------------------------------------------------------------

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterParse (AsmParser.ParseContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitParse (AsmParser.ParseContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterLine (AsmParser.LineContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitLine (AsmParser.LineContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterInstruction (AsmParser.InstructionContext _ctx) {
        currentInstructionLine = new InstructionLine(context, application);
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitInstruction (AsmParser.InstructionContext _ctx) {
//        log.debug("add currentInstruction: " + instructionLine);
//condition to avoid adding a empty instruction after macro call
//        if(!currentInstructionLine.isEmpty()){
        currentInstructionLine.getLocalization().setText("Asm line: [" + _ctx.getStart().getLine() +"]");
        currentCallable.addLine(_ctx.getStart().getLine(), currentInstructionLine);            
//        }
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterControllerInstruction (AsmParser.ControllerInstructionContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitControllerInstruction (AsmParser.ControllerInstructionContext _ctx) {
        Instruction _instruction = architectureBuilder.buildControllerInstruction(_ctx, currentCallable);
        if (_instruction == null) {
            log.error("Unknown opcode at line: " + _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine());
//			System.exit(0);
            success = false;
            return;
        }
        currentInstructionLine.setControllerInstruction(_instruction);
//        log.debug("addControllerLine: " + _ctx.getStart().getLine());
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterArrayInstruction (AsmParser.ArrayInstructionContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitArrayInstruction (AsmParser.ArrayInstructionContext _ctx) {
        Instruction _instruction = architectureBuilder.buildArrayInstruction(_ctx, currentCallable);
        if (_instruction == null) {
            log.error("Unknown opcode at line: " + _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine());
//			System.exit(0);
            success = false;
            return;
        }
        currentInstructionLine.setArrayInstruction(_instruction);
//        log.debug("addArrayLine@ " + _ctx.getStart().getLine());
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterLabel (AsmParser.LabelContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitLabel (AsmParser.LabelContext _ctx) {
        AsmParser.LbContext _lb = _ctx.lb();
        if (_lb != null) {
            String _label = _lb.name().NAME().getText();
            if (currentCallable == null) {
                log.error("exitLabel: currentPrimitive is not initialized at line: " + _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine());
//				System.exit(0);
            } else {
                currentCallable.addLabel(_label);
            }
        }
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterDirective (AsmParser.DirectiveContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitDirective (AsmParser.DirectiveContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterAssemblerdirective (AsmParser.AssemblerdirectiveContext ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitAssemblerdirective (AsmParser.AssemblerdirectiveContext ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterArchitectureId (AsmParser.ArchitectureIdContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitArchitectureId (AsmParser.ArchitectureIdContext _ctx) {
        String _archString = "";
        AsmParser.NameContext _nameContext = _ctx.name();
        if (_nameContext != null) {
            application.setArchitectureId(_nameContext.NAME().getText());
        } else {
            log.error("invalid architecture number: " + getPosition(_ctx));
        }
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterData (AsmParser.DataContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitData (AsmParser.DataContext _ctx) {
//        log.debug("AsmLinkerListener.exitData:" + getPosition(_ctx));
        AsmParser.NumberContext _numberContext = _ctx.number();
        TerminalNode _filePathContext = _ctx.FILEPATH();
        if ((_numberContext != null) && (_filePathContext != null)) {
            int _address = convertNumberContextToInt(_numberContext);
            String _filePath = _filePathContext.getText();
            linker.addData(_address, _filePath, application);
        } else {
            log.error("invalid architecture number: " + getPosition(_ctx));
        }
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterDefine (AsmParser.DefineContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitDefine (AsmParser.DefineContext _ctx) {
        application.addDefine(_ctx.name().NAME().getText(), _ctx.expression());
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterExpression (AsmParser.ExpressionContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitExpression (AsmParser.ExpressionContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterInclude (AsmParser.IncludeContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitInclude (AsmParser.IncludeContext _ctx) {
//        log.debug("include... " + _ctx);
        String _filename = _ctx.FILEPATH().getText();
        _filename = _filename.replace("\"", "");
        if(linker.getSuccess()){
            linker.loadByLinker(_filename, application);            
        }
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterFunc (AsmParser.FuncContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitFunc (AsmParser.FuncContext _ctx) {
        AsmParser.NameContext _nameContext = _ctx.name();
        String _name = _nameContext.NAME().getText();
        currentPrimitive = new Primitive(context, _name, application);
        currentCallable = currentPrimitive;
//        log.debug("create Primitive,.,,");
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterEndfunc (AsmParser.EndfuncContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitEndfunc (AsmParser.EndfuncContext _ctx) {
//        (new Throwable()).printStackTrace();
        if(application == null){
            log.debug("App is not initialized(func is not started!)");
        } else {
            application.add(currentPrimitive);            
        }
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void enterMacro(AsmParser.MacroContext ctx) { }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void exitMacro(AsmParser.MacroContext _ctx) { 
        AsmParser.NameContext _nameContext = _ctx.name();
        String _name = _nameContext.NAME().getText();
        currentMacro = new Macro(context, _name, application, _ctx.parametersNames());
        currentCallable = currentMacro;
    }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void enterEndmacro(AsmParser.EndmacroContext ctx) { }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void exitEndmacro(AsmParser.EndmacroContext ctx) {
        if(application == null){
            log.debug("App is not initialized(macro is not started!)");
        } else {
            application.add(currentMacro);            
        }        
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void enterParametersNames(AsmParser.ParametersNamesContext ctx) { }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void exitParametersNames(AsmParser.ParametersNamesContext ctx) { }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void enterMacroCall(AsmParser.MacroCallContext ctx) { }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void exitMacroCall(AsmParser.MacroCallContext _ctx) {
//        log.debug("exitMacroCall...");
        AsmParser.NameContext _nameContext = _ctx.name();
        String _macroName = _nameContext.NAME().getText();
        Macro _macroCall = application.getMacro(_macroName);
        if(_macroCall == null){
            log.error("Cannot find macro: " + _macroName);
            System.exit(0);
            return;
        }
        _macroCall = _macroCall.copyOf(_ctx.parametersInstantiation().expression());
//        AsmParser.ParametersInstantiationContext _parametersInstantiationContext = ;
//        List<AsmParser.ExpressionContext> _expressions = _parametersInstantiationContext.expression();
//        _macroCall.setInstantiationExpressions(_expressions);
//        currentInstructionLine.addLine(_macroCall);        
        currentCallable.addLine(_ctx.getStart().getLine(), _macroCall);            
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void enterParametersInstantiation(AsmParser.ParametersInstantiationContext ctx) { }
    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override public void exitParametersInstantiation(AsmParser.ParametersInstantiationContext ctx) { }



    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterLb (AsmParser.LbContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitLb (AsmParser.LbContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterName (AsmParser.NameContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitName (AsmParser.NameContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterNumber (AsmParser.NumberContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitNumber (AsmParser.NumberContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterComment (AsmParser.CommentContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitComment (AsmParser.CommentContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void enterEveryRule (ParserRuleContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void exitEveryRule (ParserRuleContext _ctx) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void visitTerminal (TerminalNode node) {
    }

    /**
     * {@inheritDoc}
     *
     * <p>The default implementation does nothing.</p>
     */
    @Override
    public void visitErrorNode (ErrorNode node) {
    }

    //-------------------------------------------------------------------------------------
    public String getPosition (ParserRuleContext _ctx) {
        return _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine();
    }


    //-------------------------------------------------------------------------------------
    public int convertNumberContextToInt (AsmParser.NumberContext _numberContext) {
        TerminalNode _valueString = _numberContext.NUMBER();
        if (_valueString == null) {
            _valueString = _numberContext.NUMBER();
        }
        return convertStringToInt(_valueString.getText());
    }

    //-------------------------------------------------------------------------------------
    public int convertStringToInt (String _s) {
        _s = _s.replaceAll("_", "");
        if (_s.toLowerCase().endsWith("h")) {
            _s = "0x" + _s.substring(0, _s.length() - 1);
        }
        return Integer.decode(_s);
    }
}
