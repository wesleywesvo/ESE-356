Processes: Connects to Server
1. prc_tx_s(); // SC_THREAD()
2. prc_rx_s(); // SC_THREAD()
Processes: Connects to Processing
3. prc_tx_p(); // SC_THREAD()
4. prc_rx_p(); // SC_THREAD()
Internal Process
5. prc_update(); // SC_METHOD()

Ports: Connects to Server
rx_s_in (each port has sc_out<bool> ack, sc_in<bool> flag, sc_in<sc_uint<16> > data;)
tx_s_out (each port has sc_in<bool> ack, sc_out<bool> flag, sc_out<sc_uint<16> > data;)

Ports: Connects to Processing
rx_s_in (each port has sc_out<bool> ack, sc_in<bool> flag, sc_in<sc_uint<16> > data;)
tx_s_out (each port has sc_in<bool> ack, sc_out<bool> flag, sc_out<sc_uint<16> > data;)

Local Signals for Inter-Process Communications
sc_signal(bool) rx_signal_s;   reset by prc_update(), set by prc_rx_s()  // not necessary
sc_signal(bool) rx_signal_p;   reset by prc_update(), set by prc_rx_p()

sc_signal(bool) tx_signal_s;   set by prc_update(), reset by prc_tx_s()  // not necessary
sc_signal(bool) tx_signal_p;   set by prc_update(), reset by prc_tx_p()

Counters for Requests
int rx_s_counter;  incremented by prc_rx_s(), decremented by prc_update();
int rx_p_counter;  incremented by prc_rx_p(), decremented by prc_update();
int tx_s_counter;  incremented by prc_update(), decremented by prc_tx_s();
int tx_p_counter;  incremented by prc_update(), decremented by prc_tx_p();

Local Table for Communications
RX_S_Table: 2 dimentional array. (Robot Index, Status, Modified) // used by prc_rx_s
TX_S_Table: 2 dimentional array. (Robot Index, Status, Modified) // used by prc_tx_s
RX_P_Table: 2 dimentional array. (Robot Index, Status, Modified) // used by prc_rx_p
TX_P_Table: 2 dimentional array. (Robot Index, Status, Modified) // used by prc_tx_p

Main Table for Robot
