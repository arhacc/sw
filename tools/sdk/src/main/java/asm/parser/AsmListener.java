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
	 * Enter a parse tree produced by {@link AsmParser#controlInstruction}.
	 * @param ctx the parse tree
	 */
	void enterControlInstruction(AsmParser.ControlInstructionContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controlInstruction}.
	 * @param ctx the parse tree
	 */
	void exitControlInstruction(AsmParser.ControlInstructionContext ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controlOpcode0}.
	 * @param ctx the parse tree
	 */
	void enterControlOpcode0(AsmParser.ControlOpcode0Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controlOpcode0}.
	 * @param ctx the parse tree
	 */
	void exitControlOpcode0(AsmParser.ControlOpcode0Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controlOpcode1}.
	 * @param ctx the parse tree
	 */
	void enterControlOpcode1(AsmParser.ControlOpcode1Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controlOpcode1}.
	 * @param ctx the parse tree
	 */
	void exitControlOpcode1(AsmParser.ControlOpcode1Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controlOpcode2}.
	 * @param ctx the parse tree
	 */
	void enterControlOpcode2(AsmParser.ControlOpcode2Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controlOpcode2}.
	 * @param ctx the parse tree
	 */
	void exitControlOpcode2(AsmParser.ControlOpcode2Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controlOpcode3}.
	 * @param ctx the parse tree
	 */
	void enterControlOpcode3(AsmParser.ControlOpcode3Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controlOpcode3}.
	 * @param ctx the parse tree
	 */
	void exitControlOpcode3(AsmParser.ControlOpcode3Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#controlOpcode4}.
	 * @param ctx the parse tree
	 */
	void enterControlOpcode4(AsmParser.ControlOpcode4Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#controlOpcode4}.
	 * @param ctx the parse tree
	 */
	void exitControlOpcode4(AsmParser.ControlOpcode4Context ctx);
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
	 * Enter a parse tree produced by {@link AsmParser#arrayOpcode2}.
	 * @param ctx the parse tree
	 */
	void enterArrayOpcode2(AsmParser.ArrayOpcode2Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arrayOpcode2}.
	 * @param ctx the parse tree
	 */
	void exitArrayOpcode2(AsmParser.ArrayOpcode2Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#arrayOpcode3}.
	 * @param ctx the parse tree
	 */
	void enterArrayOpcode3(AsmParser.ArrayOpcode3Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arrayOpcode3}.
	 * @param ctx the parse tree
	 */
	void exitArrayOpcode3(AsmParser.ArrayOpcode3Context ctx);
	/**
	 * Enter a parse tree produced by {@link AsmParser#arrayOpcode4}.
	 * @param ctx the parse tree
	 */
	void enterArrayOpcode4(AsmParser.ArrayOpcode4Context ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arrayOpcode4}.
	 * @param ctx the parse tree
	 */
	void exitArrayOpcode4(AsmParser.ArrayOpcode4Context ctx);
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
	 * Enter a parse tree produced by {@link AsmParser#arch}.
	 * @param ctx the parse tree
	 */
	void enterArch(AsmParser.ArchContext ctx);
	/**
	 * Exit a parse tree produced by {@link AsmParser#arch}.
	 * @param ctx the parse tree
	 */
	void exitArch(AsmParser.ArchContext ctx);
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