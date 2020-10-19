#include "systemc.h"

SC_MODULE(robot) {
	//inputs
	sc_in<bool> clock;

	sc_in<bool> flagFromServer;							//flag from server
	sc_in<int> dataFromServer;							//data sent from server
								//is this from server -> robot -> processing?
	sc_in<bool> flagFromProcessing;						//flag from processing
	sc_in<int> dataFromProcessing;						//data sent from processing							
								//is this from processing -> robot -> server?				

	//outputs
	sc_out<bool> flagToProcessing;						//probably connected with flagFromServer
	sc_out<int> dataToProcessing;						//probably connected to dataFromServer
								//server -> robot -> processing
	sc_out<bool> flagToServer;							//probably connected with flagFromProcessing
	sc_out<int> dataToServer;							//probably connected with dataFromProcessing
	
								//processing -> robot -> server

	sc_in<bool> ackFromServer;			//
	sc_in<bool> ackFromProcessing;		//

	sc_out<bool> ackToProcessing;		//ack
	sc_out<bool> ackToServer;			//ack

	//internal
	int robotID;
	sc_signal<bool> tx_s_signal;
	sc_signal<bool> tx_p_signal;

	// robot index, Status, Modified
	int rx_p_table[3];				//rx_p_table will transfer to tx_s_table to transmit to server
	int rx_p_counter = 0;
	
	// robot index, status, modified
	int tx_s_table[3];				//tx_s_table
	int tx_s_counter = 0;

	//main table
	// {Robot index, Status, Modified}
	int main_table[3] = { 0 };

	//robot index, status, modified
	int rx_s_table[3];
	int rx_s_counter = 0;

	//robot index, status, modified
	int tx_p_table[3];
	int tx_p_counter = 0;


	sc_event tx_s_event;
	sc_event tx_p_event;


	void prc_update();
	void prc_tx_s();
	void prc_rx_s();
	void prc_tx_p();
	void prc_rx_p();

	SC_HAS_PROCESS(robot);
	robot(sc_module_name name, int robotIndex) : sc_module(name), robotID(robotIndex) {
		SC_METHOD(prc_update);
		sensitive << clock.pos();

		SC_THREAD(prc_tx_s);
		dont_initialize();
		sensitive << tx_s_event;

		SC_THREAD(prc_rx_s);

		SC_THREAD(prc_tx_p);
		dont_initialize();
		sensitive << tx_p_event;

		SC_THREAD(prc_rx_p);


		//robot module parameterized with an index
		robotID = robotIndex;
	}

};

//from processing TX to robot RX
void robot::prc_rx_p() {
	ackToProcessing = 0;
	wait(flagFromProcessing.posedge_event());
	rx_p_table[1] = dataFromProcessing;
	rx_p_table[2] = 1;
	ackToProcessing = 1;				//acknowledge processing module: prc_tx
	rx_p_counter++;
	cout << "ROBOT: Robot " << robotID << " prc_rx_p executed" << " Now sending ack to processing prc_tx() to Robot " << robotID << endl;
}

//robot RX_p to prc_update
void robot::prc_update() {
	while (rx_p_counter > 0) {
		if (rx_p_table[2] == 1) {		//if MODIFIED
			int status = rx_p_table[1];
			main_table[1] = status;
			main_table[2] = 1;			//main_tablen now modified
			rx_p_table[2] = 0;			//clear MODIFIED
			rx_p_counter--;

			tx_s_counter++;
		}
	}

	while (rx_s_counter > 0) {
		if (rx_s_table[2] == 1) {		//if MODIFIED
			int status = rx_s_table[1];
			main_table[1] = status;
			main_table[2] = 1;			//main_table now modified
			rx_s_table[2] = 0;			//clear MODIFIED
			rx_p_counter--;

			tx_p_counter++;
		}
	}


	



	if (tx_p_counter > 0) {
		tx_p_event.notify();
	}
	if (tx_s_counter > 0) {
		tx_s_event.notify();
	}

	cout << "ROBOT: Robot " << robotID << " prc_update() finished executing" << endl;
}

//prc_update to robot TX_s -> transmit to server
void robot::prc_tx_s() {
	wait(tx_s_event);
	cout << "ROBOT: Robot " << robotID << " prc_tx_s executing" << endl;
	flagToServer = 0;
	
	int status = main_table[1];
	tx_s_table[1] = status;
	tx_s_table[2] = 1;
	
	dataToServer = status;
	flagToServer = 1;

	wait(ackFromServer.posedge_event());
	flagToServer = 0;

	cout << "ROBOT: ack from server received for Robot " << robotID << endl;
}

//from server TX to robot RX
void robot::prc_rx_s() {
	ackToServer = 0;
	wait(flagFromServer.posedge_event());
	rx_s_table[1] = dataFromServer;
	rx_s_table[2] = 1;
	ackToServer = 1;					//acknowledge server module: prc_tx

	rx_s_counter++;
	cout << "ROBOT: Robot " << robotID << " prc_rx_s executed" << endl;
}

//(robot RX_s to prc_update)

//prc_update to robot TX_p -> transmit to processing
void robot::prc_tx_p() {
	wait(tx_p_event);
	cout << "ROBOT: Robot " << robotID << " prc_tx_p executing" << endl;
	int status = main_table[1];
	tx_p_table[1] = status;
	tx_p_table[2] = 1;

	dataToProcessing = status;
	flagToProcessing = 1;

	wait(ackFromProcessing.posedge_event());			//wait for ack from Processing
	flagToProcessing = 0;

	cout << "ROBOT: Robot " << robotID << " prc_tx_p finished" << endl;
}
