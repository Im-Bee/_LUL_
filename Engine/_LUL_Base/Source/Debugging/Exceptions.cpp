#include "_LUL_Base.h"

struct _LUL_::LExceptionData::Impl
{
	LCHAR Message[LUL_MAX_ERROR_MSG] = { 0 };
	LCHAR FunctionName[LUL_MAX_ERROR_MSG] = { 0 };
	// TO DO: Stacktrace.
	LCHAR File[LUL_MAX_PATH_SIZE] = { 0 };
	LINT32 Line = 0;
};

_LUL_::LExceptionData::LExceptionData(
	LCHAR const* const sMsg, 
	LCHAR const* const sFunction, 
	LCHAR const* const sFile, 
	const LINT32 line) noexcept
{
	constexpr LCHAR emptyStr[] = "";
	constexpr LCHAR noMsgStr[] = "No message provided.";
	m_pImpl = new Impl();

	m_pImpl->Line = line;
	try
	{
		if (sMsg != nullptr)
			strcpy_s(m_pImpl->Message, sMsg);
		else
			strcpy_s(m_pImpl->File, noMsgStr);

		if (sFunction != nullptr)
			strcpy_s(m_pImpl->FunctionName, sFunction);
		else
			strcpy_s(m_pImpl->FunctionName, emptyStr);

		if (sFile != nullptr)
			strcpy_s(m_pImpl->File, sFile);
		else
			strcpy_s(m_pImpl->File, emptyStr);
	}
	catch (...)
	{
		L_LOG(Error, L"%s throws during data collection.", __FUNCTION__);
	}
}

_LUL_::LExceptionData::LExceptionData(LExceptionData& other) noexcept
{
	this->m_pImpl = new LExceptionData::Impl();	

	this->m_pImpl->Line = other.m_pImpl->Line;
	try
	{
		strcpy_s(this->m_pImpl->Message, other.m_pImpl->Message);
		strcpy_s(this->m_pImpl->File, other.m_pImpl->File);
		strcpy_s(this->m_pImpl->FunctionName, other.m_pImpl->FunctionName);
	}
	catch (...)
	{
		L_LOG(Error, L"%s throws on copy constructor.", __FUNCTION__);
	}
}

_LUL_::LExceptionData::~LExceptionData() noexcept
{
	if (m_pImpl == nullptr)
		L_LOG(Warning, L"%s had nullptr?", __FUNCTION__);
	else
		delete m_pImpl;
}

const LCHAR* _LUL_::LExceptionData::GetMsg() noexcept
{
	return m_pImpl ? 
		m_pImpl->Message : 
		DataCorruptionMessage;
}

const LCHAR* _LUL_::LExceptionData::GetFile() noexcept
{
	return m_pImpl ?
		m_pImpl->File :
		DataCorruptionMessage;
}

const LCHAR* _LUL_::LExceptionData::GetFunctionName() noexcept
{
	return m_pImpl ?
		m_pImpl->FunctionName:
		DataCorruptionMessage;
}

const LINT32& _LUL_::LExceptionData::GetLine() noexcept
{
	return m_pImpl ?
		m_pImpl->Line :
		DataCorruptionValue;
}

_LUL_::LException::LException(
	LCHAR const* const sMsg, 
	LCHAR const* const sFunction,
	LCHAR const* const sFile, 
	const LINT32 line) noexcept
	: m_Data(sMsg, sFunction, sFile, line)
{
	// Automatic logging of an exception.
	L_LOG(
		Exception,
		L"\
An exception occured in: %s.\n\
Message: %s\n\
In file: %s\n\
At line: %d\n\
Stack trace: NOT IMPLEMETED", 
		sFunction,
		sMsg, 
		sFile, 
		line);
}
