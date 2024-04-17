//Gate threshold voltage of the IRFZ44N N-Channel MOSFET. 
#define mosfet_thresh_low = 2;
#define mosfet_thresh_high = 5;

int mosfet_gate = 13;
int voltage = 2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(mosfet_gate, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = (255./5.) * voltage;
  // analogWrite(mosfet_gate, t);
  // Serial.println(t * (5./255.));
  digitalWrite(mosfet_gate, HIGH);
  delay(7000);
  digitalWrite(mosfet_gate, LOW);
  delay(7000);

}
