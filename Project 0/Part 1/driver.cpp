#include "driver.h"

void driver::prc_driver() {
	d_reset = true;
	d_data = true;
	wait(10, SC_NS);
	d_data = false;
	wait(10, SC_NS);
	d_data = true;		//data_out = 1

	wait(20, SC_NS);
	d_reset = false;	//active low reset
	wait(10, SC_NS);
	d_reset = true;
	wait(10, SC_NS);
	d_data = false;
	wait(10, SC_NS);
	d_data = true;		//data_out = 1
	d_clear = true;		//clears output
}