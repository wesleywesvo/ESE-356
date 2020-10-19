#include "systemc.h"

SC_MODULE(program_memory) {
	//inputs
	sc_in<bool> clock;
	sc_in<int> addr_in;				//8-bit address input
	
	//outputs
	sc_out<int> data_out;		//16-bit data output

	//internal variables
	int instr_in[256];			//list of 256 instructions of size 16-bit
	int* _instr;

	void prc_pm();

	SC_HAS_PROCESS(program_memory);

	program_memory(sc_module_name name, int* instr) : sc_module(name), _instr(instr) {		//_instr == local //instr == data getting from outside
		SC_METHOD(prc_pm);
		sensitive << clock.pos();

		for (int i = 0; i < 256; i++) {
			instr_in[i] = *instr;
			*instr++;
			//verification purposes - can comment out below if needed
			//cout << "PM[" << i << "]: " << instr_in[i] << endl;
		}
	}
	
};

void program_memory::prc_pm() {
	if (addr_in < 256) {
		//cout << "addr_in = " << addr_in << endl << "at timestamp " << sc_time_stamp() << endl;
		//int test = addr_in;
		data_out = instr_in[addr_in];
	}
}
