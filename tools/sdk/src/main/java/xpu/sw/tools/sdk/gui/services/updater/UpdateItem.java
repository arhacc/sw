//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;

//import org.json.*;
//import org.apache.commons.io.*;
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
public class UpdateItem extends XBasic {
    private String name;

    private String installedVersion;
    private String downloadedVersion;
    private String remoteVersion;

    private String installedPath;
    private String downloadedPath;
    private String remoteUrl;

    private String pathToSdkHome;
    private static final String XPU_SDK_REPO = "https://api.github.com/repos/arhacc/sw/releases/latest";
    private static final String XPU_SDK_LIB_REPO = "https://api.github.com/repos/arhacc/sdk-libs/releases/latest";

    private CloseableHttpClient closeableHttpClient;
    private ObjectMapper objectMapper;


//-------------------------------------------------------------------------------------
    public UpdateItem(Context _context, String _name) {
        super(_context);
        name = _name;
        pathToSdkHome = _context.getPathToSdkHome();        
        installedVersion = _context.getVersionObject().getVersion(_name);
        createPaths();
        closeableHttpClient = HttpClients.createDefault();
        objectMapper = new ObjectMapper();        
    }

//-------------------------------------------------------------------------------------
    private void createPaths(){
        FileUtils.ifDoesntExistCreate(pathToSdkHome + "/lib");
        FileUtils.ifDoesntExistCreate(pathToSdkHome + "/tmp");
        installedPath = pathToSdkHome + "/lib/" + name + "-" + installedVersion;
        if(name.equals("xpu-sdk-")){
            remoteUrl = XPU_SDK_REPO;
        } else if(name.equals("xpu-sdk-libs-")){
            remoteUrl = XPU_SDK_LIB_REPO;
        } else {
            log.error("Unknown version name in UpdateItem!");
        }
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public String getInstalledVersion() {
        return installedVersion;
    }

//-------------------------------------------------------------------------------------
    public void setInstalledVersion(String _installedVersion) {
        installedVersion = _installedVersion;
        installedPath = pathToSdkHome + "/lib/" + name + "-" + installedVersion;
    }


//-------------------------------------------------------------------------------------
    public String getDownloadedVersion() {
        return downloadedVersion;
    }

//-------------------------------------------------------------------------------------
    public void setDownloadedVersion(String _downloadedVersion) {
        downloadedVersion = _downloadedVersion;
        downloadedPath = pathToSdkHome + "/lib/" + name + "-" + downloadedVersion;
    }

//-------------------------------------------------------------------------------------
    public String getRemoteVersion() {
        return remoteVersion;
    }

//-------------------------------------------------------------------------------------
    public void setRemoteVersion(String _remoteVersion) {
        remoteVersion = _remoteVersion;
        remoteUrl = pathToSdkHome + "/tmp/" + name + "-" + downloadedVersion;
    }

//-------------------------------------------------------------------------------------
    public String getInstalledPath() {
        return installedPath;
    }

//-------------------------------------------------------------------------------------
    public String getDownloadedPath() {
        return downloadedPath;
    }

//-------------------------------------------------------------------------------------
    public boolean hasNewInstalled() {
        return !installedVersion.equals(downloadedVersion);
    }

//-------------------------------------------------------------------------------------
    public boolean hasNewDownloaded() {
        return (downloadedVersion != null ) && 
                !installedVersion.equals(downloadedVersion);
    }

//-------------------------------------------------------------------------------------
    public boolean hasNewRemote() {
        return (remoteVersion != null) &&
                !downloadedVersion.equals(remoteVersion);
    }

//-------------------------------------------------------------------------------------
    public boolean check() {
        String _url = null;
        try { 
            String _assetsUrl = getValueForKey(remoteUrl, "assets_url");
            String[] _browserDownloadUrl = getValuesForKey(_assetsUrl, "browser_download_url");
            String _downloadedVersion = getLatestVersionFromUrl(_browserDownloadUrl);
            setDownloadedVersion(_downloadedVersion);
        } catch (Throwable _t) {
            log.error("Cannot update from: " + _url + ": " + _t.getMessage());
        }

//        log.debug("_foundNewVersion=" + _foundNewVersion + ",lastVersionRemote="+lastVersionRemote+", lastVersionInstalled="+lastVersionInstalled);
        return hasNewRemote();
    }

//-------------------------------------------------------------------------------------
    public boolean download() {
        boolean _hasNewDownloaded = hasNewDownloaded();
        if(_hasNewDownloaded){
            try{
                org.apache.commons.io.FileUtils.copyURLToFile(new URL(remoteUrl), new File(downloadedPath));                
            }catch(IOException _e){
                _hasNewDownloaded = false;
            }
        }
        return _hasNewDownloaded;
    }

//-------------------------------------------------------------------------------------
    public boolean install() {
        boolean _hasNewRemote = hasNewRemote();
        if(_hasNewRemote){
            try{
                Files.copy(Paths.get(downloadedPath), Paths.get(installedPath), StandardCopyOption.REPLACE_EXISTING);
            }catch(IOException _e){
                _hasNewRemote = false;
            }
        }
        return _hasNewRemote;
    }

//-------------------------------------------------------------------------------------
    public String getValueForKey(String _url, String _key) throws IOException {
        URL urlObject = new URL(_url);
        HttpURLConnection connection = (HttpURLConnection) urlObject.openConnection();
        connection.setRequestMethod("GET");

        ObjectMapper objectMapper = new ObjectMapper();
        JsonNode rootNode = objectMapper.readTree(connection.getInputStream());
        log.debug("get "+_key + " from" + _url);
        return rootNode.get(_key).asText();        
    }

//-------------------------------------------------------------------------------------
    public static String[] getValuesForKey(String url, String key) throws IOException {
        URL urlObject = new URL(url);
        HttpURLConnection connection = (HttpURLConnection) urlObject.openConnection();
        connection.setRequestMethod("GET");

        ObjectMapper objectMapper = new ObjectMapper();
        JsonNode rootNode = objectMapper.readTree(connection.getInputStream());
        
        List<String> values = new ArrayList<>();
        for (JsonNode node : rootNode) {
            values.add(node.get(key).asText());
        }
        
        return values.toArray(new String[0]);
    }

//-------------------------------------------------------------------------------------
    public static String getLatestVersionFromUrl(String[] _urls) {
        String _latestVersion = "0";
        for(String _url: _urls){
            String _version = getVersionFromUrl(_url);
            if(compare(_version, _latestVersion)){
                _latestVersion = _version;
            }
        }
        return _latestVersion;
    }

//-------------------------------------------------------------------------------------
    public static String getVersionFromUrl(String _url) {
        String[] _array = _url.split("/");
        _array = _array[_array.length - 1].split("-");
        String _version = _array[_array.length - 1].replace(".jar","");
        return _version;
    }

//-------------------------------------------------------------------------------------
    private static boolean compare(String _version, String _currentLastVersion) {
        return _version.trim().compareTo(_currentLastVersion.trim()) > 0;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
