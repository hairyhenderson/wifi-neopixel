#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266HTTPUpdateServer.h>

// TODO: enable these someday... for now they don't work
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>

#define len(x) (sizeof(x) / sizeof(x[0]))

// ESP8266 pinouts:
// #0  no internal pullup. Connected to both a mini tactile switch and red LED.
//     Used by the ESP8266 to determine when to boot into the bootloader. If 
//     held low during power-up it will start bootloading! Can be used as an
//     output to blink the red LED.
// #2  also used to detect boot-mode. Connected to the blue LED near the WiFi
//     antenna. Can be used as any output and blink the blue LED.
// #4  I2C SDA by default. Recommended if using Arduino, connect I2C devices here!
// #5  I2C SCL by default. Recommended if using Arduino, connect I2C devices here!
// #12 regular IO
// #13 regular IO
// #14 regular IO
// #15 also used to detect boot-mode. Make sure this pin isn't pulled high on
//     startup. Can just be used as an output.
// #16 can be used to wake up out of deep-sleep mode, you'll need to connect it to
//     the RESET pin

// Notes:
// - all GPIO pins are 3.3V logic level in and out, and are NOT 5V compatible. 12mA max!
// - All except #0 have the ability to turn on an internal pullup
// - Rev A of this board had GPIO #4 and #5 swapped so if #4/#5 aren't working for you, try swapping!

#define PIN 12
// #define PIN 2

#define PIXELS 30

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2) 
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// either create password.h with these defined, or comment out this line and
// set your WiFi SSID and key below
#include "password.h"
const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater(true);

#include "sunrise_colours.h"

#include "html/index_html.h"
#include "html/e404_html.h"

void setup() {
  uint8_t wifiCounter;
  Serial.begin(115200);
  Serial.println();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);

  wifiCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    strip.setPixelColor(wifiCounter, strip.Color(128, 0, 0));
    strip.show();

    wifiCounter++;
  }
  Serial.println(" connected");
  colorWipe(strip.Color(0, 128, 0), 10);
  colorWipe(strip.Color(0, 0, 0), 5);

  if (MDNS.begin("neopixel")) {
    // MDNS.addServiceTxt("_autoupdate._http", "_tcp", 80);
    // MDNS.addServiceTxt("_http", "_tcp", "path", "/update");
    // MDNS.announce();
    // MDNS.update();
    Serial.println("MDNS responder started");
  } else {
    for (uint8_t i = 0; i < strip.numPixels(); i += 3) {
      strip.setPixelColor(i, strip.Color(240, 40, 0));
    }
    strip.show();
    delay(1000);
    ESP.restart();
  }

  httpUpdater.setup(&server);

  server.on("/", handleRoot);
  // server.on("/log", handleLog);
  server.on("/sunrise", handleSunrise);
  server.on("/sunset", handleSunset);
  server.on("/clear", handleClear);
  server.on("/wipe", handleWipe);
  server.on("/fill", handleFill);
  server.on("/rainbow", handleRainbow);
  server.on("/raw", HTTP_POST, handleRaw);
  server.on("/pixel", handlePixel);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());

  // MDNS.addService("http", "tcp", 80);
  // MDNS.addServiceTxt("_autoupdate._http._tcp", 80);
  // MDNS.addServiceTxt("http","tcp", "path", "/update");
  MDNS.update();
}

uint32_t loopCounter = 0;

void loop() {
  if (loopCounter >= 1000000) {
    bool updateResult = MDNS.update();
    if (!updateResult) {
      Serial.printf("updateResult false\n");
    }
    // bool announceResult = MDNS.announce();
    // if (!announceResult) {
    //   Serial.printf("announceResult false\n");
    // }
    loopCounter = 0;
  }
  server.handleClient();
  loopCounter++;
}

unsigned long parseArgULong(String param, unsigned long def) {
  String value = server.arg(param);

  unsigned long converted = def;
  if (value.length() > 0) {
    converted = value.toInt();
  }
  return converted;
}

