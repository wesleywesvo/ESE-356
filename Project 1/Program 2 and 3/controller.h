#include "systemc.h"

SC_MODULE(controller) {
	//pm module -> FETCH STAGE
	sc_in<int> pm_instr;
	sc_out<int> pc_to_pm;

	//rf module -> DECODING STAGE
	sc_out<int> rf_addr1, rf_addr2;			//address to read from RF; addr1 = rdest, addr2 = rsrc
	sc_out<bool> read_to_rf;				//read enable signal

	//alu module -> EXECUTION STAGE
	sc_out<int> imm_to_alu;					//input to alu Immediate
	sc_in<int> alu_to_controller_pc;
	//alu control
	sc_out<bool> add_sig, addu_sig, mul_sig, and_sig, or_sig,			//operator control signals
		xor_sig, mov_sig, jump_sig, lui_sig, bcond_sig;
	sc_out<bool> imm_sig, twos_sig, unsigned_sig, shift_sig, shiftSign;				//control signals
	sc_out<bool> alu_en;
	//alu flags
	sc_in<bool> c_flag, z_flag, n_flag, f_flag;

	//dm module -> MEMORY ACCESS STAGE
	sc_out<bool> write_to_dm, read_to_dm;

	//rf module -> WRITEBACK STAGE
	sc_out<bool> write_to_rf;				//write enable signal
	sc_out<int> addr_to_rf;					//address of Rdest to write data into RF --> WRITEBACK STAGE


	//internal variables
	sc_signal<int> instruction;
	sc_signal<int> pc;
	int temp;

	//Decode stage
	sc_signal<int> opcode, rdest, op_ext, rsrc, imm;
	bool w_RF, w_DM, r_DM;

	//Execute stage
	sc_signal<int> opcode_EXE, rdest_EXE, op_ext_EXE, rsrc_EXE, imm_EXE;

	//DM stage
	sc_signal<int> opcode_DM, rdest_DM, op_ext_DM, rsrc_DM, imm_DM;
	sc_signal<int> rf_data1_DM, rf_data2_DM, alu_output_DM;
	sc_signal<bool> w_RF_DM, w_DM_DM, r_DM_DM;

	//WB stage
	sc_signal<int> rdest_WB, output_WB;
	sc_signal<bool> w_RF_WB;

	void prc_controller();

	SC_HAS_PROCESS(controller);

	controller(sc_module_name name) : sc_module(name) {
		SC_METHOD(prc_controller);
		sensitive << pm_instr;
	}

};

