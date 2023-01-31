// Generated from java-escape by ANTLR 4.11.1
 
 	 package xpu.sw.tools.sdk.asm.parser;
 	 
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.*;
import org.antlr.v4.runtime.tree.*;
import java.util.List;
import java.util.Iterator;
import java.util.ArrayList;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue"})
public class AsmParser extends Parser {
	static { RuntimeMetaData.checkVersion("4.11.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, ARH=6, DATA=7, FILEPATH=8, INCLUDE=9, 
		FUNC=10, ENDFUNC=11, DEFINE=12, COMMENT=13, OPCODE=14, SIGN=15, NAME=16, 
		NUMBER=17, HEXADECIMAL=18, STRING=19, EOL=20, WS=21;
	public static final int
		RULE_parse = 0, RULE_line = 1, RULE_instruction = 2, RULE_instr = 3, RULE_label = 4, 
		RULE_directive = 5, RULE_assemblerdirective = 6, RULE_arh = 7, RULE_data = 8, 
		RULE_define = 9, RULE_expression = 10, RULE_multiplyingExpression = 11, 
		RULE_value = 12, RULE_include = 13, RULE_func = 14, RULE_endfunc = 15, 
		RULE_lb = 16, RULE_name = 17, RULE_number = 18, RULE_opcode = 19, RULE_comment = 20;
	private static String[] makeRuleNames() {
		return new String[] {
			"parse", "line", "instruction", "instr", "label", "directive", "assemblerdirective", 
			"arh", "data", "define", "expression", "multiplyingExpression", "value", 
			"include", "func", "endfunc", "lb", "name", "number", "opcode", "comment"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "':'", "'*'", "'/'", "'mod'", "'and'", "'.arh'", "'.data'", null, 
			"'include'", "'func'", "'endfunc'", "'define'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, "ARH", "DATA", "FILEPATH", "INCLUDE", 
			"FUNC", "ENDFUNC", "DEFINE", "COMMENT", "OPCODE", "SIGN", "NAME", "NUMBER", 
			"HEXADECIMAL", "STRING", "EOL", "WS"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}

	@Override
	public String getGrammarFileName() { return "java-escape"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	public AsmParser(TokenStream input) {
		super(input);
		_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ParseContext extends ParserRuleContext {
		public List<TerminalNode> EOL() { return getTokens(AsmParser.EOL); }
		public TerminalNode EOL(int i) {
			return getToken(AsmParser.EOL, i);
		}
		public List<LineContext> line() {
			return getRuleContexts(LineContext.class);
		}
		public LineContext line(int i) {
			return getRuleContext(LineContext.class,i);
		}
		public ParseContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_parse; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterParse(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitParse(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitParse(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ParseContext parse() throws RecognitionException {
		ParseContext _localctx = new ParseContext(_ctx, getState());
		enterRule(_localctx, 0, RULE_parse);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(46); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(43);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if (((_la) & ~0x3f) == 0 && ((1L << _la) & 97984L) != 0) {
					{
					setState(42);
					line();
					}
				}

				setState(45);
				match(EOL);
				}
				}
				setState(48); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( ((_la) & ~0x3f) == 0 && ((1L << _la) & 1146560L) != 0 );
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LineContext extends ParserRuleContext {
		public DirectiveContext directive() {
			return getRuleContext(DirectiveContext.class,0);
		}
		public LabelContext label() {
			return getRuleContext(LabelContext.class,0);
		}
		public InstructionContext instruction() {
			return getRuleContext(InstructionContext.class,0);
		}
		public CommentContext comment() {
			return getRuleContext(CommentContext.class,0);
		}
		public LineContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_line; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterLine(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitLine(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitLine(this);
			else return visitor.visitChildren(this);
		}
	}

	public final LineContext line() throws RecognitionException {
		LineContext _localctx = new LineContext(_ctx, getState());
		enterRule(_localctx, 2, RULE_line);
		try {
			setState(54);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,2,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(50);
				directive();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(51);
				label();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(52);
				instruction();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(53);
				comment();
				}
				break;
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class InstructionContext extends ParserRuleContext {
		public List<InstrContext> instr() {
			return getRuleContexts(InstrContext.class);
		}
		public InstrContext instr(int i) {
			return getRuleContext(InstrContext.class,i);
		}
		public LabelContext label() {
			return getRuleContext(LabelContext.class,0);
		}
		public CommentContext comment() {
			return getRuleContext(CommentContext.class,0);
		}
		public InstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_instruction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterInstruction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitInstruction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitInstruction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final InstructionContext instruction() throws RecognitionException {
		InstructionContext _localctx = new InstructionContext(_ctx, getState());
		enterRule(_localctx, 4, RULE_instruction);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(57);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==NAME) {
				{
				setState(56);
				label();
				}
			}

			setState(59);
			instr();
			setState(60);
			instr();
			setState(62);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT) {
				{
				setState(61);
				comment();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class InstrContext extends ParserRuleContext {
		public OpcodeContext opcode() {
			return getRuleContext(OpcodeContext.class,0);
		}
		public ValueContext value() {
			return getRuleContext(ValueContext.class,0);
		}
		public InstrContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_instr; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterInstr(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitInstr(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitInstr(this);
			else return visitor.visitChildren(this);
		}
	}

	public final InstrContext instr() throws RecognitionException {
		InstrContext _localctx = new InstrContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_instr);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(64);
			opcode();
			setState(66);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (((_la) & ~0x3f) == 0 && ((1L << _la) & 491520L) != 0) {
				{
				setState(65);
				value();
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LabelContext extends ParserRuleContext {
		public LbContext lb() {
			return getRuleContext(LbContext.class,0);
		}
		public LabelContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_label; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterLabel(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitLabel(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitLabel(this);
			else return visitor.visitChildren(this);
		}
	}

	public final LabelContext label() throws RecognitionException {
		LabelContext _localctx = new LabelContext(_ctx, getState());
		enterRule(_localctx, 8, RULE_label);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(68);
			lb();
			setState(70);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__0) {
				{
				setState(69);
				match(T__0);
				}
			}

			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DirectiveContext extends ParserRuleContext {
		public AssemblerdirectiveContext assemblerdirective() {
			return getRuleContext(AssemblerdirectiveContext.class,0);
		}
		public DefineContext define() {
			return getRuleContext(DefineContext.class,0);
		}
		public IncludeContext include() {
			return getRuleContext(IncludeContext.class,0);
		}
		public FuncContext func() {
			return getRuleContext(FuncContext.class,0);
		}
		public EndfuncContext endfunc() {
			return getRuleContext(EndfuncContext.class,0);
		}
		public DirectiveContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_directive; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterDirective(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitDirective(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitDirective(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DirectiveContext directive() throws RecognitionException {
		DirectiveContext _localctx = new DirectiveContext(_ctx, getState());
		enterRule(_localctx, 10, RULE_directive);
		try {
			setState(77);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ARH:
			case DATA:
				enterOuterAlt(_localctx, 1);
				{
				setState(72);
				assemblerdirective();
				}
				break;
			case DEFINE:
				enterOuterAlt(_localctx, 2);
				{
				setState(73);
				define();
				}
				break;
			case INCLUDE:
				enterOuterAlt(_localctx, 3);
				{
				setState(74);
				include();
				}
				break;
			case FUNC:
				enterOuterAlt(_localctx, 4);
				{
				setState(75);
				func();
				}
				break;
			case ENDFUNC:
				enterOuterAlt(_localctx, 5);
				{
				setState(76);
				endfunc();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class AssemblerdirectiveContext extends ParserRuleContext {
		public ArhContext arh() {
			return getRuleContext(ArhContext.class,0);
		}
		public DataContext data() {
			return getRuleContext(DataContext.class,0);
		}
		public AssemblerdirectiveContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_assemblerdirective; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterAssemblerdirective(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitAssemblerdirective(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitAssemblerdirective(this);
			else return visitor.visitChildren(this);
		}
	}

	public final AssemblerdirectiveContext assemblerdirective() throws RecognitionException {
		AssemblerdirectiveContext _localctx = new AssemblerdirectiveContext(_ctx, getState());
		enterRule(_localctx, 12, RULE_assemblerdirective);
		try {
			setState(81);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ARH:
				enterOuterAlt(_localctx, 1);
				{
				setState(79);
				arh();
				}
				break;
			case DATA:
				enterOuterAlt(_localctx, 2);
				{
				setState(80);
				data();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ArhContext extends ParserRuleContext {
		public TerminalNode ARH() { return getToken(AsmParser.ARH, 0); }
		public NumberContext number() {
			return getRuleContext(NumberContext.class,0);
		}
		public ArhContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arh; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArh(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArh(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArh(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArhContext arh() throws RecognitionException {
		ArhContext _localctx = new ArhContext(_ctx, getState());
		enterRule(_localctx, 14, RULE_arh);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(83);
			match(ARH);
			setState(84);
			number();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DataContext extends ParserRuleContext {
		public TerminalNode DATA() { return getToken(AsmParser.DATA, 0); }
		public NumberContext number() {
			return getRuleContext(NumberContext.class,0);
		}
		public TerminalNode FILEPATH() { return getToken(AsmParser.FILEPATH, 0); }
		public DataContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_data; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterData(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitData(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitData(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DataContext data() throws RecognitionException {
		DataContext _localctx = new DataContext(_ctx, getState());
		enterRule(_localctx, 16, RULE_data);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(86);
			match(DATA);
			setState(87);
			number();
			setState(88);
			match(FILEPATH);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class DefineContext extends ParserRuleContext {
		public TerminalNode DEFINE() { return getToken(AsmParser.DEFINE, 0); }
		public NameContext name() {
			return getRuleContext(NameContext.class,0);
		}
		public ExpressionContext expression() {
			return getRuleContext(ExpressionContext.class,0);
		}
		public DefineContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_define; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterDefine(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitDefine(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitDefine(this);
			else return visitor.visitChildren(this);
		}
	}

	public final DefineContext define() throws RecognitionException {
		DefineContext _localctx = new DefineContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_define);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(90);
			match(DEFINE);
			setState(91);
			name();
			setState(92);
			expression();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ExpressionContext extends ParserRuleContext {
		public List<MultiplyingExpressionContext> multiplyingExpression() {
			return getRuleContexts(MultiplyingExpressionContext.class);
		}
		public MultiplyingExpressionContext multiplyingExpression(int i) {
			return getRuleContext(MultiplyingExpressionContext.class,i);
		}
		public List<TerminalNode> SIGN() { return getTokens(AsmParser.SIGN); }
		public TerminalNode SIGN(int i) {
			return getToken(AsmParser.SIGN, i);
		}
		public ExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_expression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitExpression(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitExpression(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ExpressionContext expression() throws RecognitionException {
		ExpressionContext _localctx = new ExpressionContext(_ctx, getState());
		enterRule(_localctx, 20, RULE_expression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(94);
			multiplyingExpression();
			setState(99);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SIGN) {
				{
				{
				setState(95);
				match(SIGN);
				setState(96);
				multiplyingExpression();
				}
				}
				setState(101);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class MultiplyingExpressionContext extends ParserRuleContext {
		public List<ValueContext> value() {
			return getRuleContexts(ValueContext.class);
		}
		public ValueContext value(int i) {
			return getRuleContext(ValueContext.class,i);
		}
		public MultiplyingExpressionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_multiplyingExpression; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterMultiplyingExpression(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitMultiplyingExpression(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitMultiplyingExpression(this);
			else return visitor.visitChildren(this);
		}
	}

	public final MultiplyingExpressionContext multiplyingExpression() throws RecognitionException {
		MultiplyingExpressionContext _localctx = new MultiplyingExpressionContext(_ctx, getState());
		enterRule(_localctx, 22, RULE_multiplyingExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(102);
			value();
			setState(107);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (((_la) & ~0x3f) == 0 && ((1L << _la) & 60L) != 0) {
				{
				{
				setState(103);
				_la = _input.LA(1);
				if ( !(((_la) & ~0x3f) == 0 && ((1L << _la) & 60L) != 0) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(104);
				value();
				}
				}
				setState(109);
				_errHandler.sync(this);
				_la = _input.LA(1);
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class ValueContext extends ParserRuleContext {
		public NumberContext number() {
			return getRuleContext(NumberContext.class,0);
		}
		public NameContext name() {
			return getRuleContext(NameContext.class,0);
		}
		public ValueContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_value; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterValue(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitValue(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitValue(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ValueContext value() throws RecognitionException {
		ValueContext _localctx = new ValueContext(_ctx, getState());
		enterRule(_localctx, 24, RULE_value);
		try {
			setState(112);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case SIGN:
			case NUMBER:
			case HEXADECIMAL:
				enterOuterAlt(_localctx, 1);
				{
				setState(110);
				number();
				}
				break;
			case NAME:
				enterOuterAlt(_localctx, 2);
				{
				setState(111);
				name();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class IncludeContext extends ParserRuleContext {
		public TerminalNode INCLUDE() { return getToken(AsmParser.INCLUDE, 0); }
		public TerminalNode FILEPATH() { return getToken(AsmParser.FILEPATH, 0); }
		public IncludeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_include; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterInclude(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitInclude(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitInclude(this);
			else return visitor.visitChildren(this);
		}
	}

	public final IncludeContext include() throws RecognitionException {
		IncludeContext _localctx = new IncludeContext(_ctx, getState());
		enterRule(_localctx, 26, RULE_include);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(114);
			match(INCLUDE);
			setState(115);
			match(FILEPATH);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class FuncContext extends ParserRuleContext {
		public TerminalNode FUNC() { return getToken(AsmParser.FUNC, 0); }
		public NameContext name() {
			return getRuleContext(NameContext.class,0);
		}
		public FuncContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_func; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterFunc(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitFunc(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitFunc(this);
			else return visitor.visitChildren(this);
		}
	}

	public final FuncContext func() throws RecognitionException {
		FuncContext _localctx = new FuncContext(_ctx, getState());
		enterRule(_localctx, 28, RULE_func);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(117);
			match(FUNC);
			setState(118);
			name();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class EndfuncContext extends ParserRuleContext {
		public TerminalNode ENDFUNC() { return getToken(AsmParser.ENDFUNC, 0); }
		public EndfuncContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_endfunc; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterEndfunc(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitEndfunc(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitEndfunc(this);
			else return visitor.visitChildren(this);
		}
	}

	public final EndfuncContext endfunc() throws RecognitionException {
		EndfuncContext _localctx = new EndfuncContext(_ctx, getState());
		enterRule(_localctx, 30, RULE_endfunc);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(120);
			match(ENDFUNC);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class LbContext extends ParserRuleContext {
		public NameContext name() {
			return getRuleContext(NameContext.class,0);
		}
		public LbContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_lb; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterLb(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitLb(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitLb(this);
			else return visitor.visitChildren(this);
		}
	}

	public final LbContext lb() throws RecognitionException {
		LbContext _localctx = new LbContext(_ctx, getState());
		enterRule(_localctx, 32, RULE_lb);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(122);
			name();
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NameContext extends ParserRuleContext {
		public TerminalNode NAME() { return getToken(AsmParser.NAME, 0); }
		public NameContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_name; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterName(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitName(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitName(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NameContext name() throws RecognitionException {
		NameContext _localctx = new NameContext(_ctx, getState());
		enterRule(_localctx, 34, RULE_name);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(124);
			match(NAME);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class NumberContext extends ParserRuleContext {
		public TerminalNode NUMBER() { return getToken(AsmParser.NUMBER, 0); }
		public TerminalNode HEXADECIMAL() { return getToken(AsmParser.HEXADECIMAL, 0); }
		public TerminalNode SIGN() { return getToken(AsmParser.SIGN, 0); }
		public NumberContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_number; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterNumber(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitNumber(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitNumber(this);
			else return visitor.visitChildren(this);
		}
	}

	public final NumberContext number() throws RecognitionException {
		NumberContext _localctx = new NumberContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_number);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(127);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==SIGN) {
				{
				setState(126);
				match(SIGN);
				}
			}

			setState(129);
			_la = _input.LA(1);
			if ( !(_la==NUMBER || _la==HEXADECIMAL) ) {
			_errHandler.recoverInline(this);
			}
			else {
				if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
				_errHandler.reportMatch(this);
				consume();
			}
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class OpcodeContext extends ParserRuleContext {
		public TerminalNode OPCODE() { return getToken(AsmParser.OPCODE, 0); }
		public OpcodeContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_opcode; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterOpcode(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitOpcode(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitOpcode(this);
			else return visitor.visitChildren(this);
		}
	}

	public final OpcodeContext opcode() throws RecognitionException {
		OpcodeContext _localctx = new OpcodeContext(_ctx, getState());
		enterRule(_localctx, 38, RULE_opcode);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(131);
			match(OPCODE);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	@SuppressWarnings("CheckReturnValue")
	public static class CommentContext extends ParserRuleContext {
		public TerminalNode COMMENT() { return getToken(AsmParser.COMMENT, 0); }
		public CommentContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_comment; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterComment(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitComment(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitComment(this);
			else return visitor.visitChildren(this);
		}
	}

	public final CommentContext comment() throws RecognitionException {
		CommentContext _localctx = new CommentContext(_ctx, getState());
		enterRule(_localctx, 40, RULE_comment);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(133);
			match(COMMENT);
			}
		}
		catch (RecognitionException re) {
			_localctx.exception = re;
			_errHandler.reportError(this, re);
			_errHandler.recover(this, re);
		}
		finally {
			exitRule();
		}
		return _localctx;
	}

	public static final String _serializedATN =
		"\u0004\u0001\u0015\u0088\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001"+
		"\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004"+
		"\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007"+
		"\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b"+
		"\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007"+
		"\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007"+
		"\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0001\u0000\u0003"+
		"\u0000,\b\u0000\u0001\u0000\u0004\u0000/\b\u0000\u000b\u0000\f\u00000"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0003\u00017\b\u0001"+
		"\u0001\u0002\u0003\u0002:\b\u0002\u0001\u0002\u0001\u0002\u0001\u0002"+
		"\u0003\u0002?\b\u0002\u0001\u0003\u0001\u0003\u0003\u0003C\b\u0003\u0001"+
		"\u0004\u0001\u0004\u0003\u0004G\b\u0004\u0001\u0005\u0001\u0005\u0001"+
		"\u0005\u0001\u0005\u0001\u0005\u0003\u0005N\b\u0005\u0001\u0006\u0001"+
		"\u0006\u0003\u0006R\b\u0006\u0001\u0007\u0001\u0007\u0001\u0007\u0001"+
		"\b\u0001\b\u0001\b\u0001\b\u0001\t\u0001\t\u0001\t\u0001\t\u0001\n\u0001"+
		"\n\u0001\n\u0005\nb\b\n\n\n\f\ne\t\n\u0001\u000b\u0001\u000b\u0001\u000b"+
		"\u0005\u000bj\b\u000b\n\u000b\f\u000bm\t\u000b\u0001\f\u0001\f\u0003\f"+
		"q\b\f\u0001\r\u0001\r\u0001\r\u0001\u000e\u0001\u000e\u0001\u000e\u0001"+
		"\u000f\u0001\u000f\u0001\u0010\u0001\u0010\u0001\u0011\u0001\u0011\u0001"+
		"\u0012\u0003\u0012\u0080\b\u0012\u0001\u0012\u0001\u0012\u0001\u0013\u0001"+
		"\u0013\u0001\u0014\u0001\u0014\u0001\u0014\u0000\u0000\u0015\u0000\u0002"+
		"\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e"+
		" \"$&(\u0000\u0002\u0001\u0000\u0002\u0005\u0001\u0000\u0011\u0012\u0084"+
		"\u0000.\u0001\u0000\u0000\u0000\u00026\u0001\u0000\u0000\u0000\u00049"+
		"\u0001\u0000\u0000\u0000\u0006@\u0001\u0000\u0000\u0000\bD\u0001\u0000"+
		"\u0000\u0000\nM\u0001\u0000\u0000\u0000\fQ\u0001\u0000\u0000\u0000\u000e"+
		"S\u0001\u0000\u0000\u0000\u0010V\u0001\u0000\u0000\u0000\u0012Z\u0001"+
		"\u0000\u0000\u0000\u0014^\u0001\u0000\u0000\u0000\u0016f\u0001\u0000\u0000"+
		"\u0000\u0018p\u0001\u0000\u0000\u0000\u001ar\u0001\u0000\u0000\u0000\u001c"+
		"u\u0001\u0000\u0000\u0000\u001ex\u0001\u0000\u0000\u0000 z\u0001\u0000"+
		"\u0000\u0000\"|\u0001\u0000\u0000\u0000$\u007f\u0001\u0000\u0000\u0000"+
		"&\u0083\u0001\u0000\u0000\u0000(\u0085\u0001\u0000\u0000\u0000*,\u0003"+
		"\u0002\u0001\u0000+*\u0001\u0000\u0000\u0000+,\u0001\u0000\u0000\u0000"+
		",-\u0001\u0000\u0000\u0000-/\u0005\u0014\u0000\u0000.+\u0001\u0000\u0000"+
		"\u0000/0\u0001\u0000\u0000\u00000.\u0001\u0000\u0000\u000001\u0001\u0000"+
		"\u0000\u00001\u0001\u0001\u0000\u0000\u000027\u0003\n\u0005\u000037\u0003"+
		"\b\u0004\u000047\u0003\u0004\u0002\u000057\u0003(\u0014\u000062\u0001"+
		"\u0000\u0000\u000063\u0001\u0000\u0000\u000064\u0001\u0000\u0000\u0000"+
		"65\u0001\u0000\u0000\u00007\u0003\u0001\u0000\u0000\u00008:\u0003\b\u0004"+
		"\u000098\u0001\u0000\u0000\u00009:\u0001\u0000\u0000\u0000:;\u0001\u0000"+
		"\u0000\u0000;<\u0003\u0006\u0003\u0000<>\u0003\u0006\u0003\u0000=?\u0003"+
		"(\u0014\u0000>=\u0001\u0000\u0000\u0000>?\u0001\u0000\u0000\u0000?\u0005"+
		"\u0001\u0000\u0000\u0000@B\u0003&\u0013\u0000AC\u0003\u0018\f\u0000BA"+
		"\u0001\u0000\u0000\u0000BC\u0001\u0000\u0000\u0000C\u0007\u0001\u0000"+
		"\u0000\u0000DF\u0003 \u0010\u0000EG\u0005\u0001\u0000\u0000FE\u0001\u0000"+
		"\u0000\u0000FG\u0001\u0000\u0000\u0000G\t\u0001\u0000\u0000\u0000HN\u0003"+
		"\f\u0006\u0000IN\u0003\u0012\t\u0000JN\u0003\u001a\r\u0000KN\u0003\u001c"+
		"\u000e\u0000LN\u0003\u001e\u000f\u0000MH\u0001\u0000\u0000\u0000MI\u0001"+
		"\u0000\u0000\u0000MJ\u0001\u0000\u0000\u0000MK\u0001\u0000\u0000\u0000"+
		"ML\u0001\u0000\u0000\u0000N\u000b\u0001\u0000\u0000\u0000OR\u0003\u000e"+
		"\u0007\u0000PR\u0003\u0010\b\u0000QO\u0001\u0000\u0000\u0000QP\u0001\u0000"+
		"\u0000\u0000R\r\u0001\u0000\u0000\u0000ST\u0005\u0006\u0000\u0000TU\u0003"+
		"$\u0012\u0000U\u000f\u0001\u0000\u0000\u0000VW\u0005\u0007\u0000\u0000"+
		"WX\u0003$\u0012\u0000XY\u0005\b\u0000\u0000Y\u0011\u0001\u0000\u0000\u0000"+
		"Z[\u0005\f\u0000\u0000[\\\u0003\"\u0011\u0000\\]\u0003\u0014\n\u0000]"+
		"\u0013\u0001\u0000\u0000\u0000^c\u0003\u0016\u000b\u0000_`\u0005\u000f"+
		"\u0000\u0000`b\u0003\u0016\u000b\u0000a_\u0001\u0000\u0000\u0000be\u0001"+
		"\u0000\u0000\u0000ca\u0001\u0000\u0000\u0000cd\u0001\u0000\u0000\u0000"+
		"d\u0015\u0001\u0000\u0000\u0000ec\u0001\u0000\u0000\u0000fk\u0003\u0018"+
		"\f\u0000gh\u0007\u0000\u0000\u0000hj\u0003\u0018\f\u0000ig\u0001\u0000"+
		"\u0000\u0000jm\u0001\u0000\u0000\u0000ki\u0001\u0000\u0000\u0000kl\u0001"+
		"\u0000\u0000\u0000l\u0017\u0001\u0000\u0000\u0000mk\u0001\u0000\u0000"+
		"\u0000nq\u0003$\u0012\u0000oq\u0003\"\u0011\u0000pn\u0001\u0000\u0000"+
		"\u0000po\u0001\u0000\u0000\u0000q\u0019\u0001\u0000\u0000\u0000rs\u0005"+
		"\t\u0000\u0000st\u0005\b\u0000\u0000t\u001b\u0001\u0000\u0000\u0000uv"+
		"\u0005\n\u0000\u0000vw\u0003\"\u0011\u0000w\u001d\u0001\u0000\u0000\u0000"+
		"xy\u0005\u000b\u0000\u0000y\u001f\u0001\u0000\u0000\u0000z{\u0003\"\u0011"+
		"\u0000{!\u0001\u0000\u0000\u0000|}\u0005\u0010\u0000\u0000}#\u0001\u0000"+
		"\u0000\u0000~\u0080\u0005\u000f\u0000\u0000\u007f~\u0001\u0000\u0000\u0000"+
		"\u007f\u0080\u0001\u0000\u0000\u0000\u0080\u0081\u0001\u0000\u0000\u0000"+
		"\u0081\u0082\u0007\u0001\u0000\u0000\u0082%\u0001\u0000\u0000\u0000\u0083"+
		"\u0084\u0005\u000e\u0000\u0000\u0084\'\u0001\u0000\u0000\u0000\u0085\u0086"+
		"\u0005\r\u0000\u0000\u0086)\u0001\u0000\u0000\u0000\r+069>BFMQckp\u007f";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}