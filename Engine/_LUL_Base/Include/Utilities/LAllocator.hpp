#ifndef L_LALLOCATOR_H
#define L_LALLOCATOR_H

namespace _LUL_
{
	typedef LUINT64 MEM_LOCATION;

	struct LUL_API LIterator
	{
		MEM_LOCATION Location;

		LIterator() : Location(0) {};
		explicit LIterator(const MEM_LOCATION& m) : Location(m) {};
		explicit LIterator(MEM_LOCATION&& m) : Location(m) {};
		explicit LIterator(const MEM_LOCATION& m, LUINT32&& dataSize) : Location(m), m_uTypeS(dataSize) {};
		explicit LIterator(MEM_LOCATION&& m, LUINT32&& dataSize) : Location(m), m_uTypeS(dataSize) {};

		~LIterator() = default;

		void operator=(const LIterator& right);
		void operator=(const LUINT64& right);
					 
		LIterator operator+(const LIterator& right) const;
		LIterator operator+(const LUINT64& right) const;
		
		LIterator operator-(const LIterator& right) const;
		LIterator operator-(const LUINT64& right) const;

		void operator-=(const LIterator& right);
		void operator-=(const LUINT64& right);

		void operator+=(const LIterator& right);
		void operator+=(const LUINT64& right);

		LBOOL operator<(const LIterator& right) const;
		LBOOL operator<(const LUINT64& right)	const;

		LBOOL operator>(const LIterator& right) const { return !LIterator::operator<(right); }
		LBOOL operator>(const LUINT64& right)	const { return !LIterator::operator<(right); }

		LBOOL operator<=(const LIterator& right) const;
		LBOOL operator<=(const LUINT64& right)	 const;

		LBOOL operator>=(const LIterator& right) const { return !LIterator::operator<=(right); }
		LBOOL operator>=(const LUINT64& right)	 const { return !LIterator::operator<=(right); }

		LBOOL operator==(const LIterator& right) const;
		LBOOL operator==(const LUINT64& right)	 const;

		LBOOL operator!=(const LIterator& right) const { return !LIterator::operator==(right); }
		LBOOL operator!=(const LUINT64& right)	 const { return !LIterator::operator==(right); }

		LIterator operator++(int);
		LIterator& operator++();

		LIterator operator--(int);
		LIterator& operator--();

		operator void* () const
		{
			return reinterpret_cast<void*>(Location);
		}

	protected:
		LUINT32 m_uTypeS = 1;
	};

	template<class T>
	struct LIteratorT : public LIterator
	{
		LIteratorT() : LIterator(0, sizeof(T)) {};
		explicit LIteratorT(const MEM_LOCATION& m) : LIterator(m, sizeof(T)) {};
		explicit LIteratorT(MEM_LOCATION&& m) : LIterator(m, sizeof(T)) {};

		~LIteratorT() = default;

		operator LIterator() const
		{
			return dynamic_cast<LIterator>(*this);
		}

		operator T* () const
		{
			return reinterpret_cast<T*>(Location);
		}
	};

	class LUL_API LAllocator
	{
		LUINT64 m_uByteCapacity;

		MEM_LOCATION m_uBegBuffPosition;
		MEM_LOCATION m_uEndBuffPosition;

	public:
		LAllocator() = delete;
		LAllocator(const LUINT64& uInitialByteSize);

		LAllocator(const LAllocator&) noexcept = default;
		LAllocator(LAllocator&& other) noexcept;

		~LAllocator();

	public:
		void* _RawBuffer;

	public:
		const LUINT64& GetByteCapacity() { return m_uByteCapacity; }

	protected:
		const MEM_LOCATION& _BeginML();
		
		const MEM_LOCATION& _EndML();

		void _Resize(const LUINT64& uByteResize);

		LBOOL _RecalcResize(LUINT64& uCurrentCapacity, LUINT32& uResize);

	};

