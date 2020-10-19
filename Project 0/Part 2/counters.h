#include "systemc.h"

SC_MODULE(counters) {
	//inputs
	sc_in<bool> clk;
	sc_in<sc_uint<8>> in1, in2, in3;
	sc_in<bool> load1, load2;			//respective counter will load in data into in1 or in2
	sc_in<bool> dec1, dec2;				//respective counter will decrement count1 or count2
										//dec2 will decrement count2 by the value of in3
	//outputs
	sc_out<sc_uint<8>> count1, count2;	//contents of counter1 or counter2
	sc_out<bool> ended;					//high when count1==count2
										//high when either counter encounters an unsigned overflow

	//internal variable
	sc_signal<bool> overflow1, overflow2;	//detect overflow
	sc_uint<8> tempCount1, tempCount2;		//used for reading current counter contents

	void prc_counter1();
	void prc_counter2();
	void prc_ended();

	SC_CTOR(counters) {
		overflow1 = false;
		overflow2 = false;

		SC_METHOD(prc_counter1);
		sensitive << clk.pos();

		SC_METHOD(prc_counter2);
		sensitive << clk.pos();

		SC_METHOD(prc_ended);
		sensitive << count1 << count2 << overflow1 << overflow2;
		//sensitive << clk.pos();
	}
};