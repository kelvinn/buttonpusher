
#include <Servo.h>
#include <SoftwareSerial.h>  

int bluetoothTx = 6;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 7;  // RX-I pin of bluetooth mate, Arduino D3

Servo myservo;  // create servo object to control a servo
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

const unsigned long period = 3000;  //the value is a number of milliseconds
unsigned long lastPush;  //some global variables available anywhere in the program
unsigned long currentMillis;
char bluetoothCmd = 'A';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  Serial.println("Running Setup Block");
  lastPush = millis();
  delay(1000); // Wait for bluetooth to start upZ
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  delay(100);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(165); // Move to default position

  
}

void push_button() {

  myservo.write(10);                  // Moves servo forward to push button
  delay(500);                        // Wait for 500ms
  myservo.write(165);                 // Move back

}

void loop() {

  currentMillis = millis();
  
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    bluetoothCmd = (char)bluetooth.read();
    if (bluetoothCmd) {
      bluetooth.print(bluetoothCmd);
      Serial.println(bluetoothCmd);
    }
    
    // Only push button if the last button was pushed more than a little while ago.
    // Also known as my daughter held down the 'Z' key...
    if (bluetoothCmd == 'Z' && currentMillis - lastPush >= period) {
        lastPush = currentMillis;
        push_button();

    }   
  }

}
