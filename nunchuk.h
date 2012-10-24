/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef Nunchuk_h
#define Nunchuk_h

#include <inttypes.h>

// library interface description
class Nunchuk
{
  // user-accessible "public" interface
  public:
    Nunchuk(void);
	void setpowerpins(void);
	void init(void);
	void send_request(void);
	int get_data(void);
	char decode_byte(char x);
	void print_data(void);
	uint8_t nunchuck_buf[6];
	uint8_t ctrlr_type[6]; 
};

#endif

