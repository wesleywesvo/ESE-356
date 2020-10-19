#include "systemc.h"

SC_MODULE(processing) {
	//inputs
	sc_in<bool> clock;
	sc_in<bool> flagFromRobot1;			//flag to trigger prc_rx
	sc_in<int> dataFromRobot1;
	sc_in<bool> ackFromRobot1;			//ack from robot

	sc_in<bool> flagFromRobot2;
	sc_in<int> dataFromRobot2;
	sc_in<bool> ackFromRobot2;			//ack from robot


	//outputs
	sc_out<bool> flagToRobot1;			//flag to send to server
	sc_out<int> dataToRobot1;			//data to send to server


	sc_out<bool> flagToRobot2;			//flag to send to server
	sc_out<int> dataToRobot2;			//data to send to server

	sc_out<bool> ackToRobot1;
	sc_out<bool> ackToRobot2;


	//internal data
	// {Grid index, LLX, LLY, URX, URY, N, S, W, E
	int map[60][9] =								//based on given map, row 0 = very top and row 8 = very bottom
	{
		//row 0
		{ 1, 0, 16, 2, 18, -1, 11, -1, 2 },
		{ 2, 2, 16, 4, 18, -1, -1, 1, 3 },
		{ 3, 4, 16, 6, 18, -1, -1, 2, 4 },
		{ 4, 6, 16, 8, 18, -1, -1, 3, 5 },
		{ 5, 8, 16, 10, 18, -1, -1, 4, 6 },
		{ 6, 10, 16, 12, 18, -1, -1, 5, 7 },
		{ 7, 12, 16, 14, 18, -1, -1, 6, 8 },
		{ 8, 14, 16, 16, 18, -1, -1, 7, 9 },
		{ 9, 16, 16, 18, 18, -1, -1, 8, 10 },
		{ 10, 18, 16, 20, 18, -1, 12, 9, -1 },
		//row 1
		{ 11, 0, 14, 2, 16, 1, 13, -1, -1 },
		{ 12, 18, 14, 20, 16, 10, 22, -1, -1 },
		//row 2
		{ 13, 0, 12, 2, 14, 11, 23, -1, 14 },
		{ 14, 2, 12, 4, 14, -1, -1, 13, 15 },
		{ 15, 4, 12, 6, 14, -1, -1, 14, 16 },
		{ 16, 6, 12, 8, 14, -1, -1, 15, 17 },
		{ 17, 8, 12, 10, 14, -1, -1, 16, 18 },
		{ 18, 10, 12, 12, 14, -1, 24, 17, 19 },
		{ 19, 12, 12, 14, 14, -1, -1, 18, 20 },
		{ 20, 14, 12, 16, 14, -1, -1, 19, 21 },
		{ 21, 16, 12, 18, 14, -1, -1, 20, 22 },
		{ 22, 18, 12, 20, 14, 12, 25, 21, -1 },
		//row 3
		{ 23, 0, 10, 2, 12, 13, 26, -1, -1 },
		{ 24, 10, 10, 12, 12, 18, 31, -1, -1 },
		{ 25, 18, 10, 20, 12, 22, 35, -1, -1 },
		//row 4
		{ 26, 0, 8, 2, 10, 23, 36, -1, 27 },
		{ 27, 2, 8, 4, 10, -1, -1, 26, 28 },
		{ 28, 4, 8, 6, 10, -1, -1, 27, 29 },
		{ 29, 6, 8, 8, 10, -1, -1, 28, 30 },
		{ 30, 8, 8, 10, 10, -1, -1, 29, 31 },
		{ 31, 10, 8, 12, 10, 24, -1, 30, 32 },
		{ 32, 12, 8, 14, 10, -1, 37, 31, 33 },
		{ 33, 14, 8, 16, 10, -1, -1, 32, 34 },
		{ 34, 16, 8, 18, 10, -1, -1, 33, 35 },
		{ 35, 18, 8, 20, 10, 25, 38, 34, -1 },
		//row 5
		{ 36, 0, 6, 2, 8, 26, 39, -1, -1 },
		{ 37, 12, 6, 14, 8, 32, 45, -1, -1 },
		{ 38, 18, 6, 20, 8, 35, 48, -1, -1 },
		//row 6
		{ 39, 0, 4, 2, 6, 36, 49, -1, 40 },
		{ 40, 2, 4, 4, 6, -1, -1, 39, 41 },
		{ 41, 4, 4, 6, 6, -1, -1, 40, 42 },
		{ 42, 6, 4, 8, 6, -1, -1, 41, 43 },
		{ 43, 8, 4, 10, 6, -1, -1, 42, 44 },
		{ 44, 10, 4, 12, 6, -1, -1, 43, 45 },
		{ 45, 12, 4, 14, 6, 37, -1, 44, 46 },
		{ 46, 15, 4, 16, 6, -1, -1, 45, 47 },
		{ 47, 16, 4, 18, 6, -1, -1, 46, 48 },
		{ 48, 18, 4, 20, 6, 38, 50, 47, -1 },
		//row 7
		{ 49, 0, 2, 2, 4, 39, 51, -1, -1 },
		{ 50, 18, 2, 20, 4, 48, 60, -1, -1 },
		//row 8
		{ 51, 0, 0, 2, 2, 49, -1, -1, 52 },
		{ 52, 2, 0, 4, 2, -1, -1, 51, 53 },
		{ 53, 4, 0, 6, 2, -1, -1, 52, 54 },
		{ 54, 6, 0, 8, 2, -1, -1, 53, 55 },
		{ 55, 8, 0, 10, 2, -1, -1, 54, 56 },
		{ 56, 10, 0, 12, 2, -1, -1, 55, 57 },
		{ 57, 12, 0, 14, 2, -1, -1, 56, 58 },
		{ 58, 14, 0, 16, 2, -1, -1, 57, 59 },
		{ 59, 16, 0, 18, 2, -1, -1, 58, 60 },
		{ 60, 18, 0, 20, 2, 50, -1, 59, -1 },
	};

	//----------------------------------------------------------------//
	// {Robot index, Status, Modified}
	int rx_table[2][3] = {
		{ 1, 1, 0 },
		{ 2, 1, 0 }
	};
	int rx_counter = 0;

	//---------------------------------------------------------------//
	// {Robot index, Status, Modified}
	int main_table[2][3] = {
		{ 1, 1, 0 },
		{ 2, 1, 0 }
	};

	//----------------------------------------------------------------//
	// {Robot index, Status, Modified}
	int tx_table[2][3] = {
		{ 1, 1, 0 },
		{ 2, 1, 0 }
	};
	sc_signal<bool> tx_signal;
	int tx_counter = 0;



	//for robot movement - each index corresponds to robot index
	int robotGrid[2] = { 1, 13 };
	int robotDirection[2] = { 2, 2 };					//4 values for NSEW
	/*double robotCoord[2][2] = {							//starting points of each robot
		{ 0, 0 },
		{ 0, 13 }
	};*/
	double robotX[2] = { 0, 0 };
	double robotY[2] = { 0, 13 };
	double robotSpeed = 0.2;

	int obstacleDirection[2] = { 1, 1 };
	/*double obstacleCoord[2][2] = {						//starting points of each human
		{ 3, 3 },
		{ 0, 12 }
	};*/
	double obstacleX[2] = { 3, 0 };
	double obstacleY[2] = { 3, 12 };
	double obstacleSpeed = 0.4;				





	sc_event tx_prc_event;



	void prc_tx();
	void prc_rx();
	void prc_update();

	SC_HAS_PROCESS(processing);
	processing(sc_module_name name) : sc_module(name) {
		SC_THREAD(prc_rx);

		SC_METHOD(prc_update);
		sensitive << clock.pos();

		SC_THREAD(prc_tx);
		dont_initialize();
		sensitive << tx_prc_event;
	}

};


