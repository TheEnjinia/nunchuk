/*
  Test.h - Test library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
#if defined(ARDUINO) && ARDUINO >= 100 //this switches between Arduino 1.0 and previous versions
#include "Arduino.h"
#else
#include "WProgram.h"
#include "WConstants.h"
#endif

// include this library's description file
#include "Nunchuk.h"

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

#include <Wire.h>

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Nunchuk::Nunchuk(void)
{

}


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Nunchuk::init(void)
{
  /*Wire.begin();                      // join i2c bus as master
   Wire.beginTransmission(0x52);      // transmit to device 0x52
   Wire.write(0x40);            // sends memory address
   Wire.write(0x00);            // sends sent a zero.  
   Wire.endTransmission();      // stop transmitting*/

  Serial.print ("Begin nunchuck init\n");
  byte cnt;
  Wire.begin();

  Serial.print ("Step 1\n");
  // init controller
  delay(1);
  Wire.beginTransmission(0x52);      // device address
  Wire.write(0xF0);                    // 1st initialisation register
  Wire.write(0x55);                    // 1st initialisation value
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(0x52);
  Wire.write(0xFB);                    // 2nd initialisation register
  Wire.write((byte)0x00);                    // 2nd initialisation value
  Wire.endTransmission();
  delay(1);

  Serial.print ("Step 2\n");
  // read the extension type from the register block        
  Wire.beginTransmission(0x52);
  Wire.write(0xFA);                    // extension type register
  Wire.endTransmission();
  Wire.beginTransmission(0x52);
  Wire.requestFrom(0x52, 6);               // request data from controller
  for (cnt = 0; cnt < 6; cnt++) {
    if (Wire.available()) {
      ctrlr_type[cnt] = Wire.read(); // Should be 0x0000 A420 0101 for Classic Controller, 0x0000 A420 0000 for nunchuck
    }
  }
  Wire.endTransmission();
  delay(1);

  Serial.print ("Step 3\n");
  // send the crypto key (zeros), in 3 blocks of 6, 6 & 4.
  Wire.beginTransmission(0x52);
  Wire.write(0xF0);                    // crypto key command register
  Wire.write(0xAA);                    // sends crypto enable notice
  Wire.endTransmission();
  delay(1);
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // crypto key data address
  for (cnt = 0; cnt < 6; cnt++) {
    Wire.write((byte)0x00);                    // sends 1st key block (zeros)
  }
  Wire.endTransmission();
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // sends memory address
  for (cnt = 6; cnt < 12; cnt++) {
    Wire.write((byte)0x00);                    // sends 2nd key block (zeros)
  }
  Wire.endTransmission();
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // sends memory address
  for (cnt = 12; cnt < 16; cnt++) {
    Wire.write((byte)0x00);                    // sends 3rd key block (zeros)
  }
  Wire.endTransmission();
  delay(1);
  // end device init  
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Nunchuk::setpowerpins(void)
{
#define pwrpin PORTC3
#define gndpin PORTC2
  DDRC |= _BV(pwrpin) | _BV(gndpin);
  PORTC &=~ _BV(gndpin);
  PORTC |=  _BV(pwrpin);
  delay(100);  // wait for things to stabilize        
}

// Functions only available to other functions in this library

int Nunchuk::get_data(void)
{
  int cnt=0;
  Wire.requestFrom (0x52, 6);      // request data from nunchuck
  while (Wire.available ()) {
    // receive byte as an integer
    nunchuck_buf[cnt] = decode_byte(Wire.read());
    cnt++;
  }
  send_request();  // send request for next data payload
  // If we recieved the 6 bytes, then go print them
  if (cnt >= 5) {
    return 1;   // success
  }
  return 0; //failure
}

// Functions only available to other functions in this library

void Nunchuk::print_data(void)
{
  static int i=0;
  int joy_x_axis = nunchuck_buf[0];
  int joy_y_axis = nunchuck_buf[1];
  int accel_x_axis = nunchuck_buf[2]; // * 2 * 2; 
  int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
  int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;

  int z_button = 0;
  int c_button = 0;

  // byte nunchuck_buf[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((nunchuck_buf[5] >> 0) & 1) 
    z_button = 1;
  if ((nunchuck_buf[5] >> 1) & 1)
    c_button = 1;

  if ((nunchuck_buf[5] >> 2) & 1) 
    accel_x_axis += 2;
  if ((nunchuck_buf[5] >> 3) & 1)
    accel_x_axis += 1;

  if ((nunchuck_buf[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((nunchuck_buf[5] >> 5) & 1)
    accel_y_axis += 1;

  if ((nunchuck_buf[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((nunchuck_buf[5] >> 7) & 1)
    accel_z_axis += 1;

  Serial.print(i,DEC);
  Serial.print("\t");

  Serial.print("joy:");
  Serial.print(joy_x_axis,DEC);
  Serial.print(",");
  Serial.print(joy_y_axis, DEC);
  Serial.print("  \t");

  Serial.print("acc:");
  Serial.print(accel_x_axis, DEC);
  Serial.print(",");
  Serial.print(accel_y_axis, DEC);
  Serial.print(",");
  Serial.print(accel_z_axis, DEC);
  Serial.print("\t");

  Serial.print("but:");
  Serial.print(z_button, DEC);
  Serial.print(",");
  Serial.print(c_button, DEC);

  Serial.print("\r\n");  // newline
  i++;
}

// Functions only available to other functions in this library

void Nunchuk::send_request(void)
{
  Wire.beginTransmission(0x52);      // transmit to device 0x52
  Wire.write((byte)0x00);            // sends one byte
  Wire.endTransmission();      // stop transmitting
}

// Functions only available to other functions in this library

char Nunchuk::decode_byte(char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}

