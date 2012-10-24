#include <Wire.h>
#include <nunchuk.h>



void setup()
{
  Serial.begin(19200);
  Serial.print ("Begin of setup\n");
  nunchuck_setpowerpins(); // use analog pins 2&3 as fake gnd & pwr
  nunchuck_init(); // send the initilization handshake
  Serial.print ("End of setup\n");

  Nunchuk test();
}

void loop()
{
  nunchuck_get_data();
  nunchuck_print_data();
  delay(100);
}


//
// Nunchuck functions
//

static uint8_t nunchuck_buf[6];   // array to store nunchuck data,
uint8_t ctrlr_type[6]; 


// Uses port C (analog in) pins as power & ground for Nunchuck
static void nunchuck_setpowerpins()
{
  
}

// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
void nunchuck_init()
{ 

}

// Send a request for data to the nunchuck
// was "send_zero()"
void nunchuck_send_request()
{

}

// Receive data back from the nunchuck, 
int nunchuck_get_data()
{

}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void nunchuck_print_data()
{ 

}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char nunchuk_decode_byte (char x)
{

}


