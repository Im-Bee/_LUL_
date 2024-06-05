#ifndef L_QUEUE_H
#define L_QUEUE_H

#include "LAllocator.hpp"

namespace _LUL_
{
	template<class T>
	class LQueue : public LAllocatorT<T, 4>
	{
		LUINT64 m_uAllocated = 0;

		LAllocatorT<LUINT64, 4> m_Order = {};

	public:
		void Push(const LUINT32& itm)
		{
			if (m_uAllocated > this->Capacity())
			{
				this->Resize();
				m_Order.Resize();
			}

			static_cast<LUINT32*>(this->_RawBuffer)[m_uAllocated++] = itm;
		}

		const LUINT64& Size()
		{
			return m_uAllocated;
		}

		T* Front() { return this->_RawBuffer[0]; }
		T* Back() { return this->_RawBuffer[(m_uAllocated - 1)]; }

		T* begin() { return (T*)(this->Begin()); }
		T* end() { return (T*)(this->Begin() + (m_uAllocated * sizeof(T))); }
	};
}

#endif // !L_QUEUE_H