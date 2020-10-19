#include "counters.h"

void counters::prc_counter1() {
	if (load1) {		//load1 = 1 -> load in1
		count1 = in1;
		ended = false;
	}
	else if (ended) {			//hold until counter is reloaded
		
	}
	else if (dec1) {	//dec1 = 1 -> decrement 
		tempCount1 = in1.read();
		if (tempCount2 > 255) {
			overflow2 = true;
			return;
		}
		tempCount1 = count1.read();	//read current value of counter1
		if (tempCount1 == 0) {		//if already 0 then will overflow
			overflow1 = true;
			return;
		}
		tempCount1--;				//decrement
		count1.write(tempCount1);	//writeback into counter1
	}
}

void counters::prc_counter2() {
	if (load2) {		//load2 = 1 -> load in2
		tempCount2 = in2.read();
		if (tempCount2 > 255) {
			overflow2 = true;
			return;
		}
		count2 = in2;
		ended = false;
	}
	else if (ended) {			//hold until counter is reloaded

	}
	else if (dec2) {	//dec2 = 1 -> decrement by in3 
		tempCount2 = count2.read();			//read current value of counter2
		if (tempCount2 < in3.read()) {		//if counter2 value < value of in3
			overflow2 = true;
			return;
		}
		tempCount2 = tempCount2 - in3.read();	//subtract by value of in3
		count2.write(tempCount2);				//writeback into counter2
	}
}

void counters::prc_ended() {
	if (count1.read() == count2.read() || overflow1 || overflow2 || ended) {
		ended = true;
	}
	else {
		ended = false;
		overflow1 = false;
		overflow2 = false;
	}
}