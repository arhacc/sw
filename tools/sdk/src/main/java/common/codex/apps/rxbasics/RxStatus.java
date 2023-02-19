//-------------------------------------------------------------------------------------
package codex.common.apps.rxbasics;
//-------------------------------------------------------------------------------------

import codex.common.apps.uid.Uid;
import org.apache.logging.log4j.Logger;

//-------------------------------------------------------------------------------------
public class RxStatus extends RxBase {

    private int status;
    private long uid;

    private int hashCode;

    private Object statusLocker;

    //---statics:
    public static final int STATUS_INIT = 0;
    public static final int STATUS_RUNNING = 1;
    public static final int STATUS_PAUSED = 2;
    public static final int STATUS_STOPPED = 3;

//-------------------------------------------------------------------------------------
    public RxStatus(Logger _log) {
        this(_log, newUid());
    }

//-------------------------------------------------------------------------------------
    public RxStatus(Logger _log, long _uid) {
        super(_log);
        status = STATUS_INIT;
        uid = _uid;
//      log.outln(" RxStatus.... " + uid);
        hashCode = createHashcode();
        statusLocker = new Object();
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    public int getStatus() {
        synchronized (statusLocker) {
            return status;
        }
    }

//-------------------------------------------------------------------------------------
    public String getStatusAsString() {
        return getStatusAsString(status);
    }

//-------------------------------------------------------------------------------------
    public String getStatusAsString(int _status) {
        synchronized (statusLocker) {
            String _string = "";//super.toString() + " : ";
            switch (_status) {
                case STATUS_INIT: return _string + "INIT";
                case STATUS_RUNNING: return _string + "RUNNING";
//        case STATUS_FINISHED    : return _string + "FINISHED";
                case STATUS_PAUSED: return _string + "PAUSED";
                case STATUS_STOPPED: return _string + "STOPPED";
                default: return _string + "ERROR";
            }
        }
    }

//-------------------------------------------------------------------------------------
    public void setStatus(int _status) {
//        boolean _change = (status != _status);
        synchronized (statusLocker) {
            status = _status;
//        if (_change) {
            statusLocker.notifyAll();
//        }
        }
    }

//-------------------------------------------------------------------------------------
    public void setRunning() {
        setStatus(STATUS_RUNNING);
    }

//-------------------------------------------------------------------------------------
    public void setPaused() {
        setStatus(STATUS_PAUSED);
    }

//-------------------------------------------------------------------------------------
    public void setStopped() {
        setStatus(STATUS_STOPPED);
    }

//-------------------------------------------------------------------------------------
    public boolean isRunning() {
        return (getStatus() == STATUS_RUNNING);
    }

//-------------------------------------------------------------------------------------
    public boolean isNotRunning() {
        return (getStatus() != STATUS_RUNNING);
    }

//-------------------------------------------------------------------------------------
    public boolean isPaused() {
        return (getStatus() == STATUS_PAUSED);
    }

//-------------------------------------------------------------------------------------
    public boolean isNotPaused() {
        return (getStatus() != STATUS_PAUSED);
    }

//-------------------------------------------------------------------------------------
    public boolean isNotStopped() {
        return (getStatus() != STATUS_STOPPED);
    }

//-------------------------------------------------------------------------------------
    public boolean isStopped() {
        return (getStatus() == STATUS_STOPPED);
    }

//-------------------------------------------------------------------------------------
    public long getUid() {
        return uid;
    }

//-------------------------------------------------------------------------------------
    public void setUid(long _uid) {
        uid = _uid;
    }

//-------------------------------------------------------------------------------------
    private static long newUid() {
        return Uid.next();
    }

//-------------------------------------------------------------------------------------
    private int createHashcode() {
        return (int) (uid ^ (uid >> 32));
    }

//-------------------------------------------------------------------------------------
    public int hashCode() {
        return hashCode;
    }

//-------------------------------------------------------------------------------------
    public boolean equals(RxStatus _rxStatus) {
        return (hashCode() == _rxStatus.hashCode());
    }

//-------------------------------------------------------------------------------------
    public void waitUntilStopped() {
        synchronized (statusLocker) {
            while (!isStopped()) {
                try {
                    statusLocker.wait();
                } catch (InterruptedException _e) {

                }
            }
        }
    }

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
    public void load() {
        super.load();
//      setStatus(STATUS_RUNNING);
    }

//-------------------------------------------------------------------------------------
    public void save() {
        setStopped();
        super.save();
    }

//-------------------------------------------------------------------------------------
    public String toString() {
        return getStatusAsString();
    }
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
