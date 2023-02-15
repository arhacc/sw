//-------------------------------------------------------------------------------------
package codex.common.utils;
//-------------------------------------------------------------------------------------
import java.util.*;

import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class ExceptionUtils {
	private Debug debug;
	private Logger log;

//-------------------------------------------------------------------------------------
    public ExceptionUtils(Debug _debug) {
    	debug = _debug;
    }

//-------------------------------------------------------------------------------------
    public ExceptionUtils(Logger _log) {
    	log = _log;
    }

//-------------------------------------------------------------------------------------
    public void printException(Object _source, Throwable _t) {
    	printExceptionDebug(_source, _t);
    }

//-------------------------------------------------------------------------------------
    public void printExceptionDebug(Object _source, Throwable _t) {
		debug.outln(Debug.ERROR, "onError: " + "[" + _source + "]: " + _t.getMessage());
		StackTraceElement[]	_stack = _t.getStackTrace();
		for (int i = 0; i < _stack.length; i++) {
			debug.outln(Debug.ERROR, "S[" + i + "]:" + _stack[i].toString());
		}
    }

//-------------------------------------------------------------------------------------
    public void printExceptionLogger(Object _source, Throwable _t) {
		log.error("onError: " + "[" + _source + "]: " + _t.getMessage());
		StackTraceElement[]	_stack = _t.getStackTrace();
		for (int i = 0; i < _stack.length; i++) {
			log.error("S[" + i + "]:" + _stack[i].toString());
		}
    }

//-------------------------------------------------------------------------------------
    public static void printException(Debug _debug, Object _source, Throwable _t) {
    	(new ExceptionUtils(_debug)).printExceptionDebug(_source, _t);
    }

//-------------------------------------------------------------------------------------
    public static void printException(Logger _log, Object _source, Throwable _t) {
    	(new ExceptionUtils(_log)).printExceptionLogger(_source, _t);
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------

  