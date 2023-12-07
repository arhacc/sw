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
public class CustomIconRowHeader extends IconRowHeader {


    // Add your logic to determine whether to show the bookmark icon for a line
    private boolean shouldShowBookmarkIcon(int line) {
        // Add your custom logic here
        return line % 2 == 0; // For example, show bookmark icon for even lines
    }

    @Override
    protected void paintComponent(Graphics g) {
        int lineHeight = textArea.getFontMetrics(textArea.getFont()).getHeight();
        int firstLine = 1;
        int lastLine = 20;
        for (int line = firstLine; line <= lastLine; line++) {
            if (shouldShowBookmarkIcon(line)) {
                int y = (line - firstLine) * lineHeight;
                paintBookmarkIcon(g, y);
            }
        }
        super.paintComponent(g);
    }

    // Add your custom painting logic for the bookmark icon
    private void paintBookmarkIcon(Graphics g, int y) {
        // Add your custom painting logic here
        g.setColor(Color.RED);
        g.fillRect(2, y, 10, 10);
    }

    // Constructor
    public CustomIconRowHeader(Context _context, RSyntaxTextArea _rSyntaxTextArea, EditorTabDebugInformation _editorTabDebugInformation) {
        super(_rSyntaxTextArea);
    }
/*
    // Example usage
    public static void main(String[] args) {
        JFrame frame = new JFrame("Custom Icon Row Header Example");
        RSyntaxTextArea textArea = new RSyntaxTextArea();
        JScrollPane scrollPane = new JScrollPane(textArea);

        // Use the custom IconRowHeader
        CustomIconRowHeader iconRowHeader = new CustomIconRowHeader(textArea);
        scrollPane.setRowHeaderView(iconRowHeader);

        frame.add(scrollPane);
        frame.setSize(400, 300);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }*/
}
