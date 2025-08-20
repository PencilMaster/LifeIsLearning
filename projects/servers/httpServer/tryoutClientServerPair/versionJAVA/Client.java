// Here we rewrite our previously written Client from C to Java.
// This is done to further deepen Java knowledge to work on GraalVM.
//
//
import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;

// Keyword final: Prevents inheritance from main
public final class Client
{
    static final int PORT = 3490;
    static final int MAXDATASIZE = 100;

    public static void main (String[] args)
    {
        
        // check length of passed arguments (IP-Adress)
        if (args.length != 1)
        {
            System.err.println("usage: client hostname unspecified");
            System.exit(1);
        }
        String host = args[0];      // This holds IP-Address

        try
        {
            // Similar to getaddrinfo() in C, without possibility of hints it seems. TODO: check this
            InetAddress[] addrs = InetAddress.getAllByName(host);
            Socket sock = null;
            IOException last = null;

            // We implement the clock feature
            ThreadMXBean mx = ManagementFactory.getThreadMXBean();
            // Enable CpuTime support if needed:
            if (mx.isThreadCpuTimeSupported() && !mx.isThreadCpuTimeEnabled()) mx.setThreadCpuTimeEnabled(true);
            long begin = mx.getCurrentThreadCpuTime();
            // clock feature

            for (InetAddress addr : addrs)
            {
                System.out.println("client attempting to connect to: " + addr.getHostAddress());
                try
                {
                    sock = new Socket();
                    // Connect blocks by default, add timeout.
                    sock.connect(new InetSocketAddress(addr, PORT), 5000);
                    break;
                } catch (IOException e)
                {
                    last = e;
                    if (sock != null) try { sock.close(); } catch (IOException ignore) {}
                    sock = null;
                }
            }

            // Here we check if all addresses did not work out / could not be connected to
            if (sock == null)
            {
                System.err.println("client: failed to connect");
                if (last != null) last.printStackTrace();         // Prints out last Exception
                System.exit(2);
            }

            // Now we try to read out the socket TODO: read up on - try catch - syntax
            try 
            {
                // We want to read up to MAXDATASIZE-1 and print it
                InputStream in = sock.getInputStream();
                byte[] buf = new byte[MAXDATASIZE];

                int n = in.read(buf);                               
                // read() returns negative if server closed connection
                if (n < 0)
                {
                    System.err.println("client: server closed connection");
                    return;
                }

                // I am not convinced with declaring UTF_8 TODO: Find out standard charset
                String msg = new String(buf, 0, n, StandardCharsets.UTF_8);
                System.out.println("client: connected to " + sock.getRemoteSocketAddress());
                System.out.println("client: received '" + msg + "'");
            } catch (UnknownHostException e)
            {
                System.err.println("DNS Resolution failed: " + e.getMessage());
            }
            
            // We implement the clock feature
            long end = mx.getCurrentThreadCpuTime();
            System.out.println("CPU time spent in Java: " + ((end - begin) / 1_000_000.0));
            // clock feature
        }  catch (IOException e)
        {
            System.err.println("I/O error: " + e.getMessage());
        }
    }
}
