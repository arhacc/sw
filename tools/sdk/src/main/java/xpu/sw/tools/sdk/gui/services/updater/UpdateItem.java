//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.io.IOUtils;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.graphql.client.*;
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.*;


import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.context.version.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.utils.*;

//https://josh-ops.com/posts/github-download-from-github-packages/
//https://docs.github.com/en/graphql/overview/explorer

//-------------------------------------------------------------------------------------
public class UpdateItem extends XBasic {
    private String name;
    private int mode;

    private VersionItem installedVersion;
    private VersionItem downloadedVersion;
    private VersionItem remoteVersion;

    private String installedPath;
    private String downloadedPath;
    private String remoteUrl;

    private String pathToSdkHome;
    private String repository;                    
    private String artifactId;
    
    private static final String XPU_SDK_REPO = "sw";
    private static final String XPU_SDK_LIBS_REPO = "sdk-libs";
    private static final String APP_GROUP_ID = "xpu";

    private String authentificationToken;
    private GraphQlClient client;
    private String documentQuery;

//-------------------------------------------------------------------------------------
    public UpdateItem(Context _context, int _mode, VersionItem _installedVersion) {
        super(_context);
        mode = _mode;
        installedVersion = _installedVersion;
        name = installedVersion.getName();
        pathToSdkHome = _context.getPathToSdkHome();        
        authentificationToken = context.getSdkConfig().getString("github_token");
        if((authentificationToken == null) || authentificationToken.isEmpty()){
            log.warn("WARNING: Github [github_token] in sdk.conf is not set. SDK will not be able to perform self-updates!");
        }
        createPaths();
    }

//-------------------------------------------------------------------------------------
    private void createPaths(){
        FileUtils.ifDoesntExistCreate(pathToSdkHome + "/lib");
        FileUtils.ifDoesntExistCreate(pathToSdkHome + "/tmp");
//        installedPath = pathToSdkHome + "/lib/" + name + "-" + installedVersion;
        if(name.equals("xpu-sdk")){
            artifactId = "xpu-sdk";
            repository = XPU_SDK_REPO;
        } else if(name.equals("xpu-sdk-libs")){
            artifactId = "xpu-sdk-libs";
            repository = XPU_SDK_LIBS_REPO;
        } else {
            log.error("Unknown version name in UpdateItem!");
        }        
        downloadedVersion = installedVersion.copyOf();
        remoteVersion = installedVersion.copyOf();

//        createRepositorySystem();
        WebClient _wc = WebClient
                        .builder()
                        .baseUrl("https://api.github.com/graphql")
                        .defaultHeaders(h -> h.setBearerAuth(authentificationToken))
                        .build();
        client = HttpGraphQlClient.create(_wc);

        documentQuery = """
{
  repository(owner: "arhacc", name: "%s") {
    packages(first: 10, packageType: MAVEN, names: "%s.%s") {
      edges {
        node {
          id
          name
          packageType
          versions(first: 1) {
            nodes {
              id
              version
              files(first: 1, ) {
                nodes {
                  name
                  url
                }
              }
            }
          }
        }
      }
    }
  }
}
        """.formatted(repository, APP_GROUP_ID, artifactId);


        log.debug("Updater: name=" + name + ", installedVersion="+installedVersion + ", downloadedVersion=" + downloadedVersion + ", remoteVersion="+remoteVersion);
    }

//-------------------------------------------------------------------------------------
    public String getName() {
        return name;
    }

//-------------------------------------------------------------------------------------
    public String getInstalledVersion() {
        return installedVersion.getValue();
    }

//-------------------------------------------------------------------------------------
    public void setInstalledVersion(String _installedVersion) {
        installedVersion.setValue(_installedVersion);
    }


//-------------------------------------------------------------------------------------
    public String getDownloadedVersion() {
        return downloadedVersion.getValue();
    }

//-------------------------------------------------------------------------------------
    public void setDownloadedVersion(String _downloadedVersion) {
        downloadedVersion.setValue(_downloadedVersion);
    }

//-------------------------------------------------------------------------------------
    public String getRemoteVersion() {
        return remoteVersion.getValue();
    }

//-------------------------------------------------------------------------------------
    public void setRemoteVersion(String _remoteVersion) {
        remoteVersion.setValue(_remoteVersion);
    }

//-------------------------------------------------------------------------------------
    public void setRemoteUrl(String _remoteUrl) {
        remoteUrl = _remoteUrl;
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
        return (installedVersion.compareTo(downloadedVersion) > 0);
    }

//-------------------------------------------------------------------------------------
    public boolean hasNewDownloaded() {
        return (downloadedVersion.compareTo(installedVersion) > 0);
    }

//-------------------------------------------------------------------------------------
    public boolean hasNewRemote() {
        return (remoteVersion.compareTo(installedVersion) > 0);
    }

//-------------------------------------------------------------------------------------
    public boolean check() {
        String _url = null;
//        log.debug("[" + artifactId + "] check...");
//        log.debug("[" + artifactId + "] current version: " + installedVersion);
        try {
        String _remoteVersion  = client
                    .document(documentQuery)
//                .variable("ip", ipOrDomainName)
                .retrieve("repository.packages.edges[0].node.versions.nodes[0].version")
                .toEntity(String.class)
                .block();
        setRemoteVersion(_remoteVersion);

        String _remoteUrl  = client
                    .document(documentQuery)
//                .variable("ip", ipOrDomainName)
                .retrieve("repository.packages.edges[0].node.versions.nodes[0].files.nodes[0].url")
                .toEntity(String.class)
                .block();
        setRemoteUrl(_remoteUrl);
        } catch(Throwable _t){

        }

//        log.debug("check: name=" + name + ", installedVersion="+installedVersion + ", downloadedVersion=" + downloadedVersion + ", remoteVersion="+remoteVersion + ", remoteUrl=" + remoteUrl);
        return hasNewRemote();
    }

//-------------------------------------------------------------------------------------
    public boolean download() {
//        log.debug("[" + artifactId + "] download...");
        if(hasNewRemote()){
            try {
                String basicAuthenticationEncoded = Base64.getEncoder().encodeToString(authentificationToken.getBytes("UTF-8"));
                URL url = new URL(remoteUrl);
                URLConnection urlConnection = url.openConnection();
                urlConnection.setRequestProperty("Authorization", "Basic " + basicAuthenticationEncoded);
                downloadedPath = pathToSdkHome + "/tmp/" + name + "-" + remoteVersion + ".jar";
                FileOutputStream fileOutputStream = new FileOutputStream(new File(downloadedPath));
//                log.debug("download: src=" + remoteUrl + " to dst=" + downloadedPath);
//                log.debug("Current : src=" + remoteUrl + " to dst=" + downloadedPath);
                log.debug("[" + artifactId + "] current version: " + installedVersion + ", download version: " + remoteVersion + ". Please wait...");
                IOUtils.copy(urlConnection.getInputStream(), fileOutputStream);                
//                org.apache.commons.io.FileUtils.copyURLToFile(new URL(remoteUrl), new File(downloadedPath));                
                setDownloadedVersion(remoteVersion.getValue());
            } catch(IOException _e){
                log.debug("Error:" + _e.getMessage());
                return false;
            }
        }
        return hasNewDownloaded();
    }

//-------------------------------------------------------------------------------------
    public boolean install() {
//        log.debug("[" + artifactId + "] install...");
        if(hasNewDownloaded()){
            try{
                String _oldInstalledVersion = installedVersion.getValue();
                installedPath = pathToSdkHome + "/lib/" + name + "-" + downloadedVersion + ".jar";
                String _oldInstalledPath = pathToSdkHome + "/lib/" + name + "-" + installedVersion + ".jar";;
//                log.debug("install: src=" + downloadedPath + " to dst=" + installedPath + ", _oldInstalledPath="+_oldInstalledPath);
                log.debug("[" + artifactId + "] install version: " + downloadedVersion + ". Please wait...");
                Files.copy(Paths.get(downloadedPath), Paths.get(installedPath), StandardCopyOption.REPLACE_EXISTING);                
                Files.deleteIfExists(Paths.get(_oldInstalledPath)); 
                setInstalledVersion(downloadedVersion.getValue());
//                clean tmp download
                Files.deleteIfExists(Paths.get(downloadedPath));                 
                return true;
            }catch(IOException _e){
                log.debug("Error:" + _e.getMessage());
                return false;
            }
        }
        return false;
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
