#include "systemc.h"

SC_MODULE(driver) {
	sc_out<bool> d_data, d_reset, d_clear;

	void prc_driver();

	SC_CTOR(driver) {
		SC_THREAD(prc_driver);
	}
};