// parse an RGB colour param as "RRGGBB" as a hex string. does not support 4-channel (RGBW).
uint32_t parseArgRGB(String param, uint32_t def) {
  String value = server.arg(param);

  uint32_t converted = def;
  if (value.length() > 0 < 8) {
    if (value.startsWith("#")) {
      value = value.substring(1);
    }
    converted = strtol(value.c_str(), 0, 16);
  }
  return converted;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(unsigned long wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(unsigned long wait, unsigned int cycles) {
  uint16_t i, j;

  for (j=0; j < 256 * cycles; j++) {
    for (i=0; i < strip.numPixels(); i++) {
      byte pos = ((i * 256 / strip.numPixels()) + j) & 255;
      strip.setPixelColor(i, Wheel(pos));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void handleRoot() {
  server.send(200, "text/html", index_html.c_str());
}

// void handleLog() {
//   server.send(200, "text/plain", log)
// }

void handleNotFound() {
  String u = server.uri();
  size_t n = e404_html.length() + u.length() - 1;
  char msg[n];
  snprintf(msg, n, e404_html.c_str(), u.c_str());
  server.send(404, "text/html", msg);
}

void handleSunrise() {
  unsigned long wait = parseArgULong("wait", 10);
  Serial.printf("sunrise with %dms between colours\n", wait);
  server.send(202, "text/plain", "Sunrise!");
  sunriseSimple(wait);
}

void handleSunset() {
  unsigned long wait = parseArgULong("wait", 5);
  Serial.printf("sunset with %dms between colours\n", wait);
  server.send(202, "text/plain", "Sunset!");
  sunsetSimple(wait);
}

void handleRainbow() {
  unsigned long cycles = parseArgULong("cycles", 1);
  unsigned long wait = parseArgULong("wait", 20);
  String fmt = "Showing %d rainbow cycles with %dms wait\n";
  size_t n = fmt.length() + 6;
  char msg[n];
  snprintf(msg, n, fmt.c_str(), cycles, wait);
  server.send(202, "text/plain", msg);
  rainbowCycle(wait, cycles);
}

void handleClear() {
  strip.clear();
  strip.show();
  server.send(200, "text/plain", "Cleared");
}

void handleWipe() {
  unsigned long wait = parseArgULong("wait", 0);
  uint32_t c = parseArgRGB("colour", 0x808080);
  String fmt = "Wiping colour #%06X with %dms wait\n";
  size_t n = fmt.length() + 6;
  char msg[n];
  snprintf(msg, n, fmt.c_str(), c, wait);
  server.send(202, "text/plain", msg);
  colorWipe(c, wait);
}

void handleFill() {
  uint32_t c = parseArgRGB("colour", 0x808080);
  strip.fill(c);
  strip.show();
  String fmt = "Fill #%06X\n";
  size_t n = fmt.length() + 6;
  char msg[n];
  snprintf(msg, n, fmt.c_str(), c);
  // colorWipe(c, wait);
  server.send(200, "text/plain", msg);
}

void handlePixel() {
  uint32_t c = parseArgRGB("colour", 0x808080);
  String s = server.arg("pixel");
  uint16_t pixel = s.toInt();
  strip.setPixelColor(pixel, c);
  strip.show();
  String fmt = "Set pixel %d #%06X\n";
  size_t n = fmt.length() + 6;
  char msg[n];
  snprintf(msg, n, fmt.c_str(), pixel, c);
  server.send(200, "text/plain", msg);
}

// test with
// curl -d '[ 16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215, 16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215, 16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215,16777215 ]' -H 'Content-Type: application/json' -X POST http://neopixel.local/raw
void handleRaw() {
  unsigned long start = micros();
  String body = server.arg("plain");
  // https://arduinojson.org/v6/assistant/ says I need 480 bytes for a 30-element array of longs
  StaticJsonDocument<JSON_ARRAY_SIZE(PIXELS)> doc;
  DeserializationError error = deserializeJson(doc, body);
  if (error) {
    String err = "bad body: %s";
    size_t n = err.length() + len(error.c_str());
    char msg[n];
    snprintf(msg, n, err.c_str(), error.c_str());
    server.send(400, "text/plain", msg);
    return;
  }

  JsonArray arr = doc.as<JsonArray>();
  unsigned long end = micros();
  unsigned long jsonDelta = end - start;

  // unsigned long stripDelta = 0;
  start = micros();
  for (uint16_t i = 0; i < PIXELS; i++) {
    uint32_t c = arr[i];
    // uint8_t r = (uint8_t) (c >> 16);
    // uint8_t g = (uint8_t) (c >> 8);
    // uint8_t b = (uint8_t) c;
    // uint32_t color = strip.Color(r, g, b);
    strip.setPixelColor(i, c);
    strip.show();
  }
  end = micros();
  unsigned long stripDelta = end - start;

  String fmt = "OK, jsonDelta: %dus stripDelta: %dus\n";
  size_t n = fmt.length() + 32;
  char msg[n];
  snprintf(msg, n, fmt.c_str(), jsonDelta, stripDelta);
  server.send(200, "text/plain", msg);
}

// void sunset() {
//   for (uint16_t o = len(redList) - strip.numPixels(); o >= 0 && o < len(redList); o--) {
//     for (uint8_t i = 0; i < strip.numPixels(); i++) {
//       uint32_t c = strip.Color(redList[i+o], greenList[i+o], blueList[i+o]);

//       strip.setPixelColor(i, c);
//       strip.show();
//     }
//   }
// }

// looks fancier, but is broken
// void sunrise() {
//   for (uint16_t o = 0; o < (300 - 30 + 1); o++) {
//     for (uint8_t i = 0; i < 30; i++) {
//       uint32_t c = strip.Color(redList[i+o], greenList[i+o], blueList[i+o]);

//       strip.setPixelColor(i, c);
//       strip.show();
//     }
//   }
// }

// duration - time (ms) to sleep between applying each colour
void sunriseSimple(unsigned long duration) {
  Serial.print("sunrise starting");
  for (uint16_t x = 0; x < len(redList); x++) {
    uint32_t c = strip.Color(redList[x], greenList[x], blueList[x]);
    for (uint8_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(duration);
    Serial.print(".");
    if (x % 80 == 0) {
      Serial.print("/\n");
    }
  }
  Serial.println(" done");
}

// duration - time (ms) to sleep between applying each colour - usually 1/6th of sunrise length
void sunsetSimple(unsigned long duration) {
  Serial.print("sunset starting");
  for (uint16_t x = 0; x < len(redList)+1; x++) {
    uint16_t idx = len(redList)-x;
    uint32_t c = strip.Color(redList[idx], greenList[idx], blueList[idx]);
    for (uint8_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    }
    strip.show();
    delay(duration);
    Serial.print(".");
    if (x % 80 == 0) {
      Serial.print("/\n");
    }
  }
  strip.clear();
  strip.show();
  Serial.println(" done");
}
