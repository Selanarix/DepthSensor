#include "network.h"  
#include <SPI.h>
#include <Ethernet.h>


namespace Network 
{
    //-------------------------- Private Types -------------------------------------
      

    //-------------------- Private Function Prototypes -----------------------------
    boolean httpConnect();
    boolean httpDisconnect();

    void handleLAN();
   
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
            Serial.println("Failed to configure Ethernet using DHCP");
        //congifure using IP address instead of DHCP
        Ethernet.begin(mac, ip);
        }

     // give the Ethernet shield a second to initialize:
        delay(500);
        Serial.print("My IP address: ");
        Serial.println( Ethernet.localIP() );
    }

    // this method makes a HTTP connection to the server:
    void http_GET_Request(const unsigned char id, const unsigned int value) 
    {  
      if (httpConnect())
      {
          // send the HTTP PUT request:
          Serial.println("send http request...");
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
            Serial.println("http connecting...");
            return true;
        }
        Serial.println("connection failed");
        client.stop();
        return false;
    }

    boolean httpDisconnect()
    {
        client.stop();
        Serial.println("http disconneting..");
        return true;
    }

    void handleLAN()
    {
        // state of the connection last time
        static boolean lastConnected = false;            
        // if there are incoming bytes available 
        // from the server, read them and print them:
        if (client.available()) 
        {
            char c = client.read();
            Serial.println(c);
        }
      
        // if the server's disconnected, stop the client:
        if (!client.connected() && lastConnected) 
        {
            Serial.println();
            Serial.println("handle disconnect...");
            client.stop();
        }
        
        // store the state of the connection for next time
        lastConnected = client.connected();
    }


}
