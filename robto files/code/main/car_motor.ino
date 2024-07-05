#include <Servo.h>
#include <WiFiS3.h>
#include <SPI.h>
#include <ArduinoJson.h>

// 1 - Control through server 
// 2 - Obstacle detection
//Each mode is independed from one another so if you want to control 
//  the robot, you cannot use the obstacle detection, and vise versa.
#define CONTROL_MODE 2
#define URL "192.168.62.31"//"10.0.0.7"
#define GATE_VOLTAGE 2
#define MOTOR_POWER 120
#define MOTOR_DRIVE_TIME 300

//WiFi Credentials
const char *ssid  = "";
const char *pw = "";

//Connection status
int status = WL_IDLE_STATUS;

WiFiClient client;

//Server characteristics
const char server_url[] = URL;
const String server_path = "/smokerV2";
const int server_port = 5000;

//0: Default no sensor
//1: IR
//2: Ultrasonic
//3: GPY IR sensor
unsigned short int sensor_type = 0;

const unsigned short int ena3 = 7;
const unsigned short int ena4 = 8;
const unsigned short int enb = 9;

const unsigned short int ena1 = 5;
const unsigned short int ena2 = 4;
const unsigned short int ena = 6;

//Servo motor pin
Servo servo;
const unsigned short int servo_pin = 3;
unsigned int servo_pos = 0;
int count = 1;

//Sensors
const unsigned short int echo = 11;
const unsigned short int trig = 12;

//MOSFET control pin. When this pin goes low, the servo motor will be shut off
const unsigned short int fetty_pin = 10;

const unsigned short int mode_pin = 13;

//Input fromn the server. Lmotor and Rmotor will control the direction of the
//  motors with 1 being forward and -1 being back. Speed is a number from... hmmm
//  let's say 1 to 10 which will deliver 10 levels of power to the wheels cause
//  those things are speedy gonzales
short int lmotor;
short int rmotor;
short int lspeed;
short int rspeed;

//Store distance measurements for each angle
float distances[7];

