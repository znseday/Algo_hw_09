//---------------------------------------------------------------------------
#ifndef UnitUniqueRandomH
#define UnitUniqueRandomH
//---------------------------------------------------------------------------

#include <vector>


class MyUniqueRandom
{
private:
	std::vector<int> Data;
    size_t ind = 0;

public:
	MyUniqueRandom() = delete;
	MyUniqueRandom(size_t _N, size_t gap = 1);

	int GetNext();
};



#endif
