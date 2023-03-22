# LPG Gas Detection & Alert System (NETWORK) using Arduino UNO and MCP2515 CAN Bus

## Introduction
The proposed system for the LPG Detection and alert system is a Transmitter-Receiver kind of system with Arduino UNO functioning as the Micro-controller on both ends (Transmitter and Receiver) which are connected to each other using MCP2525 Can Bus module.

The transmitter end consists of a MQ-6 Gas sensor for gas detection, I2C 16x2 LCD to display the present status, Buzzer for alert in case of any leakage and finally a CAN bus to send the message to the receiver in case of any leakage detected. 

The receiver end consists of a CAN bus which monitors for a message from the transmitter which will be read, noted and communicated to the Arduino on the side for further processing. Based on the address (Transmitter address), the message received and the data received, the arduino uses I2C 16x2 LCD Display to display alert and with help of GSM Module, an alert SMS will be sent  to corresponding phone number related to the transmitter from which the leakage data is received.

The important feature of the system is that a single receiver can work with multiple transmitters placed within 1000m from the receiver. This draws the major difference from the existing systems. 

## Circuit
![Circuit](https://user-images.githubusercontent.com/128481323/226898446-3eebe813-af96-4b76-8980-aaa111cd07d5.png)
