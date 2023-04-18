//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.commons.cli.*;
import org.apache.logging.log4j.*;

//-------------------------------------------------------------------------------------
public interface UpdaterListener {

//-------------------------------------------------------------------------------------
    public void ticker(int _percent);

//-------------------------------------------------------------------------------------
}
