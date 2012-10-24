#include <Wire.h>
#include <nunchuk.h>



  
void setup()
{
  Nunchuk nunchuk;
  Serial.begin(19200);
  Serial.print ("Begin of setup\n");
  nunchuck.setpowerpins(); // use analog pins 2&3 as fake gnd & pwr
  nunchuck.init(); // send the initilization handshake
  Serial.print ("End of setup\n");


}

void loop()
{
  nunchuck.get_data();
  nunchuck.print_data();
  delay(100);
}




