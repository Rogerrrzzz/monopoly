import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

/** Владеет открытым файлом: close() закрывает файл, а память освобождает GC. */
public final class GameLog implements AutoCloseable {
    private final Path path;
    private final BufferedWriter writer;
    private boolean closed;

    public GameLog(Path path) throws IOException {
        this.path = path;
        Path parent = path.toAbsolutePath().getParent();
        Files.createDirectories(parent);
        writer = Files.newBufferedWriter(path, StandardCharsets.UTF_8);
        System.out.println("[+] Журнал открыт: " + path);
    }

    public void write(String line) throws IOException {
        if (closed) {
            throw new IOException("Журнал уже закрыт");
        }
        writer.write(line);
        writer.newLine();
    }

    public boolean isClosed() {
        return closed;
    }

    @Override
    public void close() throws IOException {
        if (!closed) {
            writer.close();
            closed = true;
            System.out.println("[-] Журнал закрыт: " + path);
        }
    }
}
