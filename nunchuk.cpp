// include this library's description file
#include "Nunchuk.h"


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
Nunchuk::Nunchuk(void)
{

}


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
void Nunchuk::init(void)
{
  byte cnt;
  
  Wire.begin();
 
  // init controller 
  write_reg(0x52,0xF0,0x55);
  write_reg(0x52,0xFB,0x00);
  
  // read the extension type from the register block         
  write_reg(0x52,0xFA);
  
  Wire.beginTransmission(0x52);
  Wire.requestFrom(0x52, 6);               // request data from controller
  for (cnt = 0; cnt < 6; cnt++) {
    if (Wire.available()) {
      ctrlr_type[cnt] = Wire.read(); // Should be 0x0000 A420 0101 for Classic Controller, 0x0000 A420 0000 for nunchuck
    }
  }
  Wire.endTransmission();
  
  
  // send the crypto key (zeros), in 3 blocks of 6, 6 & 4.
  write_reg(0x52,0xF0,0xAA); //sends crypto enable notice
  send_zero();
  send_zero();
  send_zero();
}

// Sets some Arduino Pins to power the nunchuk
// due to be scrapped, because of lack of usefullness and 
// interuption of coding style
void Nunchuk::setpowerpins(void)
{
#define pwrpin PORTC3
#define gndpin PORTC2
  DDRC |= _BV(pwrpin) | _BV(gndpin);
  PORTC &=~ _BV(gndpin);
  PORTC |=  _BV(pwrpin);
  delay(100);  // wait for things to stabilize        
}


// Reads Data from nunchuk and decodes it
int Nunchuk::get_data(void)
{
  int cnt=0;
  Wire.requestFrom (0x52, 6);      // request data from nunchuck
  while (Wire.available ()) {
    // receive byte as an integer
    nunchuck_buf[cnt] = decode_byte(Wire.read());
    cnt++;
  }
  write_reg(0x52,0x00);// send request for next data payload
  
  // If we recieved the 6 bytes, then decode them 
  if (cnt >= 5) {
     // information taken from wiibrew.org
    joy_x_axis = nunchuck_buf[0];
    joy_y_axis = nunchuck_buf[1];
    accel_x_axis = (nunchuck_buf[2] << 2) | ((nunchuck_buf[5] >> 2) & 0b00000011);
    accel_y_axis = (nunchuck_buf[3] << 2) | ((nunchuck_buf[5] >> 4) & 0b00000011);
    accel_z_axis = (nunchuck_buf[4] << 2) | ((nunchuck_buf[5] >> 6) & 0b00000011);
    z_button = ((nunchuck_buf[5] >> 0) & 1);
    c_button = ((nunchuck_buf[5] >> 1) & 1);
    return 1;   // success
  }
  return 0; //failure
}

void Nunchuk::print_data(void)
{
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

}

// private functions only available to other functions in this library

//Write a Byte (reg_value) to a Register (reg_adress)
void Nunchuk::write_reg(byte device_ID, byte reg_adress, byte reg_value)
{
  Wire.beginTransmission(device_ID);
  Wire.write(reg_adress);
  Wire.write(reg_value);
  Wire.endTransmission();
}

//overloaded if one only wants to write the reg_adress
void Nunchuk::write_reg(byte device_ID, byte reg_adress)
{
  Wire.beginTransmission(device_ID);
  Wire.write(reg_adress);
  Wire.endTransmission();
}

//sends 6 times 0x00
void Nunchuk::send_zero(void)
{
  byte i;
  Wire.beginTransmission(0x52);
  Wire.write(0x40);                    // sends memory address
  for (i = 1; i < 6; i++) {
    Wire.write((byte)0x00);                    // sends 2nd key block (zeros)
  }
  Wire.endTransmission();
}

// Functions only available to other functions in this library
char Nunchuk::decode_byte(char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}

