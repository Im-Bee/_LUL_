#ifndef LUL_EXCEPTIONS_H
#define LUL_EXCEPTIONS_H

#define L_EXCEPT() _LUL_::LException("An unknown exception occured.", __FUNCTION__, __FILE__, __LINE__)
#define L_EXCEPT_MSG(customMsg) _LUL_::LException(customMsg, __FUNCTION__, __FILE__, __LINE__)
#define L_EXCEPT_BAD_ALLOC() _LUL_::LExceptBadAlloc(__FUNCTION__, __FILE__, __LINE__)
#define L_EXCEPT_OUTSIDE_OF_BUFFER() _LUL_::LExceptOutsideOfBuffer(__FUNCTION__, __FILE__, __LINE__)


namespace _LUL_
{
	constexpr LCHAR DataCorruptionMessage[] = "Exception data is corrupted.";
	constexpr LINT32 DataCorruptionValue = -1;

	struct LUL_API LExceptionData
	{
		LExceptionData() = delete;
		explicit LExceptionData(LCHAR const* const sMsg) noexcept : LExceptionData(sMsg, 0, 0, 0) {};
		LExceptionData(
			LCHAR const* const sMsg,
			LCHAR const* const sFunction,
			LCHAR const* const sFile,
			const LINT32 line) noexcept;

		~LExceptionData() noexcept;

		LExceptionData(LExceptionData& other) noexcept;
		LExceptionData(LExceptionData&&) noexcept = default;

		const LCHAR* GetMsg() noexcept;
		const LCHAR* GetFile() noexcept;
		const LCHAR* GetFunctionName() noexcept;
		const LINT32& GetLine() noexcept;

	private:	
		struct Impl;
		Impl* m_pImpl;
	};

	class LUL_API LException
	{
		LExceptionData m_Data;

	public:
		LException() noexcept : LException("An unknown exception occured.") {};
		explicit LException(LCHAR const* const sMsg) noexcept : LException(sMsg, 0, 0, 0) {};
		LException(
			LCHAR const* const sMsg,
			LCHAR const* const sFunction,
			LCHAR const* const sFile,
			const LINT32 line) noexcept;

		~LException() noexcept = default;

		LException(LException&) noexcept = default;
		LException(LException&&) noexcept = default;

	public:
		LCHAR const* const What() { return m_Data.GetMsg(); }

	};

	class LUL_API LExceptBadAlloc
		: public _LUL_::LException
	{
	public:
		LExceptBadAlloc() noexcept : LException("Bad allocation.") {};
		LExceptBadAlloc(
			LCHAR const* const sFunction,
			LCHAR const* const sFile,
			const LINT32 line) noexcept 
			: LException("Bad allocation.", sFunction, sFile, line) {};
	};

	class LUL_API LExceptOutsideOfBuffer
		: public _LUL_::LException
	{
	public:
		LExceptOutsideOfBuffer() noexcept : LException("Index was outside of buffer's space.") {};
		LExceptOutsideOfBuffer(
			LCHAR const* const sFunction,
			LCHAR const* const sFile,
			const LINT32 line) noexcept
			: LException("Index was outside of buffer's space.", sFunction, sFile, line) {};
	};
}

#endif // !LUL_EXCEPTIONS_H
