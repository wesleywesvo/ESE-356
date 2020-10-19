#include "counters.h"
#include "driver.h"
#include "monitor.h"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> t_load1, t_load2, t_dec1, t_dec2, t_ended;
	sc_clock t_clock("clk1", 5, SC_NS);
	sc_signal<sc_uint<8>> t_in1, t_in2, t_in3;
	sc_signal<sc_uint<8>> t_count1, t_count2;

	//connect DUT
	counters myCounters("Counters");
	myCounters.clk(t_clock);
	myCounters.load1(t_load1);
	myCounters.load2(t_load2);
	myCounters.dec1(t_dec1);
	myCounters.dec2(t_dec2);
	myCounters.ended(t_ended);
	myCounters.in1(t_in1);
	myCounters.in2(t_in2);
	myCounters.in3(t_in3);
	myCounters.count1(t_count1);
	myCounters.count2(t_count2);

	//connect driver
	driver d1("d1");
	d1.d_load1(t_load1);
	d1.d_load2(t_load2);
	d1.d_dec1(t_dec1);
	d1.d_dec2(t_dec2);
	d1.d_in1(t_in1);
	d1.d_in2(t_in2);
	d1.d_in3(t_in3);

	//connect monitor
	monitor m1("m1");
	m1.m_load1(t_load1);
	m1.m_load2(t_load2);
	m1.m_dec1(t_dec1);
	m1.m_dec2(t_dec2);
	m1.m_ended(t_ended);
	m1.m_in1(t_in1);
	m1.m_in2(t_in2);
	m1.m_in3(t_in3);
	m1.m_count1(t_count1);
	m1.m_count2(t_count2);
	m1.m_overflow1(myCounters.overflow1);
	m1.m_overflow2(myCounters.overflow2);

	sc_trace_file *tfile = sc_create_vcd_trace_file("counters");
	sc_trace(tfile, t_clock, "t_clock");
	sc_trace(tfile, t_load1, "t_load1");
	sc_trace(tfile, t_load2, "t_load2");
	sc_trace(tfile, t_dec1, "t_dec1");
	sc_trace(tfile, t_dec2, "t_dec2");
	sc_trace(tfile, t_ended, "t_ended");
	sc_trace(tfile, t_in1, "t_in1");
	sc_trace(tfile, t_in2, "t_in2");
	sc_trace(tfile, t_in3, "t_in3");
	sc_trace(tfile, t_count1, "t_count1");
	sc_trace(tfile, t_count2, "t_count2");

	//trace signals
	sc_trace(tfile, myCounters.tempCount1, "myCounters.tempCount1");
	sc_trace(tfile, myCounters.tempCount2, "myCounters.tempCount2");
	sc_trace(tfile, myCounters.overflow1, "myCounters.overflow1");
	sc_trace(tfile, myCounters.overflow2, "myCounters.overflow2");

	sc_start(100, SC_NS);

	sc_close_vcd_trace_file(tfile);
	
	return 0;
}