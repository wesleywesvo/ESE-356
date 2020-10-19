Processes: Connects to Robots
1. prc_tx(); // SC_THREAD()
2. prc_rx(); // SC_THREAD()
Internal Process
3. prc_update(); // SC_METHOD()

Ports: Connects to Robots
rx_in (each port has sc_out<bool> ack, sc_in<bool> flag, sc_in<sc_uint<16> > data;)
tx_out (each port has sc_in<bool> ack, sc_out<bool> flag, sc_out<sc_uint<16> > data;)


Local Signals for Inter-Process Communications
sc_signal(bool) rx_signal;   reset by prc_update(), set by prc_rx()
sc_signal(bool) tx_signal;   set by prc_update(), reset by prc_tx()

Counters for Requests
int rx_counter;  incremented by prc_rx(), decremented by prc_update();  // not necessary
int tx_counter;  incremented by prc_update(), decremented by prc_tx();

Local Table for Communications
RX_Table: 2 dimentional array. (Robot Index, Status, Modified) // used by prc_rx
TX_Table: 2 dimentional array. (Robot Index, Status, Modified) // used by prc_tx

Main Table for Server