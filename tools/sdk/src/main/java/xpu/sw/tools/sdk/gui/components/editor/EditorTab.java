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
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.hpp.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.onnx.*;
import xpu.sw.tools.sdk.common.fileformats.py.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.rexec.*;
import xpu.sw.tools.sdk.gui.components.common.*;


//-------------------------------------------------------------------------------------
public class EditorTab extends GuiPanel implements KeyListener, MouseWheelListener{
    private Project project;
    private EditorByProject editorByProject;
    private File file;
    private String themeName;


    private Path path;
    private XpuFile xpuFile;
    private RSyntaxTextArea textArea;
    private RTextScrollPane sp;
    private ImageIcon trackPointerIcon;
    private ImageIcon bookmarkPointerIcon;

    private EditorTabDebugInformation editorTabDebugInformation;

//-------------------------------------------------------------------------------------
    public EditorTab(Gui _gui, Context _context, Project _project, EditorByProject _editorByProject, File _file, String _themeName) {
        super(_context, _gui);
        project = _project;
        editorByProject = _editorByProject;
        file = _file;
        themeName = _themeName;

        path = file.toPath();
        xpuFile = XpuFile.loadFrom(context, file.toString());
        if(xpuFile == null){
            log.warn("Cannot find: " + path);
        } else {
            editorTabDebugInformation = new EditorTabDebugInformation(gui, context, _project, xpuFile);
        }
        init();
        setVisible(false);
        setTheme(_themeName);
        refresh();        
    }

//-------------------------------------------------------------------------------------
    private void init(){
        setLayout(new BorderLayout());
        String[] _arrayExt = file.getName().split("\\.");
        if(_arrayExt.length < 2){
            log.debug("Cannot establish file type for: " + file.getAbsolutePath());
            return;
        }
        String _ext = _arrayExt[_arrayExt.length - 1];
        if(_ext.equals(OnnxFile.EXTENSION)){
//            add(new JButton("OK_0"));
            add(new OnnxViewer(gui, context, project, editorByProject, file, themeName));
//            add(new JButton("OK_1"));
        } else {
            textArea = new RSyntaxTextArea(70, 100);
            setSyntaxEditingStyle(_ext);

            textArea.setCodeFoldingEnabled(true);
            try{
                textArea.setText(xpuFile.getText());
            } catch(Throwable _e) {
                log.error("Cannot read: " + xpuFile.getPath());
                return;
            }

            CustomIconRowHeader _customIconRowHeader = new CustomIconRowHeader(context, textArea, editorTabDebugInformation);
            sp = new RTextScrollPane(textArea);
            sp.setFoldIndicatorEnabled(true);
            sp.setLineNumbersEnabled(true);
            sp.setIconRowHeaderEnabled(true);
    //        sp.setRowHeaderView(_customIconRowHeader);        
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
    }

//-------------------------------------------------------------------------------------
    public File getFile(){
        return file;
    }

//-------------------------------------------------------------------------------------
    public Path getPath(){
        return path;
    }

//-------------------------------------------------------------------------------------
    public EditorTabDebugInformation getEditorTabDebugInformation(){
        return editorTabDebugInformation;
    }

//-------------------------------------------------------------------------------------
    private void setSyntaxEditingStyle(String _ext){
//        setSyntaxEditingStyle();
        switch(_ext){
            case AsmFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_ASSEMBLER_X86);                
                break;
            }
            case "c" : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_C);                
                break;
            }
            case CppFile.EXTENSION : 
            case HppFile.EXTENSION : {
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
            case HexFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_JSON_WITH_COMMENTS);
                break;
            }
            case PyFile.EXTENSION : {
                textArea.setSyntaxEditingStyle(SyntaxConstants.SYNTAX_STYLE_PYTHON);
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
        if((_themeName != null) && (!_themeName.isEmpty()) && (textArea != null)){
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
        } else */if(_e.isControlDown() && _e.getKeyCode() == KeyEvent.VK_S) {
            save();
        } else if(_e.getKeyCode() == KeyEvent.VK_F5) {
                if(editorTabDebugInformation.isEligibleForDebug()){
                    int _lineNo = textArea.getCaretLineNumber();
                    if(editorTabDebugInformation.isEligibleForDebug(_lineNo)){
//                        boolean _alreadyBooked = sp.getGutter().toggleBookmark(_lineNo);
                        editorTabDebugInformation.toggleBreakpoint(_lineNo);
        //                log.debug("Set breakpoint @ line: " + (_lineNo +1)+ "[" + _alreadyBooked + "]");
//                        log.debug("getBookmarkIcon:" + sp.getGutter().getBookmarkIcon());
        //                GutterIconInfo _info = sp.getGutter().addLineTrackingIcon(_lineNo + 1, debugPointerIcon);
                        editorByProject.refresh();
                        _e.consume();
                    }
                }
        } else if(_e.getKeyCode() == KeyEvent.VK_F6) {//STEP_BY_STEP
                if(editorTabDebugInformation.isEligibleForDebug()){
                    int _lineNo = textArea.getCaretLineNumber();
                    if(editorTabDebugInformation.isEligibleForDebug(_lineNo)){
//                        boolean _alreadyBooked = sp.getGutter().toggleBookmark(_lineNo);
//                        editorTabDebugInformation.toggleBreakpoint(_lineNo);
        //                log.debug("Set breakpoint @ line: " + (_lineNo +1)+ "[" + _alreadyBooked + "]");
//                        log.debug("getBookmarkIcon:" + sp.getGutter().getBookmarkIcon());
        //                GutterIconInfo _info = sp.getGutter().addLineTrackingIcon(_lineNo + 1, debugPointerIcon);
//                        refresh();
                        _e.consume();
                    }
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
        refresh();
    }

//-------------------------------------------------------------------------------------
    public void refresh(){
//        gui.getDebugMode();
        editorTabDebugInformation.refresh();
        if(sp == null){
            return;
        }
        switch(context.getDebugMode()){
            case Context.DEBUG_MODE_OFF: {
                sp.setIconRowHeaderEnabled(false);
                break;
            }
            case Context.DEBUG_MODE_ON: {
                if(editorTabDebugInformation.isEligibleForDebug()){
                    try {
                        sp.getGutter().removeAllTrackingIcons();
                        int _executionLineNo = editorTabDebugInformation.getCurrentExecutionLineNo();
//                        log.debug("Debug move PC to [path=" + path.toString()+ "][line=" + _executionLineNo +"]...");
                        if(_executionLineNo >= 0){
                            sp.getGutter().addLineTrackingIcon(_executionLineNo, trackPointerIcon);
                            java.util.List<BreakpointInformation> _breakpointInformations = editorTabDebugInformation.getBreakpointInformations();
//                            log.debug("Debug BreakpointInformation.size:" + _breakpointInformations.size());
                            for(int i = 0; i < _breakpointInformations.size(); i++){
                                BreakpointInformation _breakpointInformation = _breakpointInformations.get(i);
//                                log.debug("Debug BreakpointInformation["+i+"]:" + _breakpointInformation.toString() + "\n");
                                if(_breakpointInformation.isEnabled()){
                                    int _programCounter = _breakpointInformation.getPc();
                                    int _lineNo = editorTabDebugInformation.getLineNoByPc(_programCounter);
//                                    log.debug("Debug BreakpointInformation["+i+"]:" + _breakpointInformation.toString() + ", _lineNo="+_lineNo);
                                    sp.getGutter().addLineTrackingIcon(_lineNo - 1, bookmarkPointerIcon);
                                }
                            }
                        }
                    } catch(BadLocationException _e1){
                        log.error("BadLocationException: " + _e1.getMessage());
//                        _e1.printStackTrace();
                    }                    
                } else {
                    sp.setIconRowHeaderEnabled(false);
                }
                break;
            }
            default: {
                log.error("Unknown debug mode: " + context.getDebugMode());
                break;
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void reload(){
//        log.warn("Reload [" + file.toString() + "]...");
        try{
            xpuFile = XpuFile.loadFrom(context, file.toString());
            textArea.setText(xpuFile.getText());
            refresh();
        } catch(Throwable _e) {
            log.error("Cannot reload: " + xpuFile.getPath());
            return;
        }
    }

//-------------------------------------------------------------------------------------
    public void save(){
        String _text = textArea.getText();
        log.debug("Saving [" + file.toPath() + "]...");
        try{
//            log.debug("Saving [" + file.toPath() + "]..." + _text);
//            Files.write(file.toPath(), _text.getBytes(), StandardOpenOption.WRITE);
            Writer _writer = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file.getPath()), "utf-8"));
            _writer.write(_text);
            _writer.flush();
            _writer.close();
        } catch(IOException _e){
            log.debug("Cannot write: " + file.toPath() + ": " + _e.getMessage());
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
