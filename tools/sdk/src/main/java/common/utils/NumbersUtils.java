//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------

public class NumbersUtils {

//-------------------------------------------------------------------------------------
    public static int booleansToInt(boolean _a, boolean _b, boolean _c, boolean _d) {
        return booleansToInt(new boolean[]{_a, _b, _c, _d});
    }

//-------------------------------------------------------------------------------------
    public static int booleansToInt(boolean _a, boolean _b, boolean _c, boolean _d, boolean _e, boolean _f) {
        return booleansToInt(new boolean[]{_a, _b, _c, _d, _e, _f});
    }

//-------------------------------------------------------------------------------------
    public static int booleansToInt(boolean[] arr) {
        int n = 0;
        for (boolean b : arr) {
            n = (n << 1) | (b ? 1 : 0);
        }
        return n;
    }

//-------------------------------------------------------------------------------------
    public static int bytesToInt(int[] arr) {
        int n = 0;
        for (int b : arr) {
            n = (n << 8) | b;
        }
        return n;
    }

//-------------------------------------------------------------------------------------
    public static int nibblesToInt(int[] _arr) {
        int _n = 0;
        for (int i = 0; i < _arr.length; i++) {
            int _b = _arr[i];
            _n = (_n << 4) | _b;
            //_n | (_b << ((_arr.length - 1 - i)*4));
        }
        return _n;
    }

//-------------------------------------------------------------------------------------
    public static int count(int[] _a, int _x) {
        int _count = 0;
        for (int i = 0; i < _a.length; i++) {
            if (_a[i] == _x) {
                _count++;
            }
        }
        return _count;
    }

//-------------------------------------------------------------------------------------
    public static int convert(int _x, String _order) {
        if (_order.equals("big_endian")) {
            return (_x & 0xff) << 24 | (_x & 0xff00) << 8 | (_x & 0xff0000) >> 8 | (_x >> 24) & 0xff;
        } else {
            return _x;
        }
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------

