// Import--------------------
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//----------------------------

// Initialize the OLED display
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

// Initialize Timer display
#define TIMER_X 96
#define TIMER_Y 32
#define TIMER_R 30

// Initailize timer
#define TIMER_SET 60000// 3600000
int current_time;
int clock_time;
int start_time;
int minute = 1;   // how about %? ex) 120 sec -> 120 / 60 -> minute, 120 - 60*minute -> sec
int second = 0;

// Initialize a display object 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Wire : I2C Communication | -1 : Doesn't have RESET button on OLED Display

void timer_time_display() 
{
  if(second == 0) {
    minute--;
    second = 59;
  }
  else second--;

  display.print(String(minute));
  display.print(":");
  if(second<10) display.print("0");
  display.println(String(second));
}

void timer_display(int limit) {
  double i = 0;

  display.clearDisplay();

  display.setCursor(0,5);                  // Cursor set
  display.println("Remaining");
  display.setCursor(0,15);  
  timer_time_display();
  display.drawCircle(TIMER_X, TIMER_Y, TIMER_R, WHITE);    // (x, y, radius, color)
  display.drawPixel(TIMER_X, TIMER_Y, WHITE);

  if(minute == 0 && second == 0) {
    display.fillCircle(TIMER_X, TIMER_Y, TIMER_R, WHITE);
    display.setTextSize(2);
    display.setCursor(0,45);
    display.println("STOP!");
  }
  else {
    for(i = 0; i<limit; i++) {
      display.drawLine(TIMER_X, TIMER_Y, int(TIMER_R * cos(PI * double(i/180))) + TIMER_X, int(TIMER_R * sin(PI * double(i/180))) + TIMER_Y, WHITE);
    }
  }

  display.display();
}

void timer_run() {
  int count = 1;
  start_time = millis();
  clock_time = millis();
  while(current_time - start_time < TIMER_SET) {
    current_time = millis();
    if(current_time - clock_time >= 1000) {
      clock_time = millis();
      if((current_time - start_time) > TIMER_SET/360 *count) count = (current_time - start_time) / (TIMER_SET/360);
      timer_display(count-1);
    }
  }
}

void setup() 
{
  Serial.begin(115200);

  // Initialize the OLED Display with begin()
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))    // SSD1306_SWITCHCAPVCC -> 0x02 : 3.3 | 0x01 : External display voltage source : SSD1306_EXTERNALVCC
  {
    Serial.println("SSD1306 allocation failed");
  }
  delay(2000);

  // Clear Display
  display.clearDisplay();

  // Configuration Display
  display.setTextSize(1);                   // font size
  display.setTextColor(WHITE);              // font color
  
  timer_run();

  Serial.println("Done!");
}

void loop() {}

