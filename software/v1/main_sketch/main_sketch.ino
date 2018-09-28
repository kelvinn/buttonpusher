
#include <Servo.h>
#include <SoftwareSerial.h>  

int bluetoothTx = 2;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 3;  // RX-I pin of bluetooth mate, Arduino D3

Servo myservo;  // create servo object to control a servo
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int val = 0;    // variable to store the servo position
long randNumber;
char bluetoothCmd = 'A';

void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

//  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
//  bluetooth.print("$");  // Print three times individually
//  bluetooth.print("$");
//  bluetooth.print("$");  // Enter command mode
//  delay(100);  // Short delay, wait for the Mate to send back CMD
//  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
//  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
//  bluetooth.begin(9600);  // Start bluetooth serial at 9600
//

  
}

void push_button() {
  myservo.write(5);                  // Moves servo forward to push button
  delay(500);                        // Wait for 500ms
  myservo.write(175);                 // Move back
  delay(10000);                      // TODO remove when bluetooth is hooked up
}

void loop() {
  // this is just for testing now
  push_button();
  
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    bluetoothCmd = (char)bluetooth.read();
    if (bluetoothCmd == 'Z') {
      push_button();
    }   
  }

}