void processing::prc_rx() {			//receiving process
	ackToRobot1 = ackToRobot2 = 0;
	wait(flagFromRobot1.posedge_event() | flagFromRobot2.posedge_event());
	if (flagFromRobot1) {
		int status = dataFromRobot1;
		if (status == 4) {			//sent from server -> OK to cross boundary
			status = 2;				// 2 = crossing
		}
		else if (status == 5) {		//sent from server -> STOP do not cross
			status = 3;
		}
		rx_table[0][1] = dataFromRobot1;
		rx_counter++;
		ackToRobot1 = 1;
	}
	if (flagFromRobot2) {
		int status = dataFromRobot2;
		if (status == 4) {			//sent from server -> OK to cross boundary
			status = 2;				// 2 = crossing
		}	
		else if (status == 5) {		//sent from server -> STOP do not cross
			status = 3;
		}
		rx_table[1][1] = status;
		rx_counter++;
		ackToRobot2 = 1;
	}
}

void processing::prc_update() {
	//scan rx_table and check for MODIFIED -> update main_table
	while (rx_counter > 0) {
		// if MODIFIED column is set to 1
		int i = 0;
		if (rx_table[i][2] == 1) {
			main_table[i][2] = rx_table[i][2];
			rx_table[i][2] = 0;			//reset MODIFIED in rx_table
		}
		i++;
		rx_counter--;					//decrement rx_counter
	}
	
	
	
	//four loops of operations
	//loop 1: each robot updates the position
	for (int i = 0; i < 2; i++) {
		// status 0 == stopped to obstacle, status 3 == stopped to boundary
		if (main_table[i][1] == 1 || main_table[i][1] == 2) {
			switch (robotDirection[i]) {
				case 0:		//north
					robotY[i] += robotSpeed;
					break;
				case 1:		//south
					robotY[i] -= robotSpeed;
					break;
				case 2:		//east
					robotX[i] += robotSpeed;
					break;
				case 3:		//west
					robotX[i] -= robotSpeed;
					break;
				default:
					break;
			}
		}
	}
	cout << "Robot 1 = (" << robotX[0] << ", " << robotY[0] << ")" << endl;
	cout << "Robot 2 = (" << robotX[1] << ", " << robotY[1] << ")" << endl << endl;


	//loop 2: each obstacle updates the position (update humans?) - humans move only east or west i assume
	for (int i = 0; i < 2; i++) {
		if (i == 0) {				//human 1 only moves horizontally
			obstacleX[i] += obstacleSpeed * obstacleDirection[i];
			if (obstacleX[i] < 0) { 
				obstacleDirection[i] = 1;
			}
			else if (obstacleX[i] > 20) {
				obstacleDirection[i] = -1;
			}
		}
		if (i == 1) {				//human 2 only moves vertically
			obstacleY[i] += obstacleSpeed * obstacleDirection[i];
			if (obstacleY[i] < 0) {
				obstacleDirection[i] = 1;
			}
			else if (obstacleY[i] > 20) {
				obstacleDirection[i] = -1;
			}
		}
	}
	cout << "Human 1 = (" << obstacleX[0] << ", " << obstacleY[0] << ")" << endl;
	cout << "Human 2 = (" << obstacleX[1] << ", " << obstacleY[1] << ")" << endl << endl;;


	//loop 3: for each robot, compare the distance from the obstacles
	//if close to obstacle, stop robot, sends robot the status and the robot sends status to server
	//if stopped robot is clear from obstacle, resume robot, send robot the status and the robot sends status to server
	for (int i = 0; i < 2; i++) {
		if (abs(robotX[i] - obstacleX[i]) < 3 || abs(robotY[i] - obstacleY[i]) < 3) {
			main_table[i][1] = 0;			//update robot index's status to 0 (stopped to obstacle)
			main_table[i][2] = 1;			//MODIFIED
			tx_counter++;
			cout << "robot " << i + 1 << " is close to obstacle" << endl;
		}
		else {
			main_table[i][1] = 1;			//status is 1 (moving due to obstacle clearing)
			cout << "robot " << i + 1 << " is moving because no obstacle" << endl;
		}
	}





	//loop 4: for each robot, compare the distance from the boundary
	//if close to boundary, send signal to robot, then robot sends signal to server
	for (int i = 0; i < 2; i++) {
		
	}


	if (tx_counter > 0) {
		tx_prc_event.notify();
	}
}



