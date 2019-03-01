#pragma once

// write by: Devil Lee
// date: 2014.11.19
// version: 3

#include "CApplication.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CRange.h"
#include "comdef.h"
#include <vector>
using namespace std;

enum Return_Code
{
	RET_no_excel = -2,
	RET_app_is_not_open = -3,
	RET_excel_is_not_open = -4,
	RET_sheet_is_not_open = -5,
	RET_sheetname_exists = -6,
	RET_outof_sheet_count = -7,
	RET_outof_row_count = -8,
	RET_outof_col_count = -8,

	RET_fail_open = -1,
	RET_fail_delete = -1,
	RET_fail_close = -1,
	RET_fail_set = -1,
	RET_fail_add = -1,
	RET_is_not_string = -1,
	RET_is_not_int = -1,
	RET_fail_create = -1,

	RET_success_create = 0,
	RET_success_open = 0,
	RET_success_delete = 0,
	RET_success_close = 0,
	RET_success_set = 0,
	RET_success_add = 0,
	RET_is_string = 0,
	RET_is_int = 0,
};

class CExcelManager
{
public:
	CExcelManager();			// call InitApp()
	~CExcelManager();

	void	InitApp();		// initialize com, and start excel
	void	ReleaseApp();	// excel must be released after use. This function will call CloseSheet, CloseExcel

	int 	OpenExcel(CString pathName);// open excel which name is ...
	int     CloseExcel();				// close opened excel
	int		CreateExcel(CString pathName);	// create .xlsx 

	CString GetExcelName();
	void	ShowExcel(BOOL bShow);		// show data in excel

	int 	OpenSheet(int index);
	int 	OpenSheet(CString name);
	void	CloseSheet();
	int 	AddSheet(CString name);		// create and load new sheet which will be added to the left
	int 	DeleteSheet(int index);		// delete sheet which index is ...
	int 	DeleteSheet(CString name);	// delete sheet which name is ...
	CString	GetSheetName(int index);	
	int 	SetSheetName(int index, CString name);// set sheet name which index is ...
	int		GetSheetCount();				// get all sheet number count
	BOOL	SheetNameExists(CString name);	// return TRUE: if sheet name aready exist, otherwise: return FALSE

	int 	DeleteCurSheet();				// delete the open sheet
	int     SetCurSheetName(CString name);  // set cur sheet name to ...
	CString GetCurSheetName();				// get cur sheet name

	CString GetCellName(int row, int col);	// get cell name such as A8
	CString	GetCellString(int row, int col);// get cell data in string
	int		GetCellInt(int row, int col);	// get cell data in int
	double	GetCellDouble(int row, int col);// unnecessary

	int		GetRowIndex(int col, CString val);
	int		GetColIndex(int row, CString val);
	void	GetCellIndex(CString val, int& row, int& col);
	int 	GetNextRowIndex(int startRow, int col, BOOL bValid);
	int 	GetNextColIndex(int row, int col, BOOL bValid);

	int 	SetCellString(int row, int col, CString val);	// write string to cell
	int 	SetCellInt(int row, int col, int val);			// write int to cell
	void	SetColumnWidth(int col, int width);	// imperfect, it will change all the column width

	int		IsCellString(long row, long col);	// tell if it is string in cell
	int		IsCellInt(long row, long col);		// tell if it is int int cell

	int		MergeRange(int startRow, int startCol, int endRow, int endCol); // added by Chen'gang
	int		GetRowCount();		// get all column number count
	int		GetColumnCount();	// get all row number count
	void	GetRowData(int row, vector<CString>& data, BOOL bValid = TRUE, int startCol = 1);
	void	GetColumnData(int col, vector<CString>& data, BOOL bValid = TRUE, int startRow = 1);
	void    SaveAs(CString pathName);

	CString GetCurVersion();	// return your excel version, such as: 2007,2003,etc


private:
	COleVariant GetCell(int row, int col);
	int			OpenSheet(char* name);
	char*		GetColumnName(int col);

	BOOL		 m_bInit;
	BOOL		 m_bStart;
	BOOL		 m_bOpenExcel;
	BOOL		 m_bOpenSheet;
	BOOL		 m_bDeleteSheet;
	CString		 m_pathName;

	CWorkbooks   m_workbooks;	
	CWorkbook    m_workbook;		
	CWorksheets  m_worksheets;	
	CWorksheet   m_worksheet;	
	CRange       m_range;		
	CApplication m_app;

	// AnsiתUnicode
	int AnsiToUnicode(char *szTemp, wchar_t* wStr, int size);
	wchar_t* AnsiToUnicode(char *szTemp);

	// UnicodeתAnsi
	int UnicodeToAnsi(wchar_t *wStr, char* szTemp, int size);
	char* UnicodeToAnsi(wchar_t *wStr);
 
};

