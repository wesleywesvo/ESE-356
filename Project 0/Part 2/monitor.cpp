#include "monitor.h"

void monitor::prc_monitor() {
	cout << "Time: " << sc_time_stamp() << "\n"
		<< "Load1: " << m_load1 << "\n"
		<< "Load2: " << m_load2 << "\n"
		<< "In1: " << m_in1 << "\n"
		<< "In2: " << m_in2 << "\n"
		<< "In3: " << m_in3 << "\n"
		<< "Dec1: " << m_dec1 << "\n"
		<< "Dec2: " << m_dec2 << "\n"
		<< "Count1: " << m_count1 << "\n"
		<< "Overflow1: " << m_overflow1 << "\n"
		<< "Count2: " << m_count2 << "\n"
		<< "Overflow2: " << m_overflow2 << "\n"
		<< "Ended: " << m_ended << "\n\n" << endl;
}