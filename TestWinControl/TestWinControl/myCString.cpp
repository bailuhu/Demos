#include "stdafx.h"
#include "myCString.h"

/*
��C++���NULL��ͬ��NULL��һ���궨�壬ֵΪ0��nil��ʾ��ֵ
�κα�����û�б���ֵ֮ǰ��ֵ��Ϊnil����������жϣ�ֻ��nil��false��ʾ�٣������Ϊ��
*/
TCHAR afxChNil = (TCHAR)'/0';
int _afxInitData[] = {-1, 0, 0, 0};		// ��ʼ��CStringData�ĵ�ַ
CStringData* _afxDataNil = (CStringData*)&_afxInitData;	// ��ַת��ΪCStringData*
LPCTSTR _afxPchNil = (LPCTSTR)(((BYTE*)&_afxInitData)+sizeof(CStringData));

const myCString& AfxGetEmptyString()	// ����һ���յ�myCString
{ return *(myCString*)&_afxPchNil; }

BOOL AfxIsValidString(LPCTSTR lpsz, int nLength /* = -1 */)
{
	if (lpsz == NULL)
		return FALSE;
	return ::IsBadStringPtr(lpsz, nLength) == 0;
	/*
	��ָ֤���Ƿ�ɶ�/д��
	�ں�������ڴ���������Ҫ��ָ֤����ָ������������Ƿ�ɶ�/д�� 
	ͨ������assert��p!= NULL)�ļ����ʽ�� ���ǣ�ָ���ֵ��Ϊ�ղ�������ָ��ָ���˺Ϸ��ɶ�/д�ڴ档
	Win32 API�ṩ�˺���IsBadReadPtr��IsBadWritePtr��IsBadStringPtr��IsBadCodePtr�������ָ��ָ����ڴ������Ƿ�ɶ�/д��
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
	assert(nLen <= 2147483647-1);    // (signed) int �����ֵ

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
{ return (lpsz == NULL) ? 0 : lstrlen(lpsz); }	// int lstrlen(LPCTSTR lpString);�ú�������ָ���ַ������ֽڳ��ȣ�ANSI�棩���ַ����ȣ�˫�ֽڱ�׼�棩�����в�������ֹNULL�ַ���

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
		memcpy(m_pchData, lpsz, nLen*sizeof(TCHAR));	// c��c++ʹ�õ��ڴ濽������
	}
}

myCString::myCString(LPCTSTR lpsz, int nLength)
{
	Init();
	if (nLength != 0)
	{
		assert(AfxIsValidAddress(lpsz, nLength, FALSE));	// false:ֻ���IsBadReadPtr
		AllocBuffer(nLength);
		memcpy(m_pchData, lpsz, nLength*sizeof(TCHAR));
	}
}

CStringData* myCString::GetData() const
{
	// assert���������ּ�����ʽ expression �������ֵΪ�٣���Ϊ0������ô������stderr��ӡһ��������Ϣ��Ȼ��ͨ������ abort ����ֹ�������С�
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
			// �½�һ������
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		}
		else
		{
			// ֻ����ָ��
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