void setup() {

  Serial.begin(9600);

  pinMode(mode_pin, OUTPUT);
  digitalWrite(mode_pin, HIGH);

  //motor 1
  pinMode(ena3, OUTPUT);
  pinMode(ena4, OUTPUT);
  pinMode(enb, OUTPUT);

  //motor 2
  pinMode(ena1, OUTPUT);
  pinMode(ena2, OUTPUT);
  pinMode(ena, OUTPUT);
  
  setup_ultrasonic();
  
  pinMode(fetty_pin, OUTPUT);
  trig_mosfet(true);

  //Atttach and reset servo
  pinMode(servo_pin, OUTPUT);
  servo.attach(servo_pin);
  servo.write(90);

  //Check if firmware is updated
  String firm_v = WiFi.firmwareVersion();
  if (firm_v < WIFI_FIRMWARE_LATEST_VERSION){
    Serial.println("Firmware needs to be upgraded");
  }

  //Connect to Wifi, if not connected, keep trying
  while (status != WL_CONNECTED){
    Serial.println("Connecting...");
    status = WiFi.begin(ssid, pw);
    delay(10000);
  } Serial.println("connected");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  if (analogRead(A0) > 500){
    Serial.println("------------------Manual control mode------------------");
    int req = make_request();
    if (req == -1){
      Serial.println("Error in request");
    } else if (req == -2){
      Serial.println("Error in deserialization");
    }
    int l_speed_conv = 24 * lspeed;
    int r_speed_conv = 24 * rspeed;

    if (lmotor == 1 && rmotor == 1){
      move_forward(l_speed_conv, r_speed_conv);
    } else if (lmotor == -1 && rmotor == -1){
      move_backward(l_speed_conv, r_speed_conv);
    } else if (lmotor == 0 && rmotor == 0){
      move_forward(0, 0);
    } else if (lmotor == 1 && rmotor == -1){
      rotate_right(l_speed_conv, r_speed_conv);
    } else if (lmotor == -1 && rmotor == 1){
      rotate_left(l_speed_conv, r_speed_conv);
    }

    delay(50);
    
  } else if (analogRead(A0) <= 500){
    Serial.println("------------------Detection mode------------------");
    //Scan around and fill a list of floats to populate it with the distance
    //  measurements for different angles.
    look_around();

    for (int i=0; i<sizeof(distances)/sizeof(float); i++){
      Serial.println(distances[i]);
    }

    //Post the data to the Flask server
    post_data();

    //Set servo to middle position in order to now scan what is in front
    servo.write(90);

    //It gradually increases the power to the motors to move them forward while
    //  the distance reading in the direct front of the robot is greater than
    //  20cm. It checkes every 2 milliseconds using the delay_and_check function.
    //  If the distance in the direct front of the robot is less than 20cm 
    //  during one of the checks, stop the motors and break the loop
    if (distances[4] > 20.){
      while (detect_ultrasonic() > 20.){
        for (int i = 0; i < MOTOR_POWER; i++){
          move_forward(MOTOR_POWER, MOTOR_POWER);
          delay(10);
        } move_forward(0, 0);

        if (delay_and_check(500) == 1){
          break;
        }
      } move_forward(0, 0);
    } 
    //Otherwise we determine which reading has the highest distance. Since
    //  the robot scans from its relative right to its left then the first 
    //  reading at index 0 is always the one that is furthest right while
    //  the leftmost reading is at the last index. Rotation is just taking
    //  a guesstimate as to how long to rotate left/right for. What I do is
    //  just multiply a constant by how many indexes away we are from the 
    //  center index.
    else {
      move_forward(0, 0);
      Serial.println("Stop");

      //Index of max reading 
      int index = find_max_index();
      Serial.print("Index: ");
      Serial.println(index);

      //In the case that we are against a wall or very close to something that 
      //  would make all our readings be below the threshold to activate th e
      //  motors, move back to create some space.
      if (distances[index] < 20.){
        move_backward(MOTOR_POWER, MOTOR_POWER);
        Serial.println("moving back");
        delay(750);
        move_forward(0, 0);
      }

      if (4 - index > 0){
        rotate_right(MOTOR_POWER, MOTOR_POWER);
        Serial.println("Rotating right");
        delay(250 * (abs(4-index)));
        move_forward(0, 0);
      } 
      
      else if (4 - index <= 0){
        rotate_left(MOTOR_POWER, MOTOR_POWER);
        Serial.println("Rotating left");
        delay(250 * (abs(4-index)));
        move_forward(0, 0);
      }
    }
  }
}

//move stubber forward
void move_backward(int power_left, int power_right){
  digitalWrite(ena3, HIGH);
  digitalWrite(ena4, LOW);

  digitalWrite(ena2, LOW);
  digitalWrite(ena1, HIGH);

  analogWrite(enb, power_right);
  analogWrite(ena, power_left);
}

//move stubber backwards
void move_forward(int power_left, int power_right){
  digitalWrite(ena3, LOW);
  digitalWrite(ena4, HIGH);

  digitalWrite(ena2, HIGH);
  digitalWrite(ena1, LOW);

  analogWrite(enb, power_right);
  analogWrite(ena, power_left);
}

//rotate stubber left
void rotate_left(int power_left, int power_right){
  digitalWrite(ena3, LOW);
  digitalWrite(ena4, HIGH);

  digitalWrite(ena2, LOW);
  digitalWrite(ena1, HIGH);

  analogWrite(enb, power_right);
  analogWrite(ena, power_left);
}

void rotate_right(int power_left, int power_right){
  digitalWrite(ena3, HIGH);
  digitalWrite(ena4, LOW);

  digitalWrite(ena1, LOW);
  digitalWrite(ena2, HIGH);

  analogWrite(enb, power_right);
  analogWrite(ena, power_left);
}

int detect_sensor(int pin){
  int reading = digitalRead(pin);
  char pin_number = (char) pin;
  if (reading == 0){
    //Serial.print("Motion on sensor ");
    //Serial.println(pin);
    return 0;
  } return 1;
}

void setup_ultrasonic(){
  if (sensor_type == 0){
    sensor_type = 2;
  } 
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
}

