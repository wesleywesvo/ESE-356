#include "systemc.h"

SC_MODULE(seqdet) {
	//inputs
	sc_in<bool> clk;
	sc_in<bool> reset;			//active low, clears all internal registers
	sc_in<bool> clr;			//clears output registers
	sc_in<bool> data_in;		//single bit - stored into bit0
	//outputs
	sc_out<bool> data_out;		//single bit - high if sequence found

	//internal variables
	sc_signal<bool> bit3, bit2, bit1, bit0;

	void prc_seqdet();
	void prc_output();

	SC_CTOR(seqdet) {
		SC_METHOD(prc_seqdet);
		sensitive << clk.pos() << reset;

		SC_METHOD(prc_output);
		sensitive << bit3 << bit2 << bit1 << bit0;
	}
};