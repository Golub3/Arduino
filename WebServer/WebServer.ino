#include <SPI.h>
#include <Ethernet.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);
const int LED = LED_BUILTIN;

//create a handle to publish messages to a topic
ros::NodeHandle node_handle;

std_msgs::String button_msg;
std_msgs::UInt16 led_msg;

void subscriberCallback(const std_msgs::UInt16& led_msg) {
  if (led_msg.data  == 1) {
    digitalWrite(LED, HIGH); 
  } else {
    digitalWrite(LED, LOW);
  }
}

ros::Publisher button_publisher("button_press", &button_msg);
ros::Subscriber<std_msgs::UInt16> led_subscriber("toggle_led", &subscriberCallback);


void setup() {
  pinMode(LED, OUTPUT);
  
  node_handle.initNode();
  node_handle.advertise(button_publisher);
  node_handle.subscribe(led_subscriber);
  
  Ethernet.begin(mac);
  server.begin();
  Serial.begin(9600);
  Serial.println("Your server address is:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String buffer = "";
    while(client.connected()) {
      if (client.available()) {
        char c = client.read();
        buffer += c;
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("");
          client.print("<center><br><h1>Led Control</h1><br><br><br><FORM>");
          client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"Turn ON\"></P>");
          client.print("<P> <INPUT type=\"submit\" name=\"status\" value=\"Turn OFF\"></P>");
          client.print("</FORM></center>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
          buffer="";
        }
        else
          if (c == '\r') {
          if(buffer.indexOf("GET /?status=Turn+ON")>=0) {
            digitalWrite(LED, HIGH);
            button_msg.data = "Pressed";
          }
 
          if(buffer.indexOf("GET /?status=Turn+OFF")>=0) {
            digitalWrite(LED, LOW);
            button_msg.data = "NOT pressed";
            
          }
          button_publisher.publish( &button_msg );
          node_handle.spinOnce();
        }
        else {
          currentLineIsBlank = false;
        }
      }
      button_publisher.publish( &button_msg );
      node_handle.spinOnce();
    }

    client.stop();
  }
}
