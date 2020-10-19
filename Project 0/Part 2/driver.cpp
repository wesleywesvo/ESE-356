#include "driver.h"

void driver::prc_driver() {
	//INITIAL
	d_in1 = 14;
	d_in2 = 5;
	d_in3 = 0;
	d_load1 = true;
	d_load2 = true;
	wait(5, SC_NS);

	d_dec1 = true;
	d_dec2 = false;

	//counters are loaded, only counter1 should be decreasing by 1
	//wait one cycle to stop load
	d_load1 = false;
	d_load2 = false;
	wait(55, SC_NS);

	//load counter1 again - dec1 already asserted
	//decrease counter2
	d_in1 = 7;
	d_in2 = 8;
	d_in3 = 3;
	d_load1 = true;
	d_load2 = true;
	wait(5, SC_NS);
	d_dec2 = true;
	d_load1 = false;
	d_load2 = false;
}