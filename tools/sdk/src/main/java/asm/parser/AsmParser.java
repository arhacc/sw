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
		T__149=150, T__150=151, T__151=152, T__152=153, T__153=154, T__154=155, 
		T__155=156, T__156=157, T__157=158, T__158=159, T__159=160, T__160=161, 
		T__161=162, T__162=163, T__163=164, T__164=165, T__165=166, T__166=167, 
		T__167=168, T__168=169, T__169=170, T__170=171, T__171=172, T__172=173, 
		T__173=174, T__174=175, T__175=176, T__176=177, T__177=178, T__178=179, 
		T__179=180, T__180=181, T__181=182, T__182=183, T__183=184, T__184=185, 
		T__185=186, T__186=187, T__187=188, T__188=189, T__189=190, T__190=191, 
		T__191=192, T__192=193, T__193=194, T__194=195, T__195=196, T__196=197, 
		T__197=198, T__198=199, T__199=200, T__200=201, T__201=202, T__202=203, 
		T__203=204, T__204=205, T__205=206, T__206=207, T__207=208, T__208=209, 
		T__209=210, T__210=211, T__211=212, T__212=213, T__213=214, T__214=215, 
		T__215=216, T__216=217, T__217=218, T__218=219, T__219=220, T__220=221, 
		T__221=222, T__222=223, T__223=224, T__224=225, T__225=226, T__226=227, 
		T__227=228, T__228=229, T__229=230, T__230=231, T__231=232, T__232=233, 
		T__233=234, T__234=235, T__235=236, T__236=237, T__237=238, T__238=239, 
		T__239=240, T__240=241, T__241=242, T__242=243, T__243=244, T__244=245, 
		ARCHITECTUREID=246, DATA=247, FILEPATH=248, INCLUDE=249, FUNC=250, ENDFUNC=251, 
		DEFINE=252, AsciiChars=253, SIGN=254, NAME=255, NUMBER=256, HEXADECIMAL=257, 
		STRING=258, TS=259, EOL=260, COMMA=261, COMMENT=262;
	public static final int
		RULE_parse = 0, RULE_line = 1, RULE_instruction = 2, RULE_controllerInstruction = 3, 
		RULE_arrayInstruction = 4, RULE_controllerOpcode0 = 5, RULE_controllerOpcode1 = 6, 
		RULE_controllerOpcode2 = 7, RULE_controllerOpcode3 = 8, RULE_arrayOpcode0 = 9, 
		RULE_arrayOpcode1 = 10, RULE_label = 11, RULE_directive = 12, RULE_assemblerdirective = 13, 
		RULE_architectureId = 14, RULE_data = 15, RULE_define = 16, RULE_expression = 17, 
		RULE_multiplyingExpression = 18, RULE_value = 19, RULE_include = 20, RULE_func = 21, 
		RULE_endfunc = 22, RULE_lb = 23, RULE_name = 24, RULE_number = 25, RULE_comment = 26, 
		RULE_other = 27;
	private static String[] makeRuleNames() {
		return new String[] {
			"parse", "line", "instruction", "controllerInstruction", "arrayInstruction", 
			"controllerOpcode0", "controllerOpcode1", "controllerOpcode2", "controllerOpcode3", 
			"arrayOpcode0", "arrayOpcode1", "label", "directive", "assemblerdirective", 
			"architectureId", "data", "define", "expression", "multiplyingExpression", 
			"value", "include", "func", "endfunc", "lb", "name", "number", "comment", 
			"other"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'cadd'", "'sadd'", "'cradd'", "'caddc'", "'saddc'", "'craddc'", 
			"'smult'", "'cmult'", "'crmult'", "'ssub'", "'csub'", "'crsub'", "'ssubc'", 
			"'csubc'", "'crsubc'", "'srvsub'", "'crvsub'", "'crrvsub'", "'srvsubc'", 
			"'crvsubc'", "'crrvsubc'", "'scompare'", "'ccompare'", "'crcompare'", 
			"'sand'", "'cand'", "'crand'", "'sor'", "'cor'", "'cror'", "'sxor'", 
			"'cxor'", "'crxor'", "'nop'", "'crstore'", "'cstack_store_pop'", "'crstack_store_pop'", 
			"'crswap_acc_mem'", "'sload'", "'cload'", "'cstack_push_load'", "'sstack_push_load'", 
			"'crstack_push_load'", "'shright'", "'shrightc'", "'sharight'", "'shright_fixed_amount'", 
			"'sharight_fixed_amount'", "'grshift_wob'", "'grshift_wbz'", "'grshift_wbh'", 
			"'crightins'", "'cleftins'", "'right_redins'", "'left_redins'", "'glshift_wob'", 
			"'glshift_wbz'", "'glshift_wbh'", "'rotate_right'", "'rotate_left'", 
			"'halt'", "'cc_start_w_halt'", "'cc_start_wo_halt'", "'cc_stop'", "'cc_reset'", 
			"'srstore'", "'addrstore'", "'setdec'", "'addrload'", "'stack_pop'", 
			"'stack_duplicate'", "'stack_over'", "'stack_swap'", "'stack_load_layer1'", 
			"'param'", "'setint'", "'resready'", "'vadd'", "'add'", "'radd'", "'riadd'", 
			"'vaddc'", "'addc'", "'raddc'", "'riaddc'", "'vmult'", "'mult'", "'rmult'", 
			"'rimult'", "'vsub'", "'sub'", "'rsub'", "'risub'", "'vsubc'", "'subc'", 
			"'rsubc'", "'risubc'", "'vrvsub'", "'rvsub'", "'rrvsub'", "'rirvsub'", 
			"'vrvsubc'", "'rvsubc'", "'rrvsubc'", "'rirvsubc'", "'vcompare'", "'compare'", 
			"'rcompare'", "'ricompare'", "'vand'", "'and'", "'rand'", "'riand'", 
			"'vor'", "'or'", "'ror'", "'rior'", "'vxor'", "'xor'", "'rxor'", "'rixor'", 
			"'fadd'", "'fradd'", "'fsub'", "'frsub'", "'fmult'", "'frmult'", "'frtmult'", 
			"'rfrtmult'", "'scdmult'", "'rscdmult'", "'fdiv'", "'frdiv'", "'addrinc'", 
			"'store'", "'rstore'", "'ristore'", "'sstore'", "'stack_store_pop'", 
			"'rstack_store_pop'", "'ristack_store_pop'", "'swap_acc_mem'", "'rswap_acc_mem'", 
			"'riswap_acc_mem'", "'vload'", "'load'", "'rload'", "'riload'", "'vstack_push_load'", 
			"'stack_push_load'", "'rstack_push_load'", "'ristack_push_load'", "'send'", 
			"'rsend'", "'risend'", "'csend'", "'ssend'", "'crsend'", "'insval'", 
			"'rotright_local'", "'rotleft_local'", "'shift_right'", "'shift_left'", 
			"'vleftins'", "'float'", "'sel_addrreg'", "'setval'", "'waitmatw'", "'jmp'", 
			"'brz'", "'brnz'", "'brsgn'", "'brnsgn'", "'brzdec'", "'brnzdec'", "'brbool'", 
			"'brnbool'", "'brcr'", "'brncr'", "'brcr_delayed'", "'brncr_delayed'", 
			"'brvalz'", "'brvalnz'", "'brvalsgn'", "'brvalnsgn'", "'brvalzdec'", 
			"'brvalnzdec'", "'brcmpval'", "'brcmpnval'", "'brcmpvaldec'", "'brcmpnvaldec'", 
			"'caddrinc'", "'cstore'", "'crload'", "'srcall'", "'search'", "'csearch'", 
			"'selshift'", "'delete'", "'wherezero'", "'wherepositive'", "'wherecarry'", 
			"'wherefirst'", "'wherenext'", "'whereprev'", "'wherenzero'", "'wherenegative'", 
			"'wherencarry'", "'wherenfirst'", "'wherennext'", "'wherenprev'", "'elsewhere'", 
			"'endwhere'", "'activate'", "'scannop'", "'scanadd'", "'scanmin'", "'scanmax'", 
			"'scanprefix_add'", "'scanprefix_bitwise_xor'", "'scansub'", "'scan_bitwise_or'", 
			"'scan_bitwise_xor'", "'scan_bitwise_and'", "'addrinc_acc'", "'addr_reg_stack_duplicate'", 
			"'addr_reg_stack_pop'", "'caddrstore'", "'insertio'", "'ixload'", "'srload'", 
			"'scanload'", "'loop1'", "'rloop1'", "'loop2'", "'rloop2'", "'vsrcall'", 
			"'vsearch'", "'vcsearch'", "'scansplit'", "'scanpermute'", "':'", "'*'", 
			"'/'", "'mod'", "'.architectureId'", "'.data'", null, "'include'", "'func'", 
			"'endfunc'", "'define'", null, null, null, null, null, null, null, null, 
			"','"
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
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, null, null, null, null, null, null, 
			null, null, null, null, null, null, "ARCHITECTUREID", "DATA", "FILEPATH", 
			"INCLUDE", "FUNC", "ENDFUNC", "DEFINE", "AsciiChars", "SIGN", "NAME", 
			"NUMBER", "HEXADECIMAL", "STRING", "TS", "EOL", "COMMA", "COMMENT"
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
			setState(60); 
			_errHandler.sync(this);
			_la = _input.LA(1);
			do {
				{
				{
				setState(57);
				_errHandler.sync(this);
				_la = _input.LA(1);
				if ((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & -1L) != 0) || ((((_la - 246)) & ~0x3f) == 0 && ((1L << (_la - 246)) & 66171L) != 0)) {
					{
					setState(56);
					line();
					}
				}

				setState(59);
				match(EOL);
				}
				}
				setState(62); 
				_errHandler.sync(this);
				_la = _input.LA(1);
			} while ( (((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & -1L) != 0) || ((((_la - 128)) & ~0x3f) == 0 && ((1L << (_la - 128)) & -1L) != 0) || ((((_la - 246)) & ~0x3f) == 0 && ((1L << (_la - 246)) & 82555L) != 0) );
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
			setState(68);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,2,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(64);
				directive();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(65);
				label();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(66);
				instruction();
				}
				break;
			case 4:
				enterOuterAlt(_localctx, 4);
				{
				setState(67);
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
		public ControllerInstructionContext controllerInstruction() {
			return getRuleContext(ControllerInstructionContext.class,0);
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
			setState(71);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==NAME) {
				{
				setState(70);
				label();
				}
			}

			setState(73);
			controllerInstruction();
			setState(74);
			arrayInstruction();
			setState(76);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==COMMENT) {
				{
				setState(75);
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
	public static class ControllerInstructionContext extends ParserRuleContext {
		public ControllerOpcode0Context controllerOpcode0() {
			return getRuleContext(ControllerOpcode0Context.class,0);
		}
		public ControllerOpcode1Context controllerOpcode1() {
			return getRuleContext(ControllerOpcode1Context.class,0);
		}
		public ValueContext value() {
			return getRuleContext(ValueContext.class,0);
		}
		public ControllerOpcode2Context controllerOpcode2() {
			return getRuleContext(ControllerOpcode2Context.class,0);
		}
		public LbContext lb() {
			return getRuleContext(LbContext.class,0);
		}
		public ControllerOpcode3Context controllerOpcode3() {
			return getRuleContext(ControllerOpcode3Context.class,0);
		}
		public TerminalNode COMMA() { return getToken(AsmParser.COMMA, 0); }
		public ControllerInstructionContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controllerInstruction; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControllerInstruction(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControllerInstruction(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControllerInstruction(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControllerInstructionContext controllerInstruction() throws RecognitionException {
		ControllerInstructionContext _localctx = new ControllerInstructionContext(_ctx, getState());
		enterRule(_localctx, 6, RULE_controllerInstruction);
		try {
			setState(90);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
			case T__1:
			case T__2:
			case T__3:
			case T__4:
			case T__5:
			case T__6:
			case T__7:
			case T__8:
			case T__9:
			case T__10:
			case T__11:
			case T__12:
			case T__13:
			case T__14:
			case T__15:
			case T__16:
			case T__17:
			case T__18:
			case T__19:
			case T__20:
			case T__21:
			case T__22:
			case T__23:
			case T__24:
			case T__25:
			case T__26:
			case T__27:
			case T__28:
			case T__29:
			case T__30:
			case T__31:
			case T__32:
			case T__33:
			case T__34:
			case T__35:
			case T__36:
			case T__37:
			case T__38:
			case T__39:
			case T__40:
			case T__41:
			case T__42:
			case T__43:
			case T__44:
			case T__45:
			case T__46:
			case T__47:
			case T__48:
			case T__49:
			case T__50:
			case T__51:
			case T__52:
			case T__53:
			case T__54:
			case T__55:
			case T__56:
			case T__57:
			case T__58:
			case T__59:
			case T__60:
			case T__61:
			case T__62:
			case T__63:
			case T__64:
			case T__65:
			case T__66:
			case T__67:
			case T__68:
			case T__69:
			case T__70:
			case T__71:
			case T__72:
			case T__73:
			case T__74:
			case T__75:
			case T__76:
				enterOuterAlt(_localctx, 1);
				{
				setState(78);
				controllerOpcode0();
				}
				break;
			case T__77:
			case T__78:
			case T__79:
			case T__80:
			case T__81:
			case T__82:
			case T__83:
			case T__84:
			case T__85:
			case T__86:
			case T__87:
			case T__88:
			case T__89:
			case T__90:
			case T__91:
			case T__92:
			case T__93:
			case T__94:
			case T__95:
			case T__96:
			case T__97:
			case T__98:
			case T__99:
			case T__100:
			case T__101:
			case T__102:
			case T__103:
			case T__104:
			case T__105:
			case T__106:
			case T__107:
			case T__108:
			case T__109:
			case T__110:
			case T__111:
			case T__112:
			case T__113:
			case T__114:
			case T__115:
			case T__116:
			case T__117:
			case T__118:
			case T__119:
			case T__120:
			case T__121:
			case T__122:
			case T__123:
			case T__124:
			case T__125:
			case T__126:
			case T__127:
			case T__128:
			case T__129:
			case T__130:
			case T__131:
			case T__132:
			case T__133:
			case T__134:
			case T__135:
			case T__136:
			case T__137:
			case T__138:
			case T__139:
			case T__140:
			case T__141:
			case T__142:
			case T__143:
			case T__144:
			case T__145:
			case T__146:
			case T__147:
			case T__148:
			case T__149:
			case T__150:
			case T__151:
			case T__152:
			case T__153:
			case T__154:
			case T__155:
			case T__156:
			case T__157:
			case T__158:
			case T__159:
			case T__160:
			case T__161:
			case T__162:
			case T__163:
			case T__164:
			case T__165:
			case T__166:
			case T__167:
				enterOuterAlt(_localctx, 2);
				{
				setState(79);
				controllerOpcode1();
				setState(80);
				value();
				}
				break;
			case T__168:
			case T__169:
			case T__170:
			case T__171:
			case T__172:
			case T__173:
			case T__174:
			case T__175:
			case T__176:
			case T__177:
			case T__178:
			case T__179:
			case T__180:
				enterOuterAlt(_localctx, 3);
				{
				setState(82);
				controllerOpcode2();
				setState(83);
				lb();
				}
				break;
			case T__181:
			case T__182:
			case T__183:
			case T__184:
			case T__185:
			case T__186:
			case T__187:
			case T__188:
			case T__189:
			case T__190:
				enterOuterAlt(_localctx, 4);
				{
				setState(85);
				controllerOpcode3();
				setState(86);
				lb();
				setState(87);
				match(COMMA);
				setState(88);
				value();
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
	public static class ArrayInstructionContext extends ParserRuleContext {
		public ArrayOpcode0Context arrayOpcode0() {
			return getRuleContext(ArrayOpcode0Context.class,0);
		}
		public ArrayOpcode1Context arrayOpcode1() {
			return getRuleContext(ArrayOpcode1Context.class,0);
		}
		public ValueContext value() {
			return getRuleContext(ValueContext.class,0);
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
		enterRule(_localctx, 8, RULE_arrayInstruction);
		try {
			setState(96);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case T__0:
			case T__1:
			case T__2:
			case T__3:
			case T__4:
			case T__5:
			case T__6:
			case T__7:
			case T__8:
			case T__9:
			case T__10:
			case T__11:
			case T__12:
			case T__13:
			case T__14:
			case T__15:
			case T__16:
			case T__17:
			case T__18:
			case T__19:
			case T__20:
			case T__21:
			case T__22:
			case T__23:
			case T__24:
			case T__25:
			case T__26:
			case T__27:
			case T__28:
			case T__29:
			case T__30:
			case T__31:
			case T__32:
			case T__33:
			case T__34:
			case T__35:
			case T__36:
			case T__37:
			case T__38:
			case T__39:
			case T__40:
			case T__41:
			case T__42:
			case T__43:
			case T__44:
			case T__45:
			case T__46:
			case T__47:
			case T__66:
			case T__69:
			case T__70:
			case T__71:
			case T__72:
			case T__73:
			case T__191:
			case T__192:
			case T__193:
			case T__194:
			case T__195:
			case T__196:
			case T__197:
			case T__198:
			case T__199:
			case T__200:
			case T__201:
			case T__202:
			case T__203:
			case T__204:
			case T__205:
			case T__206:
			case T__207:
			case T__208:
			case T__209:
			case T__210:
			case T__211:
			case T__212:
			case T__213:
			case T__214:
			case T__215:
			case T__216:
			case T__217:
			case T__218:
			case T__219:
			case T__220:
			case T__221:
			case T__222:
			case T__223:
			case T__224:
			case T__225:
			case T__226:
			case T__227:
			case T__228:
			case T__229:
			case T__230:
			case T__231:
				enterOuterAlt(_localctx, 1);
				{
				setState(92);
				arrayOpcode0();
				}
				break;
			case T__77:
			case T__78:
			case T__79:
			case T__80:
			case T__81:
			case T__82:
			case T__83:
			case T__84:
			case T__85:
			case T__86:
			case T__87:
			case T__88:
			case T__89:
			case T__90:
			case T__91:
			case T__92:
			case T__93:
			case T__94:
			case T__95:
			case T__96:
			case T__97:
			case T__98:
			case T__99:
			case T__100:
			case T__101:
			case T__102:
			case T__103:
			case T__104:
			case T__105:
			case T__106:
			case T__107:
			case T__108:
			case T__109:
			case T__110:
			case T__111:
			case T__112:
			case T__113:
			case T__114:
			case T__115:
			case T__116:
			case T__117:
			case T__118:
			case T__119:
			case T__120:
			case T__121:
			case T__122:
			case T__123:
			case T__124:
			case T__125:
			case T__126:
			case T__127:
			case T__128:
			case T__129:
			case T__130:
			case T__131:
			case T__132:
			case T__133:
			case T__134:
			case T__135:
			case T__136:
			case T__137:
			case T__138:
			case T__139:
			case T__140:
			case T__141:
			case T__142:
			case T__143:
			case T__144:
			case T__145:
			case T__146:
			case T__147:
			case T__148:
			case T__149:
			case T__150:
			case T__151:
			case T__159:
			case T__160:
			case T__161:
			case T__162:
			case T__164:
			case T__232:
			case T__233:
			case T__234:
			case T__235:
			case T__236:
			case T__237:
			case T__238:
			case T__239:
			case T__240:
				enterOuterAlt(_localctx, 2);
				{
				setState(93);
				arrayOpcode1();
				setState(94);
				value();
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
	public static class ControllerOpcode0Context extends ParserRuleContext {
		public ControllerOpcode0Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controllerOpcode0; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControllerOpcode0(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControllerOpcode0(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControllerOpcode0(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControllerOpcode0Context controllerOpcode0() throws RecognitionException {
		ControllerOpcode0Context _localctx = new ControllerOpcode0Context(_ctx, getState());
		enterRule(_localctx, 10, RULE_controllerOpcode0);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(98);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & -2L) != 0) || ((((_la - 64)) & ~0x3f) == 0 && ((1L << (_la - 64)) & 16383L) != 0)) ) {
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
	public static class ControllerOpcode1Context extends ParserRuleContext {
		public ControllerOpcode1Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controllerOpcode1; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControllerOpcode1(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControllerOpcode1(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControllerOpcode1(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControllerOpcode1Context controllerOpcode1() throws RecognitionException {
		ControllerOpcode1Context _localctx = new ControllerOpcode1Context(_ctx, getState());
		enterRule(_localctx, 12, RULE_controllerOpcode1);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(100);
			_la = _input.LA(1);
			if ( !(((((_la - 78)) & ~0x3f) == 0 && ((1L << (_la - 78)) & -1L) != 0) || ((((_la - 142)) & ~0x3f) == 0 && ((1L << (_la - 142)) & 134217727L) != 0)) ) {
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
	public static class ControllerOpcode2Context extends ParserRuleContext {
		public ControllerOpcode2Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controllerOpcode2; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControllerOpcode2(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControllerOpcode2(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControllerOpcode2(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControllerOpcode2Context controllerOpcode2() throws RecognitionException {
		ControllerOpcode2Context _localctx = new ControllerOpcode2Context(_ctx, getState());
		enterRule(_localctx, 14, RULE_controllerOpcode2);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(102);
			_la = _input.LA(1);
			if ( !(((((_la - 169)) & ~0x3f) == 0 && ((1L << (_la - 169)) & 8191L) != 0)) ) {
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
	public static class ControllerOpcode3Context extends ParserRuleContext {
		public ControllerOpcode3Context(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_controllerOpcode3; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterControllerOpcode3(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitControllerOpcode3(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitControllerOpcode3(this);
			else return visitor.visitChildren(this);
		}
	}

	public final ControllerOpcode3Context controllerOpcode3() throws RecognitionException {
		ControllerOpcode3Context _localctx = new ControllerOpcode3Context(_ctx, getState());
		enterRule(_localctx, 16, RULE_controllerOpcode3);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(104);
			_la = _input.LA(1);
			if ( !(((((_la - 182)) & ~0x3f) == 0 && ((1L << (_la - 182)) & 1023L) != 0)) ) {
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
		enterRule(_localctx, 18, RULE_arrayOpcode0);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(106);
			_la = _input.LA(1);
			if ( !((((_la) & ~0x3f) == 0 && ((1L << _la) & 562949953421310L) != 0) || ((((_la - 67)) & ~0x3f) == 0 && ((1L << (_la - 67)) & 249L) != 0) || ((((_la - 192)) & ~0x3f) == 0 && ((1L << (_la - 192)) & 2199023255551L) != 0)) ) {
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
		enterRule(_localctx, 20, RULE_arrayOpcode1);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(108);
			_la = _input.LA(1);
			if ( !(((((_la - 78)) & ~0x3f) == 0 && ((1L << (_la - 78)) & -1L) != 0) || ((((_la - 142)) & ~0x3f) == 0 && ((1L << (_la - 142)) & 12322815L) != 0) || ((((_la - 233)) & ~0x3f) == 0 && ((1L << (_la - 233)) & 511L) != 0)) ) {
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
		enterRule(_localctx, 22, RULE_label);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(110);
			lb();
			setState(112);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==T__241) {
				{
				setState(111);
				match(T__241);
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
		enterRule(_localctx, 24, RULE_directive);
		try {
			setState(119);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ARCHITECTUREID:
			case DATA:
				enterOuterAlt(_localctx, 1);
				{
				setState(114);
				assemblerdirective();
				}
				break;
			case DEFINE:
				enterOuterAlt(_localctx, 2);
				{
				setState(115);
				define();
				}
				break;
			case INCLUDE:
				enterOuterAlt(_localctx, 3);
				{
				setState(116);
				include();
				}
				break;
			case FUNC:
				enterOuterAlt(_localctx, 4);
				{
				setState(117);
				func();
				}
				break;
			case ENDFUNC:
				enterOuterAlt(_localctx, 5);
				{
				setState(118);
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
		enterRule(_localctx, 26, RULE_assemblerdirective);
		try {
			setState(123);
			_errHandler.sync(this);
			switch (_input.LA(1)) {
			case ARCHITECTUREID:
				enterOuterAlt(_localctx, 1);
				{
				setState(121);
				architectureId();
				}
				break;
			case DATA:
				enterOuterAlt(_localctx, 2);
				{
				setState(122);
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
		enterRule(_localctx, 28, RULE_architectureId);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(125);
			match(ARCHITECTUREID);
			setState(126);
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
		enterRule(_localctx, 30, RULE_data);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(128);
			match(DATA);
			setState(129);
			number();
			setState(130);
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
		enterRule(_localctx, 32, RULE_define);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(132);
			match(DEFINE);
			setState(133);
			name();
			setState(134);
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
		enterRule(_localctx, 34, RULE_expression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(136);
			multiplyingExpression();
			setState(141);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==SIGN) {
				{
				{
				setState(137);
				match(SIGN);
				setState(138);
				multiplyingExpression();
				}
				}
				setState(143);
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
		enterRule(_localctx, 36, RULE_multiplyingExpression);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(144);
			value();
			setState(149);
			_errHandler.sync(this);
			_la = _input.LA(1);
			while (_la==T__110 || ((((_la - 243)) & ~0x3f) == 0 && ((1L << (_la - 243)) & 7L) != 0)) {
				{
				{
				setState(145);
				_la = _input.LA(1);
				if ( !(_la==T__110 || ((((_la - 243)) & ~0x3f) == 0 && ((1L << (_la - 243)) & 7L) != 0)) ) {
				_errHandler.recoverInline(this);
				}
				else {
					if ( _input.LA(1)==Token.EOF ) matchedEOF = true;
					_errHandler.reportMatch(this);
					consume();
				}
				setState(146);
				value();
				}
				}
				setState(151);
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
		enterRule(_localctx, 38, RULE_value);
		try {
			setState(158);
			_errHandler.sync(this);
			switch ( getInterpreter().adaptivePredict(_input,12,_ctx) ) {
			case 1:
				enterOuterAlt(_localctx, 1);
				{
				setState(152);
				number();
				}
				break;
			case 2:
				enterOuterAlt(_localctx, 2);
				{
				setState(153);
				name();
				}
				break;
			case 3:
				enterOuterAlt(_localctx, 3);
				{
				setState(154);
				name();
				setState(155);
				match(COMMA);
				setState(156);
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
		enterRule(_localctx, 40, RULE_include);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(160);
			match(INCLUDE);
			setState(161);
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
		enterRule(_localctx, 42, RULE_func);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(163);
			match(FUNC);
			setState(164);
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
		enterRule(_localctx, 44, RULE_endfunc);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(166);
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
		enterRule(_localctx, 46, RULE_lb);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(168);
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
		enterRule(_localctx, 48, RULE_name);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(170);
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
		enterRule(_localctx, 50, RULE_number);
		int _la;
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(173);
			_errHandler.sync(this);
			_la = _input.LA(1);
			if (_la==SIGN) {
				{
				setState(172);
				match(SIGN);
				}
			}

			setState(175);
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
		enterRule(_localctx, 52, RULE_comment);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(177);
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

	@SuppressWarnings("CheckReturnValue")
	public static class OtherContext extends ParserRuleContext {
		public TerminalNode AsciiChars() { return getToken(AsmParser.AsciiChars, 0); }
		public OtherContext(ParserRuleContext parent, int invokingState) {
			super(parent, invokingState);
		}
		@Override public int getRuleIndex() { return RULE_other; }
		@Override
		public void enterRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).enterOther(this);
		}
		@Override
		public void exitRule(ParseTreeListener listener) {
			if ( listener instanceof AsmListener ) ((AsmListener)listener).exitOther(this);
		}
		@Override
		public <T> T accept(ParseTreeVisitor<? extends T> visitor) {
			if ( visitor instanceof AsmVisitor ) return ((AsmVisitor<? extends T>)visitor).visitOther(this);
			else return visitor.visitChildren(this);
		}
	}

	public final OtherContext other() throws RecognitionException {
		OtherContext _localctx = new OtherContext(_ctx, getState());
		enterRule(_localctx, 54, RULE_other);
		try {
			enterOuterAlt(_localctx, 1);
			{
			setState(179);
			match(AsciiChars);
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
		"\u0004\u0001\u0106\u00b6\u0002\u0000\u0007\u0000\u0002\u0001\u0007\u0001"+
		"\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004\u0007\u0004"+
		"\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007\u0007\u0007"+
		"\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b\u0007\u000b"+
		"\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002\u000f\u0007"+
		"\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002\u0012\u0007"+
		"\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002\u0015\u0007"+
		"\u0015\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002\u0018\u0007"+
		"\u0018\u0002\u0019\u0007\u0019\u0002\u001a\u0007\u001a\u0002\u001b\u0007"+
		"\u001b\u0001\u0000\u0003\u0000:\b\u0000\u0001\u0000\u0004\u0000=\b\u0000"+
		"\u000b\u0000\f\u0000>\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001"+
		"\u0003\u0001E\b\u0001\u0001\u0002\u0003\u0002H\b\u0002\u0001\u0002\u0001"+
		"\u0002\u0001\u0002\u0003\u0002M\b\u0002\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0001"+
		"\u0003\u0001\u0003\u0001\u0003\u0001\u0003\u0003\u0003[\b\u0003\u0001"+
		"\u0004\u0001\u0004\u0001\u0004\u0001\u0004\u0003\u0004a\b\u0004\u0001"+
		"\u0005\u0001\u0005\u0001\u0006\u0001\u0006\u0001\u0007\u0001\u0007\u0001"+
		"\b\u0001\b\u0001\t\u0001\t\u0001\n\u0001\n\u0001\u000b\u0001\u000b\u0003"+
		"\u000bq\b\u000b\u0001\f\u0001\f\u0001\f\u0001\f\u0001\f\u0003\fx\b\f\u0001"+
		"\r\u0001\r\u0003\r|\b\r\u0001\u000e\u0001\u000e\u0001\u000e\u0001\u000f"+
		"\u0001\u000f\u0001\u000f\u0001\u000f\u0001\u0010\u0001\u0010\u0001\u0010"+
		"\u0001\u0010\u0001\u0011\u0001\u0011\u0001\u0011\u0005\u0011\u008c\b\u0011"+
		"\n\u0011\f\u0011\u008f\t\u0011\u0001\u0012\u0001\u0012\u0001\u0012\u0005"+
		"\u0012\u0094\b\u0012\n\u0012\f\u0012\u0097\t\u0012\u0001\u0013\u0001\u0013"+
		"\u0001\u0013\u0001\u0013\u0001\u0013\u0001\u0013\u0003\u0013\u009f\b\u0013"+
		"\u0001\u0014\u0001\u0014\u0001\u0014\u0001\u0015\u0001\u0015\u0001\u0015"+
		"\u0001\u0016\u0001\u0016\u0001\u0017\u0001\u0017\u0001\u0018\u0001\u0018"+
		"\u0001\u0019\u0003\u0019\u00ae\b\u0019\u0001\u0019\u0001\u0019\u0001\u001a"+
		"\u0001\u001a\u0001\u001b\u0001\u001b\u0001\u001b\u0000\u0000\u001c\u0000"+
		"\u0002\u0004\u0006\b\n\f\u000e\u0010\u0012\u0014\u0016\u0018\u001a\u001c"+
		"\u001e \"$&(*,.0246\u0000\b\u0001\u0000\u0001M\u0001\u0000N\u00a8\u0001"+
		"\u0000\u00a9\u00b5\u0001\u0000\u00b6\u00bf\u0004\u0000\u00010CCFJ\u00c0"+
		"\u00e8\u0004\u0000N\u0098\u00a0\u00a3\u00a5\u00a5\u00e9\u00f1\u0002\u0000"+
		"oo\u00f3\u00f5\u0001\u0000\u0100\u0101\u00af\u0000<\u0001\u0000\u0000"+
		"\u0000\u0002D\u0001\u0000\u0000\u0000\u0004G\u0001\u0000\u0000\u0000\u0006"+
		"Z\u0001\u0000\u0000\u0000\b`\u0001\u0000\u0000\u0000\nb\u0001\u0000\u0000"+
		"\u0000\fd\u0001\u0000\u0000\u0000\u000ef\u0001\u0000\u0000\u0000\u0010"+
		"h\u0001\u0000\u0000\u0000\u0012j\u0001\u0000\u0000\u0000\u0014l\u0001"+
		"\u0000\u0000\u0000\u0016n\u0001\u0000\u0000\u0000\u0018w\u0001\u0000\u0000"+
		"\u0000\u001a{\u0001\u0000\u0000\u0000\u001c}\u0001\u0000\u0000\u0000\u001e"+
		"\u0080\u0001\u0000\u0000\u0000 \u0084\u0001\u0000\u0000\u0000\"\u0088"+
		"\u0001\u0000\u0000\u0000$\u0090\u0001\u0000\u0000\u0000&\u009e\u0001\u0000"+
		"\u0000\u0000(\u00a0\u0001\u0000\u0000\u0000*\u00a3\u0001\u0000\u0000\u0000"+
		",\u00a6\u0001\u0000\u0000\u0000.\u00a8\u0001\u0000\u0000\u00000\u00aa"+
		"\u0001\u0000\u0000\u00002\u00ad\u0001\u0000\u0000\u00004\u00b1\u0001\u0000"+
		"\u0000\u00006\u00b3\u0001\u0000\u0000\u00008:\u0003\u0002\u0001\u0000"+
		"98\u0001\u0000\u0000\u00009:\u0001\u0000\u0000\u0000:;\u0001\u0000\u0000"+
		"\u0000;=\u0005\u0104\u0000\u0000<9\u0001\u0000\u0000\u0000=>\u0001\u0000"+
		"\u0000\u0000><\u0001\u0000\u0000\u0000>?\u0001\u0000\u0000\u0000?\u0001"+
		"\u0001\u0000\u0000\u0000@E\u0003\u0018\f\u0000AE\u0003\u0016\u000b\u0000"+
		"BE\u0003\u0004\u0002\u0000CE\u00034\u001a\u0000D@\u0001\u0000\u0000\u0000"+
		"DA\u0001\u0000\u0000\u0000DB\u0001\u0000\u0000\u0000DC\u0001\u0000\u0000"+
		"\u0000E\u0003\u0001\u0000\u0000\u0000FH\u0003\u0016\u000b\u0000GF\u0001"+
		"\u0000\u0000\u0000GH\u0001\u0000\u0000\u0000HI\u0001\u0000\u0000\u0000"+
		"IJ\u0003\u0006\u0003\u0000JL\u0003\b\u0004\u0000KM\u00034\u001a\u0000"+
		"LK\u0001\u0000\u0000\u0000LM\u0001\u0000\u0000\u0000M\u0005\u0001\u0000"+
		"\u0000\u0000N[\u0003\n\u0005\u0000OP\u0003\f\u0006\u0000PQ\u0003&\u0013"+
		"\u0000Q[\u0001\u0000\u0000\u0000RS\u0003\u000e\u0007\u0000ST\u0003.\u0017"+
		"\u0000T[\u0001\u0000\u0000\u0000UV\u0003\u0010\b\u0000VW\u0003.\u0017"+
		"\u0000WX\u0005\u0105\u0000\u0000XY\u0003&\u0013\u0000Y[\u0001\u0000\u0000"+
		"\u0000ZN\u0001\u0000\u0000\u0000ZO\u0001\u0000\u0000\u0000ZR\u0001\u0000"+
		"\u0000\u0000ZU\u0001\u0000\u0000\u0000[\u0007\u0001\u0000\u0000\u0000"+
		"\\a\u0003\u0012\t\u0000]^\u0003\u0014\n\u0000^_\u0003&\u0013\u0000_a\u0001"+
		"\u0000\u0000\u0000`\\\u0001\u0000\u0000\u0000`]\u0001\u0000\u0000\u0000"+
		"a\t\u0001\u0000\u0000\u0000bc\u0007\u0000\u0000\u0000c\u000b\u0001\u0000"+
		"\u0000\u0000de\u0007\u0001\u0000\u0000e\r\u0001\u0000\u0000\u0000fg\u0007"+
		"\u0002\u0000\u0000g\u000f\u0001\u0000\u0000\u0000hi\u0007\u0003\u0000"+
		"\u0000i\u0011\u0001\u0000\u0000\u0000jk\u0007\u0004\u0000\u0000k\u0013"+
		"\u0001\u0000\u0000\u0000lm\u0007\u0005\u0000\u0000m\u0015\u0001\u0000"+
		"\u0000\u0000np\u0003.\u0017\u0000oq\u0005\u00f2\u0000\u0000po\u0001\u0000"+
		"\u0000\u0000pq\u0001\u0000\u0000\u0000q\u0017\u0001\u0000\u0000\u0000"+
		"rx\u0003\u001a\r\u0000sx\u0003 \u0010\u0000tx\u0003(\u0014\u0000ux\u0003"+
		"*\u0015\u0000vx\u0003,\u0016\u0000wr\u0001\u0000\u0000\u0000ws\u0001\u0000"+
		"\u0000\u0000wt\u0001\u0000\u0000\u0000wu\u0001\u0000\u0000\u0000wv\u0001"+
		"\u0000\u0000\u0000x\u0019\u0001\u0000\u0000\u0000y|\u0003\u001c\u000e"+
		"\u0000z|\u0003\u001e\u000f\u0000{y\u0001\u0000\u0000\u0000{z\u0001\u0000"+
		"\u0000\u0000|\u001b\u0001\u0000\u0000\u0000}~\u0005\u00f6\u0000\u0000"+
		"~\u007f\u00030\u0018\u0000\u007f\u001d\u0001\u0000\u0000\u0000\u0080\u0081"+
		"\u0005\u00f7\u0000\u0000\u0081\u0082\u00032\u0019\u0000\u0082\u0083\u0005"+
		"\u00f8\u0000\u0000\u0083\u001f\u0001\u0000\u0000\u0000\u0084\u0085\u0005"+
		"\u00fc\u0000\u0000\u0085\u0086\u00030\u0018\u0000\u0086\u0087\u0003\""+
		"\u0011\u0000\u0087!\u0001\u0000\u0000\u0000\u0088\u008d\u0003$\u0012\u0000"+
		"\u0089\u008a\u0005\u00fe\u0000\u0000\u008a\u008c\u0003$\u0012\u0000\u008b"+
		"\u0089\u0001\u0000\u0000\u0000\u008c\u008f\u0001\u0000\u0000\u0000\u008d"+
		"\u008b\u0001\u0000\u0000\u0000\u008d\u008e\u0001\u0000\u0000\u0000\u008e"+
		"#\u0001\u0000\u0000\u0000\u008f\u008d\u0001\u0000\u0000\u0000\u0090\u0095"+
		"\u0003&\u0013\u0000\u0091\u0092\u0007\u0006\u0000\u0000\u0092\u0094\u0003"+
		"&\u0013\u0000\u0093\u0091\u0001\u0000\u0000\u0000\u0094\u0097\u0001\u0000"+
		"\u0000\u0000\u0095\u0093\u0001\u0000\u0000\u0000\u0095\u0096\u0001\u0000"+
		"\u0000\u0000\u0096%\u0001\u0000\u0000\u0000\u0097\u0095\u0001\u0000\u0000"+
		"\u0000\u0098\u009f\u00032\u0019\u0000\u0099\u009f\u00030\u0018\u0000\u009a"+
		"\u009b\u00030\u0018\u0000\u009b\u009c\u0005\u0105\u0000\u0000\u009c\u009d"+
		"\u00032\u0019\u0000\u009d\u009f\u0001\u0000\u0000\u0000\u009e\u0098\u0001"+
		"\u0000\u0000\u0000\u009e\u0099\u0001\u0000\u0000\u0000\u009e\u009a\u0001"+
		"\u0000\u0000\u0000\u009f\'\u0001\u0000\u0000\u0000\u00a0\u00a1\u0005\u00f9"+
		"\u0000\u0000\u00a1\u00a2\u0005\u00f8\u0000\u0000\u00a2)\u0001\u0000\u0000"+
		"\u0000\u00a3\u00a4\u0005\u00fa\u0000\u0000\u00a4\u00a5\u00030\u0018\u0000"+
		"\u00a5+\u0001\u0000\u0000\u0000\u00a6\u00a7\u0005\u00fb\u0000\u0000\u00a7"+
		"-\u0001\u0000\u0000\u0000\u00a8\u00a9\u00030\u0018\u0000\u00a9/\u0001"+
		"\u0000\u0000\u0000\u00aa\u00ab\u0005\u00ff\u0000\u0000\u00ab1\u0001\u0000"+
		"\u0000\u0000\u00ac\u00ae\u0005\u00fe\u0000\u0000\u00ad\u00ac\u0001\u0000"+
		"\u0000\u0000\u00ad\u00ae\u0001\u0000\u0000\u0000\u00ae\u00af\u0001\u0000"+
		"\u0000\u0000\u00af\u00b0\u0007\u0007\u0000\u0000\u00b03\u0001\u0000\u0000"+
		"\u0000\u00b1\u00b2\u0005\u0106\u0000\u0000\u00b25\u0001\u0000\u0000\u0000"+
		"\u00b3\u00b4\u0005\u00fd\u0000\u0000\u00b47\u0001\u0000\u0000\u0000\u000e"+
		"9>DGLZ`pw{\u008d\u0095\u009e\u00ad";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}