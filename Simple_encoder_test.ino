#include <ESP32Encoder.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

ESP32Encoder encoder;

//Declare pixel size of the OLED display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define enter_pin 14
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

volatile unsigned long DebounceTime;  //Time for debounce, in ms

volatile unsigned int delayTime = 100;//Delay in ms

int16_t x_0;
int16_t y_0;
uint16_t width;
uint16_t height;

int previousValue = 0;

void setup(){
	
  Serial.begin(115200);
  Wire.begin(5, 4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  
  // Enable the weak pull up resistors
  ESP32Encoder::useInternalWeakPullResistors=UP;
  // use pin 13 and 15 for the first encoder
  encoder.attachHalfQuad(13, 15);
  // set starting count value after attaching
  encoder.setCount(0);
  encoder.setFilter(1023);
  //Attach interrupt for encoder button, while pressing this button it sets the encoder value to zero
  attachInterrupt(digitalPinToInterrupt(enter_pin), [] {if((millis() - DebounceTime) >= (delayTime )) DebounceTime = millis();encoder.setCount(0);}, RISING);
}

void loop(){
  if (previousValue != encoder.getCount()){ //Check if the encoder has change
    previousValue = encoder.getCount();     //If encoder moved then show it in OLED display
    centerText();
    delay(100);
  }
}

void centerText(){
  Serial.println("Encoder value = " + String((int32_t)encoder.getCount()));
  display.clearDisplay();
  display.setTextSize(3);             
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.getTextBounds(String((int32_t)encoder.getCount()), 64, 32, &x_0, &y_0, &width, &height);
  display.setCursor((128 - width) / 2, (64 - height) / 2);
  display.println(encoder.getCount());
  display.display();
}