void controller::prc_controller() {
	pc = pc + 1;
	cout << "\nFetch at " << sc_time_stamp() << endl;
	pc_to_pm = pc;
	instruction = pm_instr;
	cout << "Current Instruction: " << instruction << endl;

	


	// ------------------------------- DECODING -------------------------//
	cout << "\nDecode at " << sc_time_stamp() << endl;
	opcode.write((instruction & 0xF000) >> 12);		//keep only bits [15, 12]
	rdest.write((instruction & 0x0F00) >> 8);		//keep only bits [11, 8]
	op_ext.write((instruction & 0x00F0) >> 4);		//keep only bits [7, 4]
	rsrc = instruction & 0x000F;					//keep only bits [3, 0]
	imm = instruction & 0x00FF;						//keep only bits [7, 0]
	cout << "Opcode = " << opcode << "\n" <<
		"Rdest = " << rdest << "\n" <<
		"Op Ext = " << op_ext << "\n" <<
		"Rsrc = " << rsrc << "\n" <<
		"Immediate = " << imm << endl;

	cout << "[Z, C, N, F] = " << "[" << z_flag << ", " << c_flag << ", " << n_flag << ", " << f_flag << "]" << endl;

	read_to_rf = 1;					//default read enable to RF
	rf_addr1 = rdest.read();		//address to RF	to get rdest
	rf_addr2 = rsrc.read();			//address to RF to get rsrc




	// --------------------------------- EXE -----------------------------//
	cout << "\nExecute at " << sc_time_stamp() << endl;
	opcode_EXE.write(opcode);		//foward decoded signals to next stage of pipeline (EXECUTION)
	rdest_EXE.write(rdest);
	op_ext_EXE.write(op_ext);
	rsrc_EXE.write(rsrc);
	imm_EXE.write(imm);

	cout << "opcode_EXE = " << opcode_EXE << endl <<
		"rdest_addr_EXE = " << rdest_EXE << endl <<
		"op_ext_EXE = " << op_ext_EXE << endl <<
		"rsrc_addr_EXE = " << rsrc_EXE << endl <<
		"imm_value_EXE = " << imm_EXE << endl; 

	add_sig = addu_sig = mul_sig = and_sig = or_sig = xor_sig = mov_sig = jump_sig = lui_sig =
		imm_sig = twos_sig = unsigned_sig = shift_sig = shiftSign = 0;		//all control signals default to 0
	w_DM = r_DM = 0;					//clear r/w signals to DM
	w_RF = 1;							//default writeback to RF
	alu_en = 1;							//default enable alu
	
	imm_to_alu.write(imm_EXE);			//immediate input value for alu

	if (opcode_EXE == 0b0000) {			//add, addu, addc, mul, sub, subc, cmp, and, or, xor, mov
		if (op_ext_EXE == 0b0101) {				//add
			add_sig = 1;
		}
		else if (op_ext_EXE == 0b0110) {		//addu
			add_sig = 1;
			unsigned_sig = 1;
		}
		else if (op_ext_EXE == 0b0111) {		//addc
			add_sig = 1;
		}
		else if (op_ext_EXE == 0b1110) {		//mul
			mul_sig = 1;
		}
		else if (op_ext_EXE == 0b1001) {		//sub
			add_sig = 1;
			twos_sig = 1;
		}
		else if (op_ext_EXE == 0b1010) {		//subc
			add_sig = 1;
			twos_sig = 1;
		}
		else if (op_ext_EXE == 0b1011) {		//cmp
			add_sig = 1;
			twos_sig = 1;
			w_RF = 0;
		}
		else if (op_ext_EXE == 0b0001) {		//and
			and_sig = 1;
		}
		else if (op_ext_EXE == 0b0010) {		//or
			or_sig = 1;
		}
		else if (op_ext_EXE == 0b0011) {		//xor
			xor_sig = 1;
		}
		else if (op_ext_EXE == 0b1101) {		//mov
			mov_sig = 1;
		}
		else if (op_ext_EXE == 0b0000) {		//NOP
			alu_en = 0;
			w_RF = 0;
		}
		else {
			alu_en = 0;
			w_RF = 0;
		}
	}
	else if (opcode_EXE == 0b0101) {	//addi
		add_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b0110) {	//addui
		add_sig = 1;
		imm_sig = 1;
		unsigned_sig = 1;
	}
	else if (opcode_EXE == 0b0111) {	//addci
		add_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b1110) {	//muli
		mul_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b1001) {	//subi
		add_sig = 1;
		imm_sig = 1;
		twos_sig = 1;
	}
	else if (opcode_EXE == 0b1010) {	//subci
		add_sig = 1;
		imm_sig = 1;
		twos_sig = 1;
	}
	else if (opcode_EXE == 0b1011) {	//cmpi
		add_sig = 1;
		imm_sig = 1;
		twos_sig = 1;
		w_RF = 0;
	}
	else if (opcode_EXE == 0b0001) {	//andi
		and_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b0010) {	//ori
		or_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b0011) {	//xori
		xor_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b1101) {	//movi
		mov_sig = 1;
		imm_sig = 1;
	}
	else if (opcode_EXE == 0b1000) {	//lsh/lshi/ash/ashi
		shift_sig = 1;
		if (op_ext_EXE == 0b0100) {		//lsh
			unsigned_sig = 1;
		}
		else if (op_ext_EXE == 0b0000) {	//lshi left
			unsigned_sig = 0;
			shiftSign = 0;
			imm_sig = 1;
		}
		else if (op_ext_EXE == 0b0001) {	//lshi right
			unsigned_sig = 1;
			shiftSign = 1;
			imm_sig = 1;
		}
		else if (op_ext_EXE == 0b0110) {	//ash
			unsigned_sig = 0;
		}
		else if (op_ext_EXE == 0b0010) {	//ashi left
			shiftSign = 0;
			imm_sig = 1;
		}
		else if (op_ext_EXE == 0b0011) {	//ashi right
			shiftSign = 1;
			imm_sig = 1;
		}
	}
	else if (opcode_EXE == 0b1111) {	//lui
		lui_sig = 1;
		imm_sig = 1;
		shift_sig = 1;
	}
	else if (opcode_EXE == 0b0100) {			//load or store
		if (op_ext_EXE == 0b0000) {
			w_RF = 1;
			r_DM = 1;
			alu_en = 0;
		}
		else if (op_ext_EXE == 0b0100) {		//store
			w_RF = 0;
			w_DM = 1;
			alu_en = 0;
		}
		else if (op_ext_EXE == 0b1100) {		//jump not equal
			w_RF = 0;
			alu_en = 0;
		}
	}
	else {
		alu_en = 0;
		w_RF = 0;
	}




	// -------------------------------- MEM ACCESS ------------------------------//
	cout << "\nMemory Access stage at " << sc_time_stamp() << endl;
	opcode_DM.write(opcode_EXE);			//forward signals from EXECUTION stage to MEM ACCESS stage
	rdest_DM.write(rdest_EXE);
	op_ext_DM.write(op_ext_EXE);
	rsrc_DM.write(rsrc_EXE);
	imm_DM.write(imm_EXE);
	w_RF_DM.write(w_RF);
	r_DM_DM.write(r_DM);
	w_DM_DM.write(w_DM);
	
	cout << "opcode_DM = " << opcode_DM << endl <<
		"rdest_addr_DM = " << rdest_DM << endl <<
		"op_ext_DM = " << op_ext_DM << endl <<
		"rsrc_addr_DM = " << rsrc_DM << endl <<
		"imm_value_DM = " << imm_DM << endl <<
		"w_RF_DM = " << w_RF_DM << endl <<
		"r_DM_DM = " << r_DM_DM << endl <<
		"w_DM_DM = " << w_DM_DM << endl;

	read_to_dm.write(r_DM_DM);				//read port for DM
	write_to_dm.write(w_DM_DM);				//write port for DM


	//for jump instruction
	if (opcode_DM == 0b0100) {			//load or store
		if (rdest_DM == 0b0001) {		//jump not equal
			if (!z_flag) {
				temp = alu_to_controller_pc;
				temp = temp / 2;
				pc = temp;
				cout << "Jumping to pc " << temp << " at timestamp " << sc_time_stamp() << endl;
			}
		}
	}



	//---------------------------------- WB ------------------------------//
	w_RF_WB.write(w_RF_DM);					//forward signals from MEM ACESS stage to WRITEBACK stage
	rdest_WB.write(rdest_DM);

	write_to_rf = w_RF_WB;					//write port for RF
	addr_to_rf = rdest_WB;					//rdest port for RF
}
