#ifndef L_TUPLE_H
#define L_TUPLE_H

#include "LAllocator.hpp"

namespace _LUL_
{
	template<class... UTypes>
	struct LTuple : public _LUL_::LAllocatorU
	{
		LTuple(UTypes... inputs)
		{
			for (const auto p : { inputs... })
				this->AddSlot(sizeof(p));
		}
	};	
}

#endif // !L_TUPLE_H
