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
import javax.script.*;

//import com.formdev.flatlaf.FlatLightLaf;
import org.fife.ui.rtextarea.*;
import org.fife.ui.rsyntaxtextarea.*;
import org.apache.logging.log4j.*;

import ai.onnxruntime.*;

import com.mxgraph.layout.mxCircleLayout;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.view.mxGraph;

import org.graalvm.polyglot.Context;
import org.graalvm.polyglot.Value;
import javax.script.ScriptEngineManager;
import javax.script.ScriptEngine;
import javax.script.Invocable;
import java.io.IOException;
import org.graalvm.polyglot.Source;


import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.debug.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.hpp.*;
import xpu.sw.tools.sdk.common.fileformats.hex.*;
import xpu.sw.tools.sdk.common.fileformats.py.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;
import xpu.sw.tools.sdk.common.project.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.rexec.*;
import xpu.sw.tools.sdk.gui.components.common.*;


//-------------------------------------------------------------------------------------
public class OnnxViewer extends GuiPanel implements KeyListener, MouseWheelListener{
    private Project project;
    private EditorByProject editorByProject;
    private File file;

    private OrtSession session;


    private Path path;
    private XpuFile xpuFile;

    private EditorTabDebugInformation editorTabDebugInformation;


    public static final int WARMUP = 30;
    public static final int ITERATIONS = 10;
    public static final String BENCHFILE = "src/bench.js";

