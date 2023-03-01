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
import xpu.sw.tools.sdk.common.isa.builders.*;
//import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.asm.parser.*;

//-------------------------------------------------------------------------------------

public class AsmLinkerListener extends AsmBaseListener {
	private Context context;
    private Logger log;
    private AsmLinker linker;
    private Application app;

    private String currentArhCode;
    private Primitive currentProgram;
    private InstructionLine currentInstructionLine;

    private InstructionBuilder instructionBuilder;

    private boolean success;
//-------------------------------------------------------------------------------------
    public AsmLinkerListener(Context _context, AsmLinker _linker, Application _app) {
    	context = _context;
        log = _context.getLog();
        linker = _linker;
        app = _app;
        currentArhCode = _context.getArchitectureImplementations().getDefault().getName();
        instructionBuilder = new InstructionBuilder(_context);
        success = true;
    }

//-------------------------------------------------------------------------------------
    public boolean getSuccess() {
    	return success;
    }

//-------------------------------------------------------------------------------------

	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterParse(AsmParser.ParseContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitParse(AsmParser.ParseContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterLine(AsmParser.LineContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitLine(AsmParser.LineContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterInstruction(AsmParser.InstructionContext _ctx) {
		currentInstructionLine = new InstructionLine();
	 }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitInstruction(AsmParser.InstructionContext _ctx) { 
//        log.debug("add currentInstruction: " + instructionLine);
        currentProgram.addInstruction(currentInstructionLine);
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterInstr(AsmParser.InstrContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitInstr(AsmParser.InstrContext _ctx) {
/*		Opcode _opcode = Opcode.getOpcode(_ctx.c_opcode().C_OPCODE().getText());
		AsmParser.OperandContext _operandContext = _ctx.operand();
		String _operandString = "";
		if(_operandContext != null){
			AsmParser.NumberContext _numberContext = _operandContext.number();
			if(_numberContext != null){
				_operandString = _numberContext.NUMBER().getText();
			}
		}
		Operand _operand = Operand.getOperand(_operandString);
		ControlInstruction _controlInstruction = new ControlInstruction(_opcode, _operand);
		currentInstruction.setControlInstruction(_controlInstruction);
*/
		String _opcodeString = _ctx.opcode().OPCODE().getText();
//		Opcode _opcode = Opcode.getOpcode(_opcodeString);
		AsmParser.ValueContext _valueContext = _ctx.value();
		String _valueString = "";
		int _valueNumber = 0;
		if(_valueContext != null){
			AsmParser.NameContext _nameContext = _valueContext.name();
			if(_nameContext != null){
				_valueString = _nameContext.NAME().getText();
			}

			AsmParser.NumberContext _numberContext = _valueContext.number();
			if(_numberContext != null){
				_valueNumber = Integer.parseInt(_numberContext.NUMBER().getText());
			}
		}
//		Value _value = Value.getValue(_valueString);
		Instruction _instruction = instructionBuilder.build(_opcodeString, _valueString, _valueNumber, currentProgram);
		if(_instruction == null){
			log.error("Unknown opcode at line: " + _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine());
//			System.exit(0);
			success = false;
			return;
		}
		currentInstructionLine.add(_instruction);
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterLabel(AsmParser.LabelContext _ctx) {
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitLabel(AsmParser.LabelContext _ctx) {
		AsmParser.LbContext _lb = _ctx.lb();
		if(_lb != null){
			String _label = _lb.name().NAME().getText();
			if(currentProgram == null){
				log.error("exitLabel: currentProgram is not initialized at line: " + _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine());
				System.exit(0);
			} else {
				currentProgram.addLabel(_label);				
			}
		}		
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterDirective(AsmParser.DirectiveContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitDirective(AsmParser.DirectiveContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterAssemblerdirective(AsmParser.AssemblerdirectiveContext ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitAssemblerdirective(AsmParser.AssemblerdirectiveContext ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterArh(AsmParser.ArhContext _ctx) { 
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitArh(AsmParser.ArhContext _ctx) {
		String _arhString = "";
		AsmParser.NameContext _nameContext = _ctx.name();
		if(_nameContext != null){
			currentArhCode = _nameContext.NAME().getText();
		} else {
			log.error("invalid architecture number: " + getPosition(_ctx));
		}

//		linker.setArchitecture(_arhString);		
	}

	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterData(AsmParser.DataContext _ctx) { 
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitData(AsmParser.DataContext _ctx) {
//        log.debug("AsmLinkerListener.exitData:" + getPosition(_ctx));
		AsmParser.NumberContext _numberContext = _ctx.number();
		TerminalNode _filePathContext = _ctx.FILEPATH();
		if((_numberContext != null) && (_filePathContext != null)){
			int _address = convertNumberContextToInt(_numberContext);
			String _filePath = _filePathContext.getText();
			linker.addData(_address, _filePath);
		} else {
			log.error("invalid architecture number: " + getPosition(_ctx));
		}
	}

	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterDefine(AsmParser.DefineContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitDefine(AsmParser.DefineContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterExpression(AsmParser.ExpressionContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitExpression(AsmParser.ExpressionContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterMultiplyingExpression(AsmParser.MultiplyingExpressionContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitMultiplyingExpression(AsmParser.MultiplyingExpressionContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterValue(AsmParser.ValueContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitValue(AsmParser.ValueContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterInclude(AsmParser.IncludeContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitInclude(AsmParser.IncludeContext _ctx) { 
//        log.debug("include... " + _ctx);
        String _filename = _ctx.FILEPATH().getText();
        _filename = _filename.replace("\"","");
        linker.loadByLinker(_filename);        
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterFunc(AsmParser.FuncContext _ctx) {
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitFunc(AsmParser.FuncContext _ctx) { 
		AsmParser.NameContext _nameContext = _ctx.name();
		String _name = _nameContext.NAME().getText();
		currentProgram = new Primitive(log, currentArhCode, _name);
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterEndfunc(AsmParser.EndfuncContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitEndfunc(AsmParser.EndfuncContext _ctx) {
		app.add(currentProgram);
	}
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterLb(AsmParser.LbContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitLb(AsmParser.LbContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterName(AsmParser.NameContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitName(AsmParser.NameContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterNumber(AsmParser.NumberContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitNumber(AsmParser.NumberContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterOpcode(AsmParser.OpcodeContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitOpcode(AsmParser.OpcodeContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterComment(AsmParser.CommentContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitComment(AsmParser.CommentContext _ctx) { }

	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void enterEveryRule(ParserRuleContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void exitEveryRule(ParserRuleContext _ctx) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void visitTerminal(TerminalNode node) { }
	/**
	 * {@inheritDoc}
	 *
	 * <p>The default implementation does nothing.</p>
	 */
	@Override public void visitErrorNode(ErrorNode node) { }

//-------------------------------------------------------------------------------------
	public String getPosition(ParserRuleContext _ctx){
		return  _ctx.getStart().getLine() + ":" + _ctx.getStart().getCharPositionInLine();
	}


//-------------------------------------------------------------------------------------
    public int convertNumberContextToInt(AsmParser.NumberContext _numberContext) {
    	TerminalNode _valueString = _numberContext.NUMBER();
    	if(_valueString == null){
    		_valueString = _numberContext.HEXADECIMAL();
    	}
    	return convertStringToInt(_valueString.getText());
    }	

//-------------------------------------------------------------------------------------
    public int convertStringToInt(String _s) {
    	_s = _s.replaceAll("_", "");
        if(_s.startsWith("0x")){
            _s = _s.substring(2);
            return Integer.parseInt(_s, 16);
        }
        return Integer.parseInt(_s);
    }	
}