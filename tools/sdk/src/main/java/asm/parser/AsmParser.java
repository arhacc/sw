// Generated from Asm.g4 by ANTLR 4.12.0
 
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
	static { RuntimeMetaData.checkVersion("4.12.0", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		T__0=1, T__1=2, T__2=3, T__3=4, T__4=5, T__5=6, T__6=7, T__7=8, T__8=9, 
		T__9=10, T__10=11, T__11=12, T__12=13, T__13=14, T__14=15, T__15=16, T__16=17, 
		T__17=18, T__18=19, T__19=20, T__20=21, T__21=22, T__22=23, T__23=24, 
		T__24=25, T__25=26, T__26=27, T__27=28, T__28=29, T__29=30, T__30=31, 
		T__31=32, T__32=33, T__33=34, T__34=35, T__35=36, T__36=37, T__37=38, 
		T__38=39, T__39=40, T__40=41, T__41=42, T__42=43, T__43=44, T__44=45, 
		T__45=46, T__46=47, T__47=48, T__48=49, T__49=50, T__50=51, T__51=52, 
		T__52=53, T__53=54, T__54=55, T__55=56, T__56=57, T__57=58, T__58=59, 
		T__59=60, T__60=61, T__61=62, T__62=63, T__63=64, T__64=65, T__65=66, 
		T__66=67, T__67=68, T__68=69, T__69=70, T__70=71, T__71=72, T__72=73, 
		T__73=74, T__74=75, T__75=76, T__76=77, T__77=78, T__78=79, T__79=80, 
		T__80=81, T__81=82, T__82=83, T__83=84, T__84=85, T__85=86, T__86=87, 
		T__87=88, T__88=89, T__89=90, T__90=91, T__91=92, T__92=93, T__93=94, 
		T__94=95, T__95=96, T__96=97, T__97=98, T__98=99, T__99=100, T__100=101, 
		T__101=102, T__102=103, T__103=104, T__104=105, T__105=106, T__106=107, 
		T__107=108, T__108=109, T__109=110, T__110=111, T__111=112, T__112=113, 
		T__113=114, T__114=115, T__115=116, T__116=117, T__117=118, T__118=119, 
		T__119=120, T__120=121, T__121=122, T__122=123, T__123=124, T__124=125, 
		T__125=126, T__126=127, T__127=128, T__128=129, T__129=130, T__130=131, 
		T__131=132, T__132=133, T__133=134, T__134=135, T__135=136, T__136=137, 
		T__137=138, T__138=139, T__139=140, T__140=141, T__141=142, T__142=143, 
		T__143=144, T__144=145, T__145=146, T__146=147, T__147=148, T__148=149, 
		T__149=150, T__150=151, T__151=152, T__152=153, T__153=154, ARCHITECTUREID=155, 
		DATA=156, FILEPATH=157, INCLUDE=158, FUNC=159, ENDFUNC=160, DEFINE=161, 
		COMMENT=162, SIGN=163, NAME=164, NUMBER=165, HEXADECIMAL=166, STRING=167, 
		EOL=168, WS=169, COMMA=170;
	public static final int
		RULE_parse = 0, RULE_line = 1, RULE_instruction = 2, RULE_controlInstruction = 3, 
		RULE_controlOpcode0 = 4, RULE_controlOpcode1 = 5, RULE_controlOpcode2 = 6, 
		RULE_controlOpcode3 = 7, RULE_controlOpcode4 = 8, RULE_arrayInstruction = 9, 
		RULE_arrayOpcode0 = 10, RULE_arrayOpcode1 = 11, RULE_arrayOpcode2 = 12, 
		RULE_arrayOpcode3 = 13, RULE_arrayOpcode4 = 14, RULE_label = 15, RULE_directive = 16, 
		RULE_assemblerdirective = 17, RULE_architectureId = 18, RULE_data = 19, 
		RULE_define = 20, RULE_expression = 21, RULE_multiplyingExpression = 22, 
		RULE_value = 23, RULE_include = 24, RULE_func = 25, RULE_endfunc = 26, 
		RULE_lb = 27, RULE_name = 28, RULE_number = 29, RULE_comment = 30;
	private static String[] makeRuleNames() {
		return new String[] {
			"parse", "line", "instruction", "controlInstruction", "controlOpcode0", 
			"controlOpcode1", "controlOpcode2", "controlOpcode3", "controlOpcode4", 
			"arrayInstruction", "arrayOpcode0", "arrayOpcode1", "arrayOpcode2", "arrayOpcode3", 
			"arrayOpcode4", "label", "directive", "assemblerdirective", "architectureId", 
			"data", "define", "expression", "multiplyingExpression", "value", "include", 
			"func", "endfunc", "lb", "name", "number", "comment"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'riadd'", "'cadd'", "'caadd'", "'cradd'", "'vaddc'", "'addc'", 
			"'raddc'", "'riaddc'", "'caddc'", "'caaddc'", "'craddc'", "'vsub'", "'sub'", 
			"'rsub'", "'risub'", "'csub'", "'casub'", "'crsub'", "'vrvsub'", "'rvsub'", 
			"'rrvsub'", "'rirvsub'", "'crvsub'", "'carvsub'", "'crrvsub'", "'vsubc'", 
			"'subc'", "'rsubc'", "'risubc'", "'csubc'", "'casubc'", "'crsubc'", "'vrvsubc'", 
			"'rvsubc'", "'rrvsubc'", "'rirvsubc'", "'crvsubc'", "'carvsubc'", "'crrvsubc'", 
			"'vmult'", "'mult'", "'rmult'", "'rimult'", "'cmult'", "'camult'", "'crmult'", 
			"'vand'", "'and'", "'rand'", "'riand'", "'cand'", "'caand'", "'crand'", 
			"'vor'", "'or'", "'ror'", "'rior'", "'cor'", "'caor'", "'cror'", "'vxor'", 
			"'xor'", "'rxor'", "'rixor'", "'cxor'", "'caxor'", "'crxor'", "'vload'", 
			"'load'", "'rload'", "'riload'", "'cload'", "'caload'", "'crload'", "'ixload'", 
			"'srload'", "'ioload'", "'insval'", "'addrld'", "'store'", "'rstore'", 
			"'ristore'", "'cstore'", "'srstore'", "'iostore'", "'insertio'", "'compare'", 
			"'rcompare'", "'ricompare'", "'vcompare'", "'ccompare'", "'cacompare'", 
			"'crcompare'", "'shrightc'", "'shright'", "'sharight'", "'rotright'", 
			"'shr'", "'shl'", "'prun'", "'pload'", "'param'", "'getv'", "'sendv'", 
			"'setint'", "'nop'", "'jmp'", "'brz'", "'brnz'", "'brsgn'", "'brnsgn'", 
			"'brzdec'", "'brnzdec'", "'brbool'", "'brnbool'", "'brcr'", "'brncr'", 
			"'halt'", "'start'", "'stop'", "'wherezero'", "'wherecarry'", "'wherefirst'", 
			"'wherenext'", "'whereprev'", "'wherenzero'", "'wherencarry'", "'wherenfirst'", 
			"'wherennext'", "'wherenprev'", "'elsewhere'", "'endwhere'", "'activate'", 
			"'rednop'", "'redadd'", "'redmin'", "'redmax'", "'setval'", "'waitmatw'", 
			"'resready'", "'brcmpnvaldec'", "'setdec'", "'addrinc'", "'caddrinc'", 
			"'addrinc_acc'", "'addr_reg_stack_duplicate'", "'addr_reg_stack_pop'", 
			"'vadd'", "'add'", "'radd'", "':'", "'*'", "'/'", "'mod'", "'.architectureId'", 
			"'.data'", null, "'include'", "'func'", "'endfunc'", "'define'", null, 
			null, null, null, null, null, null, null, "','"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, "ARCHITECTUREID", 
			"DATA", "FILEPATH", "INCLUDE", "FUNC", "ENDFUNC", "DEFINE", "COMMENT", 
			"SIGN", "NAME", "NUMBER", "HEXADECIMAL", "STRING", "EOL", "WS", "COMMA"
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
	public String getGrammarFileName() { return "Asm.g4"; }

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
			setState(66); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(63);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 102416515071L) != 0)) {
					{
					setState(62);
					line();
					}
				}

				setState(65);
				match(EOL);
				}
				}
				setState(68); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( (((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 1201928142847L) != 0) );
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
			setState(74);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,2,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(70);
				directive();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(71);
				label();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(72);
				instruction();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(73);
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
		public ControlInstructionContext controlInstruction() {
			return getRuleContext(ControlInstructionContext.class,0);
		}
		public ArrayInstructionContext arrayInstruction() {
			return getRuleContext(ArrayInstructionContext.class,0);
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
			setState(77);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==NAME) {
				{
				setState(76);
				label();
				}
			}

			setState(79);
			controlInstruction();
			setState(80);
			arrayInstruction();
			setState(82);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT) {
				{
				setState(81);
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
	public static class ControlInstructionContext extends ParserRuleContext {
		public ControlOpcode0Context controlOpcode0() {
			return getRuleContext(ControlOpcode0Context.class,0);
		}
		public ControlOpcode1Context controlOpcode1() {
			return getRuleContext(ControlOpcode1Context.class,0);
		}
		public List<ValueContext> value() {
			return getRuleContexts(ValueContext.class);
		}
		public ValueContext value(int i) {
			return getRuleContext(ValueContext.class,i);
		}
		public ControlOpcode2Context controlOpcode2() {
			return getRuleContext(ControlOpcode2Context.class,0);
		}
		public TerminalNode COMMA() { return getToken(AsmParser.COMMA, 0); }
		public ControlOpcode3Context controlOpcode3() {
			return getRuleContext(ControlOpcode3Context.class,0);
		}
		public LbContext lb() {
			return getRuleContext(LbContext.class,0);
		}
		public ControlOpcode4Context controlOpcode4() {
			return getRuleContext(ControlOpcode4Context.class,0);
		}
		public ControlInstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controlInstruction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControlInstruction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControlInstruction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControlInstruction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControlInstructionContext controlInstruction() throws RecognitionException {
		ControlInstructionContext _localctx = new ControlInstructionContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_controlInstruction);
		try {
			setState(101);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,5,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(84);
				controlOpcode0();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(85);
				controlOpcode1();
				setState(86);
				value();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(88);
				controlOpcode2();
				setState(89);
				value();
				setState(90);
				match(COMMA);
				setState(91);
				value();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(93);
				controlOpcode3();
				setState(94);
				lb();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(96);
				controlOpcode4();
				setState(97);
				lb();
				setState(98);
				match(COMMA);
				setState(99);
				value();
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
	public static class ControlOpcode0Context extends ParserRuleContext {
		public ControlOpcode0Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controlOpcode0; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControlOpcode0(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControlOpcode0(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControlOpcode0(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControlOpcode0Context controlOpcode0() throws RecognitionException {
		ControlOpcode0Context _localctx = new ControlOpcode0Context(_ctx, getState());
		enterRule(_localctx, 8, RULE_controlOpcode0);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(103);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 1048575L) != 0)) ) {
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
	public static class ControlOpcode1Context extends ParserRuleContext {
		public ControlOpcode1Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controlOpcode1; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControlOpcode1(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControlOpcode1(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControlOpcode1(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControlOpcode1Context controlOpcode1() throws RecognitionException {
		ControlOpcode1Context _localctx = new ControlOpcode1Context(_ctx, getState());
		enterRule(_localctx, 10, RULE_controlOpcode1);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(105);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ControlOpcode2Context extends ParserRuleContext {
		public ControlOpcode2Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controlOpcode2; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControlOpcode2(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControlOpcode2(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControlOpcode2(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControlOpcode2Context controlOpcode2() throws RecognitionException {
		ControlOpcode2Context _localctx = new ControlOpcode2Context(_ctx, getState());
		enterRule(_localctx, 12, RULE_controlOpcode2);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(107);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ControlOpcode3Context extends ParserRuleContext {
		public ControlOpcode3Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controlOpcode3; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControlOpcode3(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControlOpcode3(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControlOpcode3(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControlOpcode3Context controlOpcode3() throws RecognitionException {
		ControlOpcode3Context _localctx = new ControlOpcode3Context(_ctx, getState());
		enterRule(_localctx, 14, RULE_controlOpcode3);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(109);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ControlOpcode4Context extends ParserRuleContext {
		public ControlOpcode4Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controlOpcode4; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControlOpcode4(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControlOpcode4(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControlOpcode4(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControlOpcode4Context controlOpcode4() throws RecognitionException {
		ControlOpcode4Context _localctx = new ControlOpcode4Context(_ctx, getState());
		enterRule(_localctx, 16, RULE_controlOpcode4);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(111);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ArrayInstructionContext extends ParserRuleContext {
		public ArrayOpcode0Context arrayOpcode0() {
			return getRuleContext(ArrayOpcode0Context.class,0);
		}
		public ArrayOpcode1Context arrayOpcode1() {
			return getRuleContext(ArrayOpcode1Context.class,0);
		}
		public List<ValueContext> value() {
			return getRuleContexts(ValueContext.class);
		}
		public ValueContext value(int i) {
			return getRuleContext(ValueContext.class,i);
		}
		public ArrayOpcode2Context arrayOpcode2() {
			return getRuleContext(ArrayOpcode2Context.class,0);
		}
		public TerminalNode COMMA() { return getToken(AsmParser.COMMA, 0); }
		public ArrayOpcode3Context arrayOpcode3() {
			return getRuleContext(ArrayOpcode3Context.class,0);
		}
		public LbContext lb() {
			return getRuleContext(LbContext.class,0);
		}
		public ArrayOpcode4Context arrayOpcode4() {
			return getRuleContext(ArrayOpcode4Context.class,0);
		}
		public ArrayInstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayInstruction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArrayInstruction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArrayInstruction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArrayInstruction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArrayInstructionContext arrayInstruction() throws RecognitionException {
		ArrayInstructionContext _localctx = new ArrayInstructionContext(_ctx, getState());
		enterRule(_localctx, 18, RULE_arrayInstruction);
		try {
			setState(130);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,6,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(113);
				arrayOpcode0();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(114);
				arrayOpcode1();
				setState(115);
				value();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(117);
				arrayOpcode2();
				setState(118);
				value();
				setState(119);
				match(COMMA);
				setState(120);
				value();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(122);
				arrayOpcode3();
				setState(123);
				lb();
				}
				break;
			case 5:
				enterOuterAlt(_localctx, 5);
				{
				setState(125);
				arrayOpcode4();
				setState(126);
				lb();
				setState(127);
				match(COMMA);
				setState(128);
				value();
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
	public static class ArrayOpcode0Context extends ParserRuleContext {
		public ArrayOpcode0Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayOpcode0; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArrayOpcode0(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArrayOpcode0(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArrayOpcode0(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArrayOpcode0Context arrayOpcode0() throws RecognitionException {
		ArrayOpcode0Context _localctx = new ArrayOpcode0Context(_ctx, getState());
		enterRule(_localctx, 20, RULE_arrayOpcode0);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(132);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 1048575L) != 0)) ) {
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
	public static class ArrayOpcode1Context extends ParserRuleContext {
		public ArrayOpcode1Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayOpcode1; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArrayOpcode1(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArrayOpcode1(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArrayOpcode1(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArrayOpcode1Context arrayOpcode1() throws RecognitionException {
		ArrayOpcode1Context _localctx = new ArrayOpcode1Context(_ctx, getState());
		enterRule(_localctx, 22, RULE_arrayOpcode1);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(134);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ArrayOpcode2Context extends ParserRuleContext {
		public ArrayOpcode2Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayOpcode2; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArrayOpcode2(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArrayOpcode2(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArrayOpcode2(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArrayOpcode2Context arrayOpcode2() throws RecognitionException {
		ArrayOpcode2Context _localctx = new ArrayOpcode2Context(_ctx, getState());
		enterRule(_localctx, 24, RULE_arrayOpcode2);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(136);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ArrayOpcode3Context extends ParserRuleContext {
		public ArrayOpcode3Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayOpcode3; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArrayOpcode3(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArrayOpcode3(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArrayOpcode3(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArrayOpcode3Context arrayOpcode3() throws RecognitionException {
		ArrayOpcode3Context _localctx = new ArrayOpcode3Context(_ctx, getState());
		enterRule(_localctx, 26, RULE_arrayOpcode3);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(138);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
	public static class ArrayOpcode4Context extends ParserRuleContext {
		public ArrayOpcode4Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_arrayOpcode4; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArrayOpcode4(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArrayOpcode4(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArrayOpcode4(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArrayOpcode4Context arrayOpcode4() throws RecognitionException {
		ArrayOpcode4Context _localctx = new ArrayOpcode4Context(_ctx, getState());
		enterRule(_localctx, 28, RULE_arrayOpcode4);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(140);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & 8388607L) != 0)) ) {
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
		enterRule(_localctx, 30, RULE_label);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(142);
			lb();
			setState(144);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__150) {
				{
				setState(143);
				match(T__150);
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
		enterRule(_localctx, 32, RULE_directive);
		try {
			setState(151);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ARCHITECTUREID:
			case DATA:
				enterOuterAlt(_localctx, 1);
				{
				setState(146);
				assemblerdirective();
				}
				break;
			case DEFINE:
				enterOuterAlt(_localctx, 2);
				{
				setState(147);
				define();
				}
				break;
			case INCLUDE:
				enterOuterAlt(_localctx, 3);
				{
				setState(148);
				include();
				}
				break;
			case FUNC:
				enterOuterAlt(_localctx, 4);
				{
				setState(149);
				func();
				}
				break;
			case ENDFUNC:
				enterOuterAlt(_localctx, 5);
				{
				setState(150);
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
		public ArchitectureIdContext architectureId() {
			return getRuleContext(ArchitectureIdContext.class,0);
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
		enterRule(_localctx, 34, RULE_assemblerdirective);
		try {
			setState(155);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ARCHITECTUREID:
				enterOuterAlt(_localctx, 1);
				{
				setState(153);
				architectureId();
				}
				break;
			case DATA:
				enterOuterAlt(_localctx, 2);
				{
				setState(154);
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
	public static class ArchitectureIdContext extends ParserRuleContext {
		public TerminalNode ARCHITECTUREID() { return getToken(AsmParser.ARCHITECTUREID, 0); }
		public NameContext name() {
			return getRuleContext(NameContext.class,0);
		}
		public ArchitectureIdContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_architectureId; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterArchitectureId(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitArchitectureId(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitArchitectureId(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ArchitectureIdContext architectureId() throws RecognitionException {
		ArchitectureIdContext _localctx = new ArchitectureIdContext(_ctx, getState());
		enterRule(_localctx, 36, RULE_architectureId);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(157);
			match(ARCHITECTUREID);
			setState(158);
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
		enterRule(_localctx, 38, RULE_data);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(160);
			match(DATA);
			setState(161);
			number();
			setState(162);
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
		enterRule(_localctx, 40, RULE_define);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(164);
			match(DEFINE);
			setState(165);
			name();
			setState(166);
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
		enterRule(_localctx, 42, RULE_expression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(168);
			multiplyingExpression();
			setState(173);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SIGN) {
				{
				{
				setState(169);
				match(SIGN);
				setState(170);
				multiplyingExpression();
				}
				}
				setState(175);
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
		enterRule(_localctx, 44, RULE_multiplyingExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(176);
			value();
			setState(181);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__47 || ((((_la - 152)) & ~0x3f) == 0 && ((1L << (_la - 152)) & 7L) != 0)) {
				{
				{
				setState(177);
				_la = _input.LA(1);
				if ( !(_la==T__47 || ((((_la - 152)) & ~0x3f) == 0 && ((1L << (_la - 152)) & 7L) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(178);
				value();
				}
				}
				setState(183);
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
		public TerminalNode COMMA() { return getToken(AsmParser.COMMA, 0); }
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
		enterRule(_localctx, 46, RULE_value);
		try {
			setState(190);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,12,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(184);
				number();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(185);
				name();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(186);
				name();
				setState(187);
				match(COMMA);
				setState(188);
				number();
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
		enterRule(_localctx, 48, RULE_include);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(192);
			match(INCLUDE);
			setState(193);
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
		enterRule(_localctx, 50, RULE_func);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(195);
			match(FUNC);
			setState(196);
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
		enterRule(_localctx, 52, RULE_endfunc);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(198);
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
		enterRule(_localctx, 54, RULE_lb);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(200);
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
		enterRule(_localctx, 56, RULE_name);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(202);
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
		enterRule(_localctx, 58, RULE_number);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(205);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==SIGN) {
				{
				setState(204);
				match(SIGN);
				}
			}

			setState(207);
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
		enterRule(_localctx, 60, RULE_comment);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(209);
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
		"\u0004\u0001\u00aa\u00d4\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001"+
		"\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004"+
		"\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007"+
		"\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b"+
		"\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007"+
		"\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007"+
		"\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007"+
		"\u0015\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002\u0018\u0007"+
		"\u0018\u0002\u0019\u0007\u0019\u0002\u001a\u0007\u001a\u0002\u001b\u0007"+
		"\u001b\u0002\u001c\u0007\u001c\u0002\u001d\u0007\u001d\u0002\u001e\u0007"+
		"\u001e\u0001\u0000\u0003\u0000@\b\u0000\u0001\u0000\u0004\u0000C\b\u0000"+
		"\u000b\u0000\f\u0000D\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0003\u0001K\b\u0001\u0001\u0002\u0003\u0002N\b\u0002\u0001\u0002\u0001"+
		"\u0002\u0001\u0002\u0003\u0002S\b\u0002\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0003\u0003f\b\u0003\u0001\u0004\u0001"+
		"\u0004\u0001\u0005\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0007\u0001"+
		"\u0007\u0001\b\u0001\b\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t"+
		"\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0003\t\u0083\b\t\u0001\n\u0001\n\u0001\u000b\u0001"+
		"\u000b\u0001\f\u0001\f\u0001\r\u0001\r\u0001\u000e\u0001\u000e\u0001\u000f"+
		"\u0001\u000f\u0003\u000f\u0091\b\u000f\u0001\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0010\u0003\u0010\u0098\b\u0010\u0001\u0011\u0001\u0011"+
		"\u0003\u0011\u009c\b\u0011\u0001\u0012\u0001\u0012\u0001\u0012\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0014\u0001\u0014\u0001\u0014"+
		"\u0001\u0014\u0001\u0015\u0001\u0015\u0001\u0015\u0005\u0015\u00ac\b\u0015"+
		"\n\u0015\f\u0015\u00af\t\u0015\u0001\u0016\u0001\u0016\u0001\u0016\u0005"+
		"\u0016\u00b4\b\u0016\n\u0016\f\u0016\u00b7\t\u0016\u0001\u0017\u0001\u0017"+
		"\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0017\u0003\u0017\u00bf\b\u0017"+
		"\u0001\u0018\u0001\u0018\u0001\u0018\u0001\u0019\u0001\u0019\u0001\u0019"+
		"\u0001\u001a\u0001\u001a\u0001\u001b\u0001\u001b\u0001\u001c\u0001\u001c"+
		"\u0001\u001d\u0003\u001d\u00ce\b\u001d\u0001\u001d\u0001\u001d\u0001\u001e"+
		"\u0001\u001e\u0001\u001e\u0000\u0000\u001f\u0000\u0002\u0004\u0006\b\n"+
		"\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c\u001e \"$&(*,.0246"+
		"8:<\u0000\u0004\u0001\u0000\u0001\u0093\u0001\u0000\u0001\u0096\u0002"+
		"\u000000\u0098\u009a\u0001\u0000\u00a5\u00a6\u00ce\u0000B\u0001\u0000"+
		"\u0000\u0000\u0002J\u0001\u0000\u0000\u0000\u0004M\u0001\u0000\u0000\u0000"+
		"\u0006e\u0001\u0000\u0000\u0000\bg\u0001\u0000\u0000\u0000\ni\u0001\u0000"+
		"\u0000\u0000\fk\u0001\u0000\u0000\u0000\u000em\u0001\u0000\u0000\u0000"+
		"\u0010o\u0001\u0000\u0000\u0000\u0012\u0082\u0001\u0000\u0000\u0000\u0014"+
		"\u0084\u0001\u0000\u0000\u0000\u0016\u0086\u0001\u0000\u0000\u0000\u0018"+
		"\u0088\u0001\u0000\u0000\u0000\u001a\u008a\u0001\u0000\u0000\u0000\u001c"+
		"\u008c\u0001\u0000\u0000\u0000\u001e\u008e\u0001\u0000\u0000\u0000 \u0097"+
		"\u0001\u0000\u0000\u0000\"\u009b\u0001\u0000\u0000\u0000$\u009d\u0001"+
		"\u0000\u0000\u0000&\u00a0\u0001\u0000\u0000\u0000(\u00a4\u0001\u0000\u0000"+
		"\u0000*\u00a8\u0001\u0000\u0000\u0000,\u00b0\u0001\u0000\u0000\u0000."+
		"\u00be\u0001\u0000\u0000\u00000\u00c0\u0001\u0000\u0000\u00002\u00c3\u0001"+
		"\u0000\u0000\u00004\u00c6\u0001\u0000\u0000\u00006\u00c8\u0001\u0000\u0000"+
		"\u00008\u00ca\u0001\u0000\u0000\u0000:\u00cd\u0001\u0000\u0000\u0000<"+
		"\u00d1\u0001\u0000\u0000\u0000>@\u0003\u0002\u0001\u0000?>\u0001\u0000"+
		"\u0000\u0000?@\u0001\u0000\u0000\u0000@A\u0001\u0000\u0000\u0000AC\u0005"+
		"\u00a8\u0000\u0000B?\u0001\u0000\u0000\u0000CD\u0001\u0000\u0000\u0000"+
		"DB\u0001\u0000\u0000\u0000DE\u0001\u0000\u0000\u0000E\u0001\u0001\u0000"+
		"\u0000\u0000FK\u0003 \u0010\u0000GK\u0003\u001e\u000f\u0000HK\u0003\u0004"+
		"\u0002\u0000IK\u0003<\u001e\u0000JF\u0001\u0000\u0000\u0000JG\u0001\u0000"+
		"\u0000\u0000JH\u0001\u0000\u0000\u0000JI\u0001\u0000\u0000\u0000K\u0003"+
		"\u0001\u0000\u0000\u0000LN\u0003\u001e\u000f\u0000ML\u0001\u0000\u0000"+
		"\u0000MN\u0001\u0000\u0000\u0000NO\u0001\u0000\u0000\u0000OP\u0003\u0006"+
		"\u0003\u0000PR\u0003\u0012\t\u0000QS\u0003<\u001e\u0000RQ\u0001\u0000"+
		"\u0000\u0000RS\u0001\u0000\u0000\u0000S\u0005\u0001\u0000\u0000\u0000"+
		"Tf\u0003\b\u0004\u0000UV\u0003\n\u0005\u0000VW\u0003.\u0017\u0000Wf\u0001"+
		"\u0000\u0000\u0000XY\u0003\f\u0006\u0000YZ\u0003.\u0017\u0000Z[\u0005"+
		"\u00aa\u0000\u0000[\\\u0003.\u0017\u0000\\f\u0001\u0000\u0000\u0000]^"+
		"\u0003\u000e\u0007\u0000^_\u00036\u001b\u0000_f\u0001\u0000\u0000\u0000"+
		"`a\u0003\u0010\b\u0000ab\u00036\u001b\u0000bc\u0005\u00aa\u0000\u0000"+
		"cd\u0003.\u0017\u0000df\u0001\u0000\u0000\u0000eT\u0001\u0000\u0000\u0000"+
		"eU\u0001\u0000\u0000\u0000eX\u0001\u0000\u0000\u0000e]\u0001\u0000\u0000"+
		"\u0000e`\u0001\u0000\u0000\u0000f\u0007\u0001\u0000\u0000\u0000gh\u0007"+
		"\u0000\u0000\u0000h\t\u0001\u0000\u0000\u0000ij\u0007\u0001\u0000\u0000"+
		"j\u000b\u0001\u0000\u0000\u0000kl\u0007\u0001\u0000\u0000l\r\u0001\u0000"+
		"\u0000\u0000mn\u0007\u0001\u0000\u0000n\u000f\u0001\u0000\u0000\u0000"+
		"op\u0007\u0001\u0000\u0000p\u0011\u0001\u0000\u0000\u0000q\u0083\u0003"+
		"\u0014\n\u0000rs\u0003\u0016\u000b\u0000st\u0003.\u0017\u0000t\u0083\u0001"+
		"\u0000\u0000\u0000uv\u0003\u0018\f\u0000vw\u0003.\u0017\u0000wx\u0005"+
		"\u00aa\u0000\u0000xy\u0003.\u0017\u0000y\u0083\u0001\u0000\u0000\u0000"+
		"z{\u0003\u001a\r\u0000{|\u00036\u001b\u0000|\u0083\u0001\u0000\u0000\u0000"+
		"}~\u0003\u001c\u000e\u0000~\u007f\u00036\u001b\u0000\u007f\u0080\u0005"+
		"\u00aa\u0000\u0000\u0080\u0081\u0003.\u0017\u0000\u0081\u0083\u0001\u0000"+
		"\u0000\u0000\u0082q\u0001\u0000\u0000\u0000\u0082r\u0001\u0000\u0000\u0000"+
		"\u0082u\u0001\u0000\u0000\u0000\u0082z\u0001\u0000\u0000\u0000\u0082}"+
		"\u0001\u0000\u0000\u0000\u0083\u0013\u0001\u0000\u0000\u0000\u0084\u0085"+
		"\u0007\u0000\u0000\u0000\u0085\u0015\u0001\u0000\u0000\u0000\u0086\u0087"+
		"\u0007\u0001\u0000\u0000\u0087\u0017\u0001\u0000\u0000\u0000\u0088\u0089"+
		"\u0007\u0001\u0000\u0000\u0089\u0019\u0001\u0000\u0000\u0000\u008a\u008b"+
		"\u0007\u0001\u0000\u0000\u008b\u001b\u0001\u0000\u0000\u0000\u008c\u008d"+
		"\u0007\u0001\u0000\u0000\u008d\u001d\u0001\u0000\u0000\u0000\u008e\u0090"+
		"\u00036\u001b\u0000\u008f\u0091\u0005\u0097\u0000\u0000\u0090\u008f\u0001"+
		"\u0000\u0000\u0000\u0090\u0091\u0001\u0000\u0000\u0000\u0091\u001f\u0001"+
		"\u0000\u0000\u0000\u0092\u0098\u0003\"\u0011\u0000\u0093\u0098\u0003("+
		"\u0014\u0000\u0094\u0098\u00030\u0018\u0000\u0095\u0098\u00032\u0019\u0000"+
		"\u0096\u0098\u00034\u001a\u0000\u0097\u0092\u0001\u0000\u0000\u0000\u0097"+
		"\u0093\u0001\u0000\u0000\u0000\u0097\u0094\u0001\u0000\u0000\u0000\u0097"+
		"\u0095\u0001\u0000\u0000\u0000\u0097\u0096\u0001\u0000\u0000\u0000\u0098"+
		"!\u0001\u0000\u0000\u0000\u0099\u009c\u0003$\u0012\u0000\u009a\u009c\u0003"+
		"&\u0013\u0000\u009b\u0099\u0001\u0000\u0000\u0000\u009b\u009a\u0001\u0000"+
		"\u0000\u0000\u009c#\u0001\u0000\u0000\u0000\u009d\u009e\u0005\u009b\u0000"+
		"\u0000\u009e\u009f\u00038\u001c\u0000\u009f%\u0001\u0000\u0000\u0000\u00a0"+
		"\u00a1\u0005\u009c\u0000\u0000\u00a1\u00a2\u0003:\u001d\u0000\u00a2\u00a3"+
		"\u0005\u009d\u0000\u0000\u00a3\'\u0001\u0000\u0000\u0000\u00a4\u00a5\u0005"+
		"\u00a1\u0000\u0000\u00a5\u00a6\u00038\u001c\u0000\u00a6\u00a7\u0003*\u0015"+
		"\u0000\u00a7)\u0001\u0000\u0000\u0000\u00a8\u00ad\u0003,\u0016\u0000\u00a9"+
		"\u00aa\u0005\u00a3\u0000\u0000\u00aa\u00ac\u0003,\u0016\u0000\u00ab\u00a9"+
		"\u0001\u0000\u0000\u0000\u00ac\u00af\u0001\u0000\u0000\u0000\u00ad\u00ab"+
		"\u0001\u0000\u0000\u0000\u00ad\u00ae\u0001\u0000\u0000\u0000\u00ae+\u0001"+
		"\u0000\u0000\u0000\u00af\u00ad\u0001\u0000\u0000\u0000\u00b0\u00b5\u0003"+
		".\u0017\u0000\u00b1\u00b2\u0007\u0002\u0000\u0000\u00b2\u00b4\u0003.\u0017"+
		"\u0000\u00b3\u00b1\u0001\u0000\u0000\u0000\u00b4\u00b7\u0001\u0000\u0000"+
		"\u0000\u00b5\u00b3\u0001\u0000\u0000\u0000\u00b5\u00b6\u0001\u0000\u0000"+
		"\u0000\u00b6-\u0001\u0000\u0000\u0000\u00b7\u00b5\u0001\u0000\u0000\u0000"+
		"\u00b8\u00bf\u0003:\u001d\u0000\u00b9\u00bf\u00038\u001c\u0000\u00ba\u00bb"+
		"\u00038\u001c\u0000\u00bb\u00bc\u0005\u00aa\u0000\u0000\u00bc\u00bd\u0003"+
		":\u001d\u0000\u00bd\u00bf\u0001\u0000\u0000\u0000\u00be\u00b8\u0001\u0000"+
		"\u0000\u0000\u00be\u00b9\u0001\u0000\u0000\u0000\u00be\u00ba\u0001\u0000"+
		"\u0000\u0000\u00bf/\u0001\u0000\u0000\u0000\u00c0\u00c1\u0005\u009e\u0000"+
		"\u0000\u00c1\u00c2\u0005\u009d\u0000\u0000\u00c21\u0001\u0000\u0000\u0000"+
		"\u00c3\u00c4\u0005\u009f\u0000\u0000\u00c4\u00c5\u00038\u001c\u0000\u00c5"+
		"3\u0001\u0000\u0000\u0000\u00c6\u00c7\u0005\u00a0\u0000\u0000\u00c75\u0001"+
		"\u0000\u0000\u0000\u00c8\u00c9\u00038\u001c\u0000\u00c97\u0001\u0000\u0000"+
		"\u0000\u00ca\u00cb\u0005\u00a4\u0000\u0000\u00cb9\u0001\u0000\u0000\u0000"+
		"\u00cc\u00ce\u0005\u00a3\u0000\u0000\u00cd\u00cc\u0001\u0000\u0000\u0000"+
		"\u00cd\u00ce\u0001\u0000\u0000\u0000\u00ce\u00cf\u0001\u0000\u0000\u0000"+
		"\u00cf\u00d0\u0007\u0003\u0000\u0000\u00d0;\u0001\u0000\u0000\u0000\u00d1"+
		"\u00d2\u0005\u00a2\u0000\u0000\u00d2=\u0001\u0000\u0000\u0000\u000e?D"+
		"JMRe\u0082\u0090\u0097\u009b\u00ad\u00b5\u00be\u00cd";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}