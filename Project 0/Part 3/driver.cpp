#include "driver.h"

void driver::prc_driver() {
	d_inData = "0x1F1";
	d_reset = true;
	wait(10, SC_NS);

	d_inData = "0x0E0";
	wait(10, SC_NS);

	d_inData = "0x171";
	wait(10, SC_NS);

	d_inData = "0x0E0";
	wait(10, SC_NS);

	d_inData = "0x1F0";
	wait(10, SC_NS);

	d_inData = "0x170";
	wait(10, SC_NS);

	d_reset = false;
	wait(10, SC_NS);

	d_reset = true;
	d_inData = "0x1F1";
	wait(10, SC_NS);

	d_inData = "0x0E0";
	wait(10, SC_NS);

	d_inData = "0x171";
	wait(10, SC_NS);

	d_inData = "0x0E0";
	wait(10, SC_NS);

	d_inData = "0x1F0";
	wait(10, SC_NS);

	d_inData = "0x170";
	wait(10, SC_NS);

	d_inData = "0x171";
	wait(20, SC_NS);

	d_clear = true;
	wait(10, SC_NS);
	d_clear = false;
}