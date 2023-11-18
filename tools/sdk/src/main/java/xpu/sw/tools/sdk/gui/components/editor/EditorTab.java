//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.editor;

//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.nio.charset.*;
import java.nio.file.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.imageio.*;

//import com.formdev.flatlaf.FlatLightLaf;
import org.fife.ui.rtextarea.*;
import org.fife.ui.rsyntaxtextarea.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;


//-------------------------------------------------------------------------------------
public class EditorTab extends GuiPanel implements KeyListener, MouseWheelListener{
    private File file;


    private Path path;
    private XpuFile xpuFile;
    private RSyntaxTextArea textArea;
    private RTextScrollPane sp;
    private ImageIcon trackPointerIcon;
    private ImageIcon bookmarkPointerIcon;

    private EditorTabDebugInformation editorTabDebugInformation;

//-------------------------------------------------------------------------------------
    public EditorTab(Gui _gui, Context _context, File _file, String _themeName) {
        super(_context, _gui);
        file = _file;

        path = file.toPath();
        xpuFile = XpuFile.loadFrom(context, file.toString());
        editorTabDebugInformation = new EditorTabDebugInformation(gui, context, xpuFile);
        init();
        setVisible(false);
        setTheme(_themeName);
    }

//-------------------------------------------------------------------------------------
    private void init(){
        setLayout(new BorderLayout());
        textArea = new RSyntaxTextArea(70, 100);
        setSyntaxEditingStyle();

        textArea.setCodeFoldingEnabled(true);
        try{
            textArea.setText(xpuFile.getText());
        } catch(Throwable _e) {
            log.error("Cannot read: " + xpuFile.getPath());
            return;
        }

        sp = new RTextScrollPane(textArea);
        sp.setFoldIndicatorEnabled(true);
        sp.setLineNumbersEnabled(true);
        sp.setIconRowHeaderEnabled(true);
        sp.getGutter().setBookmarkingEnabled(true);
//        debugPointerIcon = new ImageIcon("resources/editor/debug/debug_arrow_000.png");

//            InputStream _stream0 = getClass().getResourceAsStream("/editor/debug/track_arrow_002.png");
            trackPointerIcon = gui.getServices().getUtils().getIconFromResources("editor/debug/track_arrow_002.png");
//            InputStream _stream1 = getClass().getResourceAsStream("/editor/debug/red_circle_0.png");
        bookmarkPointerIcon = gui.getServices().getUtils().getIconFromResources("editor/debug/red_circle_0.png");
        sp.getGutter().setBookmarkIcon(bookmarkPointerIcon);
        textArea.addKeyListener(this);
        textArea.addMouseWheelListener(this);
        add(sp);
    }

//-------------------------------------------------------------------------------------
    public File getFile(){
        return file;
    }

//-------------------------------------------------------------------------------------
    public Path getPath(){
        return path;
    }
/*
//-------------------------------------------------------------------------------------
    public String getPath(){
        return file.getAbsolutePath();
    }
*/
//-------------------------------------------------------------------------------------
    private void setSyntaxEditingStyle(){
//        setSyntaxEditingStyle();
        String[] _arrayExt = file.getName().split("\\.");
        if(_arrayExt.length < 2){
            log.debug("Cannot establish file type for: " + file.getAbsolutePath());
            return;
        }
        String _ext = _arrayExt[_arrayExt.length - 1];
        switch(_ext){
            case AsmFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_ASSEMBLER_X86);                
                break;
            }
            case "c" : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_C);                
                break;
            }
            case CppFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_CPLUSPLUS);                
                break;
            }
            case "java" : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_JAVA);                
                break;
            }
            case JsonFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_JSON_WITH_COMMENTS);                
                break;
            }
            case XpuprjFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_JSON_WITH_COMMENTS);                
                break;
            }
            case "hex" : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_JSON_WITH_COMMENTS);                
                break;
            }
            default:{
                log.debug("Unknown syntax for file type: " + _ext);
                break;
            }

        }
    }

