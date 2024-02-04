// Generated from Asm.g4 by ANTLR 4.13.0

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
	 * Visit a parse tree produced by {@link AsmParser#directive}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitDirective(AsmParser.DirectiveContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#instruction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitInstruction(AsmParser.InstructionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controllerInstruction}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControllerInstruction(AsmParser.ControllerInstructionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controllerOpcode0}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControllerOpcode0(AsmParser.ControllerOpcode0Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controllerOpcode1}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControllerOpcode1(AsmParser.ControllerOpcode1Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controllerOpcode2}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControllerOpcode2(AsmParser.ControllerOpcode2Context ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#controllerOpcode3}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitControllerOpcode3(AsmParser.ControllerOpcode3Context ctx);
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
	 * Visit a parse tree produced by {@link AsmParser#label}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitLabel(AsmParser.LabelContext ctx);
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
	 * Visit a parse tree produced by {@link AsmParser#const}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitConst(AsmParser.ConstContext ctx);
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
	 * Visit a parse tree produced by {@link AsmParser#signedAtom}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitSignedAtom(AsmParser.SignedAtomContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#atom}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitAtom(AsmParser.AtomContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#function}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFunction(AsmParser.FunctionContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#funcname}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitFuncname(AsmParser.FuncnameContext ctx);
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
	 * Visit a parse tree produced by {@link AsmParser#macro}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMacro(AsmParser.MacroContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#endmacro}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitEndmacro(AsmParser.EndmacroContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#parametersNames}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitParametersNames(AsmParser.ParametersNamesContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#macroCall}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitMacroCall(AsmParser.MacroCallContext ctx);
	/**
	 * Visit a parse tree produced by {@link AsmParser#parametersInstantiation}.
	 * @param ctx the parse tree
	 * @return the visitor result
	 */
	T visitParametersInstantiation(AsmParser.ParametersInstantiationContext ctx);
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