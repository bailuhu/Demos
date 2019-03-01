#include "stdafx.h"
#include "myCString.h"

/*
与C++里的NULL不同，NULL是一个宏定义，值为0，nil表示无值
任何变量在没有被赋值之前的值都为nil，对于真假判断，只有nil与false表示假，其余均为真
*/
TCHAR afxChNil = (TCHAR)'/0';
int _afxInitData[] = {-1, 0, 0, 0};		// 初始化CStringData的地址
CStringData* _afxDataNil = (CStringData*)&_afxInitData;	// 地址转化为CStringData*
LPCTSTR _afxPchNil = (LPCTSTR)(((BYTE*)&_afxInitData)+sizeof(CStringData));

const myCString& AfxGetEmptyString()	// 建立一个空的myCString
{ return *(myCString*)&_afxPchNil; }

BOOL AfxIsValidString(LPCTSTR lpsz, int nLength /* = -1 */)
{
	if (lpsz == NULL)
		return FALSE;
	return ::IsBadStringPtr(lpsz, nLength) == 0;
	/*
	验证指针是否可读/写。
	在函数的入口处，经常需要验证指针所指向的内容区域是否可读/写。 
	通常采用assert（p!= NULL)的检测形式。 但是，指针的值不为空并不代表指针指向了合法可读/写内存。
	Win32 API提供了函数IsBadReadPtr、IsBadWritePtr、IsBadStringPtr、IsBadCodePtr用来检测指针指向的内存区域是否可读/写。
	*/
}

BOOL AfxIsValidAddress(const void* lp,UINT nBytes, BOOL bReadWrite /* = TRUE */)
{
	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
			(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));
}

void myCString::Init()
{ m_pchData = AfxGetEmptyString().m_pchData;}

void myCString::AllocBuffer(int nLen)
{
	assert(nLen >= 0);
	assert(nLen <= 2147483647-1);    // (signed) int 的最大值

	if (nLen == 0)
		Init();
	else
	{
		CStringData* pData;
		{
			pData = (CStringData*) new BYTE[sizeof(CStringData) + (nLen+1)*sizeof(TCHAR)];
			pData->nAllocLength = nLen;
		}
		pData->nRefs = 1;
		pData->data()[nLen] = (TCHAR)'/0';
		pData->nDataLength = nLen;
		m_pchData = pData->data();
	}
}

myCString::~myCString()
{
	if (GetData() != _afxDataNil)
	{
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			FreeData(GetData());
	}
}

myCString::myCString()
{ Init(); }

int myCString::SafeStrLen(LPCTSTR lpsz)
{ return (lpsz == NULL) ? 0 : lstrlen(lpsz); }	// int lstrlen(LPCTSTR lpString);该函数返回指定字符串的字节长度（ANSI版）或字符长度（双字节标准版）；其中不包括终止NULL字符。

myCString::myCString(const myCString& stringSrc)
{
	assert(stringSrc.GetData()->nRefs != 0);
	if(stringSrc.GetData()->nRefs >= 0)
	{
		assert(stringSrc.GetData() != _afxDataNil);
		m_pchData = stringSrc.m_pchData;
		InterlockedIncrement(&GetData()->nRefs);
	}
	else
	{
		Init();
		*this = stringSrc.m_pchData;
	}
}

myCString::myCString(TCHAR ch, int nLength/* = 1*/)
{
	//...
}

myCString::myCString(LPCTSTR lpsz)
{
	Init();
	int nLen = SafeStrLen(lpsz);
	if (nLen != 0)
	{
		AllocBuffer(nLen);
		memcpy(m_pchData, lpsz, nLen*sizeof(TCHAR));	// c和c++使用的内存拷贝函数
	}
}

myCString::myCString(LPCTSTR lpsz, int nLength)
{
	Init();
	if (nLength != 0)
	{
		assert(AfxIsValidAddress(lpsz, nLength, FALSE));	// false:只检查IsBadReadPtr
		AllocBuffer(nLength);
		memcpy(m_pchData, lpsz, nLength*sizeof(TCHAR));
	}
}

CStringData* myCString::GetData() const
{
	// assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。
	assert(m_pchData != NULL);
	return ((CStringData*)m_pchData)-1;
}

void myCString::FreeData(CStringData* pData)
{
	delete[] (BYTE*)pData;
}

void myCString::Release()
{
	if (GetData() != _afxDataNil)
	{
		assert(GetData()->nRefs != 0);
		if (InterlockedDecrement(&GetData()->nRefs) <= 0)
			FreeData(GetData());
		Init();
	}
}

void myCString::Release(CStringData* pData)
{
	if (pData != _afxDataNil)
	{
		assert(pData->nRefs != 0);
		if (InterlockedDecrement(&pData->nRefs) <= 0)
			FreeData(pData);
	}
}

void myCString::AllocBeforeWrite(int nLen)
{
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength)
	{
		Release();
		AllocBuffer(nLen);
	}
	assert(GetData()->nRefs <= 1);
}

void myCString::AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
{
	AllocBeforeWrite(nSrcLen);
	memcpy(m_pchData, lpszSrcData, nSrcLen*sizeof(TCHAR));
	GetData()->nDataLength = nSrcLen;
	m_pchData[nSrcLen] = (TCHAR)'/0';
}

int myCString::GetLength() const
{ return GetData()->nDataLength; }

int myCString::GetAllocLength() const
{ return GetData()->nAllocLength; }

BOOL myCString::IsEmpty() const
{ return GetData()->nDataLength == 0; }

myCString::operator LPCTSTR() const
{ return m_pchData; }

void myCString::Empty()
{
	if (GetData()->nDataLength == 0)
		return;
	if (GetData()->nRefs >= 0)
		;
	else
		*this = &afxChNil;
	assert(GetData()->nDataLength == 0);
	assert(GetData()->nRefs < 0 || GetData()->nAllocLength == 0);
}

const myCString& myCString::operator=(const myCString& stringSrc)
{
	if (m_pchData != stringSrc.m_pchData)
	{
		if ((GetData()->nRefs < 0 && GetData() != _afxDataNil) ||
			stringSrc.GetData()->nRefs < 0)
		{
			// 新建一块数据
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// 只拷贝指针
			Release();
			assert(stringSrc.GetData() != _afxDataNil);
			m_pchData = stringSrc.m_pchData;
			InterlockedIncrement(&GetData()->nRefs);
		}
	}
	return *this;
}

const myCString& myCString::operator=(LPCTSTR lpsz)
{
	assert(lpsz == NULL || AfxIsValidString(lpsz));
	AssignCopy(SafeStrLen(lpsz), lpsz);
	return *this;
}

const myCString& myCString::operator=(TCHAR ch)
{
	AssignCopy(1, &ch);
	return *this;
}




// const myCString& operator+=(const myCString& string)
//{
//	
//}