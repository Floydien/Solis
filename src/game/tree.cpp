#include "tree.h"

int Tree::chopWood(size_t amt) {
	if(amt > woodAmt) {
		int tmp = woodAmt;
		woodAmt = 0;
		return tmp;
	} else {
		woodAmt -= amt;
		return amt;
	}
} 