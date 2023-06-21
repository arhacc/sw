//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.isa;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;

import org.apache.commons.lang3.*;
import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.isa.*;
import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.arch.*;
import xpu.sw.tools.sdk.common.xbasics.*;


//-------------------------------------------------------------------------------------
public class Location extends XBasic {
    private int relativeAddress;
    private int absoluteAddress;


//-------------------------------------------------------------------------------------
    public Location(Context _context, int _relativeAddress) {
        super(_context);
        relativeAddress = _relativeAddress;
    }

//-------------------------------------------------------------------------------------
    public int getRelativeAddress(){
        return relativeAddress;
    }

//-------------------------------------------------------------------------------------
    public int getAbsoluteAddress(){
       return absoluteAddress;
    }

//-------------------------------------------------------------------------------------
    public void setAbsoluteAddress(int _absoluteAddress){
       absoluteAddress = _absoluteAddress;
    }

    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