void processing::prc_tx() {
	while (true) {
		wait(tx_prc_event);				//wait for tx_signal from prc_update
		cout << "processing prc_tx now executing" << endl;
		flagToRobot1 = flagToRobot2 = 0;
		while (tx_counter > 0) {
			/*for (int i = 0; i < 2; i++) {
				if (main_table[i][2] == 1) {
					int status = main_table[i][1];
					tx_table[i][1] = status;				//bring status to tx_table
					main_table[i][2] = 0;					//clear MODIFIED
					if (i == 0) {
						dataToRobot1 = status;		//output status to robot 1
						flagToRobot1 = 1;			//output flag to robot 1
						wait(ackFromRobot1.posedge_event());		//wait for ack from robot 1 prc_rx
						cout << "Robot 1 ack received in processing prc_tx()" << endl;
						flagToRobot1 = 0;			//clear output flag
						//tx_counter--;
					}
					if (i == 1) {
						dataToRobot2 = status;		//output status to robot 2
						flagToRobot2 = 1;			//output flag to robot 2
						wait(ackFromRobot2.posedge_event());		//wait for ack from robot 2 prc_rx
						cout << "Robot 2 ack received in processing prc_tx()" << endl;
						flagToRobot2 = 0;			//clear output flag
						//tx_counter--;
					}
				}
			}*/
			if (main_table[0][2] == 1) {				//robot 1
				int status = main_table[0][1];
				tx_table[0][1] = status;				//bring status to tx_table
				main_table[0][2] = 0;					//clear MODIFIED
				dataToRobot1 = status;		//output status to robot 1
				flagToRobot1 = 1;			//output flag to robot 1
				wait(ackFromRobot1.posedge_event());		//wait for ack from robot 1 prc_rx
				cout << "Robot 1 ack received in processing prc_tx()" << endl;
				flagToRobot1 = 0;			//clear output flag
				//tx_counter--;
			}
			if (main_table[1][2] == 1) {				//robot 2
				int status = main_table[1][1];
				tx_table[1][1] = status;
				main_table[1][2] = 0;		//clear MODIFIED
				dataToRobot2 = status;		//output status to robot 2
				flagToRobot2 = 1;			//output flag to robot 2
				wait(ackFromRobot2.posedge_event());		//wait for ack from robot 2 prc_rx
				cout << "Robot 2 ack received in processing prc_tx()" << endl;
				flagToRobot2 = 0;			//clear output flag
				//tx_counter--;
			}
			tx_counter--;
		}
	}
}