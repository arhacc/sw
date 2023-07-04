//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.util.*;
import java.nio.file.*;

import org.apache.http.*;
import org.apache.http.client.methods.*;
import org.apache.http.impl.client.*;
import org.apache.http.util.*;

import com.fasterxml.jackson.core.*;
import com.fasterxml.jackson.databind.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.utils.*;

//-------------------------------------------------------------------------------------
public class UpdateList extends XBasic {
    private List<UpdateItem> updateList;

//-------------------------------------------------------------------------------------
    public UpdateList(Context _context, int _mode) {
        super(_context);
        updateList = new ArrayList<UpdateItem>();
        String[] _items = _context.getVersionObject().getItems();
        for(String _item : _items){
            add(new UpdateItem(_context, _mode, _item));
        }
    }

//-------------------------------------------------------------------------------------
    public void clear() {
        updateList.clear();
    }

//-------------------------------------------------------------------------------------
    private boolean isEmpty() {
        return (updateList.size() == 0);
    }

//-------------------------------------------------------------------------------------
    public void add(UpdateItem _item) {
        updateList.add(_item);
    }

//-------------------------------------------------------------------------------------
    public boolean check() {
        boolean _success = false;
        for(UpdateItem _updateItem : updateList){
            _success |= _updateItem.check();
        }
        return _success;
    }

//-------------------------------------------------------------------------------------
    public boolean download() {
        boolean _success = false;
        for(UpdateItem _updateItem : updateList){
            _success |= _updateItem.download();
        }
        return _success;
    }

//-------------------------------------------------------------------------------------
    public boolean install() {
        boolean _success = false;
        for(UpdateItem _updateItem : updateList){
            _success |= _updateItem.install();
        }
        return _success;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
