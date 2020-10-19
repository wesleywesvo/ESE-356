#include "driver.h"
#include "communications.h"
#include "monitor.h"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> t_reset, t_clear;
	sc_signal<sc_uint<12>> t_inData;
	sc_signal<sc_uint<4>> t_payload;
	sc_signal<sc_uint<8>> t_count, t_error;
	sc_clock t_clock("clk1", 10, SC_NS);

	//connect DUT
	communications myComms("Comms");
	myComms.clk(t_clock);
	myComms.reset(t_reset);
	myComms.clear(t_clear);
	myComms.inData(t_inData);
	myComms.payload(t_payload);
	myComms.count(t_count);
	myComms.error(t_error);

	//connect driver
	driver d1("d1");
	d1.d_reset(t_reset);
	d1.d_clear(t_clear);
	d1.d_inData(t_inData);

	//connect monitor
	monitor m1("m1");
	m1.m_reset(t_reset);
	m1.m_clear(t_clear);
	m1.m_inData(t_inData);
	m1.m_payload(t_payload);
	m1.m_count(t_count);
	m1.m_error(t_error);

	sc_trace_file *tfile = sc_create_vcd_trace_file("Comms");
	sc_trace(tfile, t_clock, "t_clock");
	sc_trace(tfile, t_reset, "t_reset");
	sc_trace(tfile, t_clear, "t_clear");
	sc_trace(tfile, t_inData, "t_inData");
	sc_trace(tfile, t_payload, "t_payload");
	sc_trace(tfile, t_count, "t_count");
	sc_trace(tfile, t_error, "t_error");
	//trace signals
	sc_trace(tfile, myComms.headerVar, "myComms.headerVar");
	sc_trace(tfile, myComms.payloadVar, "myComms.payloadVar");
	sc_trace(tfile, myComms.countVar, "myComms.countVar");
	sc_trace(tfile, myComms.errorVar, "myComms.errorVar");
	sc_trace(tfile, myComms.parityVar, "myComms.parityVar");

	sc_start(200, SC_NS);

	sc_close_vcd_trace_file(tfile);

	return 0;
}