
#define GPIO_ELECTROMAGNETIC    9
#define GPIO_DOORSENSOR         8
#define UARTBAUD                115200

#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
#define MYIPADDR 192,168,1,6
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

#define LISTENPORT 1000


#include <UIPEthernet.h>
#include "utility/logging.h"

EthernetServer server = EthernetServer(LISTENPORT);

void networkInfoSend(void) {
    EthernetClient client;
    long  next = millis() + 5000;
    if (client.connect(IPAddress(192,168,0,247), 9999)) {
        Serial.println("Client connected");
        client.print("IP:");
        client.println(Ethernet.localIP());
        client.print("Gateway:");
        client.println(Ethernet.gatewayIP());
        client.print("Subnet:");
        client.println(Ethernet.subnetMask());
        client.print("DNS:");
        client.println(Ethernet.dnsServerIP());
        while(client.available()==0)
        {
            if (next - millis() < 0)
                goto close;
        }
close:
        //disconnect client
        Serial.println("Client disconnect");
        client.stop();
    }
}

void setup() {
    LogObject.begin(UARTBAUD);

    pinMode(GPIO_ELECTROMAGNETIC, OUTPUT);
    pinMode(GPIO_DOORSENSOR, INPUT);
    digitalWrite(GPIO_DOORSENSOR, HIGH);

    uint8_t mac[6] = {MACADDRESS};
    uint8_t myIP[4] = {MYIPADDR};
    uint8_t myMASK[4] = {MYIPMASK};
    uint8_t myDNS[4] = {MYDNS};
    uint8_t myGW[4] = {MYGW};

    Ethernet.begin(mac);
    //Ethernet.begin(mac,myIP,myDNS,myGW,myMASK);
    //LogObject.uart_send_str(Ethernet.localIP());
    Serial.println(Ethernet.localIP());
    server.begin();

    networkInfoSend();
}

void loop() {

    size_t size;

    if (EthernetClient client = server.available())
    {
        LogObject.uart_send_str(F("DATA from Client:'"));
        while((size = client.available()) > 0)
        {
            uint8_t* msg = (uint8_t*)malloc(size+1);
            memset(msg, 0, size+1);
            size = client.read(msg,size);
            LogObject.write(msg,size);

            if (memcmp(msg, "lock", 4) == 0) {
                digitalWrite(GPIO_ELECTROMAGNETIC, 1);
            }
            else if (memcmp(msg, "unlock", 5) == 0) {
                digitalWrite(GPIO_ELECTROMAGNETIC, 0);
            }
            else if (memcmp(msg, "get", 3) == 0) {
                int door = digitalRead(GPIO_DOORSENSOR);
                Serial.print(door);
                if (door) {
                    client.write("open", 4);
                }
                else {
                    client.write("close", 5);
                }
            }

            free(msg);


        }
        LogObject.uart_send_strln(F("'"));
        //client.stop();
    }
}

