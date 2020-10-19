#include "seqdet.h"
#include "driver.h"
#include "monitor.h"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> t_reset, t_clear, t_data_in, t_data_out;
	sc_clock t_clock("clk1", 10, SC_NS);

	//connect the DUT
	seqdet mySeqdet("SeqDet");
	mySeqdet.clk(t_clock);
	mySeqdet.reset(t_reset);
	mySeqdet.clr(t_clear);
	mySeqdet.data_in(t_data_in);
	mySeqdet.data_out(t_data_out);
	
	//connect driver
	driver d1("d1");
	d1.d_data(t_data_in);
	d1.d_reset(t_reset);
	d1.d_clear(t_clear);

	//connect monitor
	monitor m1("m1");
	m1.m_bit0(mySeqdet.bit0);
	m1.m_bit1(mySeqdet.bit1);
	m1.m_bit2(mySeqdet.bit2);
	m1.m_bit3(mySeqdet.bit3);
	m1.m_reset(t_reset);
	m1.m_clear(t_clear);
	m1.m_data_in(t_data_in);
	m1.m_data_out(t_data_out);

	sc_trace_file *tfile = sc_create_vcd_trace_file("seqdet");
	sc_trace(tfile, t_clock, "t_clock");
	sc_trace(tfile, t_reset, "t_reset");
	sc_trace(tfile, t_clear, "t_clear");
	sc_trace(tfile, t_data_in, "t_data_in");
	sc_trace(tfile, t_data_out, "t_data_out");
	
	//trace signals
	sc_trace(tfile, mySeqdet.bit0, "mySeqdet.bit0");
	sc_trace(tfile, mySeqdet.bit1, "mySeqdet.bit1");
	sc_trace(tfile, mySeqdet.bit2, "mySeqdet.bit2");
	sc_trace(tfile, mySeqdet.bit3, "mySeqdet.bit3");

	sc_start(100, SC_NS);

	sc_close_vcd_trace_file(tfile);

	return 0;
}