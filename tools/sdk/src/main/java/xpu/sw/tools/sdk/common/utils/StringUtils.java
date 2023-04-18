//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.nio.*;
import java.util.*;
import java.math.*;
import java.text.*;

//-------------------------------------------------------------------------------------
public class StringUtils {
    private static final String BANNER = "----------------------------------------------------------------------------------------------------------------------------------";

//-------------------------------------------------------------------------------------
    public static String trim(String _value) {
        return _value.replace("_", "");
    }

//-------------------------------------------------------------------------------------
  public static String trim(String value, char c) {

    if (c <= 32) return value.trim();

    int len = value.length();
    int st = 0;
    char[] val = value.toCharArray();    /* avoid getfield opcode */

    while ((st < len) && (val[st] == c)) {
        st++;
    }
    while ((st < len) && (val[len - 1] == c)) {
        len--;
    }
    return ((st > 0) || (len < value.length())) ? value.substring(st, len) : value;
}

//-------------------------------------------------------------------------------------
    public static String getFilename(String _filename){
        File _file = new File(_filename);
        String _fn = _file.getName();
        String _name = _fn.substring(0, _fn.length() - 1);
        return _name;
    }

//-------------------------------------------------------------------------------------
    public static boolean isANumber(String _text){
        if(_text == null){
            return false;
        }
        if(_text.startsWith("0x")){
            return true;
        }
        try {
            Long.decode(_text);
            return true;
        } catch(NumberFormatException _er1) {
//            System.out.println("---"+_er1.getMessage()+ "text=["+_text+"]");
//            _er1.printStackTrace();
            return false;
        }
    }

//-------------------------------------------------------------------------------------
/*    public static boolean isInScope(String _scope1, String _scope2){        
        return false;
    }*/

//-------------------------------------------------------------------------------------
    public static int getBit(String _number, int _index){
//        long _n = Long.decode(_number);
//        return (int)((long)(_n >> _index) & 1);
		_number = _number.replaceAll("_", "");                   
        BigInteger _b;
        if(_number.startsWith("0x")){
            _number = _number.substring(2);
            _b = new BigInteger(_number, 16); 
        } else {
            _b = new BigInteger(_number); 
        }
        return _b.testBit(_index) ? 1 : 0;
    }

//-------------------------------------------------------------------------------------
    public static int getNumber(String _text){
    	return getIntNumber(_text);
    }

//-------------------------------------------------------------------------------------
    public static int getIntNumber(String _text){
        int _radix = 10;
        _text = _text.replaceAll("_", "");
        if(_text.startsWith("0x")){
            _text = _text.substring(2);
            _radix = 16;
        }
        BigInteger _n = new BigInteger(_text, _radix);
        int _int = _n.intValue();
        return _int;
    }

//-------------------------------------------------------------------------------------
    public static long getLongNumber(String _text){
        if(_text == null){
            return 0;
        }
        int _radix = 10;
        _text = _text.replaceAll("_", "");
        if(_text.startsWith("0x")){
            _text = _text.substring(2);
            _radix = 16;
        }
        BigInteger _n = new BigInteger(_text, _radix);
        long _long = _n.longValue();
        return _long;
    }

//-------------------------------------------------------------------------------------
    public static String getLongAsString(long _number){
    	String _text = String.format("%08d", (int)_number);
    	return _text;
    }

//-------------------------------------------------------------------------------------
    public static int getNumber(String _text, int _index, String _order) {
    	int _radix = 10;
		_text = _text.replaceAll("_", "");
    	if(_text.startsWith("0x")){
    		_text = _text.substring(2);
    		_radix = 16;
    	}
    	BigInteger _n = new BigInteger(_text, _radix);
    	_n = _n.shiftRight(32 * _index);
    	int _int = _n.intValue();
		return NumbersUtils.convert(_int, _order);
    }

//-------------------------------------------------------------------------------------
private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
/*public static String bytesToHex(byte[] _bytes) {
    return bytesToHex(_bytes, 0, _bytes.length - 1);
}

//-------------------------------------------------------------------------------------
public static String bytesToHex(byte[] _bytes, int _start, int _stop) {
*/
public static String bytesToHex(byte[] _bytes) {
    if(_bytes == null){
        return "--";
    }
    int _length = _bytes.length;
    char[] hexChars = new char[_length * 2];
    for ( int j = 0; j < _length; j++ ) {
        int v = _bytes[j] & 0xFF;
        hexChars[j * 2] = hexArray[v >>> 4];
        hexChars[j * 2 + 1] = hexArray[v & 0x0F];
    }
    return new String(hexChars);
}

//-------------------------------------------------------------------------------------
private static final String    HEXES    = "0123456789ABCDEF";

public static String getHex(byte[] raw) {
    final StringBuilder hex = new StringBuilder(2 * raw.length);
    for (final byte b : raw) {
        hex.append(HEXES.charAt((b & 0xF0) >> 4)).append(HEXES.charAt((b & 0x0F)));
    }
    return hex.toString();
}

public static String getHex(int[] _raw) {
    final StringBuilder _hex = new StringBuilder(8 * _raw.length);
    for (final int _i : _raw) {
    _hex.append(HEXES.charAt((_i & 0xF0000000) >>> 28))
        .append(HEXES.charAt((_i & 0x0F000000) >> 24))
        .append(HEXES.charAt((_i & 0x00F00000) >> 20))
        .append(HEXES.charAt((_i & 0x000F0000) >> 16))
        .append(HEXES.charAt((_i & 0x0000F000) >> 12))
        .append(HEXES.charAt((_i & 0x00000F00) >>  8))
        .append(HEXES.charAt((_i & 0x000000F0) >>  4))
        .append(HEXES.charAt((_i & 0x0000000F)      ));
    }
    return _hex.toString();
}

//-------------------------------------------------------------------------------------
public static String getHex(byte b) {
    final StringBuilder hex = new StringBuilder(2);
        hex.append(HEXES.charAt((b & 0xF0) >> 4))
        .append(HEXES.charAt((b & 0x0F)));
    return hex.toString();
}

//-------------------------------------------------------------------------------------
public static String getHex(short _s) {
    final StringBuilder hex = new StringBuilder(4);
        hex.append(HEXES.charAt((_s & 0xF000) >> 12))
        .append(HEXES.charAt((_s & 0x0F00) >> 8))
        .append(HEXES.charAt((_s & 0x00F0) >> 4))
        .append(HEXES.charAt((_s & 0x000F)));
    return hex.toString();
}

//-------------------------------------------------------------------------------------
public static String getHex(int  _i) {
    return getHex(toBytes(_i));
}

//-------------------------------------------------------------------------------------
public static String getHex(long  _i) {
    return getHex(toBytes(_i));
}

//-------------------------------------------------------------------------------------
    public static int toInt(byte[] _bytes) {
        ByteBuffer _wrapped = ByteBuffer.wrap(_bytes); // big-endian by default
        return _wrapped.getInt(); 
    }

//-------------------------------------------------------------------------------------
    public static long toLong(byte[] _bytes) {
        ByteBuffer _wrapped = ByteBuffer.wrap(_bytes); // big-endian by default
        return _wrapped.getLong(); 
    }

//-------------------------------------------------------------------------------------
    public static String toString(byte[] _bytes) {
        String _string = _bytes.toString();
        return _string;
    }

//-------------------------------------------------------------------------------------
    public  static byte[] toBytes(String _string) {
        return StringUtils.asBytes(_string);
    }

//-------------------------------------------------------------------------------------
    public  static byte[] toBytes(int _int) {
        return ByteBuffer.allocate(4).putInt(_int).array();
    }

//-------------------------------------------------------------------------------------
    public  static byte[] toBytes(long _int) {
        return ByteBuffer.allocate(8).putLong(_int).array();
    }

//-------------------------------------------------------------------------------------
    public static byte[] asBytes(String s) {
    		s = s.replace("0x", "");                   
    		s = s.replaceAll("_", "");                   
           String tmp;
           byte[] b = new byte[s.length() / 2];
           int i;
           for (i = 0; i < s.length() / 2; i++) {
             tmp = s.substring(i * 2, i * 2 + 2);
             b[i] = (byte)(Integer.parseInt(tmp, 16) & 0xff);
           }
           return b;                                            //return bytes
    }

//-------------------------------------------------------------------------------------
    public static int parseInt( final String s ) {
        // Check for a sign.
        int num  = 0;
        int sign = -1;
        final int len  = s.length( );
        final char ch  = s.charAt( 0 );
        if ( ch == '-' )
            sign = 1;
        else
            num = '0' - ch;

        // Build the number.
        int i = 1;
        while ( i < len )
            num = num*10 + '0' - s.charAt( i++ );

        return sign * num;
    } 
    
//-------------------------------------------------------------------------------------
    public static String formatTime(long _time) {
        String __time = String.format("%04d", _time/3600) + ":" +
                        String.format("%02d", (_time/60)%60) + ":" + 
                        String.format("%02d", _time%60);
        return __time;
    }

//-------------------------------------------------------------------------------------
	public static String captializeAllFirstLetter(String name) {
    char[] array = name.toCharArray();
    array[0] = Character.toUpperCase(array[0]);

    for (int i = 1; i < array.length; i++) {
        if (Character.isWhitespace(array[i - 1])) {
            array[i] = Character.toUpperCase(array[i]);
        }
    }

    return new String(array);
}

//-------------------------------------------------------------------------------------
	public static String captializeFirstLetter(String _name) {
    char[] array = _name.toCharArray();
    array[0] = Character.toUpperCase(array[0]);
    return new String(array);
}

//-------------------------------------------------------------------------------------
	public static String lowerizeFirstLetter(String _name) {
    char[] array = _name.toCharArray();
    array[0] = Character.toLowerCase(array[0]);
    return new String(array);
}

//-------------------------------------------------------------------------------------
    public static String[] intArrayToStringArray(int[] _intArray) {
        int _length = _intArray.length;
        String[] _list = new String[_length];
        for (int i = 0; i < _length ; i++) {
            _list[i] = Integer.toString(_intArray[i]);
        }
        return _list;
    }

//-------------------------------------------------------------------------------------
    public static int[] stringArrayToIntArray(String[] _stringArray) {
        int _length = _stringArray.length;
        int[] _list = new int[_length];
        for (int i = 0; i < _length ; i++) {
            _list[i] = Integer.parseInt(_stringArray[i]);
        }
        return _list;
    }

//-------------------------------------------------------------------------------------
    public static String insertUnderscores(String _in) {
    	String _out = _in.replaceAll("(.{8})(?!$)", "$1_");
        return _out;
    }



//-------------------------------------------------------------------------------------
    public static String formatTimeAsFilename(long _time) {
        Date _date = new Date(_time);
        SimpleDateFormat _dateFormat = new SimpleDateFormat("yyyy_MM_dd_hh_mm_ss") ;
        String __time = _dateFormat.format(_date);
        return __time;
    }

//-------------------------------------------------------------------------------------
    public static String createBanner(String _text) {
    	StringBuilder _str = new StringBuilder(BANNER);
    	int _bannerLength = BANNER.length();
    	if(_text.length() != 0){
    		_text = "[ " + _text + " ]";    		
    	}
    	int _textLength = _text.length();
    	int _start = _bannerLength/2 - _textLength/2;
    	int _stop = _start + _textLength;
    	_str.replace(_start, _stop, _text);
		return _str.toString();
    }

//-------------------------------------------------------------------------------------
    public static List<Integer> convertStringToIntegerList(String _text) {
		List<Integer> _peersList = new ArrayList<Integer>();
//		_text = trim(_text, '{');
//		_text = trim(_text, '}');
//		String _text1 = _text.trim();
		String _text2 = _text.substring(1, _text.length() - 1);
		String[] _row = _text2.split(";");
		for (int i = 0; i < _row.length; i++) {
			if(!_row[i].isEmpty()){
				_peersList.add(Integer.parseInt(_row[i]));
			}
		}
		return _peersList;
	}

//-------------------------------------------------------------------------------------
    public static List<Long> convertStringToLongList(String _text) {
        List<Long> _peersList = new ArrayList<Long>();
//      _text = trim(_text, '{');
//      _text = trim(_text, '}');
//      String _text1 = _text.trim();
        String _text2 = _text.substring(1, _text.length() - 1);
        String[] _row = _text2.split(";");
        for (int i = 0; i < _row.length; i++) {
            if(!_row[i].isEmpty()){
                _peersList.add(Long.parseLong(_row[i]));
            }
        }
        return _peersList;
    }

//-------------------------------------------------------------------------------------
    public static List<String> convertStringToStringList(String _text) {
        List<String> _peersList = new ArrayList<String>();
//      _text = trim(_text, '{');
//      _text = trim(_text, '}');
//      String _text1 = _text.trim();
        String _text2 = _text.substring(1, _text.length() - 1);
        String[] _row = _text2.split(";");
        for (int i = 0; i < _row.length; i++) {
            if(!_row[i].isEmpty()){
                _peersList.add(_row[i]);
            }
        }
        return _peersList;
    }

//-------------------------------------------------------------------------------------
    public static String convertIntegerListToString(List<Integer> _intList) {
		String _text = "{";
		for (int i = 0; i < _intList.size(); i++) {
			_text += String.valueOf(_intList.get(i));
			if(i < _intList.size() - 1){
				_text += ";";
			}
		}
		return _text + "}";
	}

//-------------------------------------------------------------------------------------
    public static String convertLongListToString(List<Long> _intList) {
		String _text = "{";
		for (int i = 0; i < _intList.size(); i++) {
			_text += String.valueOf(_intList.get(i));
			if(i < _intList.size() - 1){
				_text += ";";
			}
		}
		return _text + "}";
	}

//-------------------------------------------------------------------------------------
    public static String convertStringListToString(List<String> _stringList) {
        String _text = "{";
        for (int i = 0; i < _stringList.size(); i++) {
            _text += _stringList.get(i);
            if(i < _stringList.size() - 1){
                _text += ";";
            }
        }
        return _text + "}";
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------

   