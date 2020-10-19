#include "systemc.h"

SC_MODULE(driver) {
	sc_out<bool> d_load1, d_load2, d_dec1, d_dec2;
	sc_out<sc_uint<8>> d_in1, d_in2, d_in3;

	void prc_driver();

	SC_CTOR(driver) {
		SC_THREAD(prc_driver);
	}
};
