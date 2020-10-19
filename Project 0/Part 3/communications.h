#include "systemc.h"

SC_MODULE(communications) {
	//inputs
	sc_in<bool> clk, reset, clear;			//active low reset, active high clear
	sc_in<sc_uint<12>> inData;				//input data: 
											//inData[11:8] contains header
											//inData[7:4] contains the data payload
											//inData[3:1] are not used
											//inData[0] is a parity bit
											//parity = 1 if inData[7:4] is meant to be even parity
											//a new inData arrives every clock
	//outputs
	sc_out<sc_uint<4>> payload;				//when inData is of type1
	sc_out<sc_uint<8>> count;				//the total count of type1 payload
	sc_out<sc_uint<8>> error;				//the number of type1 payload with wrong parity

	//internal variables
	sc_uint<4> headerVar, payloadVar;
	sc_uint<8> countVar, errorVar;
	sc_uint<1> parityVar;

	void prc_output();

	SC_CTOR(communications) {
		SC_METHOD(prc_output);
		sensitive << clk.pos() << reset;
	}
};