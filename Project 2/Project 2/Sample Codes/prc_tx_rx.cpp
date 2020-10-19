// for prc_tx()
// out_link has three ports
// sc_out<bool> flag;
// sc_out<sc_uint<16> > data;
// sc_in<bool> ack;
while(true) {
    wait(); // wait for signal/event from prc_update
    flag = 0
    while(tx_counter != 0) {
        tx_signal = 0; // it ws set by prc_update
        flag = 1; // creating posedge
        data = status; // information for tx_table
        wait(); wait for ack form the receiving process
        flag = 0; 
        tx_counter--; // repeat until no outstanding request
    }
}

// for prc_rx()
// in_link has three ports
// sc_in<bool> flag;
// sc_in<sc_uint<16> > data;
// sc_out<bool> ack;
while(true) {
    ack = 0;
    wait(); // wait for request (flag.pos()) from sending process
    status = data; // update rx_table
    ack = 1;  // creating posedge
    rx_counter++; // this counter will be checked by prc_update()
}