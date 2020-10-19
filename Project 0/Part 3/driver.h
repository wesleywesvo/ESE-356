#include "systemc.h"

SC_MODULE(driver) {
	sc_out<bool> d_reset, d_clear;
	sc_out<sc_uint<12>> d_inData;

	void prc_driver();

	SC_CTOR(driver) {
		SC_THREAD(prc_driver);
	}
};