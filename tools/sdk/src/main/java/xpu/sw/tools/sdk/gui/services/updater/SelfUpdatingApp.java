import org.apache.maven.repository.internal.MavenRepositorySystemUtils;
import org.eclipse.aether.*;
import org.eclipse.aether.artifact.*;
import org.eclipse.aether.collection.*;
import org.eclipse.aether.util.artifact.*;
import org.eclipse.aether.connector.basic.BasicRepositoryConnectorFactory;
import org.eclipse.aether.graph.Dependency;
import org.eclipse.aether.graph.DependencyFilter;
import org.eclipse.aether.impl.DefaultServiceLocator;
import org.eclipse.aether.internal.impl.DefaultRepositorySystem;
import org.eclipse.aether.repository.LocalRepository;
import org.eclipse.aether.repository.RemoteRepository;
import org.eclipse.aether.resolution.DependencyRequest;
import org.eclipse.aether.resolution.DependencyResolutionException;
import org.eclipse.aether.resolution.DependencyResult;
import org.eclipse.aether.spi.connector.RepositoryConnectorFactory;
//import org.eclipse.aether.transport.http.HttpTransporterFactory;
//import org.eclipse.aether.transport.httpclient.HttpClientTransporterFactory;
import org.eclipse.aether.transport.file.FileTransporterFactory;
import org.eclipse.aether.transport.file.FileTransporterFactory;
import org.eclipse.aether.util.filter.DependencyFilterUtils;
import org.eclipse.aether.util.graph.visitor.PreorderNodeListGenerator;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.Arrays;
import java.util.List;

public class SelfUpdatingApp {

    private static final String GITHUB_PACKAGES_URL = "https://maven.pkg.github.com/arhacc/sw";

    private static final String APP_GROUP_ID = "com.example";
    private static final String APP_ARTIFACT_ID = "my-app";
    private static final String APP_FILE_PATH = "<path to app file>";

    private static final RepositorySystem repositorySystem = new DefaultRepositorySystem();
    private static final RepositorySystemSession session = newSession(repositorySystem);

    private static void updateApp() throws IOException {
        Artifact artifact = new DefaultArtifact(APP_GROUP_ID, APP_ARTIFACT_ID, "jar", "[0,)");
        RemoteRepository repository = new RemoteRepository.Builder("github", "default", GITHUB_PACKAGES_URL).build();
        Dependency dependency = new Dependency(artifact, null);
//        DependencyRequest request = new DependencyRequest(dependency, DependencyFilterUtils.classpathFilter(JavaScopes.COMPILE));

CollectRequest collectRequest = new CollectRequest(dependency, Arrays.asList(repository));
collectRequest.setRepositories(Arrays.asList(repository));

DependencyRequest request = new DependencyRequest();
request.setCollectRequest(collectRequest);
request.setFilter(DependencyFilterUtils.classpathFilter(JavaScopes.COMPILE));

        DependencyResult result;
        try {
            result = repositorySystem.resolveDependencies(session, request);
        } catch (DependencyResolutionException e) {
            throw new IOException("Failed to resolve dependencies", e);
        }
        PreorderNodeListGenerator nodeListGenerator = new PreorderNodeListGenerator();
        result.getRoot().accept(nodeListGenerator);

        List<File> resolvedFiles = nodeListGenerator.getFiles();
        if (resolvedFiles.isEmpty()) {
            throw new IOException("No files resolved");
        }

        File latestFile = resolvedFiles.get(resolvedFiles.size() - 1);
        Path appFilePath = Path.of(APP_FILE_PATH);
        Files.copy(latestFile.toPath(), appFilePath, StandardCopyOption.REPLACE_EXISTING);
    }

    private static RepositorySystemSession newSession(RepositorySystem system) {
        DefaultRepositorySystemSession session = MavenRepositorySystemUtils.newSession();
        LocalRepository localRepo = new LocalRepository(getLocalRepositoryPath());
        session.setLocalRepositoryManager(system.newLocalRepositoryManager(session, localRepo));
        return session;
    }

    private static File getLocalRepositoryPath() {
        String homeDir = System.getProperty("user.home");
        return new File(homeDir, ".m2/repository");
    }


}