//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.terminal;

//-------------------------------------------------------------------------------------
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;
import org.apache.logging.log4j.core.*;
import org.apache.logging.log4j.core.appender.*;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import org.antlr.v4.runtime.dfa.*;
import org.antlr.v4.runtime.atn.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.buttons.*;

//-------------------------------------------------------------------------------------
/*8import javax.swing.JTextArea;
import org.apache.log4j.AppenderSkeleton;
import org.apache.log4j.Level;
import org.apache.log4j.spi.LoggingEvent;*/


public class ConsoleAppender extends AbstractAppender implements ANTLRErrorListener {
    private JTextArea jTextArea;
    private JScrollPane jScrollPane;

//-------------------------------------------------------------------------------------
    public ConsoleAppender(JTextArea _jTextArea, JScrollPane _jScrollPane) {
        super("???", null, null, true, null);
        jTextArea = _jTextArea;
        jScrollPane = _jScrollPane;
        jScrollPane.getVerticalScrollBar().addAdjustmentListener(new AdjustmentListener() {  
            public void adjustmentValueChanged(AdjustmentEvent e) {  
                e.getAdjustable().setValue(e.getAdjustable().getMaximum());  
            }
        });        
    }

//-------------------------------------------------------------------------------------
    public void append(LogEvent event) {
        jTextArea.append(event.getMessage().getFormattedMessage() + "\n");
        jTextArea.setCaretPosition(jTextArea.getDocument().getLength());            
/*
        String _message = event.getMessage().getFormattedMessage() + "\n";
        Runnable  runnable = new Runnable() {
            public void run(){
                jTextArea.append(_message);
                jTextArea.setCaretPosition(jTextArea.getDocument().getLength());
            }
        };
        SwingUtilities.invokeLater(runnable);
*/        
    }

//-------------------------------------------------------------------------------------
    public void close() {
    }

//-------------------------------------------------------------------------------------
    public boolean requiresLayout() {
        return false;
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    public void syntaxError(Recognizer<?,?> recognizer,
               Object offendingSymbol,
               int _line,
               int _charPositionInLine,
               String _msg,
               RecognitionException e) {

            jTextArea.append(">>> " +_line + ":" + _charPositionInLine + ": " + _msg + "\n");
            jTextArea.setCaretPosition(jTextArea.getDocument().getLength());            

    }

//-------------------------------------------------------------------------------------
    public void reportAmbiguity(Parser recognizer,
                   DFA dfa,
                   int startIndex,
                   int stopIndex,
                   boolean exact,
                   BitSet ambigAlts,
                   ATNConfigSet configs) {

    }

//-------------------------------------------------------------------------------------
    public void reportAttemptingFullContext(Parser recognizer,
                               DFA dfa,
                               int startIndex,
                               int stopIndex,
                               BitSet conflictingAlts,
                               ATNConfigSet configs) {

    }

//-------------------------------------------------------------------------------------
    public void reportContextSensitivity(Parser recognizer,
                            DFA dfa,
                            int startIndex,
                            int stopIndex,
                            int prediction,
                            ATNConfigSet configs) {

    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
