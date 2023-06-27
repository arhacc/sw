// Generated from Asm.g4 by ANTLR 4.13.0

 	 package xpu.sw.tools.sdk.asm.parser;

import org.antlr.v4.runtime.tree.ParseTreeListener;

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link AsmParser}.
 */
public interface AsmListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by {@link AsmParser#parse}.
	 * @param ctx the parse tree
	 */
	void enterParse(AsmParser.ParseContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#parse}.
	 * @param ctx the parse tree
	 */
	void exitParse(AsmParser.ParseContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#line}.
	 * @param ctx the parse tree
	 */
	void enterLine(AsmParser.LineContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#line}.
	 * @param ctx the parse tree
	 */
	void exitLine(AsmParser.LineContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#directive}.
	 * @param ctx the parse tree
	 */
	void enterDirective(AsmParser.DirectiveContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#directive}.
	 * @param ctx the parse tree
	 */
	void exitDirective(AsmParser.DirectiveContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#instruction}.
	 * @param ctx the parse tree
	 */
	void enterInstruction(AsmParser.InstructionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#instruction}.
	 * @param ctx the parse tree
	 */
	void exitInstruction(AsmParser.InstructionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controllerInstruction}.
	 * @param ctx the parse tree
	 */
	void enterControllerInstruction(AsmParser.ControllerInstructionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controllerInstruction}.
	 * @param ctx the parse tree
	 */
	void exitControllerInstruction(AsmParser.ControllerInstructionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controllerOpcode0}.
	 * @param ctx the parse tree
	 */
	void enterControllerOpcode0(AsmParser.ControllerOpcode0Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controllerOpcode0}.
	 * @param ctx the parse tree
	 */
	void exitControllerOpcode0(AsmParser.ControllerOpcode0Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controllerOpcode1}.
	 * @param ctx the parse tree
	 */
	void enterControllerOpcode1(AsmParser.ControllerOpcode1Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controllerOpcode1}.
	 * @param ctx the parse tree
	 */
	void exitControllerOpcode1(AsmParser.ControllerOpcode1Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controllerOpcode2}.
	 * @param ctx the parse tree
	 */
	void enterControllerOpcode2(AsmParser.ControllerOpcode2Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controllerOpcode2}.
	 * @param ctx the parse tree
	 */
	void exitControllerOpcode2(AsmParser.ControllerOpcode2Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controllerOpcode3}.
	 * @param ctx the parse tree
	 */
	void enterControllerOpcode3(AsmParser.ControllerOpcode3Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controllerOpcode3}.
	 * @param ctx the parse tree
	 */
	void exitControllerOpcode3(AsmParser.ControllerOpcode3Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#arrayInstruction}.
	 * @param ctx the parse tree
	 */
	void enterArrayInstruction(AsmParser.ArrayInstructionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arrayInstruction}.
	 * @param ctx the parse tree
	 */
	void exitArrayInstruction(AsmParser.ArrayInstructionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#arrayOpcode0}.
	 * @param ctx the parse tree
	 */
	void enterArrayOpcode0(AsmParser.ArrayOpcode0Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arrayOpcode0}.
	 * @param ctx the parse tree
	 */
	void exitArrayOpcode0(AsmParser.ArrayOpcode0Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#arrayOpcode1}.
	 * @param ctx the parse tree
	 */
	void enterArrayOpcode1(AsmParser.ArrayOpcode1Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arrayOpcode1}.
	 * @param ctx the parse tree
	 */
	void exitArrayOpcode1(AsmParser.ArrayOpcode1Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#label}.
	 * @param ctx the parse tree
	 */
	void enterLabel(AsmParser.LabelContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#label}.
	 * @param ctx the parse tree
	 */
	void exitLabel(AsmParser.LabelContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#assemblerdirective}.
	 * @param ctx the parse tree
	 */
	void enterAssemblerdirective(AsmParser.AssemblerdirectiveContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#assemblerdirective}.
	 * @param ctx the parse tree
	 */
	void exitAssemblerdirective(AsmParser.AssemblerdirectiveContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#architectureId}.
	 * @param ctx the parse tree
	 */
	void enterArchitectureId(AsmParser.ArchitectureIdContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#architectureId}.
	 * @param ctx the parse tree
	 */
	void exitArchitectureId(AsmParser.ArchitectureIdContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#data}.
	 * @param ctx the parse tree
	 */
	void enterData(AsmParser.DataContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#data}.
	 * @param ctx the parse tree
	 */
	void exitData(AsmParser.DataContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#define}.
	 * @param ctx the parse tree
	 */
	void enterDefine(AsmParser.DefineContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#define}.
	 * @param ctx the parse tree
	 */
	void exitDefine(AsmParser.DefineContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#expression}.
	 * @param ctx the parse tree
	 */
	void enterExpression(AsmParser.ExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#expression}.
	 * @param ctx the parse tree
	 */
	void exitExpression(AsmParser.ExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#multiplyingExpression}.
	 * @param ctx the parse tree
	 */
	void enterMultiplyingExpression(AsmParser.MultiplyingExpressionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#multiplyingExpression}.
	 * @param ctx the parse tree
	 */
	void exitMultiplyingExpression(AsmParser.MultiplyingExpressionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#signedAtom}.
	 * @param ctx the parse tree
	 */
	void enterSignedAtom(AsmParser.SignedAtomContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#signedAtom}.
	 * @param ctx the parse tree
	 */
	void exitSignedAtom(AsmParser.SignedAtomContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#atom}.
	 * @param ctx the parse tree
	 */
	void enterAtom(AsmParser.AtomContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#atom}.
	 * @param ctx the parse tree
	 */
	void exitAtom(AsmParser.AtomContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#function}.
	 * @param ctx the parse tree
	 */
	void enterFunction(AsmParser.FunctionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#function}.
	 * @param ctx the parse tree
	 */
	void exitFunction(AsmParser.FunctionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#funcname}.
	 * @param ctx the parse tree
	 */
	void enterFuncname(AsmParser.FuncnameContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#funcname}.
	 * @param ctx the parse tree
	 */
	void exitFuncname(AsmParser.FuncnameContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#include}.
	 * @param ctx the parse tree
	 */
	void enterInclude(AsmParser.IncludeContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#include}.
	 * @param ctx the parse tree
	 */
	void exitInclude(AsmParser.IncludeContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#func}.
	 * @param ctx the parse tree
	 */
	void enterFunc(AsmParser.FuncContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#func}.
	 * @param ctx the parse tree
	 */
	void exitFunc(AsmParser.FuncContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#endfunc}.
	 * @param ctx the parse tree
	 */
	void enterEndfunc(AsmParser.EndfuncContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#endfunc}.
	 * @param ctx the parse tree
	 */
	void exitEndfunc(AsmParser.EndfuncContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#macro}.
	 * @param ctx the parse tree
	 */
	void enterMacro(AsmParser.MacroContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#macro}.
	 * @param ctx the parse tree
	 */
	void exitMacro(AsmParser.MacroContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#endmacro}.
	 * @param ctx the parse tree
	 */
	void enterEndmacro(AsmParser.EndmacroContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#endmacro}.
	 * @param ctx the parse tree
	 */
	void exitEndmacro(AsmParser.EndmacroContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#parametersNames}.
	 * @param ctx the parse tree
	 */
	void enterParametersNames(AsmParser.ParametersNamesContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#parametersNames}.
	 * @param ctx the parse tree
	 */
	void exitParametersNames(AsmParser.ParametersNamesContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#macroCall}.
	 * @param ctx the parse tree
	 */
	void enterMacroCall(AsmParser.MacroCallContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#macroCall}.
	 * @param ctx the parse tree
	 */
	void exitMacroCall(AsmParser.MacroCallContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#parametersInstantiation}.
	 * @param ctx the parse tree
	 */
	void enterParametersInstantiation(AsmParser.ParametersInstantiationContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#parametersInstantiation}.
	 * @param ctx the parse tree
	 */
	void exitParametersInstantiation(AsmParser.ParametersInstantiationContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#lb}.
	 * @param ctx the parse tree
	 */
	void enterLb(AsmParser.LbContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#lb}.
	 * @param ctx the parse tree
	 */
	void exitLb(AsmParser.LbContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#name}.
	 * @param ctx the parse tree
	 */
	void enterName(AsmParser.NameContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#name}.
	 * @param ctx the parse tree
	 */
	void exitName(AsmParser.NameContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#number}.
	 * @param ctx the parse tree
	 */
	void enterNumber(AsmParser.NumberContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#number}.
	 * @param ctx the parse tree
	 */
	void exitNumber(AsmParser.NumberContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#comment}.
	 * @param ctx the parse tree
	 */
	void enterComment(AsmParser.CommentContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#comment}.
	 * @param ctx the parse tree
	 */
	void exitComment(AsmParser.CommentContext ctx);
}