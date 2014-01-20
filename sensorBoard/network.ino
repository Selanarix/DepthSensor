#include "network.h"  
#include "logger.h"
//#include <SPI.h>
#include <Ethernet.h>


namespace Network 
{
    //-------------------------- Private Types -------------------------------------
      

    //-------------------- Private Function Prototypes -----------------------------
    boolean httpConnect();
    boolean httpDisconnect();   
    //------------------------- Private Data ---------------------------------------

    static EthernetClient client;

    /* 
    * Enter a MAC address for your controller below.
    */
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

   

    //------------------------ Read only ------------------------------------------
    /*
    * Set the static IP address to use if the DHCP fails to assign
    */
    const IPAddress ip(95,175,153,209);
    //const IPAddress myDns(77,244,128,44);

    /* if you don't want to use DNS (and reduce your sketch size)
    * use the numeric IP instead of the name for the server:
    */
    const IPAddress server(109,175,148,72);  // numeric IP for my Notebook (no DNS)
    //char server[] = "www.google.com";    // name address for Google (using DNS)

    //------------------------------- Public Functions -----------------------------
 
    void initNetworkStack()
    {
        // start the Ethernet connection:
        if (Ethernet.begin(mac) == 0) 
        {
            Logger::log(Logger::WARNING,"Failed to configure Ethernet using DHCP");
            //congifure using IP address instead of DHCP
            Ethernet.begin(mac, ip);
        }

     // give the Ethernet shield a second to initialize:
        delay(500);
        String logStr = "My IP address: " + Ethernet.localIP();
        Logger::logString(Logger::INFO, logStr);
        delay(10);
        Logger::log(Logger::INFO, "network initialized");
        delay(10);
    }

    // this method makes a HTTP connection to the server:
    void http_GET_Request(const unsigned char id, const unsigned int value) 
    {  
      if (httpConnect())
      {
          // send the HTTP PUT request:
          Logger::log(Logger::INFO, "send http GET request");
          client.print("PUT /input.php?sensor=");
          client.print(id);
          client.print("&value=");
          client.println(value);  
      }
      httpDisconnect();
    }

    // ---------------------------- Private Functions -------------------------------

    boolean httpConnect()
    {
        // if there's a successful connection:
        if (client.connect(server, 80)) 
        {
            Logger::log(Logger::INFO, "tcp connection established");
            return true;
        }
        Logger::log(Logger::ERROR, "tcp connection failed");
        client.stop();
        return false;
    }

    boolean httpDisconnect()
    {
        client.stop();
        Logger::log(Logger::INFO, "close tcp connection");
        return true;
    }
}
