//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.services.updater;
//-------------------------------------------------------------------------------------
import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;

import org.apache.commons.io.IOUtils;

import org.eclipse.aether.*;
import org.eclipse.aether.artifact.*;
import org.eclipse.aether.collection.*;
import org.eclipse.aether.util.artifact.*;
//import org.eclipse.aether.connector.basic.*;
import org.eclipse.aether.graph.*;
import org.eclipse.aether.impl.*;
import org.eclipse.aether.version.*;
import org.eclipse.aether.internal.impl.*;
import org.eclipse.aether.repository.*;
import org.eclipse.aether.resolution.*;
import org.eclipse.aether.connector.basic.*;
import org.eclipse.aether.spi.connector.*;
import org.eclipse.aether.spi.connector.transport.*;
import org.eclipse.aether.spi.connector.layout.*;
//import org.eclipse.aether.transport.file.*;
import org.eclipse.aether.util.filter.*;
import org.eclipse.aether.util.graph.visitor.*;
import org.apache.maven.repository.internal.*;
import org.eclipse.aether.transport.http.*;

import xpu.sw.tools.sdk.common.context.*;
import xpu.sw.tools.sdk.common.xbasics.*;
import xpu.sw.tools.sdk.common.utils.*;
import static org.twdata.maven.mojoexecutor.MojoExecutor.*;
import org.apache.maven.project.*;
import org.apache.maven.execution.*;
import org.apache.maven.plugin.*;

//-------------------------------------------------------------------------------------
public class UpdateItem extends XBasic {
    private String name;
    private int mode;

    private String installedVersion;
    private String downloadedVersion;
    private String remoteVersion;

    private String installedPath;
    private String downloadedPath;
    private String remoteUrl;

    private String pathToSdkHome;
    private String artifactId;
    private String baseRemoteUrl;                    
    private static final String AUTH_TOKEN = "arhacc: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";
    private static final String XPU_SDK_REPO = "https://maven.pkg.github.com/arhacc/sw/xpu/xpu-sdk/";
    private static final String XPU_SDK_LIBS_REPO = "https://maven.pkg.github.com/arhacc/sdk-libs/xpu/xpu-sdk-libs/";
    private static final String APP_GROUP_ID = "xpu";


