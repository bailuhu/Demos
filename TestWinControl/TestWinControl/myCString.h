
#include <windows.h>
#include <assert.h>

struct CStringData
{
	long nRefs;			// ���ü���:reference counting
	int nDataLength;	// �ַ�ʹ�ó���:character uses length
	int nAllocLength;	// ���䳤��:allocated length
	TCHAR* data() { return (TCHAR*)(this+1); }	// ����ַ����ĵط� 
	// this+1 �൱����CStringData[1]; ����TCHAR* data()ָ����CStringData[1]�ĵ�ַ
};

class myCString
{
public:
	// constructor
	myCString();
	myCString(const myCString& stringSrc);
	myCString(TCHAR ch, int nLength = 1);
	myCString(LPCTSTR lpsz);	// myCString(LPCSTR lpsz); ANSI�汾
								// myCString(LPCWSTR lpsz); UNICODE�汾
	myCString(LPCTSTR lpsz, int nLength);	// myCString(LPCSTR lpch, int nLenght); ANSI�汾
											// myCString(LPCWSTR lpch, int nLenght); UNICODE�汾
	myCString(const unsigned char* psz);
	~myCString();

	// CStringData attribute
	int GetLength() const;		// get the character length
	int GetAllocLength() const; // get the memory allocated length
	BOOL IsEmpty() const;		// to determine whether the character length is 0
	operator LPCTSTR() const;	// type conversion
	void Empty();				// clear CStringData

	// operator overloading
	const myCString& operator=(const myCString& stringSrc);
	const myCString& operator=(LPCTSTR lpsz);
	const myCString& operator=(TCHAR ch);
	const myCString& operator+=(const myCString& string);
	const myCString& operator+=(LPCSTR lpsz);
	const myCString& operator+=(TCHAR ch);
	TCHAR operator[](int nIndex) const;

	friend myCString operator+(const myCString& cstring1, const myCString& string2);
	friend myCString operator+(const myCString& string, TCHAR ch);
	friend myCString operator+(TCHAR ch, const myCString& cstring);
	friend myCString operator+(const myCString& string, LPCTSTR lpsz);
	friend myCString operator+(LPCTSTR lpsz, const myCString& string);

	// ����,���빲�����ݿ�
	int Delete(int nIndex, int nCount = 1);// ɾ����nIndex��ʼ����ΪnCount������
	int Insert(int nIndex, TCHAR ch);	// ����һ���ַ�
	int Insert(int nIndex, LPCTSTR lpsz);//����һ���ַ���
	int Replace(LPCTSTR lpszOld, TCHAR chNew);//�滻����
	int Replace(TCHAR chOld, TCHAR chNew);//�滻����
	int Remove(TCHAR chRemove);//�Ƴ�һ���ַ�

private:
	LPTSTR m_pchData;	// ָ��CStringData��������
	void Init();
	CStringData* GetData() const;	// ͨ��m_pchData-1 �õ�CStringData
	void AllocBuffer(int nLen);		// ��CStringData�����ڴ�,����������
	void AllocBeforeWrite(int nLen);// ��CStringData�����ڴ�,��������
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);	// �����ڴ�,������lpszSrcData����
	
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, 
					int nSrc2Len, LPCTSTR lpszSrc2Data);// ��������lpszSrc1Data+lpszSrc2Data
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);// �����ַ���
	
	static void FreeData(CStringData* pData);
	void Release();	// ���ü��������ж��Ƿ�ɾ���ڴ�,��ɾ������ʼ��
	static void Release(CStringData* pData);// ���ü��������ж��Ƿ�ɾ���ڴ�
	static int SafeStrLen(LPCTSTR lpsz);	// �õ�����
};


//////////////////////////////////////////////////////////////////////
// ���lpsz�Ƿ���Ч,������IsBadStringPtr
BOOL AfxIsValidString(LPCTSTR lpsz, int nLength = -1);
// ���lp�Ƿ��ܶ�дȨ��,������IsBadReadPtr,IsBadStringPtr
BOOL AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite = TRUE);