// Here we rewrite the Server code which we wrote in C before
//

import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;

public final class Server
{
    static final int PORT = 3490;
    static final int BACKLOG = 10;

    public static void main(String[] args)
    {
        // first create a listener
        try (ServerSocket listener = new ServerSocket())
        {
            // Here just like in C SO_REUSEADDR
            listener.setReuseAddress(true);

            // Here we also bind to a free address like in C, with the keyword null as address
            listener.bind(new InetSocketAddress((InetAddress) null, PORT),  BACKLOG);

            System.out.println("server: listening on port " +PORT);

            while (true)
            {
                // Accept blocks until a client connects
                Socket sock = listener.accept();

                // Here we start a thread per accepted request. In production a thread pool is better
                new Thread(() -> handleClient(sock)).start();
            } catch (IOException e)
            {
                e.printStackTrace();
            }
        }
    }

    private static void handleClient(Socket sock)
    {
        try (sock)
        {
            // Here we print the client address IPv4 or IPv6
            SocketAddress remote = sock.getRemoteSocketAddress();
            System.out.println("server got connection from: " + remote);

            // Now we send "Hello World!" and close the socket afterwards
            OutputStream out = sock.getOutputStream();
            out.write("Hello World!".getBytes(StandardCharsets.UTF_8));
            out.flush();                // Flush output stream to be sure
        } catch (IOExcpetion e)
        {
            // In C we would have perror("send")/EPIPE
            System.err.println("socket-handler error: " + e.getMessage());
        }
    }
}
