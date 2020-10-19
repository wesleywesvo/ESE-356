#include "systemc.h"

SC_MODULE(register_file) {
	//inputs
	sc_in<bool> clock, read, write;		
	sc_in<int> addr_1, addr_2, addr_3;				//two 4-bit address inputs		max value == 15
													//addr_1 is for Rdest 
													//addr_2 is for Rsrc
													//addr_3 is for writeback 
	sc_in<int> data_in;						//16-bit data input (to write into RF)

	//outputs
	sc_out<int> data_out_1, data_out_2;		//two 16-bit data outputs (from reading)

	//internal variables
	int reg[16] = { 0 };							//an array of 16 internal registers each 16-bits wide
	int* _data;

	void prc_read();								//read from registers on falling edge of clock
	void prc_write();								//write into register on rising edge of clock

	SC_HAS_PROCESS(register_file);

	register_file(sc_module_name name, int* data) : sc_module(name), _data(data) {
		SC_METHOD(prc_read);
		sensitive << clock.pos();

		SC_METHOD(prc_write);
		sensitive << clock.neg();

		//for verification purposes - can comment out below if needed - expected 0 in all registers when instantiated
		for (int i = 0; i < 16; i++) {
			reg[i] = *data;
			*data++;
			cout << "R[" << i << "] = " << reg[i] << endl;
		}
	}
	
};

void register_file::prc_read() {
	if (read) {
		if (addr_1.read() < 16) {
			//cout << "addr_1: " << addr_1 << "timestamp @ " << sc_time_stamp() << endl;
			data_out_1 = reg[addr_1];
		}
		if (addr_2.read() < 16) {
			//cout << "addr_2: " << addr_2 << "timestamp @ " << sc_time_stamp() << endl;
			data_out_2 = reg[addr_2];
		}
	}
}

void register_file::prc_write() {
	if (write) {
		if (addr_3 < 16) {
			reg[addr_3] = data_in;
			cout << "Written " << data_in << " into RF[" << addr_3 << "] at timestamp " << sc_time_stamp() << endl << endl;
			cout << "Updated Register File: " << endl;
			for (int i = 0; i < 16; i++) {
				cout << "R[" << i << "] = " << reg[i] << endl;
			}
			cout << endl;
		}
	}
}