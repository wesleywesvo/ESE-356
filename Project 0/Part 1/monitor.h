#include "systemc.h"

SC_MODULE(monitor) {
	sc_in<bool> m_reset, m_clear, m_data_in, m_bit0, m_bit1, m_bit2, m_bit3, m_data_out;

	void prc_monitor();

	SC_CTOR(monitor) {
		SC_METHOD(prc_monitor);
		sensitive << m_reset << m_clear << m_data_in << m_data_out;
	}
};
