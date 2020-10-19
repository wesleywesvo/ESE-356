#include "systemc.h"

SC_MODULE(data_memory) {
	//inputs
	sc_in<bool> clock, read, write;
	sc_in<int> addr_in;					//8-bit
	sc_in<int> data_in;					//16-bit

	//outputs
	sc_out<int> data_out;				//16-bit

	//internal variables
	int dm_data[256];					//16-bit data
	int* _dm_memory;

	void prc_dm_read();
	void prc_dm_write();

	SC_HAS_PROCESS(data_memory);

	data_memory(sc_module_name name, int* dm_memory) : sc_module(name), _dm_memory(dm_memory) {
		SC_METHOD(prc_dm_read);
		sensitive << clock.pos();

		SC_METHOD(prc_dm_write);
		sensitive << clock.pos();

		for (int i = 0; i < 256; i++) {
			dm_data[i] = *dm_memory;
			*dm_memory++;
		}
	}

};

void data_memory::prc_dm_write() {
	if (write) {
		if (addr_in < 256) {
			dm_data[addr_in] = data_in;
			cout << "Written " << data_in << " into DM[" << addr_in << "] at timestamp " << sc_time_stamp() << endl << endl;
			cout << "Updated Data Memory: " << endl;
			for (int i = 0; i < 15; i++) {
				cout << "DM[" << i << "] = " << dm_data[i] << endl;
			}
			cout << "DM[" << 46 << "] = " << dm_data[46] << endl;
			cout << endl;
		}
	}
	else {
		data_out = data_in;
	}
}

void data_memory::prc_dm_read() {
	if (read) {
		if (addr_in < 256) {
			cout << "timestamp " << sc_time_stamp() << " dataMem addrIn = " << addr_in << endl;
			data_out = dm_data[addr_in];
		}
	}
	else {
		data_out = data_in;
	}
}