	template<class T, LUINT32 InitialSize = 32>
	class LAllocatorT : public LAllocator
	{
		LUINT32 m_uResize = InitialSize;
		LUINT64 m_uCapacity = m_uResize;

		LUINT32 m_uSizeOfT = sizeof(T);
		LUINT32 m_uByteResize = m_uResize * m_uSizeOfT;

	public:
		LAllocatorT() : LAllocator(sizeof(T) * InitialSize) {};

		LAllocatorT(const LAllocatorT&) noexcept = default;
		LAllocatorT(LAllocatorT&& other) noexcept : LAllocator(other) {};

		~LAllocatorT() = default;

	public:
		T& operator[](const LUINT64& i)
		{
			if (i >= m_uCapacity)
			{
				throw new L_EXCEPT_OUTSIDE_OF_BUFFER();
			}

			return (reinterpret_cast<T*>(_RawBuffer)[i]);
		}

		T& operator[](const LIterator& i)
		{
			if (i <   Begin() ||
				i >= (End()))
			{
				throw new L_EXCEPT_OUTSIDE_OF_BUFFER();
			}

			return (reinterpret_cast<T*>(i.Location));
		}

		const LUINT64& Capacity()
		{
			return m_uCapacity;
		}

		void Reserve(const LUINT64& uAmount)
		{
			if (uAmount <= m_uCapacity)
				return;

			_Resize((uAmount * m_uSizeOfT) - this->GetByteCapacity());
			m_uCapacity += (uAmount - m_uCapacity);
		}

		const MEM_LOCATION& Begin() { return this->_BeginML(); }
		const MEM_LOCATION& End() { return this->_EndML(); }

	protected:
		void Resize()
		{
			_Resize(m_uByteResize);
			m_uCapacity += m_uResize;

			if (_RecalcResize(m_uCapacity, m_uResize))
				m_uByteResize = m_uResize * m_uSizeOfT;
		}
	};

	class LAllocatorU : public LAllocator
	{
		LAllocatorT<LUINT8, 0> m_ByteOffsets = {};

	public:
		LAllocatorU() : LAllocator(0) {};

	public:
		void* Get(const LUINT64& uIndex)
		{
			if (uIndex >= m_ByteOffsets.Capacity())
			{
				throw new L_EXCEPT_OUTSIDE_OF_BUFFER();
			}

			MEM_LOCATION offset = this->_BeginML();
			for (LUINT64 i = 0; i < uIndex; ++i)
			{
				offset += m_ByteOffsets[i];
			}

			if (offset > this->_EndML() - m_ByteOffsets[uIndex])
			{
				throw new L_EXCEPT_OUTSIDE_OF_BUFFER();
			}
			return reinterpret_cast<void*>(offset);
		}

	protected:
		void AddSlot(const LUINT8& uByteSize)
		{
			m_ByteOffsets.Reserve(m_ByteOffsets.Capacity() + 1);
			m_ByteOffsets[m_ByteOffsets.Capacity() - 1] = uByteSize;
			this->_Resize(uByteSize);
		}

	};

	template<class T, LUINT32 R = 32>
	class DExperimentalAllocator
	{
		LUINT32 m_uResize = R;
		LUINT64 m_uCapacity = m_uResize;

		LUINT32 m_uSizeT = sizeof(T);
		LUINT32 m_uByteResize = m_uResize * m_uSizeT;
		LUINT64 m_uByteCapacity = m_uByteResize;

		T* m_RawBuffer;

		MEM_LOCATION m_uBegBuffPosition;
		MEM_LOCATION m_uEndBuffPosition;
		MEM_LOCATION m_uCurBuffPosition;

		LIterator m_uCurItr = LIterator();

