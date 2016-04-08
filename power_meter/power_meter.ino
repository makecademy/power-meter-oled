// Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>

// INA sensor
Adafruit_INA219 ina219;

// OLED screen
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Measurement variables
float current_mA;
float power_mW;

void setup()   {     
             
  Serial.begin(115200);

  // Init INA219
  ina219.begin();
  ina219.setCalibration_16V_400mA();

  // Init OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);  

  // Display welcome message
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,26);
  display.println("Ready!");
  display.display();
  delay(1000);

  // LED
  pinMode(7, OUTPUT);

}


void loop() {

  // LED OFF
  digitalWrite(7, LOW);

  // Measure
  current_mA = measureCurrent();
  power_mW = measurePower();

  // Display data
  displayData(current_mA, power_mW);
  delay(2000);

  // LED ON
  digitalWrite(7, HIGH);

  // Measure
  current_mA = measureCurrent();
  power_mW = measurePower();

   // Display data
  displayData(current_mA, power_mW);
  delay(2000);
  
}

// Function to measure current
float measureCurrent() {

  // Measure
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.println("");

  // If negative, set to zero
  if (current_mA < 0) {
    current_mA = 0.0; 
  }
 
  return current_mA;
  
}

// Function to measure power
float measurePower() {

  // Measure
  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.println("");

  // If negative, set to zero
  if (current_mA < 0) {
    current_mA = 0.0; 
  }
 
  return current_mA * loadvoltage;
  
}

// Display measurement data
void displayData(float current, float power) {

  // Clear
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  // Current
  display.setCursor(0,0);
  display.println("Current: ");
  display.print(current);
  display.println(" mA");

  // Power
  display.println("Power: ");
  display.print(power);
  display.println(" mW");

  // Displays
  display.display();
  
}
