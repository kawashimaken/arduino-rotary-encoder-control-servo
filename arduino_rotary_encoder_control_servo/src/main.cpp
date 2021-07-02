
// address：0x3C
// SSD1306
// oled SCK  -->ArduinoのSCL
// oled SDA  -->ArduinoのSDA
// oled VDD(Vcc) -->Arduino 5V
// olde GND --> Arduino GND
//=====================================================================
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;
int encoder0PinALast = 0;
int n = 0;
int outPin13 = 13;
int outPin12 = 12;
// pin for rotary encoder
#define ENC_SW_PIN         7
int enc_switch, enc_switch_old;
// oled display related
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//

#include <Servo.h>
Servo myservo;

//
void setup() {
  // servo setup
  myservo.attach(9);

  // rotary encoder input
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin (9600);
  // for led
  pinMode(outPin13, OUTPUT);
  pinMode(outPin12, OUTPUT);
  // for rotary encoder switch
  pinMode( ENC_SW_PIN, INPUT_PULLUP);


  // oled display related
  //
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);

}

void loop() {
  display.clearDisplay();

  // READ SWITCH
  enc_switch = digitalRead(ENC_SW_PIN);
  if ((enc_switch_old == 1) && (enc_switch == 0)) { // 1->0 transition
    Serial.println("SWITCH is PRESSED");
    display.setCursor(10, 20);
    display.println("Switch:ON");
    display.display();
    delay(10);
  }
  enc_switch_old = enc_switch;


  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
      digitalWrite(outPin13, LOW);
      digitalWrite(outPin12, HIGH);
      delay(abs(encoder0Pos));
    } else {
      encoder0Pos++;
      digitalWrite(outPin12, LOW);
      digitalWrite(outPin13, HIGH);
      delay(abs(encoder0Pos));
    }
    Serial.println (encoder0Pos);
    if (encoder0Pos > 90) {
      encoder0Pos = 90;
    }
    if (encoder0Pos < -90) {
      encoder0Pos = -90;
    }

    int servoValue = map(encoder0Pos, -90, 90, 0, 180);
    myservo.write(servoValue);
    display.setCursor(10, 20);
    display.print(encoder0Pos);
    display.display();
  }
  encoder0PinALast = n;
}