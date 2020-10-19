Three parts

Part 1: Received information processing
See if rx_counter is not zero. This counter is incremented by prc_rx
if rx_counter is not zero, scan RX Table and update Main Table

Part 2: Update processing using Main Table
If Communication to the other module is needed,
modify TX Table and increment the tx_counter

Part 3: Transmit information processing
if TX Table is modified, tx_signal to prc_tx