//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.io.targetmanager;
//-------------------------------------------------------------------------------------
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.io.*;
import java.nio.channels.*;
import java.util.*;
import javax.swing.*;
import java.lang.reflect.*;

import org.apache.commons.configuration2.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.context.version.*;

//-------------------------------------------------------------------------------------
public interface TargetStatusListener {

//-------------------------------------------------------------------------------------
    public void targetStatusChanged(TargetConnection _connection);
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
