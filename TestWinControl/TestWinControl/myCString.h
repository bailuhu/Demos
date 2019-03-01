
#include <windows.h>
#include <assert.h>

struct CStringData
{
	long nRefs;			// 引用计数:reference counting
	int nDataLength;	// 字符使用长度:character uses length
	int nAllocLength;	// 分配长度:allocated length
	TCHAR* data() { return (TCHAR*)(this+1); }	// 存放字符串的地方 
	// this+1 相当与是CStringData[1]; 所以TCHAR* data()指的是CStringData[1]的地址
};

class myCString
{
public:
	// constructor
	myCString();
	myCString(const myCString& stringSrc);
	myCString(TCHAR ch, int nLength = 1);
	myCString(LPCTSTR lpsz);	// myCString(LPCSTR lpsz); ANSI版本
								// myCString(LPCWSTR lpsz); UNICODE版本
	myCString(LPCTSTR lpsz, int nLength);	// myCString(LPCSTR lpch, int nLenght); ANSI版本
											// myCString(LPCWSTR lpch, int nLenght); UNICODE版本
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

	// 操作,脱离共享数据块
	int Delete(int nIndex, int nCount = 1);// 删除从nIndex开始长度为nCount的数据
	int Insert(int nIndex, TCHAR ch);	// 插入一个字符
	int Insert(int nIndex, LPCTSTR lpsz);//插入一个字符串
	int Replace(LPCTSTR lpszOld, TCHAR chNew);//替换数据
	int Replace(TCHAR chOld, TCHAR chNew);//替换数据
	int Remove(TCHAR chRemove);//移除一个字符

private:
	LPTSTR m_pchData;	// 指向CStringData的数据区
	void Init();
	CStringData* GetData() const;	// 通过m_pchData-1 得到CStringData
	void AllocBuffer(int nLen);		// 给CStringData分配内存,不带记数器
	void AllocBeforeWrite(int nLen);// 给CStringData分配内存,带记数器
	void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData);	// 分配内存,并拷贝lpszSrcData内容
	
	void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, 
					int nSrc2Len, LPCTSTR lpszSrc2Data);// 连接数据lpszSrc1Data+lpszSrc2Data
	void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData);// 连接字符串
	
	static void FreeData(CStringData* pData);
	void Release();	// 引用记数器并判断是否删除内存,如删除并初始化
	static void Release(CStringData* pData);// 引用记数器并判断是否删除内存
	static int SafeStrLen(LPCTSTR lpsz);	// 得到长度
};


//////////////////////////////////////////////////////////////////////
// 检测lpsz是否有效,调用了IsBadStringPtr
BOOL AfxIsValidString(LPCTSTR lpsz, int nLength = -1);
// 检测lp是否能读写权限,调用了IsBadReadPtr,IsBadStringPtr
BOOL AfxIsValidAddress(const void* lp, UINT nBytes, BOOL bReadWrite = TRUE);