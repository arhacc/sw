//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.common.utils;
//-------------------------------------------------------------------------------------

import java.time.*;
import java.sql.Timestamp;
import java.time.format.DateTimeFormatter;
import java.time.ZoneId;

//-------------------------------------------------------------------------------------
public class TimeUtils {

    private static ZoneId ZONE_ID;

//-------------------------------------------------------------------------------------
    public static void setZoneId(ZoneId _zoneId) {
        ZONE_ID = _zoneId;
    }

//-------------------------------------------------------------------------------------
    public static ZonedDateTime convertLongToZonedDateTime(long _long) {
        return (new Timestamp(_long)).toInstant().atZone(ZONE_ID);
//    	return (new Timestamp(_long)).toLocalDateTime().atZone(ZONE_ID);
    }

//-------------------------------------------------------------------------------------
    public static long convertZonedDateTimeToLong(ZonedDateTime _zonedDateTime) {
//        return Timestamp.valueOf(_zonedDateTime.toLocalDateTime()).getTime();
        return Timestamp.from(_zonedDateTime.toInstant()).getTime();
    }

//-------------------------------------------------------------------------------------
    public static String getTime(DateTimeFormatter _formatter) {
        if (ZONE_ID == null) {
            return "-";
        }
        ZonedDateTime _now = ZonedDateTime.now(ZONE_ID);
        return getTime(_now, _formatter);
    }

//-------------------------------------------------------------------------------------
    public static String getTime(long _long, DateTimeFormatter _formatter) {
        ZonedDateTime _time = convertLongToZonedDateTime(_long);
        return getTime(_time, _formatter);
    }

//-------------------------------------------------------------------------------------
    public static String getTime(ZonedDateTime _time, DateTimeFormatter _formatter) {
//	    SimpleDateFormat formatter = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
//	    SimpleDateFormat _formatter = new SimpleDateFormat(_format);
//	    Date _date = new Date();
        return _time.format(_formatter);
    }

//-------------------------------------------------------------------------------------
    public static String getTimeAsString() {
        DateTimeFormatter _formatter = DateTimeFormatter.ofPattern("yyyy_MM_dd_HH_mm_ss");
//	    SimpleDateFormat _formatter = new SimpleDateFormat(_format);
//	    Date _date = new Date();
        ZonedDateTime _time = ZonedDateTime.now();
        return _time.format(_formatter);
    }

//-------------------------------------------------------------------------------------
    public static int getSeconds(String _time) {
        if (_time.isEmpty() || _time.trim().equals("instant")) {
            return 0;
        } else if (_time.trim().equals("indefinitely")) {
            return Integer.MAX_VALUE;
        }
        int _len = _time.length();

        String _counter = _time.substring(0, _len - 1);
        String _multiplierString = _time.substring(_len - 1, _len);
        int _multiplier = 0;
        int _timeInt = 0;
        switch (_multiplierString) {
            case "s": _multiplier = 1;
                break;
            case "m": _multiplier = 60;
                break;
            case "h": _multiplier = 3600;
                break;
            case "d": _multiplier = 24 * 3600;
                break;
            case "w": _multiplier = 7 * 24 * 3600;
                break;
            case "l": _multiplier = 30 * 24 * 3600;
                break;//month
            default: _multiplier = 0;
                break;
        }
        try {
            _timeInt = Integer.parseInt(_counter) * _multiplier;
        } catch (Exception _e) {
        }
        return _timeInt;
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------

