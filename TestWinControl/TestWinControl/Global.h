#ifndef GLOBAL_H
#define GLOBAL_H

#pragma once

#include <atlstr.h>	// for LPCTSTR
#include <atltime.h>// for CTime
//#include <afx.h>	// for CFile

#define max4(a, b, c, d)    max(max((a),(b)), max((c),(d)))
#define min4(a, b, c, d)    min(min((a),(b)), min((c),(d)))

#define SWAP_SHORT(n)    (USHORT((n)>>8 | (n)<<8))


LPCTSTR GetExePath(void);
LPCTSTR GetExeName(void);

LPCTSTR GetCurrentTimeString(void);
UINT	GetWeek(const UINT year, const UINT month, const UINT date);

LPCTSTR GetMAC(int lana_num);
LPCTSTR GetFirstMAC(void);
void	GetAllMAC(void);

void	ExecuteOpenFile(LPCTSTR filePath);
BYTE*	ReadBinaryFile(LPCTSTR filePath);
BOOL	SaveBinaryFile(LPCTSTR filePath, BYTE *data, const LONG length);
BOOL	SaveFileCSV(LPCTSTR filePath, LPCTSTR headers, LPCTSTR contents);
BOOL	WriteFileString(LPCTSTR filePath, LPCTSTR contents);

LPSTR	LPWSTRtoLPSTR(LPWSTR strIn);

BOOL	CheckEqual(BYTE *data1, BYTE *data2, int length);
int		CalcCheckSum(BYTE *data, int length, int mod);



#endif  /* GLOBAL_H */


