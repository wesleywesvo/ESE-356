#include "monitor.h"

void monitor::prc_monitor() {
	cout << "Time: " << sc_time_stamp() << "\n"
		<< "Reset: " << m_reset << "\n"
		<< "Clear: " << m_clear << "\n"
		<< "InData: " << m_inData.read() << "\n"
		<< "Payload: " << m_payload.read() << "\n"
		<< "Count: " << m_count.read() << "\n"
		<< "Error: " << m_error.read() << "\n\n" << endl;
}