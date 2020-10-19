#include "controller.h"
#include "pm.h"
#include "rf.h"
#include "alu.h"
#include "dm.h"

int sc_main(int argc, char* argv[]) {
	/* Program 1 Listing //
	int pm_data[256] = {
		0x0400,						//placeholder instructions since controller unit is sensitive to pm_instr
		0x0000,						//used to keep the controller running in the pipeline after the 'last' instruction
		0x0400,
		0x0000,
		0x0400,

		0x4203,
		0x4405,
		0x5811,
		0x0000,
		0x0352,
		0x4607,
		0x980F,
		0x4205,
		0x0258,
		0x4843,
		0x4445,
		0x0000,

		0x0400,						//placeholder instructions since controller unit is sensitive to pm_instr
		0x0000,						//used to keep the controller running in the pipeline after the 'last' instruction
		0x0400,
		0x0000,
		0x0400,
		0x0000,

	};

	// RF Data used for Program 1 //
	int rf_data[16] = { 0 };
	rf_data[3] = 10;
	rf_data[5] = 12;
	rf_data[7] = 14;
	rf_data[8] = 12;

	// DM Data used for Program 1 //
	int dm_data[256] = { 0 };
	dm_data[10] = 36;
	dm_data[12] = 12;
	dm_data[14] = -24;
	*/
	
	// Program 2 Listing //
	int pm_data[256] = {
		0x0400,						//placeholder instructions since controller unit is sensitive to pm_instr
		0x0000,						//used to keep the controller running in the pipeline after the 'last' instruction
		0x0400,
		0x0000,
		0x0400,

		0x0152,
		0x0153,
		0x0154,
		0x0155,
		0xB10A,
		0x0000,
		0x0400,
		0x41CA,
		0x0000,
		0x0156,
		0x0157,
		0x0158,
		0x0000,
		
		0x0400,						//placeholder instructions since controller unit is sensitive to pm_instr
		0x0000,						//used to keep the controller running in the pipeline after the 'last' instruction
		0x0400,
		0x0000,
		0x0400,
		0x0000,
	};

	//RF Data used for Program 2 and 3 //
	int rf_data[16] = { 0 };
	rf_data[1] = 39;
	rf_data[2] = -25;
	rf_data[3] = 14;
	rf_data[4] = -50;
	rf_data[5] = 70;
	rf_data[6] = 6;
	rf_data[7] = 7;
	rf_data[8] = 8;
	rf_data[10] = 32;

	//DM Data used for Program 2 and 3 //
	int dm_data[256] = { 0 };
	


	/*
	// Program 3 Listing //
	int pm_data[256] = {
		0x0400,						//placeholder instructions since controller unit is sensitive to pm_instr
		0x0000,						//used to keep the controller running in the pipeline after the 'last' instruction
		0x0400,
		0x0000,
		0x0400,

		0x0152,
		0x0153,
		0x0154,
		0x0155,
		0xB135,						//CMPI 53, R1
		0x0000,
		0x0400,
		0x41CA,
		0x0000,
		0x0156,
		0x0157,
		0x0158,
		0x0000,

		0x0400,						//placeholder instructions since controller unit is sensitive to pm_instr
		0x0000,						//used to keep the controller running in the pipeline after the 'last' instruction
		0x0400,
		0x0000,
		0x0400,
		0x0000,
	};
	*/

	//ports
	sc_clock t_clock("clk1", 10, SC_NS);

	//PM ports that connect controller
	sc_signal<int> controller_to_pm_pc;
	sc_signal<int> pm_to_controller_data_out;

	//connect modules
	//PM module
	program_memory pm1("pm1", &pm_data[0]);
	pm1.clock(t_clock);
	pm1.addr_in(controller_to_pm_pc);
	pm1.data_out(pm_to_controller_data_out);
	//------------------------------------------------------------------//

	//RF ports that conect to controller
	sc_signal<bool> controller_to_rf_read, controller_to_rf_write;
	sc_signal<int> controller_to_rf_addr1, controller_to_rf_addr2, controller_to_rf_addr3;
	//RF ports that connect to ALU inputs
	sc_signal<int> rf_to_alu_data1, rf_to_alu_data2;
	//RF ports that connect to DM output <--- Writeback stage
	sc_signal<int> dm_to_rf_data;

	//RF module
	register_file rf1("rf1", &rf_data[0]);
	rf1.clock(t_clock);
	rf1.read(controller_to_rf_read);
	rf1.write(controller_to_rf_write);
	rf1.addr_1(controller_to_rf_addr1);
	rf1.addr_2(controller_to_rf_addr2);
	rf1.addr_3(controller_to_rf_addr3);
	rf1.data_in(dm_to_rf_data);
	rf1.data_out_1(rf_to_alu_data1);
	rf1.data_out_2(rf_to_alu_data2);
	//------------------------------------------------------------------------//

	//ALU ports that connect to controller
	sc_signal<int> controller_to_alu_imm;
	sc_signal<bool> alu_add, alu_addu, alu_mul, alu_and, alu_or, alu_xor, alu_mov, alu_lui, alu_imm,
		alu_twos, alu_unsigned, alu_shift, alu_shiftSign, alu_jump, alu_bcond;
	sc_signal<int> alu_to_dm_rsrc, alu_to_dm_output;
	sc_signal<bool> alu_cflago, alu_fflago, alu_nflago, alu_zflago;
	sc_signal<bool> controller_to_alu_en;

	//ALU module
	alu alu1("alu1");
	alu1.clock(t_clock);
	alu1.rdest(rf_to_alu_data1);
	alu1.rsrc(rf_to_alu_data2);
	alu1.imm(controller_to_alu_imm);
	alu1.en(controller_to_alu_en);
	alu1.add_sig(alu_add);
	alu1.addu_sig(alu_addu);
	alu1.mul_sig(alu_mul);
	alu1.and_sig(alu_and);
	alu1.or_sig(alu_or);
	alu1.xor_sig(alu_xor);
	alu1.mov_sig(alu_mov);
	alu1.lui_sig(alu_lui);
	alu1.imm_sig(alu_imm);
	alu1.twos_sig(alu_twos);
	alu1.unsigned_sig(alu_unsigned);
	alu1.shift_sig(alu_shift);
	alu1.shiftSign(alu_shiftSign);
	alu1.bcond_sig(alu_bcond);
	alu1.jump_sig(alu_jump);
	alu1.output(alu_to_dm_output);
	alu1.rsrc_out(alu_to_dm_rsrc);
	alu1.f_flago(alu_fflago);
	alu1.c_flago(alu_cflago);
	alu1.z_flago(alu_zflago);
	alu1.n_flago(alu_nflago);
	//---------------------------------------------------------//
	sc_signal<bool> controller_to_dm_read, controller_to_dm_write;

	//DM module
	data_memory dm1("dm1", &dm_data[0]);
	dm1.clock(t_clock);
	dm1.addr_in(alu_to_dm_rsrc);
	dm1.data_in(alu_to_dm_output);
	dm1.data_out(dm_to_rf_data);
	dm1.read(controller_to_dm_read);
	dm1.write(controller_to_dm_write);
	//---------------------------------------------------------------------//
	
	//Controller module
	controller controller1("controller1");
	//Instruction fetch
	controller1.pc_to_pm(controller_to_pm_pc);
	controller1.pm_instr(pm_to_controller_data_out);
	//Instruction decode, Register Read
	controller1.rf_addr1(controller_to_rf_addr1);
	controller1.rf_addr2(controller_to_rf_addr2);
	controller1.read_to_rf(controller_to_rf_read);
	//Execute ALU
	controller1.imm_to_alu(controller_to_alu_imm);
	controller1.alu_en(controller_to_alu_en);
	controller1.add_sig(alu_add);
	controller1.addu_sig(alu_addu);
	controller1.mul_sig(alu_mul);
	controller1.and_sig(alu_and);
	controller1.or_sig(alu_or);
	controller1.xor_sig(alu_xor);
	controller1.mov_sig(alu_mov);
	controller1.lui_sig(alu_lui);
	controller1.imm_sig(alu_imm);
	controller1.twos_sig(alu_twos);
	controller1.unsigned_sig(alu_unsigned);
	controller1.shift_sig(alu_shift);
	controller1.shiftSign(alu_shiftSign);
	controller1.bcond_sig(alu_bcond);
	controller1.jump_sig(alu_jump);
	controller1.c_flag(alu_cflago);
	controller1.z_flag(alu_zflago);
	controller1.f_flag(alu_fflago);
	controller1.n_flag(alu_nflago);
	//Data Memory Access
	controller1.alu_to_controller_pc(alu_to_dm_rsrc);		//

	controller1.read_to_dm(controller_to_dm_read);
	controller1.write_to_dm(controller_to_dm_write);
	//Writeback
	controller1.write_to_rf(controller_to_rf_write);
	controller1.addr_to_rf(controller_to_rf_addr3);



	sc_start(250, SC_NS);

	return 0;
}