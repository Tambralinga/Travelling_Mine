/*
 * WiFlyHQ Example httpclient.ino
 *
 * This sketch implements a simple Web client that connects to a
 * web server, sends a GET, and then sends the result to the
 * Serial monitor.
 *
 * This sketch is released to the public domain.
 *
 */

#include <WiFlyHQ.h>

#include <SoftwareSerial.h>
//SoftwareSerial wifiSerial(8,9);
#define wifiSerial Serial3

//#include <AltSoftSerial.h>
//AltSoftSerial wifiSerial(8,9);

WiFly wifly;

/* Change these to match your WiFi network */
const char mySSID[] = "STEST";
const char myPassword[] = "87654321";

//const char site[] = "arduino.cc";
//const char site[] = "www.google.co.nz";
const char site[] = "seeedstudio.com";

unsigned long t;

void terminal();

void setup()
{
    char buf[32];

    Serial.begin(115200);
    Serial.println("Starting");
    Serial.print("Free memory: ");
    Serial.println(wifly.getFreeMemory(),DEC);

    wifiSerial.begin(9600);
    if (!wifly.begin(&wifiSerial, &Serial)) {
        Serial.println("Failed to start wifly");
	terminal();
    }
    wifly.setBaud(115200);
    wifiSerial.begin(115200);

    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println("Joining network");
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	    terminal();
	}
    } else {
        Serial.println("Already joined network");
    }

    //terminal();

    Serial.print("MAC: ");
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    Serial.print("IP: ");
    Serial.println(wifly.getIP(buf, sizeof(buf)));
    Serial.print("Netmask: ");
    Serial.println(wifly.getNetmask(buf, sizeof(buf)));
    Serial.print("Gateway: ");
    Serial.println(wifly.getGateway(buf, sizeof(buf)));

    wifly.setDeviceID("Wifly-WebClient");
    Serial.print("DeviceID: ");
    Serial.println(wifly.getDeviceID(buf, sizeof(buf)));

    if (wifly.isConnected()) {
        Serial.println("Old connection active. Closing");
	wifly.close();
    }

    if (wifly.open(site, 80)) {
        Serial.print("Connected to ");
	Serial.println(site);

	/* Send the request */
	wifly.println("GET /webservice/wechat/pm25.php?pm25=100&lati=23.137466&longi=113.352425 HTTP/1.0");
	wifly.println("host: seeedstudio.com");
	wifly.println();
    } else {
        Serial.println("Failed to connect");
    }
    t = millis();
}


void loop()
{
    if (millis()-t > 5000)
    {
        t = millis();
        if (wifly.open(site, 80)) {
        Serial.print("Connected to ");
        Serial.println(site);

        /* Send the request */
        wifly.println("GET /webservice/wechat/pm25.php?pm25=100&lati=23.137466&longi=113.352425 HTTP/1.0");
        wifly.println("host: seeedstudio.com");
        wifly.println();
        } else {
            Serial.println("Failed to connect");
        }
    }
    if (wifly.available() > 0) {
	char ch = wifly.read();
	Serial.write(ch);
	if (ch == '\n') {
	    /* add a carriage return */
	    Serial.write('\r');
	}
    }

    if (Serial.available() > 0) {
	wifly.write(Serial.read());
    }
}

/* Connect the WiFly serial to the serial monitor. */
void terminal()
{
    while (1) {
	if (wifly.available() > 0) {
	    Serial.write(wifly.read());
	}


	if (Serial.available() > 0) {
	    wifly.write(Serial.read());
	}
    }
}