    private RepositorySystem repositorySystem;
    private DefaultRepositorySystemSession repositorySession;
    private Artifact artifact;
    private RemoteRepository remoteRepository;


private MavenProject mavenProject;

private MavenSession mavenSession;

private BuildPluginManager pluginManager;

//-------------------------------------------------------------------------------------
    public UpdateItem(Context _context, int _mode, String _name) {
        super(_context);
        name = _name;
        mode = _mode;
        pathToSdkHome = _context.getPathToSdkHome();        
        createPaths();
    }

//-------------------------------------------------------------------------------------
    private void createPaths(){
        FileUtils.ifDoesntExistCreate(pathToSdkHome + "/lib");
        FileUtils.ifDoesntExistCreate(pathToSdkHome + "/tmp");
//        installedPath = pathToSdkHome + "/lib/" + name + "-" + installedVersion;
        if(name.equals("xpu-sdk-")){
            artifactId = "xpu-sdk";
            baseRemoteUrl = XPU_SDK_REPO;
        } else if(name.equals("xpu-sdk-libs-")){
            artifactId = "xpu-sdk-libs";
            baseRemoteUrl = XPU_SDK_LIBS_REPO;
        } else {
            log.error("Unknown version name in UpdateItem!");
        }        
        String _currentVersion = context.getVersionObject().getVersion(name);
        setInstalledVersion(_currentVersion);
        setDownloadedVersion(_currentVersion);
        setRemoteVersion(_currentVersion);
//        createRepositorySystem();
        mavenProject = new MavenProject();
        mavenSession = newMavenSession(mavenProject);
        pluginManager = new DefaultBuildPluginManager();
        log.debug("Updater: name=" + name + ", installedVersion="+installedVersion + ", downloadedVersion=" + downloadedVersion + ", remoteVersion="+remoteVersion);
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
        installedPath = pathToSdkHome + "/lib/" + name + installedVersion + ".jar";
    }


//-------------------------------------------------------------------------------------
    public String getDownloadedVersion() {
        return downloadedVersion;
    }

//-------------------------------------------------------------------------------------
    public void setDownloadedVersion(String _downloadedVersion) {
        downloadedVersion = _downloadedVersion;
        downloadedPath = pathToSdkHome + "/tmp/" + name + downloadedVersion + ".jar";
    }

//-------------------------------------------------------------------------------------
    public String getRemoteVersion() {
        return remoteVersion;
    }

//-------------------------------------------------------------------------------------
    public void setRemoteVersion(String _remoteVersion) {
        remoteVersion = _remoteVersion;
        remoteUrl = baseRemoteUrl + remoteVersion + "/" + name + remoteVersion + ".jar";
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
                !installedVersion.equals(remoteVersion);
    }

//-------------------------------------------------------------------------------------
    public boolean check() {
        String _url = null;
        log.debug("UpdateItem["+artifactId+"] check...");
        try { 
/*
            VersionRangeRequest request = new VersionRangeRequest(artifact, Arrays.asList(remoteRepository), null);
            VersionRangeResult versionResult = repositorySystem.resolveVersionRange(repositorySession, request);
//            System.out.println("highest version=" + versionResult.getHighestVersion());
            setRemoteVersion(versionResult.getHighestVersion().toString());
            setDownloadedVersion(remoteVersion);
*/
            executeMojo(
                plugin(
                    groupId("xpu"),
                    artifactId(artifactId),
                    version(downloadedVersion)
                ),
                goal("update"),
                configuration(
                    element(
                        name("outputDirectory"),
                        attributes(
                            attribute("dir", "/Users/marius/.xpu/lib/"),
                            attribute("force", "true")
                        )
                    )
                ),
                executionEnvironment(
                    mavenProject,
                    mavenSession,
                    pluginManager
                )
            );

        } catch (Throwable _t) {
            log.error("Cannot update from: " + _url + ": " + _t.getMessage());
            _t.printStackTrace();
            log.debug("After Mojo:mavenProject="+mavenProject+", mavenSession="+mavenSession + ", pluginManager="+ pluginManager);
            System.exit(0);
        }

        log.debug("check: name=" + name + ", installedVersion="+installedVersion + ", downloadedVersion=" + downloadedVersion + ", remoteVersion="+remoteVersion);
        return hasNewRemote();
    }

//-------------------------------------------------------------------------------------
    public boolean download() {
        if(hasNewRemote()){
            try {
                String basicAuthenticationEncoded = Base64.getEncoder().encodeToString(AUTH_TOKEN.getBytes("UTF-8"));
                URL url = new URL(remoteUrl);
                URLConnection urlConnection = url.openConnection();
                urlConnection.setRequestProperty("Authorization", "Basic " + basicAuthenticationEncoded);
                FileOutputStream fileOutputStream = new FileOutputStream(new File(downloadedPath));
                IOUtils.copy(urlConnection.getInputStream(), fileOutputStream);                
                log.debug("download: src=" + remoteUrl + " to dst=" + downloadedPath);
//                org.apache.commons.io.FileUtils.copyURLToFile(new URL(remoteUrl), new File(downloadedPath));                
            } catch(IOException _e){
                log.debug("Error:" + _e.getMessage());
                return false;
            }
        }
        return hasNewDownloaded();
    }

//-------------------------------------------------------------------------------------
    public boolean install() {
        if(hasNewDownloaded()){
            try{
                String _oldInstalledVersion = installedVersion;
                String _oldInstalledPath = installedPath;
                setInstalledVersion(downloadedVersion);
                log.debug("install: src=" + downloadedPath + " to dst=" + installedPath);
                Files.copy(Paths.get(downloadedPath), Paths.get(installedPath), StandardCopyOption.REPLACE_EXISTING);                
                Files.deleteIfExists(Paths.get(_oldInstalledPath)); 
                return true;
            }catch(IOException _e){
                log.debug("Error:" + _e.getMessage());
                return false;
            }
        }
        return false;
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
/*
//-------------------------------------------------------------------------------------
    private void createRepositorySystem() {
        artifact = new DefaultArtifact(APP_GROUP_ID, artifactId, "jar", "[0,)");
        remoteRepository = new RemoteRepository.Builder("github", "default", baseRemoteUrl).build();

        repositorySession = MavenRepositorySystemUtils.newSession();

        DefaultServiceLocator serviceLocator = new DefaultServiceLocator();
//        serviceLocatorBuilder.setRepositorySession(repositorySession).build();

        // Get the repository system
        repositorySystem = newRepositorySystem();//serviceLocator.getService(DefaultRepositorySystem.class);

        // Set the local repository path
        LocalRepository localRepository = new LocalRepository(".m2/repository");
        LocalRepositoryManager localRepoManager = repositorySystem.newLocalRepositoryManager(repositorySession, localRepository);

//        LocalRepository localRepository = new LocalRepository(".xpu/lib");

        repositorySession.setLocalRepositoryManager(localRepoManager);


        // Set the remote repositories
        RemoteRepository centralRepo = new RemoteRepository.Builder("central", "default", baseRemoteUrl).build();
//        repositorySession.setRemoteRepositoryManager(repositorySystem.newRemoteRepositoryManager(repositorySession));
    }

//-------------------------------------------------------------------------------------
 private static RepositorySystem newRepositorySystem() {
        DefaultServiceLocator locator = MavenRepositorySystemUtils.newServiceLocator();
        locator.addService(RepositoryConnectorFactory.class, BasicRepositoryConnectorFactory.class);
        locator.addService(TransporterFactory.class, HttpTransporterFactory.class);
        locator.setErrorHandler(new DefaultServiceLocator.ErrorHandler() {
            @Override
            public void serviceCreationFailed(Class<?> type, Class<?> impl, Throwable exception) {
                exception.printStackTrace();
            }
        });
        return locator.getService(RepositorySystem.class);
    }
*/

protected MavenSession newMavenSession( MavenProject project )
{
  MavenExecutionRequest request = new DefaultMavenExecutionRequest();
  MavenExecutionResult result = new DefaultMavenExecutionResult();
  MavenSession session = new MavenSession( null, MavenRepositorySystemUtils.newSession(), request, result );
  session.setCurrentProject( project );
  session.setProjects( Arrays.asList( project ) );
  return session;
}    
//-------------------------------------------------------------------------------------
    private static File getLocalRepositoryPath() {
        String homeDir = System.getProperty("user.home");
        return new File(homeDir, ".m2/repository");
    }

//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
