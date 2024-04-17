#include <Wire.h>
// Preliminary pass at getting GP2Y0E03 working.
// It appears to work but i haven’t the slightest idea how.
// The same command of Wire.read() is assumed to get two different sets of data.</p>
#define SENSOR_ADRS 0x40 // I2C address of GP2Y0E03
#define DISTANCE_ADRS 0x5E // Data address of Distance Value // Functions to process only at power-on and reset
void setup ()
{
  // Initialize serial communication
  Wire.begin(); // Initialize I2C,
  delay(1000); // start after 1 second
  Serial.begin(9600);
  // Main processing function repeatedly executed
  
}
void loop ()
{
  int ans;
  byte c[2];
  Wire.beginTransmission (SENSOR_ADRS); // start communication processing
  Wire.write (DISTANCE_ADRS); // specify the address of the table storing the distance value
  ans = Wire.endTransmission(); // send and close data
  delay(200);
  if (ans == 0) {
    ans = Wire.requestFrom(SENSOR_ADRS, 2) ;
    c[0] = Wire.read(); // Read the 11th to 4th bits of data c [1]
    c[1] = Wire.read(); // Read the 3rd and 0th bits of the data
    ans = ((c [0] * 16 + c [1]) / 16) / 4; // distance</p>
    Serial.print(ans);
    Serial.println ("cm"); //to display on serial monitor;
  }
  else {
    Serial.print ("ERROR NO. ="); // Can not communicate with GP2Y0E03
    Serial.println (ans);
  }
  delay (500); // repeat after 200 ms
}

// 7 highest bits

// #define ADDRESS         (0x80 >> 1)


// #define SHIFT_ADDR      0x35

// #define DISTANCE_ADDR   0x5E

// #define RIGHT_EDGE_ADDR 0xF8 // C

// #define LEFT_EDGE_ADDR  0xF9 // A

// #define PEAK_EDGE_ADDR  0xFA // B

// uint8_t distance_raw[2] = { 0 };
// uint8_t shift = 0;
// uint8_t distance_cm = 0;
// char buf[100];

// void setup()
// {
//   Wire.begin();
//   Serial.begin(9600);

//   delay(2000);

//   Serial.println("Initializing");

//   Wire.beginTransmission(ADDRESS);
//   Wire.write(byte(SHIFT_ADDR));
//   Wire.endTransmission();

//   Wire.requestFrom(ADDRESS, 1);
//   if (1 <= Wire.available())
//   {
//     shift = Wire.read();
//   }

//   Serial.print("Read shift bit: ");
//   Serial.println(shift, HEX);
// }

// void loop()
// {
//   // Read basic measurement
//   Wire.beginTransmission(ADDRESS);
//   Wire.write(byte(DISTANCE_ADDR));
//   Wire.endTransmission();

//   Wire.requestFrom(ADDRESS, 2);

//   if (2 <= Wire.available())
//   {
//     distance_raw[0] = Wire.read();
//     distance_raw[1] = Wire.read();

//     // Print distance in cm
//     distance_cm = (distance_raw[0] * 16 + distance_raw[1]) / 16 / (int)pow(2, shift);
//     sprintf(buf, "Distance %u cm", distance_cm);
//     Serial.println(buf);
//   }
//   else
//   {
//     Serial.println("Read error");
//   }
//   delay(1000);
// }
// const int sens = A0;

// void setup() {
//   Serial.begin(9600);
//   pinMode(sens, INPUT);
// }

// void loop() {
//   auto raw = analogRead(sens);
//   Serial.println(raw);
//   delay(100);
// }

// int IRSensor = 9; // connect IR sensor module to Arduino pin D9
// int IRsensor2 = 10; // connect LED to Arduino pin 13
// int IRSensor3 = 11;

// void setup(){
//   Serial.begin(9600); // Init Serial at 115200 Baud Rate.
//   Serial.println("Serial Working"); // Test to check if serial is working or not
//   pinMode(IRSensor, INPUT); // IR Sensor pin INPUT
//   pinMode(IRsensor2, INPUT);
//   pinMode(IRSensor3, INPUT);
// }

// void loop(){
//   int sensorStatus = digitalRead(IRSensor); // Set the GPIO as Input
//   if (sensorStatus == 0) // Check if the pin high or not
//   {
//     // if the pin is high turn off the onboard Led
//     Serial.println("Motion Detected on sensor 1!"); // print Motion Detected! on the serial monitor window
//   }
//   int sensorStatus2 = digitalRead(IRsensor2);
//   if (sensorStatus2 == 0) // Check if the pin high or not
//   {
//     // if the pin is high turn off the onboard Led
//     Serial.println("Motion Detected on sensor 2!"); // print Motion Detected! on the serial monitor window
//   }
//   int sensorStatus3 = digitalRead(IRSensor3);
//   if (sensorStatus3 == 0) // Check if the pin high or not
//   {
//     // if the pin is high turn off the onboard Led
//     Serial.println("Motion Detected on sensor 3!"); // print Motion Detected! on the serial monitor window
//   }
// }