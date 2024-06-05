#ifndef L_VECTOR_H
#define L_VECTOR_H

#include "LAllocator.hpp"

namespace _LUL_
{
	template<class T>
	class LVector : public LAllocatorT<T, 4>
	{
		LUINT64 m_uAllocated = 0;

	public:
		void Push(const LUINT32& itm)
		{
			if (m_uAllocated > this->Capacity())
				this->Resize();

			static_cast<LUINT32*>(this->_RawBuffer)[m_uAllocated++] = itm;
		}

		const LUINT64& Size()
		{
			return m_uAllocated;
		}

		void Pop()
		{
			if (m_uAllocated <= 0)
				return; 

			--m_uAllocated;
		}

		T* Front() { return this->_RawBuffer[0]; }
		T* Back() { return this->_RawBuffer[(m_uAllocated - 1)]; }


		T* begin() { return (T*)(this->Begin()); }
		T* end() { return (T*)(this->Begin() + (m_uAllocated * sizeof(T))); }
	};
}

#endif // !L_VECTOR_H