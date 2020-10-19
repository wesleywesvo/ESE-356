#include "communications.h"

void communications::prc_output() {
	//asynch reset or active clear
	if (!reset || clear) {
		payload.write(0);
		count.write(0);
		error.write(0);
		payloadVar = 0;
		errorVar = 0;
		countVar = 0;
		return;
	}

	headerVar = inData.read().range(11, 8);
	payloadVar = inData.read().range(7, 4);
	parityVar = inData.read().range(0, 0);

	//checks if header == 1, writes into payload and increments count
	if (headerVar == 1) {
		payload.write(inData.read().range(7,4));
		countVar++;

		//xor middle four bits with each other to see if 0 for even, 1 for odd
		if (!payloadVar.xor_reduce() && parityVar == 0) {		//middle is even but parity = 0 (should be 1)
			errorVar++;
		}
		else if (payloadVar.xor_reduce() && parityVar == 1) {	//middle is odd but parity = 1 (should be 0)
			errorVar++;
		}
	}

	//write internal variables into output
	count.write(countVar);
	error.write(errorVar);
}