#include <FreeSixIMU.h>
#include <FIMU_ADXL345.h>
#include <FIMU_ITG3200.h>

#include <Wire.h>
float conv = 0.00390625;
float angles[3]; // yaw pitch roll
float allValues[6];

// Set the FreeSixIMU object
FreeSixIMU sixDOF = FreeSixIMU();

void setup() { 
  Serial.begin(115200);
  Wire.begin();
  
  delay(5);
  sixDOF.init(); //begin the IMU
  delay(5);
}

void loop() { 
  
  sixDOF.getValues(allValues);
  Serial.print("Velocities ");
  Serial.print(allValues[0] * conv);
  Serial.print(" | ");  
  Serial.print(allValues[1] * conv);
  Serial.print(" | ");
  Serial.println(allValues[2] * conv);
  
  
  delay(100); 
}

