// Generated from Asm.g4 by ANTLR 4.12.0
 
 	 package xpu.sw.tools.sdk.asm.parser;
 	 
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link AsmParser}.
 *
 * @param <T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface AsmVisitor<T> extends ParseTreeVisitor<T> {
	/**
	 * Visit a parse tree produced by {@link AsmParser#parse}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitParse(AsmParser.ParseContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#line}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLine(AsmParser.LineContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#instruction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitInstruction(AsmParser.InstructionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controlInstruction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControlInstruction(AsmParser.ControlInstructionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controlOpcode0}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControlOpcode0(AsmParser.ControlOpcode0Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controlOpcode1}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControlOpcode1(AsmParser.ControlOpcode1Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controlOpcode2}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControlOpcode2(AsmParser.ControlOpcode2Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controlOpcode3}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControlOpcode3(AsmParser.ControlOpcode3Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controlOpcode4}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControlOpcode4(AsmParser.ControlOpcode4Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#arrayInstruction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArrayInstruction(AsmParser.ArrayInstructionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#arrayOpcode0}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArrayOpcode0(AsmParser.ArrayOpcode0Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#arrayOpcode1}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArrayOpcode1(AsmParser.ArrayOpcode1Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#arrayOpcode2}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArrayOpcode2(AsmParser.ArrayOpcode2Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#arrayOpcode3}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArrayOpcode3(AsmParser.ArrayOpcode3Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#arrayOpcode4}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArrayOpcode4(AsmParser.ArrayOpcode4Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#label}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLabel(AsmParser.LabelContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#directive}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDirective(AsmParser.DirectiveContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#assemblerdirective}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAssemblerdirective(AsmParser.AssemblerdirectiveContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#architectureId}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitArchitectureId(AsmParser.ArchitectureIdContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#data}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitData(AsmParser.DataContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#define}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDefine(AsmParser.DefineContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#expression}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitExpression(AsmParser.ExpressionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#multiplyingExpression}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMultiplyingExpression(AsmParser.MultiplyingExpressionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#value}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitValue(AsmParser.ValueContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#include}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitInclude(AsmParser.IncludeContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#func}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFunc(AsmParser.FuncContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#endfunc}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitEndfunc(AsmParser.EndfuncContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#lb}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLb(AsmParser.LbContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#name}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitName(AsmParser.NameContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#number}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitNumber(AsmParser.NumberContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#comment}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitComment(AsmParser.CommentContext ctx);
}