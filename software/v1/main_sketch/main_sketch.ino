
#include <Servo.h>
#include <SoftwareSerial.h>  
#include <CurieBLE.h>

int powerControl = 3; // D3 pin connected to MOSFET gate
int bluetoothTx = 6;  // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 7;  // RX-I pin of bluetooth mate, Arduino D3
int servoPin = 9;     // D9 pin connected to servo orange wire

Servo servo;          // create servo object to control a servo
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

const unsigned long period = 5000;  // Push button wait period in milliseconds
unsigned long lastPush;             // Store last push time
unsigned long currentMillis;        // Store current time
char bluetoothCmd = 'A';            // Read a char via bluetooth

void setup() {
  // The below code only runs once
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  Serial.println("Running Setup Block");
  
  pinMode(powerControl, OUTPUT); // powerControl connects to base pin of MOSFET
  
  lastPush = millis();
  delay(1000);              // Wait for bluetooth to start upZ
  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  bluetooth.print("$");     // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");     // Enter command mode
  
  delay(100);               // Short delay, wait for the Mate to send back CMD
  
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
                                  // 115200 can be too fast at times for 
                                  // NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);          // Start bluetooth serial at 9600
  
  delay(100);               // Short delay, wait for Mat to change baud rate

  Serial.println("Turning ON Mosfet");
  digitalWrite(powerControl, HIGH); // Turn on MOSFET
  servo.attach(servoPin);           // Attach servo
  servo.write(165);                 // Move to default position

  delay(500);               // Short delay to let servo return to default position

  // initialize the BLE hardware
  BLE.begin();
  BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  
  
  Serial.println("Turning OFF Mosfet");
  digitalWrite(powerControl, LOW);
  servo.detach();
  
}

void push_button() {

  Serial.println("Attaching Servo And Turning ON Mosfet");
  servo.attach(servoPin);
  digitalWrite(powerControl, HIGH);
  Serial.println("Servo Power is ON");
  delay(200);

  Serial.println("Pushing Button");
  servo.write(10);                  // Moves servo forward to push button
  delay(500);                       // Wait for 500ms
  servo.write(165);                 // Move back

  delay(1000);                      // Short delay to let servo return to default position
  
  Serial.println("Attaching Servo And Turning OFF Mosfet");
   digitalWrite(powerControl, LOW);
   servo.detach();
   Serial.println("Servo detached");

   delay(1000);                     // Let servo detach
}

void controlServo(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }
  
  while (peripheral.connected()) {
    // while the peripheral is connection

    // read the button pin
    int buttonState = digitalRead(buttonPin);

    if (oldButtonState != buttonState) {
      // button changed
      oldButtonState = buttonState;

      if (buttonState) {
        Serial.println("button pressed");

        // button is pressed, write 0x01 to turn the LED on
        ledCharacteristic.writeByte(0x01);
      } else {
        Serial.println("button released");

      }
    }
  }
}

void loop() {

  currentMillis = millis();

    // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // stop scanning
    BLE.stopScan();

    controlServo(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19b10000-e8f2-537e-4f6c-d104768a1214");
  }
  
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
