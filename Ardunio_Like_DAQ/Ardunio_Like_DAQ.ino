// This Code is written frm G. Porzio for chapter of 
// "LabVIEW - History, Applications, Current Trends and Perspectives", 
// ISBN 978-1-83968-841-6.
//----------------------------------------------------------------------
// Variable Declarations
int opCode = 0;           // incoming serial byte
int Ts=1000;              // Sampling time [msec]; inizialize at 1 second
String output;            // msg to send at serial port with analog value
// each bit corresponds a sensor, the value at 1 means the sensor is present the LSB bit = Analog Sensor in A0 
byte Sensors = 0;         
// each bit corresponds a Digital line, the value at 1 means that the line is configurated as input.
byte digital_line_OUT=0;  
// each bit corresponds a Digital line, the value at 1 means that the line is configurated as output. 
byte digital_line_IN=0;   
// each bit corresponds a PWD line, the value at 1 means that the line is. 
byte Analog_write_PWM_LB=0;
String input_from_LV="";  // String contains the op-code
int val;
void(* Reset)(void) = 0;  // is a code for software RESET
//----------------------------------------------------------------------
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // We use it to blink when Arduino recevices the string configuration from LaBView
   pinMode(LED_BUILTIN, OUTPUT); 
   digitalWrite(LED_BUILTIN, LOW);                             // we turn it OFF
    
  // start serial port at 2Mbit/sec and wait for port to open:
  Serial.begin(2000000);
  
  while (!Serial) {
    ; // wait for serial port to connect. 
  }
  // we send a msg to LabView
  Serial.println("connection estabilized");
  
  delay(500); //  wait 500 msecond
  /* the following code is used to configure, in this example, 
  the analog input A0-A5, the digial I/O (pin 2 and pin 4), and PWM line (pin 3)
  of course it is possible to expand it as want as you wan.
  We use in the follow "SWICTH CASE" statement The ASCII code:
  a,b,c,d,e,f to configure A0-A5;
  g,h to Configure the pin 2 and pin4 like digital in;
  i,l to Configure the pin 2 and pin4 like digital out;
  m to configure the pin 3 like digital out and we'll use in PWM mode;
  A,B,C,D to set the Ts (Sample rate) at 100 msec, 10msec, 1 msec, 100 usec;
  z to finish the setup().
    */
    Serial.flush();
    while (opCode!=122){ // exit with statement "z"
     // get incoming byte:
       opCode = Serial.read(); //reads the Bytes sequence on USB
       switch (opCode){
          case 97:  // "a"
          Serial.println("channel A configurated");
          bitSet(Sensors,0);
          blinking();
          break;
          case 98:  // "b"
          Serial.println("channel B configurated");
          bitSet(Sensors,1);
          blinking();
          break;
          case 99:  // "c"
          Serial.println("channel C configurated");
          bitSet(Sensors,2);
          blinking();
          break;
          case 100: // "d"
          Serial.println("channel D configurated");
          bitSet(Sensors,3);
          blinking();
          break;
          case 101: // "e"
          Serial.println("channel E configurated");
          bitSet(Sensors,4);
          blinking();
          break;    
          case 102: // "f"
          Serial.println("channel F configurated");
          bitSet(Sensors,5);
          blinking();
          break;
          case 103: // "g"
          Serial.println("Digital 2 is input");
          pinMode(2, INPUT_PULLUP);
          bitSet(digital_line_IN,0);
          blinking();
          break;
          case 104: // "h"
          Serial.println("Digital 4 is input");
          pinMode(4, INPUT_PULLUP);
          bitSet(digital_line_IN,1);
          blinking();
          break;
          case 105: // "i"
          Serial.println("Digital 2 is output");
          pinMode(2, OUTPUT);
          bitSet(digital_line_OUT,0);
          blinking();
          break;
          case 106: // "j"
          Serial.println("Digital 4 is output");
          pinMode(4, OUTPUT);
          bitSet(digital_line_OUT,1);
          blinking();
          break;
          case 107: // "k"
          Serial.println("Digital 3 in PWD mode");
          pinMode(3, OUTPUT);
          bitSet(Analog_write_PWM_LB,0);
          blinking();
          break;
          case 65:  // "A"
          Serial.println("set Ts at 100 msec");
          Ts=100;
          blinking();
          break;
          case 66:  // "B"
          Serial.println("set Ts at 10 msec");
          Ts=10;
          blinking();
          break;
          case 67:  // "C"
          Serial.println("set Ts at 1 msec");
          Ts=1;
          blinking();
          break;
          case 68:  // "D"
          Serial.println("set Ts at 500 ucsec");
          Ts=0,1;
          blinking();
          break;
          case -1:  // null
          case 10:  // "\n"
          case 122: // "z"
          break;
          default:
          Serial.println("channel error");
          break;
        }           
      }
  }
/* In the loop() code we use the bitRead to Know if the channel
 *  is configurated in the setup() statment
 */
void loop() {
output= String ("");
// Serial.println(Sensors);
 if (bitRead(Sensors, 0)==1){
      output+="#";
      output+= String (analogRead(A0));
  }
  if (bitRead(Sensors, 1)==1){
      output+="#";
      output+= String (analogRead(A1));
  }
    if (bitRead(Sensors, 2)==1){
      output+="#";
      output+= String (analogRead(A2));
  }
  if (bitRead(Sensors, 3)==1){
      output+="#";
      output+= String (analogRead(A3));
  }
  if (bitRead(Sensors, 4)==1){
      output+="#";
      output+= String (analogRead(A4));
  }
  if (bitRead(Sensors, 5)==1){
      output+="#";
      output+= String (analogRead(A5));
  }
  if (bitRead(digital_line_IN, 0)==1){
      output+="&";
      output+= String (digitalRead(2));
  }
  if (bitRead(digital_line_IN, 1)==1){
      output+="&";
      output+= String (digitalRead(4));
  }
Serial.println(output);

delay(Ts);

}

// blinking function- Called during the channel configuration
void blinking() {
  for (int i=0; i<2;i++){
    digitalWrite(LED_BUILTIN, HIGH);     // turn the LED on (HIGH is the voltage level)
    delay(150);                         // wait for 150 msecond
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(150);                       // wait for 150 msecond
                                     // wait for 150 msecond
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
    // get the new messagio from master
    String input_from_LV = Serial.readString();
    if (input_from_LV == "R\n"){
      Serial.println("Reset, you have to wait...");
      Reset();
    }
    if (input_from_LV == "D0_ON\n"){
      Serial.println("DIGITAL ZERO_ON");
      digitalWrite(2, HIGH);
    }
    if (input_from_LV == "D0_OFF\n"){
      Serial.println("DIGITAL ZERO_OFF");
      digitalWrite(2, LOW);
    }
    if (input_from_LV == "D1_ON\n"){
      Serial.println("DIGITAL UNO_ON");
      digitalWrite(4, HIGH);
    }
    if (input_from_LV == "D1_OFF\n"){
      Serial.println("DIGITAL UNO_OFF");
      digitalWrite(4, LOW);
    }
    if ((input_from_LV.substring(0,3)) == "PWM"){
      val=input_from_LV.substring(3,6).toFloat();
      analogWrite(3, val);
    }
    Serial.println(input_from_LV);
}
