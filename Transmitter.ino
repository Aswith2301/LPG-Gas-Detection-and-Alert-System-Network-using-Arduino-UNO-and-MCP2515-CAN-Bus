#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <mcp2515.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

const int buzzer = 5;  // buzzer to arduino pin 5
int GAS_VAL = 0;       // decalre and initialise variables

struct can_frame canMsg;  // decalre struct data type for storing the CAN message
MCP2515 mcp2515(10);      // set PIN for SPI CS

void setup() {

  Serial.begin(9600);  // initialize serial console for reading or writing on PC and setting its baud rate

  lcd.init();                     // initialize the lcd
  lcd.backlight();                // turn LCD back light on
  lcd.setCursor(0, 0);            // set cursor on lcd (column, row)
  lcd.print("Initialising....");  // print on lcd screen
  lcd.setCursor(0, 1);
  lcd.print("Device at Room 1");
  delay(2000);                    // stops running for value mentioned (milli seconds)
  lcd.clear();

  pinMode(A1, INPUT);  // set analog pin A1 to read data from MQ-6 Gas Sensor (Input)

  pinMode(buzzer, OUTPUT);  // set digital pin 5 as an Output

  SPI.begin();                                // initialize SPI bus (Intiate SPI COmmunication)
  mcp2515.reset();                            // reset MCP2515
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);  // set MCP2515 speed to 500kbps and clock frequency to 8MHz
  mcp2515.setNormalMode();                    // set MCP2515 to normal mode
}

void loop() {

  GAS_VAL = analogRead(A1);  // read and store the gas level

  Serial.print("GAS_VAL : ");
  Serial.println(GAS_VAL);    // print Gas Value on PC screen

  if (GAS_VAL > 300) {

    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Leakage Detected");
    lcd.setCursor(0, 1);
    lcd.print("Gas Value:");
    lcd.setCursor(10, 1);
    lcd.print(GAS_VAL);

    tone(buzzer, GAS_VAL * 5);  // scaled frequency based on gas detected at sensor

    canMsg.can_id = 0xAA;           // set CAN Id to 0xAA
    canMsg.can_dlc = 1;             // set CAN data length to 1
    canMsg.data[0] = GAS_VAL / 5;   // update gas value at [0]
                                    // (divide by 5 at transmitter and multiply by 5 at receiver for rounding off to 5's)
    mcp2515.sendMessage(&canMsg);   // send CAN message

    delay(5000);

    lcd.clear();  // clear the LCD Screen
  }


  if (GAS_VAL < 300) {

    lcd.noBacklight();  //turn LCD back light off
    lcd.setCursor(0, 0);
    lcd.print("AT Normal Level");
    delay(100);

    noTone(buzzer);  //turn buzzer off
  }
  
}