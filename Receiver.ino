#include <SPI.h>
#include <mcp2515.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

struct can_frame canMsg;  // decalre struct data type for storing the CAN message
MCP2515 mcp2515(10);      // set PIN for SPI CS

SoftwareSerial mySerial(3, 2);  // set pins for communication with GSM Module

void setup() {

  Serial.begin(9600);  // initialize serial console for reading or writing on PC and setting its baud rate

  lcd.init();                     // initialize the lcd
  lcd.backlight();                // use to turn on and turn off LCD back light
  lcd.setCursor(0, 0);            // set cursor on lcd (column, row)
  lcd.print("Initialising....");  // print on lcd screen
  delay(2000);                    // stops running for value mentioned (milli seconds)
  lcd.clear();  // clear the LCD Screen


  SPI.begin();                                // initialize SPI bus (Intiate SPI COmmunication)
  mcp2515.reset();                            // reset MCP2515
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);  // set MCP2515 speed to 500kbps and clock frequency to 8MHz
  mcp2515.setNormalMode();                    // set MCP2515 to normal mode

  mySerial.begin(9600);  // set the baud rate of GSM Module
}


void loop() {

  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {      // receive data from CAN bus

    if (canMsg.can_id == 0xAA) {    // read data received from CAN bus (or transmitter) with address 0xAA 

      int x = canMsg.data[0] * 5;     // read data from CAN message and storing it to x

      int y = 1;  //store room or flat number (actual address where respective transmitter is placed)

      String Ph = "+917303341730";   // store 1st phone number to string Ph
      String Ph2 = "+918510009486";  // store 2nd phone number to string Ph2
       
      alert(Ph, Ph2, x, y);   //call alert function for alerting on LCD, PC serial monitor (if connected) and SMS
    }



    if (canMsg.can_id == 0xAB) {   // to read data received from CAN bus (or transmitter) with address 0xAB

      int x = canMsg.data[0] * 5;

      int y = 2;

      String Ph = "+917569987098";   // store 1st phone number to string Ph
      String Ph2 = "+919849922663";  // store 2nd phone number to string Ph2
       
      alert(Ph, Ph2, x, y);   //call alert function for alerting on LCD, PC serial monitor (if connected) and SMS
    }
  }

  lcd.noBacklight();  // turn LCD back light off
  lcd.setCursor(0, 0);
  lcd.print("No Leakage");
  lcd.setCursor(0, 1);
  lcd.print("Near any Sensor");
  delay(10);
}

void alert(String Ph, String Ph2, int x, int y){      //define alert function for alerting on LCD, PC serial monitor (if connected) and SMS
  
  alertDisp(x, y);    // call function for alert display on LCD and PC serial monitor
  delay(500);

  SendSMS(Ph, x, y, 1);         // call function to send alert SMS to 1st phone number
  delay(1000);
  SendSMS(Ph2, x, y, 2);        // call function to send alert SMS to 2nd phone number
}


void SendSMS(String Ph, int x, int y, int z) {       // define function for sending SMS using GSM Module

  SMSstatDisp(z);   // call function to display SMS sending status

  String ATnum = "AT+CMGS=\"" + Ph + "\"";  // define string with AT command to send SMS along with phone number

  mySerial.println("AT+CMGF=1\r");  // set the GSM Module in Text Mode
  delay(500);                     

  mySerial.println(ATnum);                     // send SMS (using string with AT command and phone number fed earlier)
  delay(500);
  mySerial.print("Detected leakage at Room");  // SMS Text
  mySerial.println(y);                         // SMS Text
  mySerial.print("Gas Value");                 // SMS Text
  mySerial.println(x);                         // SMS Text

  alertDisp(x, y);

  mySerial.println((char)26);  // ASCII code of CTRL+Z
  delay(3500);

  SMSsentDisp(z);    //  call function to display SMS sent status
}



void alertDisp(int x, int y){      //  define function for displaying alert, gas value on LCD and PC serial monitor

  Serial.print("Detected leakage at Room ");       //
  Serial.println(y);                               // display alert message on PC screen

  Serial.print("GAS_VAL : ");    //
  Serial.println(x);             // display gas value on PC screen

  lcd.clear();  // clear the LCD Screen
  lcd.backlight();                  //
  lcd.setCursor(0, 0);              //
  lcd.print("Leakage Detected");    //
  lcd.setCursor(0, 1);              //
  lcd.print("At R");                //
  lcd.setCursor(4, 1);              //
  lcd.print(y);                     // display alert message on LCD
  lcd.setCursor(5, 1);              //
  lcd.print(", Val: ");             //
  lcd.setCursor(12, 1);             //
  lcd.print(x);                     // display gas value on LCD
  delay(100);                        
}


void SMSstatDisp(int z){        //define function to display SMS sending status

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending Alert");
  lcd.setCursor(0, 1);
  lcd.print("SMS ");
  lcd.setCursor(4, 1);
  lcd.print(z);
  lcd.setCursor(5, 1);
  lcd.print(".....");

  Serial.print("Sending Alert SMS ");
  Serial.println(z);
}


void SMSsentDisp(int z){        //define function to display SMS sent status

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alert SMS ");
  lcd.setCursor(10, 0);
  lcd.print(z);
  lcd.setCursor(0, 1);
  lcd.print("Sent.....");

  Serial.print("Alert SMS ");
  Serial.print(z);
  Serial.println(" sent...");

  delay(1000);
}