    public static final String SOURCE = ""
            + "var N = 2000;\n"
            + "var EXPECTED = 17393;\n"
            + "\n"
            + "function Natural() {\n"
            + "    x = 2;\n"
            + "    return {\n"
            + "        'next' : function() { return x++; }\n"
            + "    };\n"
            + "}\n"
            + "\n"
            + "function Filter(number, filter) {\n"
            + "    var self = this;\n"
            + "    this.number = number;\n"
            + "    this.filter = filter;\n"
            + "    this.accept = function(n) {\n"
            + "      var filter = self;\n"
            + "      for (;;) {\n"
            + "          if (n % filter.number === 0) {\n"
            + "              return false;\n"
            + "          }\n"
            + "          filter = filter.filter;\n"
            + "          if (filter === null) {\n"
            + "              break;\n"
            + "          }\n"
            + "      }\n"
            + "      return true;\n"
            + "    };\n"
            + "    return this;\n"
            + "}\n"
            + "\n"
            + "function Primes(natural) {\n"
            + "    var self = this;\n"
            + "    this.natural = natural;\n"
            + "    this.filter = null;\n"
            + "\n"
            + "    this.next = function() {\n"
            + "        for (;;) {\n"
            + "            var n = self.natural.next();\n"
            + "            if (self.filter === null || self.filter.accept(n)) {\n"
            + "                self.filter = new Filter(n, self.filter);\n"
            + "                return n;\n"
            + "            }\n"
            + "        }\n"
            + "    };\n"
            + "}\n"
            + "\n"
            + "function primesMain() {\n"
            + "    var primes = new Primes(Natural());\n"
            + "    var primArray = [];\n"
            + "    for (var i=0;i<=N;i++) { primArray.push(primes.next()); }\n"
            + "    if (primArray[N] != EXPECTED) { throw new Error('wrong prime found: '+primArray[N]); }\n"
            + "}\n";

//-------------------------------------------------------------------------------------
    public OnnxViewer(Gui _gui, xpu.sw.tools.sdk.common.context.Context _context, Project _project, EditorByProject _editorByProject, File _file, String _themeName) {
        super(_context, _gui);
        project = _project;
        editorByProject = _editorByProject;
        file = _file;

        path = file.toPath();
        xpuFile = XpuFile.loadFrom(context, file.toString());
        if(xpuFile == null){
            log.warn("Cannot find: " + path);
        } else {
            editorTabDebugInformation = new EditorTabDebugInformation(gui, context, _project, xpuFile);
        }
        init();
        setVisible(false);
//        setTheme(_themeName);
        refresh();        
    }

//-------------------------------------------------------------------------------------
    private void init(){
        setLayout(new BorderLayout());
        try{
            OrtEnvironment _env = OrtEnvironment.getEnvironment();
            session = _env.createSession(path.toString(), new OrtSession.SessionOptions());
        } catch(OrtException _e){
            _e.printStackTrace();
        }

        // Initialize graph
        mxGraph graph = new mxGraph();
        Object parent = graph.getDefaultParent();

        // Define nodes and edges
        graph.getModel().beginUpdate();
        try {
            Object v1 = graph.insertVertex(parent, null, "Node 1", 100, 100, 80, 30);
            Object v2 = graph.insertVertex(parent, null, "Node 2", 240, 100, 80, 30);
            Object v3 = graph.insertVertex(parent, null, "Node 3", 380, 100, 80, 30);
            Object v4 = graph.insertVertex(parent, null, "Node 4", 170, 200, 80, 30);
            Object v5 = graph.insertVertex(parent, null, "Node 5", 310, 200, 80, 30);

            // Create edges between nodes
            graph.insertEdge(parent, null, "", v1, v2);
            graph.insertEdge(parent, null, "", v2, v3);
            graph.insertEdge(parent, null, "", v3, v4);
            graph.insertEdge(parent, null, "", v4, v5);
            graph.insertEdge(parent, null, "", v5, v1);
        } finally {
            graph.getModel().endUpdate();
        }

        log.debug("Engines:");
java.util.List<ScriptEngineFactory> engines = new ScriptEngineManager().getEngineFactories();
for (ScriptEngineFactory f : engines) {
    System.out.println(f.getLanguageName() + " " + f.getEngineName() + " " + f.getNames());
}

/*
        System.out.println("=== Graal.js via org.graalvm.polyglot.Context === ");
        long sum = 0;
        try (Context context = Context.create()) {
            context.eval(Source.newBuilder("js", SOURCE, "src.js").build());
            Value primesMain = context.getBindings("js").getMember("primesMain");
            System.out.println("warming up ...");
            for (int i = 0; i < WARMUP; i++) {
                primesMain.execute();
            }
            System.out.println("warmup finished, now measuring");
            for (int i = 0; i < ITERATIONS; i++) {
                long start = System.currentTimeMillis();
                primesMain.execute();
                long took = System.currentTimeMillis() - start;
                sum += took;
                System.out.println("iteration: " + took);
            }
        }catch(Throwable _t){
            _t.printStackTrace();
        }*/

/*
        // Layout for organizing the nodes
        mxCircleLayout layout = new mxCircleLayout(graph);
        layout.execute(parent);

        // Create the graph component for Swing
        mxGraphComponent graphComponent = new mxGraphComponent(graph);
//        add(new JButton("OK0"));
//        add(graphComponent);
        JButton button = new JButton("Execute JavaScript");

        // Create a JavaScript execution context
        org.graalvm.polyglot.Context context = org.graalvm.polyglot.Context.create();

        // JavaScript code to execute
        String jsCode = "function sayHello() { return 'Hello from JavaScript!'; }";

        // Load the JavaScript code
        context.eval("js", jsCode);

        // Action listener to execute JavaScript function
        button.addActionListener(e -> {
            // Call JavaScript function directly
            Value sayHelloFunction = context.getBindings("js").getMember("sayHello");
            String result = sayHelloFunction.execute().asString();
            JOptionPane.showMessageDialog(gui, result);
        });

        add(button);
//        add(new JButton("OK1"));*/
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
    public void keyPressed(KeyEvent e) {
    }

//-------------------------------------------------------------------------------------
    public void keyTyped(KeyEvent _e) {
//        if(((KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, java.awt.event.InputEvent.ALT_DOWN_MASK)) != null)  &&  _e.getKeyCode() == KeyEvent.VK_F4){
//        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

//-------------------------------------------------------------------------------------
    public void keyReleased(KeyEvent _e) {

    }

//-------------------------------------------------------------------------------------
    public void mouseWheelMoved(MouseWheelEvent mouseWheelEvent) {

    }

//-------------------------------------------------------------------------------------
    public void refresh(){

    }

//-------------------------------------------------------------------------------------
    public void reload(){
//        log.warn("Reload [" + file.toString() + "]...");
        try{
            xpuFile = XpuFile.loadFrom(context, file.toString());
            refresh();
        } catch(Throwable _e) {
            log.error("Cannot reload: " + xpuFile.getPath());
            return;
        }
    }

//-------------------------------------------------------------------------------------
    public void save(){

    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