//-------------------------------------------------------------------------------------
    public void setTheme(String _themeName){
        if((_themeName != null) && (!_themeName.isEmpty())){
            try {
                String _path = "/editor/themes/" + _themeName.toLowerCase() + ".xml";
//                log.debug("Load theme [" + _path + "]...");
                Theme _theme = Theme.load(getClass().getResourceAsStream(_path));
                _theme.apply(textArea);
            } catch (IOException _e) {
//                _e.printStackTrace();
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void keyPressed(KeyEvent e) {
    }

//-------------------------------------------------------------------------------------
    public void keyTyped(KeyEvent _e) {
//        if(((KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, java.awt.event.InputEvent.ALT_DOWN_MASK)) != null)  &&  _e.getKeyCode() == KeyEvent.VK_F4){
//        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

//-------------------------------------------------------------------------------------
    public void keyReleased(KeyEvent _e) {
//        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
/*        if(_e.getKeyCode() == KeyEvent.VK_F4) {
            try {
                if(editorTabDebugInformation.isEligibleForDebug()){
                    int _lineNo = textArea.getCaretLineNumber();
    //                boolean _alreadyBooked = sp.getGutter().toggleBookmark(_lineNo);
    //                log.debug("Set breakpoint @ line: " + (_lineNo +1)+ "[" + _alreadyBooked + "]");
    //                log.debug("getBookmarkIcon:" + sp.getGutter().getBookmarkIcon());
                    GutterIconInfo _info = sp.getGutter().addLineTrackingIcon(_lineNo, trackPointerIcon);
                    _e.consume();                    
                }
            } catch(BadLocationException _e1){
                log.error("BadLocationException: " + _e1.getMessage());
            }
        } else */if(_e.getKeyCode() == KeyEvent.VK_F5) {
            try {
                if(editorTabDebugInformation.isEligibleForDebug()){
                    int _lineNo = textArea.getCaretLineNumber();
                    if(editorTabDebugInformation.toggleBookmarkAtLine(_lineNo)){
                        boolean _alreadyBooked = sp.getGutter().toggleBookmark(_lineNo);
        //                log.debug("Set breakpoint @ line: " + (_lineNo +1)+ "[" + _alreadyBooked + "]");
                        log.debug("getBookmarkIcon:" + sp.getGutter().getBookmarkIcon());
        //                GutterIconInfo _info = sp.getGutter().addLineTrackingIcon(_lineNo + 1, debugPointerIcon);
                        _e.consume();
                    }
                }
            } catch(BadLocationException _e1){
                log.error("BadLocationException: " + _e1.getMessage());
            }
        } else if(_e.isControlDown() && 
                ((_e.getKeyCode() == KeyEvent.VK_PLUS) || (_e.getKeyCode() == KeyEvent.VK_UP))) {
            Font font = textArea.getFont();
            int fontSize = font.getSize();
            fontSize += 1;
            Font newFont = new Font(font.getFontName(), font.getStyle(), fontSize);
            textArea.setFont(newFont);
        } else if(_e.isControlDown() && 
                ((_e.getKeyCode() == KeyEvent.VK_MINUS) || (_e.getKeyCode() == KeyEvent.VK_DOWN))) {
            Font font = textArea.getFont();
            int fontSize = font.getSize();
            fontSize -= 1;
            Font newFont = new Font(font.getFontName(), font.getStyle(), fontSize);
            textArea.setFont(newFont);
        }
    }

//-------------------------------------------------------------------------------------
    @Override
    public void mouseWheelMoved(MouseWheelEvent mouseWheelEvent) {
        if (mouseWheelEvent.isControlDown())
        {
            int scrolled = mouseWheelEvent.getScrollAmount();
            int direction = mouseWheelEvent.getWheelRotation();
            Font font = textArea.getFont();
            int fontSize = font.getSize();
            fontSize += -(scrolled * direction / 3);
            Font newFont = new Font(font.getFontName(), font.getStyle(), fontSize);
            textArea.setFont(newFont);
        } else {
            textArea.getParent().dispatchEvent(mouseWheelEvent);
        }
    }

//-------------------------------------------------------------------------------------
    public void refresh(){
//        gui.getDebugMode();

    }

//-------------------------------------------------------------------------------------
    public void save(){
        String _text = textArea.getText();
        log.debug("Saving [" + file.toPath() + "]...");
        try{
//            log.debug("Saving [" + file.toPath() + "]..." + _text);
            Files.write(file.toPath(), _text.getBytes(), StandardOpenOption.WRITE);
        } catch(IOException _e){
            log.debug("Cannot write: " + file.toPath() + ": " + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
