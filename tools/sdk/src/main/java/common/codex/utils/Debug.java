//-------------------------------------------------------------------------------------
package codex.common.utils;

//-------------------------------------------------------------------------------------
import codex.utils.FileUtils;
import java.io.FileWriter;
import java.io.IOException;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import org.apache.commons.lang3.tuple.MutablePair;

//-------------------------------------------------------------------------------------
public class Debug {

    private Debug parent;
    private String filename;
    private FileWriter fw;
    private int level;
//    private int colorCode;

    private boolean dumpToFileOnly;

    private ArrayList<MutablePair<String, Integer>> prefixes;
    private DateTimeFormatter formatter;

    public static final int INFO = 1;
    public static final int WARNING = 2;
    public static final int ERROR = 3;

    public static final int IMPORTANT0 = 100;
    public static final int IMPORTANT1 = 101;
    public static final int IMPORTANT2 = 102;
    public static final int IMPORTANT3 = 103;
    public static final int IMPORTANT4 = 104;
    public static final int IMPORTANT5 = 105;
    public static final int IMPORTANT = IMPORTANT0;

//-------------------------------------------------------------------------------------
    public Debug(int _level) {
        this(null, _level);
    }

//-------------------------------------------------------------------------------------
    public Debug(String _filename, int _level) {
        this(_filename, _level, false);
    }

//-------------------------------------------------------------------------------------
    public Debug(String _filename, int _level, boolean _dumpToFileOnly) {
        this(_filename, _level, _dumpToFileOnly, null, null, -1);
        /*
        if(_filename != null){
            try{
                 fw = new FileWriter(_filename);
            } catch(IOException _e){
                _e.printStackTrace();
            }
        }
        level = _level;
        dumpToFileOnly = _dumpToFileOnly;*/
    }

//-------------------------------------------------------------------------------------
    public Debug(Debug _debug, String _prefixText) {
        this(_debug, _prefixText, INFO);
    }

//-------------------------------------------------------------------------------------
    public Debug(Debug _debug, String _prefixText, int _prefixColor) {
        this(null, false, _debug, _prefixText, _prefixColor);
    }

//-------------------------------------------------------------------------------------
    public Debug(String _filename, boolean _dumpToFileOnly, Debug _debug, String _prefixText, int _prefixColor) {
        this(_filename, (_debug != null) ? _debug.level : INFO, _dumpToFileOnly, _debug, _prefixText, _prefixColor);
    }

//-------------------------------------------------------------------------------------
    public Debug(String _filename, int _level, boolean _dumpToFileOnly, Debug _debug, String _prefixText,
            int _prefixColor) {
        this(_filename, _level, _dumpToFileOnly, _debug, _prefixText, _prefixColor, null);
    }

//-------------------------------------------------------------------------------------
    public Debug(String _filename, int _level, boolean _dumpToFileOnly, Debug _debug, String _prefixText,
            int _prefixColor, String _directory) {
        if (_debug != null) {
            fw = _debug.fw;
        }

//        dumpToFileOnly = _debug.dumpToFileOnly;
        filename = _filename;
        if (_directory != null) {
            if (_debug != null) {
                filename = FileUtils.putFileInDirectory(_debug.filename, _directory);
            }
        }

        if (filename != null) {
            try {
                fw = new FileWriter(filename);
            } catch (IOException _e) {
                _e.printStackTrace();
            }
        }
//        level = _level;
        dumpToFileOnly = _dumpToFileOnly;

        parent = _debug;
//        level = _debug.level;
        level = _level;
//        prefixText0 = _prefixText;
//        prefixColor0 = INFO;
//        prefixes = (ArrayList<MutablePair<String, Integer>>)(_debug.prefixes).clone();
        if (prefixes == null) {
            initiatePrefixes();
        }
        if (_prefixText != null) {
            prefixes.add(new MutablePair<String, Integer>(_prefixText, getColorCode(_prefixColor)));
        }
//        formatter =  DateTimeFormatter.ofPattern("yyyy_MM_dd_HH:mm:ss.SSS");
//        formatter =  DateTimeFormatter.ofPattern("yyyy.MM.dd.HH:mm:ss.SSS");
        formatter = DateTimeFormatter.ofPattern("dd.HH:mm:ss.SSS");
    }

//-------------------------------------------------------------------------------------
    private void initiatePrefixes() {
        prefixes = new ArrayList<MutablePair<String, Integer>>();
    }

//-------------------------------------------------------------------------------------
    public void reset() {
    }

//-------------------------------------------------------------------------------------
/*
+~~~~~~+~~~~~~+~~~~~~~~~~~+
|  fg  |  bg  |  color    |
+~~~~~~+~~~~~~+~~~~~~~~~~~+
|  30  |  40  |  black    |
|  31  |  41  |  red      |
|  32  |  42  |  green    |
|  33  |  43  |  yellow   |
|  34  |  44  |  blue     |
|  35  |  45  |  magenta  |
|  36  |  46  |  cyan     |
|  37  |  47  |  white    |
|  39  |  49  |  default  |
+~~~~~~+~~~~~~+~~~~~~~~~~~+
https://stackoverflow.com/questions/1448858/how-to-color-system-out-println-output/24415186
     */
//-------------------------------------------------------------------------------------
    public void setPrefix0(int _prefixColorLevel, String _prefixText) {
        /*        String _prefixText0 = _prefixText;
        int _prefixColor0 = getColorCode(_prefixColorLevel);
        if(prefixes == null){
        	initiatePrefixes();
        }
        if(prefixes.size() == 0){
			prefixes.add(new MutablePair<String, Integer>(_prefixText0, _prefixColor0));
        } else {
			prefixes.set(0, new MutablePair<String, Integer>(_prefixText0, _prefixColor0));
        }*/
        setPrefix(0, _prefixColorLevel, _prefixText);
    }

//-------------------------------------------------------------------------------------
    public void setPrefix1(int _prefixColorLevel, String _prefixText) {
        /*        int _prefixColor1 = getColorCode(_prefixColorLevel);
        String _prefixText1 = _prefixText;
        if(prefixes == null){
        	initiatePrefixes();
        }
        if(prefixes.size() < 2){
			prefixes.add(new MutablePair<String, Integer>(_prefixText1, _prefixColor1));
        } else {
			prefixes.set(1, new MutablePair<String, Integer>(_prefixText1, _prefixColor1));
        }*/
        setPrefix(1, _prefixColorLevel, _prefixText);
    }

//-------------------------------------------------------------------------------------
    public void setPrefix(int _level, int _prefixColorLevel, String _prefixText) {
        while (prefixes.size() <= _level) {
            prefixes.add(new MutablePair<String, Integer>());
        }
        MutablePair<String, Integer> _prefix = prefixes.get(_level);
        _prefix.setLeft(_prefixText);
        _prefix.setRight(getColorCode(_prefixColorLevel));
    }

//-------------------------------------------------------------------------------------
    public String getPrefixes() {
        return printPrefixes();
    }

//-------------------------------------------------------------------------------------
    private int getColorCode(int _level) {
        switch (_level) {
            case INFO: return 32;//green;
            case WARNING: return 33;
            case ERROR: return 31;
            case IMPORTANT0: return 31;
            case IMPORTANT1: return 33;
            case IMPORTANT2: return 34;
            case IMPORTANT3: return 37;
            case IMPORTANT4: return 36;
            case IMPORTANT5: return 35;
            default: return 39;
        }
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    public void outln(List<String> _list) {
        outln(INFO, _list);
    }

//-------------------------------------------------------------------------------------
    public void outln(int _level, List<String> _list) {
        for (int i = 0; i < _list.size(); i++) {
            outln(_level, _list.get(i));
        }
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    public void out(String _txt) {
        out(INFO, _txt);
    }

//-------------------------------------------------------------------------------------
    public void out(String _txt, boolean _includePrefix) {
        out(INFO, _txt, _includePrefix);
    }

//-------------------------------------------------------------------------------------
    public void outln(String _txt) {
        out(INFO, _txt + "\n");
    }

//-------------------------------------------------------------------------------------
    public void outln(int _level, String _txt) {
        out(_level, _txt + "\n", true);
    }

//-------------------------------------------------------------------------------------
    public void outln(String _txt, boolean _includePrefix) {
        out(INFO, _txt + "\n", _includePrefix);
    }

//-------------------------------------------------------------------------------------
    public void outln(int _level, String _txt, boolean _includePrefix) {
        out(_level, _txt + "\n", _includePrefix);
    }

//-------------------------------------------------------------------------------------
    public void out(int _level, String _txt) {
        out(_level, _txt, true);
    }

//-------------------------------------------------------------------------------------
    public void out(int _level, String _txt, boolean _includePrefix) {
        String _prefixes = "";
        /*    	if(_txt.equals("SchedulerOutputQueue.nextOrder: type[0]")){
    		System.out.println("_includePrefix="+_includePrefix+",_level="+_level+", level="+level);
    	}*/
        if (level <= _level) {
            if (!dumpToFileOnly) {
//                _txt = _txt+"["+dumpToFileOnly+"]";
                if (_includePrefix) {
                    _prefixes = printPrefixes();
                    if ((prefixes != null) && (parent != null) && (!_prefixes.isEmpty())) {
//            		if((prefixes != null) && (!_prefixes.isEmpty())){
                        _prefixes += ": ";
                    }
                }

//                if(_level != INFO){
                int _colorCode = getColorCode(_level);
                System.out.print(_prefixes + (char) 27 + "[" + _colorCode + "m" + _txt + (char) 27 + "[39m");
                /*                } else {
                    System.out.print(_txt);
                }*/
            }
            if (fw != null) {
                try {
                    String _time = "";
                    if (_includePrefix) {
                        _prefixes = strip(_prefixes);
                        if (formatter != null) {
                            _time = "[" + TimeUtils.getTime(formatter) + "]" + _prefixes;
                        }
                    }
                    fw.write(_time + _txt);
                    fw.flush();
                } catch (IOException _e) {
                    _e.printStackTrace();
                }
            }
        }
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    private String printPrefixes() {
        String _text = "";
        if (parent != null) {
            _text = parent.printPrefixes();
        }
        if (prefixes != null) {
            for (int i = 0; i < prefixes.size(); i++) {
                _text += printPrefix(prefixes.get(i));
            }
//    		(_prefix -> printPrefix(_prefix));
        }
        return _text;
    }

//-------------------------------------------------------------------------------------
    private String printPrefix(MutablePair<String, Integer> _prefix) {
        String _prefixText = _prefix.getLeft();
        int _prefixColor = _prefix.getRight();
//        System.out.print((char)27 + "["+_prefixColor+"m" + "["+_prefixText+"]" + (char)27 + "[39m");
        if ((_prefixText == null) || (_prefixText.isEmpty())) {
            return "";
        }
        return (char) 27 + "[" + _prefixColor + "m" + "[" + _prefixText + "]" + (char) 27 + "[39m";
        /*            if(prefixText1 != null){
                System.out.print((char)27 + "["+prefixColor1+"m" + "["+prefixText1+"]: " + (char)27 + "[39m");
            } else {
                System.out.print((char)27 + "["+prefixColor0+"m" + ": " + (char)27 + "[39m");
            }*/
    }

//-------------------------------------------------------------------------------------
    private String strip(String _txt) {
        return _txt.replaceAll("" + (char) 27 + "\\[(\\d+)m", "");
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------

