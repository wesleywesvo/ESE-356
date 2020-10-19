#include "systemc.h"

SC_MODULE(monitor) {
	sc_in<bool> m_reset, m_clear;
	sc_in<sc_uint<12>> m_inData;
	sc_in<sc_uint<4>> m_payload;
	sc_in<sc_uint<8>> m_count, m_error;

	void prc_monitor();

	SC_CTOR(monitor) {
		SC_METHOD(prc_monitor);
		sensitive << m_reset << m_clear << m_inData << m_payload << m_count << m_error;
	}
};
