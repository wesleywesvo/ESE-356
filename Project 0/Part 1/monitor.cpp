#include "monitor.h"

void monitor::prc_monitor() {
	cout << "Time: " << sc_time_stamp() << "\n"
		<< "Reset: " << m_reset << "\n"
		<< "Clear: " << m_clear << "\n"
		<< "Data_in: " << m_data_in << "\n"
		<< "[3, 2, 1, 0]: " << m_bit3 << m_bit2 << m_bit1 << m_bit0 << "\n"
		<< "Data_out: " << m_data_out << "\n\n" << endl;
}