#include <stdlib.h>
#include "Memory.hpp"

int64_t MemoryLog::usedAmount_ = 0;

void MemoryLog::accrue(int64_t amount) {
	usedAmount_ += amount;
	//std::cout<<"Memory in use is - "<<usedAmount_<<" bytes. Just added - "<<amount<<" bytes.\n";
}
