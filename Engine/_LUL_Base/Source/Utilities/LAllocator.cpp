#include "_LUL_Base.h"

#define LUL_RIGHT_MUL_TSIZE (right * m_uTypeS)
#define LUL_CASE_

// LIterator -------------------------------------------------------------------
// -----------------------------------------------------------------------------
void _LUL_::LIterator::operator=(const LIterator& right)
{
    this->Location = right.Location;
}

// -----------------------------------------------------------------------------
void _LUL_::LIterator::operator=(const LUINT64& right)
{
    Location = right;
}

// -----------------------------------------------------------------------------
_LUL_::LIterator _LUL_::LIterator::operator+(const LIterator& right) const
{
    return LIterator(this->Location + right.Location);
}

// -----------------------------------------------------------------------------
_LUL_::LIterator _LUL_::LIterator::operator+(const LUINT64& right) const
{
    return LIterator(Location + LUL_RIGHT_MUL_TSIZE);
}

// -----------------------------------------------------------------------------
_LUL_::LIterator _LUL_::LIterator::operator-(const LIterator& right) const
{
    return LIterator(this->Location - right.Location);
}

// -----------------------------------------------------------------------------
_LUL_::LIterator _LUL_::LIterator::operator-(const LUINT64& right) const
{
    return LIterator(Location - LUL_RIGHT_MUL_TSIZE);
}

// -----------------------------------------------------------------------------
void _LUL_::LIterator::operator-=(const LIterator& right)
{
    this->Location -= right.Location;
}

// -----------------------------------------------------------------------------
void _LUL_::LIterator::operator-=(const LUINT64& right)
{
    Location -= LUL_RIGHT_MUL_TSIZE;
}

// -----------------------------------------------------------------------------
void _LUL_::LIterator::operator+=(const LIterator& right)
{
    this->Location += right.Location;
}

// -----------------------------------------------------------------------------
void _LUL_::LIterator::operator+=(const LUINT64& right)
{
    Location += LUL_RIGHT_MUL_TSIZE;
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LIterator::operator<(const LIterator& right) const
{
    return (this->Location < right.Location);
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LIterator::operator<(const LUINT64& right) const
{
    return (Location < right);
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LIterator::operator<=(const LIterator& right) const
{
    return (this->Location <= right.Location);
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LIterator::operator<=(const LUINT64& right) const
{
    return (Location <= right);
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LIterator::operator==(const LIterator& right) const
{
    return (this->Location == right.Location);
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LIterator::operator==(const LUINT64& right) const
{
    return (Location == right);
}

// -----------------------------------------------------------------------------
_LUL_::LIterator _LUL_::LIterator::operator++(int)
{
    auto r = LIterator(this->Location);
    this->Location += this->m_uTypeS;
    return std::move(r);
}

// -----------------------------------------------------------------------------
_LUL_::LIterator& _LUL_::LIterator::operator++()
{
    this->Location += this->m_uTypeS;
    return *this;
}

// -----------------------------------------------------------------------------
_LUL_::LIterator _LUL_::LIterator::operator--(int)
{
    auto r = LIterator(this->Location);
    this->Location -= this->m_uTypeS;
    return std::move(r);
}

// -----------------------------------------------------------------------------
_LUL_::LIterator& _LUL_::LIterator::operator--()
{
    this->Location -= this->m_uTypeS;
    return *this;
}

// LAllocator ------------------------------------------------------------------
// -----------------------------------------------------------------------------
_LUL_::LAllocator::LAllocator(const LUINT64& uInitialByteSize)
    : m_uByteCapacity(uInitialByteSize)
{
#if _WIN32 || _WIN64
    _RawBuffer = HeapAlloc(GetProcessHeap(), NULL, uInitialByteSize);
#else
#   error "_LUL_::LAllocator::LAllocator(const LUINT64& uInitialByteSize) not implemented on current system."
#endif
    if (_RawBuffer == nullptr)
        throw new L_EXCEPT_BAD_ALLOC();

    m_uBegBuffPosition = reinterpret_cast<MEM_LOCATION>(_RawBuffer);
    m_uEndBuffPosition = m_uBegBuffPosition + m_uByteCapacity;
}

// -----------------------------------------------------------------------------
_LUL_::LAllocator::LAllocator(LAllocator&& other) noexcept
{
    this->m_uByteCapacity = other.m_uByteCapacity;

#if _WIN32 || _WIN64
    this->_RawBuffer = HeapAlloc(GetProcessHeap(), NULL, this->m_uByteCapacity);
#else
#   error "_LUL_::LAllocator::LAllocator(LAllocator&& other) not implemented on current system."
#endif

    errno_t e = memcpy_s(
        this->_RawBuffer,
        this->m_uByteCapacity,
        other._RawBuffer,
        other.m_uByteCapacity);
    L_LOG(Error, L"%s noexcept: errno_t %d", __FUNCTION__, e);

    m_uBegBuffPosition = reinterpret_cast<MEM_LOCATION>(_RawBuffer);
    m_uEndBuffPosition = m_uBegBuffPosition + m_uByteCapacity;
}

// -----------------------------------------------------------------------------
_LUL_::LAllocator::~LAllocator()
{
    if (!HeapFree(GetProcessHeap(), NULL, _RawBuffer))
    {
        L_LOG(
            Error,
            L"%s heap free was unsuccessful | GetLastError = %lu",
            __FUNCTION__,
            GetLastError());
    }
}

// Protected -------------------------------------------------------------------
// -----------------------------------------------------------------------------
const _LUL_::MEM_LOCATION& _LUL_::LAllocator::_BeginML()
{
    return m_uBegBuffPosition;
}

// -----------------------------------------------------------------------------
const _LUL_::MEM_LOCATION& _LUL_::LAllocator::_EndML()
{
    return m_uEndBuffPosition;
}

//  ----------------------------------------------------------------------------
void _LUL_::LAllocator::_Resize(const LUINT64& uByteResize)
{
    m_uByteCapacity += uByteResize;

#if _WIN32 || _WIN64
    void* reallocPtr = HeapReAlloc(GetProcessHeap(), NULL, _RawBuffer, m_uByteCapacity);
#else
#   error "void _LUL_::LAllocator::_Resize(const LUINT64& uByteResize) not implemented on current system."
#endif

    if (reallocPtr == nullptr)
    {
        // TO DO: Fiugre out what to do after HeapReAlloc fails.
        throw new L_EXCEPT_BAD_ALLOC();
    }

    _RawBuffer          = reallocPtr;
    m_uBegBuffPosition  = reinterpret_cast<MEM_LOCATION>(_RawBuffer);
    m_uEndBuffPosition  = m_uBegBuffPosition + m_uByteCapacity;
}

// -----------------------------------------------------------------------------
LBOOL _LUL_::LAllocator::_RecalcResize(LUINT64& uCurrentCapacity, LUINT32& uResize)
{
    switch (m_uByteCapacity)
    {


    default:
        return false;
    }
}
