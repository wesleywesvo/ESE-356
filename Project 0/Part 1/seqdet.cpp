#include "seqdet.h"

void seqdet::prc_seqdet() {
	if (!reset) {		//active-low
		bit0.write(false);
		bit1.write(false);
		bit2.write(false);
		bit3.write(false);
	}
	else {
		bit0 = data_in.read();
		bit1 = bit0.read();
		bit2 = bit1.read();
		bit3 = bit2.read();
	}
}

void seqdet::prc_output() {
	if (clr) {
		data_out.write(false);
	}
	else {
		data_out = bit3 & (!bit2) & bit1 & bit0;
	}
}