float detect_ultrasonic(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  float duration = pulseIn(echo, HIGH);

  float distance = 0.017 * duration;

  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

//Send HTTP GET resquest to flask server. 
int make_request(){
  String jsonData = "";
  if (client.connect(server_url, server_port)){
    Serial.println("Making request");
    client.println("GET " + server_path + " HTTP/1.1");
    client.println("Host: " + String(server_url));
    client.println("Connection: close");
    client.println();
    while (client.connected()){
      if (client.available()){  
        char c = client.read();
        jsonData += c;
      }
    }  
    //Serial.println(jsonData);
  } else {
    Serial.println("No Connection to server");
    return -1;
  }
  client.stop();
  Serial.println("disconnected");

  String d = process_response(jsonData);
  //Serial.println(d);
  StaticJsonDocument<256> jdoc;
  DeserializationError error = deserializeJson(jdoc, d);
  if (error){
    return -2;
  }

  Serial.print("lmotor: ");
  lmotor = jdoc["lmotor"];
  Serial.println(lmotor);

  Serial.print("rmotor: ");
  rmotor = jdoc["rmotor"];
  Serial.println(rmotor);

  Serial.print("left speed: ");
  lspeed = jdoc["lspeed"];
  Serial.println(lspeed);

  Serial.print("right speed: ");
  rspeed = jdoc["rspeed"];
  Serial.println(rspeed);
}

//Send HTTP POST request to flask server
int post_data(){
  String json;
  int angles[] = {45, 60, 75, 90, 105, 120, 135};

  JsonDocument jdoc;
  jdoc["type"] = 1;
  for (int i = 0; i < sizeof(distances)/sizeof(float); i++){
    String name = "angle" + String(angles[i]);
    jdoc[name] = distances[i];
    // JsonObject obj = jdoc.add<JsonObject>();
    // obj["key"] = name;
    // obj["value"] = distances[i];
    Serial.println("Key: " + name);
    Serial.println("Value: " + String(distances[i]));
  }

  if (jdoc.overflowed()){
    Serial.println("JSON doc overflowed. Increase size");
    return -2;
  } else {
    serializeJson(jdoc, json);
  }

  if (client.connect(server_url, server_port)){
    Serial.println("Posting data to server");
    client.println("POST " + server_path + " HTTP/1.1");
    client.println("Host: " + String(server_url));
    client.println("Content-Type: application/json");
    client.println("Connection: Close");
    client.println("Content-Length: " + String(json.length()));
    client.println();
    Serial.println(json);
    client.println(json);
  } else {
    Serial.println("No connection to server");
    return -1;
  }
  client.stop();
}

//Cut out all the non JSON stuff out of the response string
String process_response(String data){
  int index2 = data.lastIndexOf("}") + 1;
  int index1 = data.indexOf("{");


  return data.substring(index1, index2);
}

//Changle boolean value of variable to determine if mosfet will be on or 
//  off. Only peripherals not controlled by the mosfet are the wheels which
//  we will jus write 0 to their speed.
void trig_mosfet(bool turn_on){
  if (turn_on){
    digitalWrite(fetty_pin, HIGH);
  } else {
    digitalWrite(fetty_pin, LOW);
  }
  move_forward(0, 0);
}

//Move the servo to  different angles and 
void look_around(){
  float reading;
  int angles[] = {45, 60, 75, 90, 105, 120, 135};

  for (int i=0; i<sizeof(angles)/sizeof(int); i++){
    servo.write(angles[i]);
    Serial.println("Angle: " + String(angles[i]));
    delay(500);
    reading = detect_ultrasonic();
    distances[i] = reading;
    delay(500);
  }

  servo.write(45);
}

//Return the index of the smallest element in the list of distance
//  measurements
int find_min_index(){
  int min_index = 0;
  for (int i=0; i<sizeof(distances)/sizeof(float); i++){
    if (distances[i] < distances[min_index]){
      min_index = i;
    }
  } return min_index;
}

int find_max_index(){
  int max_index = 0;
  for (int i=0; i<sizeof(distances)/sizeof(float); i++){
    if (distances[i] > distances[max_index]){
      max_index = i;
    } 
  } return max_index;
}

int delay_and_check(int duration){
  for (int i = 0; i < duration/2; i++){
    if (detect_ultrasonic() < 20.0){
      return 1;
    } delay(2);
  } return 0;
}


