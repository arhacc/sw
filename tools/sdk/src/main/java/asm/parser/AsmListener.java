// Generated from Asm.g4 by ANTLR 4.12.0
 
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
	 * Enter a parse tree produced by {@link AsmParser#instr}.
	 * @param ctx the parse tree
	 */
	void enterInstr(AsmParser.InstrContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#instr}.
	 * @param ctx the parse tree
	 */
	void exitInstr(AsmParser.InstrContext ctx);
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
	 * Enter a parse tree produced by {@link AsmParser#arh}.
	 * @param ctx the parse tree
	 */
	void enterArh(AsmParser.ArhContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arh}.
	 * @param ctx the parse tree
	 */
	void exitArh(AsmParser.ArhContext ctx);
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
	 * Enter a parse tree produced by {@link AsmParser#value}.
	 * @param ctx the parse tree
	 */
	void enterValue(AsmParser.ValueContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#value}.
	 * @param ctx the parse tree
	 */
	void exitValue(AsmParser.ValueContext ctx);
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
	 * Enter a parse tree produced by {@link AsmParser#opcode}.
	 * @param ctx the parse tree
	 */
	void enterOpcode(AsmParser.OpcodeContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#opcode}.
	 * @param ctx the parse tree
	 */
	void exitOpcode(AsmParser.OpcodeContext ctx);
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