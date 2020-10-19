#include "systemc.h"

SC_MODULE(monitor) {
	sc_in<bool> m_load1, m_load2, m_dec1, m_dec2, m_ended, m_overflow1, m_overflow2;
	sc_in<sc_uint<8>> m_in1, m_in2, m_in3, m_count1, m_count2;

	void prc_monitor();

	SC_CTOR(monitor) {
		SC_METHOD(prc_monitor);
		sensitive << m_load1 << m_load2 << m_dec1 << m_dec2 << m_ended << m_in1 << m_in2 << m_in3
			<< m_count1 << m_count2 << m_overflow1 << m_overflow2;
	}
};