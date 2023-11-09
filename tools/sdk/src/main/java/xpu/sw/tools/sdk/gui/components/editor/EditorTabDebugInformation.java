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

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.fileformats.core.*;
import xpu.sw.tools.sdk.common.fileformats.asm.*;
import xpu.sw.tools.sdk.common.fileformats.cpp.*;
import xpu.sw.tools.sdk.common.fileformats.json.*;
import xpu.sw.tools.sdk.common.fileformats.xpuprj.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.common.*;

//-------------------------------------------------------------------------------------
public class EditorTabDebugInformation extends GuiBasic {
    private XpuFile xpuFile;

    private boolean isEligibleForDebug;

//-------------------------------------------------------------------------------------
    public EditorTabDebugInformation(Gui _gui, Context _context, XpuFile _xpuFile) {
        super(_context, _gui);
        xpuFile = _xpuFile;
        refresh();
    }

//-------------------------------------------------------------------------------------
    public void refresh() {
        String _extension = xpuFile.getExtension();
        isEligibleForDebug = _extension.equals(AsmFile.EXTENSION) ||
                            _extension.equals(CppFile.EXTENSION);
    }

//-------------------------------------------------------------------------------------
    public boolean isEligibleForDebug(){
        return isEligibleForDebug;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
