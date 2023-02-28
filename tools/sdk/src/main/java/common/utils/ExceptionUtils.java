//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------
import java.util.*;

import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public class ExceptionUtils {
	private Logger log;

//-------------------------------------------------------------------------------------
    public ExceptionUtils(Logger _log) {
    	log = _log;
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
}
//-------------------------------------------------------------------------------------

  