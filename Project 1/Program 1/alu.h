#include "systemc.h"

SC_MODULE(alu) {
	//inputs
	sc_in<bool> clock, en;
	sc_in<int> rdest, rsrc, imm;			//rdest is always 'operand1'

	//controller signals
	sc_in<bool> add_sig, addu_sig, mul_sig, and_sig, or_sig,			//operator control signals
		xor_sig, mov_sig, lui_sig;														
	sc_in<bool> imm_sig, twos_sig, unsigned_sig, shift_sig, shiftSign;				//control signals
	sc_in<bool> bcond_sig, jump_sig;

	//outputs
	sc_out<int> output, rsrc_out;
	//flags
	sc_out<bool> f_flago, c_flago, n_flago, z_flago;		//overflow, carry, negative, zero flag

	//internal variables
	int operand1;						//value of rdest
	int operand2;						//value of rsrc or immediate
	int result;
	sc_uint<16> op1;					//unsigned used for addu/addui/lsh/lshi
	sc_uint<16> op2;					//unsigned used when unsigned_sig asserted


	void prc_alu();

	SC_HAS_PROCESS(alu);

	alu(sc_module_name name) : sc_module(name) {
		SC_METHOD(prc_alu);
		//sensitive << add_sig << addu_sig << and_sig << or_sig << xor_sig << mov_sig << shift_sig << 
			//bcond_sig << jump_sig << lui_sig << imm_sig;
		sensitive << clock.pos();
	}

};

void alu::prc_alu() {
	if (en) {
		operand1 = rdest.read();
		operand2 = rsrc.read();
		//if imm_sig then operand2 contains the immediate value
		//else, operand2 contains the data from Rsrc
		if (imm_sig) {
			operand2 = imm;
		}
		else {
			operand2 = rsrc;
		}

		//check if data is needed as negative for operators ie: sub, cmp, or negative immediate
		if (twos_sig) {
			operand2 = operand2 * -1;
		}
		else if (unsigned_sig) {
			op1 = operand1;
			op2 = operand2;
		}

		//alu operators
		if (add_sig) {										//add, addi, sub, subi, cmp, cmpi
			result = operand1 + operand2;
		}

		else if (addu_sig) {										//addu, addui
			result = op1 + op2;
		}

		else if (mul_sig) {									//mul, muli
			result = operand1 * operand2;
		}

		else if (and_sig) {									//and, andi
			result = operand1 & operand2;
		}

		else if (or_sig) {									//or, ori
			result = operand1 | operand2;
		}

		else if (xor_sig) {									//xor, xori
			result = operand1 ^ operand2;
		}

		else if (mov_sig) {									//mov, movi
			result = operand2;
		}

		else if (shift_sig) {
			//check for LUI first
			if (lui_sig) {
				if (operand2 > 15) {					//lui can only load up to 4-bit values i think?
					operand2 = 15;
				}
				result = operand2 << 4;					//shift left 4 times
			}

			//logical or arithmetic shift
			else if (unsigned_sig) {								//logical shift -> use unsigned type
				if (shiftSign) {							//1 == right shift
					result = op1 >> op2;
				}
				else {
					result = op1 << op2;
				}
			}
			else if (!unsigned_sig) {											//arithmetic shift -> use int type
				if (shiftSign) {							//1 == right shift
					result = operand1 >> operand2;
				}
				else {
					result = operand1 << operand2;
				}
			}
		}

		else if (jump_sig) {		//rsrc is target address -> just return to controller and update pc with rsrc?

		}

		else if (bcond_sig) {   // branch if condition

		}


		//write to output of alu
		output = result;
		rsrc_out = rsrc.read();
		cout << "alu timestamp: " << sc_time_stamp() << " " << output.read() << endl;

		//assign flags
		//zero flag
		if (result == 0) {
			z_flago = 1;
		}
		else {
			z_flago = 0;
		}
		//carry flag
		if (result > 32767) {
			c_flago = 1;
		}
		else {
			c_flago = 0;
		}
		//negative flag
		if (result < 0) {
			n_flago = 1;
		}
		else {
			n_flago = 0;
		}
	}
	else {
		cout << "timestamp: " << sc_time_stamp() << " rsrc.read() = " << rsrc.read();
		rsrc_out = rsrc.read();
		output = rdest.read();
	}
}