	public:
		DExperimentalAllocator()
		{
			m_RawBuffer = reinterpret_cast<T*>(HeapAlloc(GetProcessHeap(), NULL, m_uByteCapacity));
			if (m_RawBuffer == nullptr)
				throw new L_EXCEPT_BAD_ALLOC();
			m_uBegBuffPosition = m_uCurBuffPosition = reinterpret_cast<MEM_LOCATION>(m_RawBuffer);
			m_uEndBuffPosition = m_uBegBuffPosition + m_uByteCapacity;
		}

		~DExperimentalAllocator()
		{
			if (m_RawBuffer != nullptr)
				HeapFree(GetProcessHeap(), NULL, m_RawBuffer);
		}

		DExperimentalAllocator(const DExperimentalAllocator<T, R>& other) noexcept
		{
			this->m_uResize		= other.m_uResize;
			this->m_uCapacity	= other.m_uByteCapacity;

			this->m_uSizeT			= other.m_uSizeT;
			this->m_uByteResize		= other.m_uByteResize;
			this->m_uByteCapacity	= other.m_uByteCapacity;

			m_uBegBuffPosition = (MEM_LOCATION)HeapAlloc(GetProcessHeap(), NULL, this->m_uByteCapacity);
			errno_t e = memcpy_s(
				(void*)this->m_uBegBuffPosition,
				this->m_uByteCapacity,
				(void*)other.m_uBegBuffPosition,
				other.m_uByteCapacity);
			L_LOG(Error, L"DExperimentalAllocator(const DExperimentalAllocator<T, R>& other) noexcept: errno_t %d", e);

			LUINT64 otherBtoCdiff = other.m_uCurBuffPosition - other.m_uBegBuffPosition;
			this->m_uCurBuffPosition = this->m_uBegBuffPosition + otherBtoCdiff;
			this->m_uEndBuffPosition = this->m_uBegBuffPosition + this->m_uByteCapacity;
		}
		DExperimentalAllocator(DExperimentalAllocator<T, R>&&) noexcept = default;

	public:
		LIteratorT<T> Begin() { return LIteratorT<T>(m_uBegBuffPosition); }
		LIteratorT<T> End() { return LIteratorT<T>(m_uEndBuffPosition); }
		T& operator[](const LIterator& location)
		{
			if (location.Location >= m_uCapacity)
			 	throw new L_EXCEPT_OUTSIDE_OF_BUFFER();

			return *(T*)(m_uBegBuffPosition + (location.Location * m_uSizeT));
		}
		T& operator[](const MEM_LOCATION& location)
		{
			if (location >= m_uCapacity)
				throw new L_EXCEPT_OUTSIDE_OF_BUFFER();

			return *(T*)(m_uBegBuffPosition + (location * m_uSizeT));
		}
		void Resize()
		{
			m_uByteCapacity += m_uByteResize;
			const LUINT64 bToCdiff = m_uCurBuffPosition - m_uBegBuffPosition;
			auto tmpPtr = reinterpret_cast<T*>(HeapReAlloc(GetProcessHeap(), NULL, m_RawBuffer, m_uByteCapacity));
			if (tmpPtr == nullptr)
			{
				throw new L_EXCEPT_BAD_ALLOC();
			}

			m_RawBuffer = tmpPtr;
			m_uBegBuffPosition = reinterpret_cast<MEM_LOCATION>(m_RawBuffer);
			m_uEndBuffPosition = m_uBegBuffPosition + m_uByteCapacity;
			m_uCurBuffPosition = m_uBegBuffPosition + bToCdiff;

			m_uCapacity += m_uResize;
		}
		void Push(const T& itm)
		{
			if (m_uCurItr.Location >= m_uCapacity)
				Resize();
			
			static_cast<T*>(m_RawBuffer)[m_uCurItr.Location++] = itm;
		}
		void Push(T&& itm)
		{
			if ((m_uEndBuffPosition - m_uCurBuffPosition) <= m_uSizeT)
				Resize();

			static_cast<T*>(m_RawBuffer)[m_uCurItr.Location++] = std::move(itm);
		}
	};
}

#endif // !L_LALLOCATOR_H