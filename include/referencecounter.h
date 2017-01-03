#pragma once
#include <string>

class ReferenceCounter{
public:
	ReferenceCounter():
		refCount(1) {};

	inline int getReferenceCount() const { return refCount; };

	inline void addReference() { refCount++; };
	inline bool removeReference() { refCount--; return refCount == 0; };

private:
	unsigned int refCount;
};