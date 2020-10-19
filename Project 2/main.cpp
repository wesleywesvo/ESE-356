#include "robot.h"
#include "server.h"
#include "processing.h"

int sc_main(int argc, char* argv[]) {
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

	sc_clock t_clock("clk1", 1, SC_MS);

	/*sc_signal<bool> flagFromR1_to_P, flagFromR2_to_P;
	sc_signal<int> dataFromR1_to_P, dataFromR2_to_P;
	sc_signal<bool> flagFromP_to_R1, flagFromP_to_R2;
	sc_signal<int> dataFromP_to_R1, dataFromP_to_R2;

	sc_signal<bool> flagFromR1_to_S, flagFromR2_to_S;
	sc_signal<int> dataFromR1_to_S, dataFromR2_to_S;
	sc_signal<bool> flagFromS_to_R1, flagFromS_to_R2;
	sc_signal<int> dataFromS_to_R1, dataFromS_to_R2;

	sc_signal<bool> ackFromR1_to_P, ackFromR2_to_P;

	sc_signal<bool> ackFromR1_to_S, ackFromR2_to_S;


	processing p1("p1");
	p1.clock(t_clock);
	p1.flagFromRobot1(flagFromR1_to_P);
	p1.dataFromRobot1(dataFromR1_to_P);
	p1.ackFromRobot1(ackFromR1_to_P);		//ack

	p1.flagFromRobot2(flagFromR2_to_P);
	p1.dataFromRobot2(dataFromR2_to_P);
	p1.ackFromRobot2(ackFromR2_to_P);		//ack
	
	p1.flagToRobot1(flagFromP_to_R1);
	p1.dataToRobot1(dataFromP_to_R1);

	p1.flagToRobot2(flagFromP_to_R2);
	p1.dataToRobot2(dataFromP_to_R2);

	robot r1("r1", 1);
	r1.clock(t_clock);
	r1.flagToProcessing(flagFromR1_to_P);
	r1.dataToProcessing(dataFromR1_to_P);
	r1.ackToProcessing(ackFromR1_to_P);			//ack to input P
	r1.ackFromServer(ackFromR1_to_S);		//ack to input S
	//
	r1.flagFromProcessing(flagFromP_to_R1);
	r1.dataFromProcessing(dataFromP_to_R1);
	//
	r1.flagToServer(flagFromR1_to_S);
	r1.dataToServer(dataFromR1_to_S);
	//
	r1.flagFromServer(flagFromS_to_R1);
	r1.dataFromServer(dataFromS_to_R1);


	robot r2("r2", 2);
	r2.clock(t_clock);
	r2.flagToProcessing(flagFromR2_to_P);
	r2.dataToProcessing(dataFromR2_to_P);
	r2.ackToProcessing(ackFromR2_to_P);			//ack
	//
	r2.flagFromProcessing(flagFromP_to_R2);
	r2.dataFromProcessing(dataFromP_to_R2);
	//
	r2.flagToServer(flagFromR2_to_S);
	r2.dataToServer(dataFromR2_to_S);
	//
	r2.flagFromServer(flagFromS_to_R2);
	r2.dataFromServer(dataFromS_to_R2);


	server s1("s1");
	s1.clock(t_clock);
	s1.flagFromRobot1(flagFromR1_to_S);
	s1.dataFromRobot1(dataFromR1_to_S);
	//
	s1.flagFromRobot2(flagFromR2_to_S);
	s1.dataFromRobot2(dataFromR2_to_S);
	//
	s1.flagToRobot1(flagFromS_to_R1);
	s1.dataToRobot1(dataFromS_to_R1);
	//
	s1.flagToRobot2(flagFromS_to_R2);
	s1.dataToRobot2(dataFromS_to_R2);
	*/

	processing p1("p1");
	robot r1("r1", 1);
	robot r2("r2", 2);
	server s1("s1");


	//--------------------- Processing TX and R1/R2 RX_p Connections -------------------//
	//Processing TX -> Robot1 RX_P and Robot1 RX_P -> Processing TX
	sc_signal<bool> flag_P_R1;
	sc_signal<int> data_P_R1;
	sc_signal<bool> ack_R1_P;
	p1.flagToRobot1(flag_P_R1);
	r1.flagFromProcessing(flag_P_R1);
	//
	p1.dataToRobot1(data_P_R1);
	r1.dataFromProcessing(data_P_R1);
	//
	r1.ackToProcessing(ack_R1_P);
	p1.ackFromRobot1(ack_R1_P);
	//---------------------------------------//
	//Processing TX -> Robot2 RX_P and Robot2 RX_P -> Processing TX
	sc_signal<bool> flag_P_R2;
	sc_signal<int> data_P_R2;
	sc_signal<bool> ack_R2_P;
	p1.flagToRobot2(flag_P_R2);
	r2.flagFromProcessing(flag_P_R2);
	//
	p1.dataToRobot2(data_P_R2);
	r2.dataFromProcessing(data_P_R2);
	//
	r2.ackToProcessing(ack_R2_P);
	p1.ackFromRobot2(ack_R2_P);
	//---------------------------------------------------------------------//

	//-------------------- Robot TX_S to Server RX connection -----------------------------//
	//R1 TX_S -> Server RX and Server RX -> R1 TX_S
	sc_signal<bool> flag_R1_S;
	sc_signal<int> data_R1_S;
	sc_signal<bool> ack_S_R1;
	r1.flagToServer(flag_R1_S);
	s1.flagFromRobot1(flag_R1_S);
	//
	r1.dataToServer(data_R1_S);
	s1.dataFromRobot1(data_R1_S);
	//
	s1.ackToRobot1(ack_S_R1);
	r1.ackFromServer(ack_S_R1);
	//-----------------------------------------//
	//R2 TX_S -> Server RX and Server RX -> R2 TX_S
	sc_signal<bool> flag_R2_S;
	sc_signal<int> data_R2_S;
	sc_signal<bool> ack_S_R2;
	r2.flagToServer(flag_R2_S);
	s1.flagFromRobot2(flag_R2_S);
	//
	r2.dataToServer(data_R2_S);
	s1.dataFromRobot2(data_R2_S);
	//
	s1.ackToRobot2(ack_S_R2);
	r2.ackFromServer(ack_S_R2);
	//-----------------------------------------------------------------------------------//

	//-------------- Server TX to Robot RX_S connection --------------------------------//
	//Server TX -> R1 RX_S and R1 RX_S -> Server TX
	sc_signal<bool> flag_S_R1;
	sc_signal<int> data_S_R1;
	sc_signal<bool> ack_R1_S;
	s1.flagToRobot1(flag_S_R1);
	r1.flagFromServer(flag_S_R1);
	//
	s1.dataToRobot1(data_S_R1);
	r1.dataFromServer(data_S_R1);
	//
	r1.ackToServer(ack_R1_S);
	s1.ackFromRobot1(ack_R1_S);
	//------------------------------------------//
	//Server TX -> R2 RX_S and R2 RX_S -> Server TX
	sc_signal<bool> flag_S_R2;
	sc_signal<int> data_S_R2;
	sc_signal<bool> ack_R2_S;
	s1.flagToRobot2(flag_S_R2);
	r2.flagFromServer(flag_S_R2);
	//
	s1.dataToRobot2(data_S_R2);
	r2.dataFromServer(data_S_R2);
	//
	r2.ackToServer(ack_R2_S);
	s1.ackFromRobot2(ack_R2_S);
	//----------------------------------------------------------------------------------//

	//------------------ Robot TX_P to processing RX connection -------------------------------//
	//R1 TX_P -> processing RX and processing RX -> R1 TX_P
	sc_signal<bool> flag_R1_P;
	sc_signal<int> data_R1_P;
	sc_signal<bool> ack_P_R1;
	r1.flagToProcessing(flag_R1_P);
	p1.flagFromRobot1(flag_R1_P);
	//
	r1.dataToProcessing(data_R1_P);
	p1.dataFromRobot1(data_R1_P);
	//
	p1.ackToRobot1(ack_P_R1);
	r1.ackFromProcessing(ack_P_R1);
	//-------------------------------------------//
	//R2 TX_P -> processing RX and processing RX -> R2 TX_P
	sc_signal<bool> flag_R2_P;
	sc_signal<int> data_R2_P;
	sc_signal<bool> ack_P_R2;
	r2.flagToProcessing(flag_R2_P);
	p1.flagFromRobot2(flag_R2_P);
	//
	r2.dataToProcessing(data_R2_P);
	p1.dataFromRobot2(data_R2_P);
	//
	p1.ackToRobot2(ack_P_R2);
	r2.ackFromProcessing(ack_P_R2);
	//----------------------------------------------------------------------------//
	p1.clock(t_clock);
	r1.clock(t_clock);
	r2.clock(t_clock);
	s1.clock(t_clock);









	sc_start(20, SC_MS);

	return 0;
}