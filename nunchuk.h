/*
 nunchuk.h - library for usage of the Wii Nunchuk
 
 License: GNU General Public License V3        
 (C) Copyright 2012 Le-Minh Pham-Dang
 */

 
// ensure this library description is only included once
#ifndef Nunchuk_h
#define Nunchuk_h

// include core Wiring API
// this switches between Arduino 1.0 and previous versions
#if defined(ARDUINO) && ARDUINO >= 100 
#include "Arduino.h"
#else
#include "WProgram.h"
#include "WConstants.h"
#endif

// include description files for other libraries used (if any)
#include "HardwareSerial.h"
#include <Wire.h>
#include <inttypes.h>

// library interface description
class Nunchuk
{
 // user-accessible interface
 public:
  Nunchuk(void);
  void setpowerpins(void);
  void init(void);
  int get_data(void);
  void print_data(void);

  //Joystick data
  uint16_t joy_x_axis;
  uint16_t joy_y_axis;

  //Accelerometer data
  uint16_t accel_x_axis;
  uint16_t accel_y_axis; 
  uint16_t accel_z_axis; 

  //Push Buttons
  // 0 = pressed
  bool z_button;
  bool c_button;

 private:
  //array which holds all variables.
  uint8_t nunchuck_buf[6];

  uint8_t ctrlr_type[6];  
  void write_reg(byte device_ID, byte reg_adress, byte reg_value);
  void write_reg(byte device_ID, byte reg_adress);
  char decode_byte(char x);
  void send_zero(void);
};
#endif

