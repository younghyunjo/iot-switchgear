#include <avr/io.h>
#include <util/delay.h>

#include <EtherCard.h>
#define STATIC 1  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,0,200 };
// gateway ip address
static byte gwip[] = { 192,168,0,1 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char page[] PROGMEM =

"HTTP/1.0 200 Success\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<body>"
  "<form method=get>"
  "<input type='radio' name=r value='1'> Lock<br>"
  "<input type='radio' name=r value='2'> Unlock<br>"
  "<input type=submit value=submit></form>"
  
 
  "</body>"
"</html>"
;
int toggleButton = 8; //PB0
int ledPin = 9; //PB1

void setup(){
  
  Serial.begin(57600);
  Serial.println("\n[backSoon]");

  /*DDRB |= B00000010;//2; //PB1 SET as OUTPUT
  DDRB &= B11111110;//~1; //PB0 SET as INPUT
  PORTB |= B00000011;//3;*/
  pinMode(toggleButton, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(toggleButton, HIGH);
  digitalWrite(ledPin, HIGH);

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  if (!ether.staticSetup(myip, gwip))
    Serial.println("Static IP failed");
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
}



static BufferFiller buf;

void process_request(char *data)
{
  Serial.println(data);

  buf = ether.tcpOffset ();

      if (!strncmp (data, "GET / ", 6))
    {
        buf.emit_p (PSTR(
          "HTTP/1.0 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "\r\n"
          "<html>"
            "<body>"
            "<div style='border:1px dashed $BDBDBD; width:340px; height:285px id='menuContainer'>"
              "<ul id = 'mainNav'"
                "<li>"
                  "<a class='mainNavSelected' href='/setip'>HOME</a>"
                "</li>"
                "<li>"
                  "<a class='beginners' href='/'>Beginners</a>"
                "</li>"
            "</div>"
              "<h1>door "));


                if (PINB & 1)  //현재 문이 열렸는지 닫혔는지 상태 표시
                  buf.emit_p (PSTR("opened"));
                else
                  buf.emit_p (PSTR("closed"));
  
              buf.emit_p (PSTR("</h1>"

              "<form method=get>"
              /*"Connection Type : <input type='radio' name='connection_type' value='0'>DHCP" "<input type='radio' name='connection' value='1'>STATIC<br>"
              "IP Address      : <input type = 'text' name='ip'><br>"
              "SubnetMask      : <input type = 'text' name='sm'><br>"
              "Gateway         : <input type = 'text' name='gw'><br>"
              "DNS            : <input type = 'text' name='dns'><br>"*/
              "<input type='radio' name=r value='1'> Lock<br>"
              "<input type='radio' name=r value='2'> Unlock<br>"
              "<input type=submit value=submit></form>"

      
          "</body>"
        "</html>"
    ));


    }
    else if (!strncmp (data, "GET /setip", 10))
    {
              buf.emit_p (PSTR(
          "HTTP/1.0 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "\r\n"
          "<html>"
            "<body>"
              "<form method=get>"
              "Connection Type : <input type='radio' name='connection_type' value='0'>DHCP" "<input type='radio' name='connection' value='1'>STATIC<br>"
              "IP Address      : <input type = 'text' name='ip'><br>"
              "SubnetMask      : <input type = 'text' name='sm'><br>"
              "Gateway         : <input type = 'text' name='gw'><br>"
              /*"DNS            : <input type = 'text' name='dns'><br>"*/
              "<input type=submit value=submit></form>"
          "</body>"
        "</html>"
    ));
}
    else if (!strncmp (data, "GET /?r=1 ", 9))
    {
      digitalWrite(ledPin,HIGH);
      buf.emit_p (PSTR(
          "HTTP/1.0 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "\r\n"
          "<html>"
            "<body>"
              "<h1>door "));

                if (PINB & 1)  //현재 문이 열렸는지 닫혔는지 상태 표시
                  buf.emit_p (PSTR("opened"));
                else
                  buf.emit_p (PSTR("closed"));
  
              buf.emit_p (PSTR("</h1>"
              "<form method=get>"
              "<input type='radio' name=r value='1'> Lock<br>"
              "<input type='radio' name=r value='2'> Unlock<br>"
              "<input type=submit value=submit></form>"

              "Executed LOCK Command"      
          "</body>"
        "</html>"
        ));
    }
    else if (!strncmp (data, "GET /?r=2 ", 9))
    {
      digitalWrite(ledPin,LOW);
      buf.emit_p (PSTR(
          "HTTP/1.0 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "\r\n"
          "<html>"
            "<body>"
              "<h1>door "));

                if (PINB & 1)  //현재 문이 열렸는지 닫혔는지 상태 표시
                  buf.emit_p (PSTR("opened"));
                else
                  buf.emit_p (PSTR("closed"));
  
              buf.emit_p (PSTR("</h1>"
              "<form method=get>"
              "<input type='radio' name=r value='1'> Lock<br>"
              "<input type='radio' name=r value='2'> Unlock<br>"
              "<input type=submit value=submit></form>"

              "Executed UNLOCK Command"      
          "</body>"
        "</html>"        ));
    }
    else
    {
    
      buf.emit_p (PSTR(
        "HTTP/1.0 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html>"
          "<body>"
            "<h1>Bad Request</h1>"
          "</body>"
        "</html>"
        ));
    }



  ether.httpServerReply(buf.position());
}

void loop()
{
  static int prev;
  
  if(digitalRead(toggleButton))//if (PINB & 1) //문열림 닫힘 상태 체크
  {
    //digitalWrite(ledPin,HIGH);//PORTB |= 2;
    if (!prev)
      Serial.println("on\n");
    prev = 1;
  }
  else
  {
    //digitalWrite(ledPin,LOW);//PORTB &= ~2;
    if (prev)
      Serial.println("off\n");
    prev = 0;
  }
    
  // wait for an incoming TCP packet, but ignore its contents
  uint16_t pos;

  pos = ether.packetLoop(ether.packetReceive());
  if (pos != 0) {
    char *data = (char*)&Ethernet::buffer[pos];

    process_request (data);    
  }
}

