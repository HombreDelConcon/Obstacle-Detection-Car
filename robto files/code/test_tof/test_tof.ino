#include "Adafruit_VL53L0X.h"

#define IRQ_PIN 2
#define XSHUT_PIN 3

Adafruit_VL53L0X vl53 = Adafruit_VL53L0X(XSHUT_PIN, IRQ_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println(F("Adafruit VL53L0X sensor demo"));

  Wire.begin();
  if (! vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("VL53L0X sensor OK!"));

  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());

  /*
  vl.VL53L0X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L0X_SetInterruptPolarity(0);
  */
}

void loop() {
  int16_t distance;

  if (vl53.dataReady()) {
    // new measurement for the taking!
    distance = vl53.distance();
    if (distance == -1) {
      // something went wrong!
      Serial.print(F("Couldn't get distance: "));
      Serial.println(vl53.vl_status);
      return;
    }
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(" mm");

    // data is read out, time for another reading!
    vl53.clearInterrupt();
  }
}
// #include <Wire.h>
// // Preliminary pass at getting GP2Y0E03 working.
// // It appears to work but i haven’t the slightest idea how.
// // The same command of Wire.read() is assumed to get two different sets of data.</p>
// #define SENSOR_ADRS 0x29 // I2C address of GP2Y0E03
// #define WRITE_REG 0x52 // Data address of Distance Value // Functions to process only at power-on and reset
// #define READ_REG 0x53

// byte l[8] = {}
// void setup ()
// {
//   // Initialize serial communication
//   Wire.begin(); // Initialize I2C,
//   delay(1000); // start after 1 second
//   Serial.begin(9600);
//   // Main processing function repeatedly executed
  
// }
// void loop ()
// {
//   int ans;
//   byte c[4];
//   Wire.beginTransmission (SENSOR_ADRS); // start communication processing
//   Wire.write (WRITE_REG); // specify the address of the table storing the distance value
//   ans = Wire.endTransmission(); // send and close data
//   Serial.print("Status: ");
//   Serial.println(ans);
//   delay(200);
//   if (ans == 0) {
//     ans = Wire.requestFrom(SENSOR_ADRS, 4) ;
//     Serial.print("Bytes returned: ");
//     Serial.println(ans);
//     // int v1 = c[0] = Wire.read(); // Read the 31st to 24th bits of data c [1]
//     // int v2 = c[1] = Wire.read(); // Read the 23rd and 16th bits of the data
//     // int v3 = c[2] = Wire.read(); // Read 15th to 8th bits of the data
//     // int v4 = c[3] = Wire.read(); // Read 7th to 1st bit

//     // Serial.print("LSB: ");
//     // Serial.println(v4);
//     // Serial.print("23 - 16th bit: ");
//     // Serial.println(v3);
//     // Serial.print("15 - 8th bit: ");
//     // Serial.println(v2);
//     // Serial.print("MSB: ");
//     // Serial.println(v1);
//     int count = 0;
//     while (Wire.available()){
//       int i = Wire.read();
//       if (i > 0){
//         Serial.print("value: ");
//         Serial.println(i);
//         Serial.print("count: ");
//         Serial.println(count);
//       } count++;
//     }
//     Serial.println();
//     Serial.println();
//   }
//   else {
//     Serial.print ("ERROR NO. ="); // Can not communicate with GP2Y0E03
//     Serial.println (ans);
//   }
//   delay (500); // repeat after 200 ms
// }
/*
* LAB Name: Arduino I2C Scanner
* Author: Khaled Magdy
* For More Info Visit: www.DeepBlueMbedded.com
*/
// #include <Wire.h>
 
// void setup()
// {
//   Serial.begin(9600);
//   Wire.begin();
// }
// void loop()
// {
//   byte error, address;
//   int nDevices;
//   Serial.println("Scanning...");
//   nDevices = 0;
//   for(address = 1; address < 127; address++ )
//   {
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     if (error == 0)
//     {
//       Serial.print("I2C device found at address 0x");
//       if (address<16)
//       { Serial.print("0"); }
//       Serial.print(address,HEX);
//       Serial.println("  !");
//       nDevices++;
//     }
//     else if (error==4)
//     {
//       Serial.print("Unknown error at address 0x");
//       if (address<16)
//       { Serial.print("0"); }
//       Serial.println(address,HEX);
//     }    
//   }
//   if (nDevices == 0)
//   { Serial.println("No I2C devices found\n"); }
//   else
//   { Serial.println("done\n"); }
//   delay(5000);           // wait 5 seconds for next scan
// }