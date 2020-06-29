//---------------------------------------------------------------------------

#include <random>
#include <algorithm>
#include <stdexcept>

#include "UnitUniqueRandom.h"
//---------------------------------------------------------------------------

MyUniqueRandom::MyUniqueRandom(size_t _N, size_t gap)
{
	Data.resize(_N);
	size_t i = 0;
	for (auto & v : Data)
	{
		v = i;
        i += gap;
	}

	std::random_device rd;
    std::mt19937 g(rd());

	std::shuffle(Data.begin(), Data.end(), g);
//	std::shuffle(Data.begin(), Data.end());
}
//----------------------------------------------------------------------------

int MyUniqueRandom::GetNext()
{
	if (ind < Data.size())
		return Data[ind++];
	else
	{
        throw std::runtime_error("End of MyUniqueRandom");

		return -1;
	}
}
//----------------------------------------------------------------------------
