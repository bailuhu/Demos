#include "StdAfx.h"
#include "ExcelManager.h"

COleVariant	covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);   


CExcelManager::CExcelManager()
{
	m_bInit = FALSE;
	m_bStart = FALSE;
	m_pathName = _T("");

	m_bOpenExcel = FALSE;
	m_bOpenSheet = FALSE;
	m_bDeleteSheet = FALSE;
}

CExcelManager::~CExcelManager()
{
	m_range.ReleaseDispatch(); 
	m_worksheet.ReleaseDispatch(); 
	m_worksheets.ReleaseDispatch(); 
	m_workbook.ReleaseDispatch(); 
	m_workbooks.ReleaseDispatch(); 
}

void CExcelManager::InitApp()
{
	// 初始化COM
	if (!m_bInit)
	{
		if (::CoInitialize( NULL ) == E_INVALIDARG)	
			AfxMessageBox(_T("初始化Com失败!"));
		else										
			m_bInit = TRUE;
	}

	// 创建Excel 2000服务器(启动Excel) 
	if (!m_bStart) 
	{
		if (!m_app.CreateDispatch(_T("Excel.Application"), NULL)) 
		{ 
			AfxMessageBox(_T("创建Excel服务失败!")); 
			exit(1); 
		}
		else
			m_bStart = TRUE;
	}

}

void CExcelManager::CloseSheet()
{
	m_range.ReleaseDispatch(); 
	m_worksheet.ReleaseDispatch(); 
	m_worksheets.ReleaseDispatch(); 

	m_range		 = NULL;
	m_worksheet  = NULL;
	m_worksheets = NULL;
	m_bOpenSheet = FALSE;
}

int CExcelManager::CloseExcel()
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (m_bOpenSheet) CloseSheet();		// Avoid directly closed excel before closed sheet
	m_workbook.SaveCopyAs(COleVariant(m_pathName));
	if (m_bDeleteSheet)	// after delete sheet function, 2003 format .xls will be update to 2007 format .xlsx
	{
		if (m_pathName.Right(1) != _T("x")) // save 2003 format .xls to 2007 format .xlsx
		{
			CString oldName= m_pathName;
			m_pathName += _T("x");
			CFile::Rename(oldName, m_pathName);
		}
		m_bDeleteSheet = FALSE;
	}

	m_workbook.put_Saved(TRUE);			// save change

	m_workbook.Close(covOptional, COleVariant(m_pathName), covFalse);
	m_workbooks.Close();
	m_workbook.ReleaseDispatch(); 
	m_workbooks.ReleaseDispatch(); 

	m_workbook	 = NULL;
	m_workbooks  = NULL;
	m_bOpenExcel = FALSE;
	m_pathName = _T("");

	return RET_success_close;
}

void CExcelManager::ReleaseApp()
{
	if (m_bOpenSheet)
	{
		CloseSheet();
	}

	if (m_bOpenExcel)
	{
		CloseExcel();
	}

	m_app.Quit();
	m_app.ReleaseDispatch();
	m_app = NULL;

	m_bInit	 = FALSE;
	m_bStart = FALSE;


}

// get workbooks, workbook, worksheets in turn
int CExcelManager::OpenExcel(CString pathName)
{	
	if (!PathFileExists(pathName))	return RET_no_excel;
	else							m_pathName = pathName;

	// 打开excel文件,利用模板文件建立新文档 
	m_workbooks.AttachDispatch(m_app.get_Workbooks(), TRUE); 
 
	LPDISPATCH lpDis = NULL;
	lpDis = m_workbooks.Add(_variant_t(pathName)); 
	if (lpDis)
	{
		m_workbook.AttachDispatch(lpDis); 
		m_worksheets.AttachDispatch(m_workbook.get_Worksheets(), TRUE); // 得到Worksheets 
		m_bOpenExcel = TRUE;
		return RET_success_open;
	}
	else
	{
		m_bOpenExcel = FALSE;
		return RET_fail_open;
	}
}

int CExcelManager::CreateExcel(CString pathName)
{
	CloseExcel();

	m_workbooks.AttachDispatch(m_app.get_Workbooks(), TRUE); 
	m_workbook.AttachDispatch(m_workbooks.Add(_variant_t(covOptional))); 
	m_worksheets = m_workbook.get_Worksheets();
	while (m_worksheets.get_Count() != 3 )	// create three sheets
	{
		m_worksheet = m_worksheets.get_Item(COleVariant((short)1));
		m_worksheet.Delete();// 删除worksheet
	}

	if (pathName.Right(4) != _T("xlsx"))
		pathName = pathName.Left(pathName.ReverseFind(_T('.'))) + _T(".xlsx");

	m_workbook.SaveAs((COleVariant)(pathName), covOptional, covOptional, covOptional, covOptional, 
		covOptional, GRANT_ACCESS, covOptional, covOptional, covOptional, covOptional, covOptional);

	OpenExcel(pathName);
	OpenSheet(1);
	return RET_success_create;
}

int CExcelManager::DeleteCurSheet()
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	if (m_pathName.Right(1) != _T("x")) // save 2003 format .xls to 2007 format .xlsx
 		m_workbook.Save();
	m_worksheet.Delete();
//	m_workbook.put_Saved(TRUE);
	m_bDeleteSheet = TRUE;
	m_bOpenSheet = FALSE;
	return RET_success_delete;
}

int CExcelManager::DeleteSheet(int index)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (index < 1 || index > GetSheetCount()) return RET_outof_sheet_count;

	CWorksheet worksheet;
	worksheet.AttachDispatch(m_worksheets.get_Item(_variant_t(long(index))), TRUE);
	if (m_pathName.Right(1) != _T("x")) // save 2003 format .xls to 2007 format .xlsx
		m_workbook.Save();

	worksheet.Delete();
	m_bDeleteSheet = TRUE;
	return RET_success_delete;

}

int CExcelManager::DeleteSheet(CString name)	
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!SheetNameExists(name)) return RET_sheetname_exists;

	CWorksheet worksheet;
	worksheet.AttachDispatch(m_worksheets.get_Item(_variant_t(name)), TRUE);
	if (m_pathName.Right(1) != _T("x")) // save 2003 format .xls to 2007 format .xlsx
		m_workbook.Save();
	worksheet.Delete();

	m_bDeleteSheet = TRUE;
	return RET_success_delete;
}

CString CExcelManager::GetCurVersion()
{
	/*判断当前Excel的版本*/
	CString version = m_app.get_Version();
	int iStart = 0;
	version = version.Tokenize(_T("."), iStart);
	if (_T("11") == version)
	{
		return _T("2003");
	}
	else if (_T("12") == version)
	{
		 return _T("2007");
	}
	else
	{
		return _T("当前Excel的版本是其他版本");
	}
}

CString CExcelManager::GetExcelName()
{
	return m_pathName;
}

int CExcelManager::GetSheetCount()
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	return m_worksheets.get_Count();
}

CString CExcelManager::GetSheetName(int index)
{
	if (!m_bOpenExcel) return _T(""); //_T("warning: haven't opened excel!");


	CWorksheet worksheet;
	worksheet.AttachDispatch(m_worksheets.get_Item(_variant_t((long)index)), TRUE);
	CString sheetName = worksheet.get_Name();
	worksheet.ReleaseDispatch();
	return sheetName;
}

CString CExcelManager::GetCurSheetName()
{
	if (!m_bOpenExcel) return _T(""); // _T("warning: haven't loaded sheet!");
	if (!m_bOpenSheet) return _T(""); // _T("warning: haven't opened sheet!");

	CString sheetName = m_worksheet.get_Name();
	m_worksheet.ReleaseDispatch();
	return sheetName;
}

int CExcelManager::SetCurSheetName(CString name)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open; 
	if (!m_bOpenSheet) return  RET_sheet_is_not_open; 
	if (SheetNameExists(name)) return RET_sheetname_exists;

	m_worksheet.put_Name(name);  // set sheet name.
	return RET_success_set;
}

int CExcelManager::SetSheetName(int index, CString name)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (SheetNameExists(name)) return RET_sheetname_exists;
	if (index < 1 || index > GetSheetCount()) return RET_outof_sheet_count;

	CWorksheet worksheet;
	worksheet.AttachDispatch(m_worksheets.get_Item(_variant_t(index)), TRUE);
	if (SheetNameExists(name)) return FALSE;

	worksheet.put_Name(name);
	worksheet.DetachDispatch();
	return RET_success_set;
}

BOOL CExcelManager::SheetNameExists(CString name)
{
	int sheetCnt = GetSheetCount();
	for (int i=1; i <= sheetCnt; i++)
	{
		if (name == GetSheetName(i))	return RET_sheetname_exists;
	}
	return FALSE;
}

int CExcelManager::AddSheet(CString name)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;

	LPDISPATCH lpDis = m_worksheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
	if (lpDis)
	{
		OpenSheet(1);	// load the new sheet.
		SetCurSheetName(name);	// set the new sheet name.

		return RET_success_add;
	}
	return RET_fail_add;
}

int CExcelManager::OpenSheet(CString name)
{
	if (!SheetNameExists(name)) return RET_sheetname_exists;

	char* temp = UnicodeToAnsi((wchar_t*)name.GetString());
	BOOL ret = OpenSheet(temp);
	delete[] temp;
	return ret;
}

int CExcelManager::OpenSheet(int index)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (index < 1 || index > GetSheetCount()) return RET_outof_sheet_count;

	LPDISPATCH lpDis = NULL;
	m_range.ReleaseDispatch();
	m_worksheet.ReleaseDispatch();
	lpDis = m_worksheets.get_Item(_variant_t((long)index));
	if (lpDis)
	{
		m_worksheet.AttachDispatch(lpDis, TRUE);
		m_range.AttachDispatch(m_worksheet.get_Cells(), TRUE);
		m_bOpenSheet = TRUE;
		return RET_success_open;
	}
	else
	{
		m_bOpenSheet = FALSE;
		return RET_fail_open;
	}
	
}

int CExcelManager::OpenSheet(char* name)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;

	LPDISPATCH lpDis = NULL;
	m_range.ReleaseDispatch();
	m_worksheet.ReleaseDispatch();
	lpDis = m_worksheets.get_Item(_variant_t(name));
	if (lpDis)
	{
		m_worksheet.AttachDispatch(lpDis, TRUE);
		m_range.AttachDispatch(m_worksheet.get_Cells(), TRUE);
		m_bOpenSheet = TRUE;
		return RET_success_open;
	}	
	else
	{
		m_bOpenSheet = FALSE;
		return RET_fail_open;
	}
}

int CExcelManager::GetColumnCount()
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;
	
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(m_worksheet.get_UsedRange(), TRUE);
	range.AttachDispatch(usedRange.get_Columns(), TRUE);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

void CExcelManager::GetRowData(int row, vector<CString>& data, BOOL bValid /* = TRUE */, int startCol /* = 1 */)
{
	if (!m_bOpenExcel) return ;
	if (!m_bOpenSheet) return ;

	data.clear();
	int colCnt = GetColumnCount();
	for (int col = startCol; col < colCnt; col++)
	{
		CString temp = GetCellString(row, col);
		if (temp.IsEmpty())	
		{
			if (!bValid)	data.push_back(temp);
		}
		else
		{
			data.push_back(temp);
		}
	}
}

void CExcelManager::GetColumnData(int col, vector<CString>& data, BOOL bValid /* = TRUE */, int startRow /* = 1 */)
{
	if (!m_bOpenExcel) return ;
	if (!m_bOpenSheet) return ;

	data.clear();
	int rowCnt = GetRowCount();
	for (int row = startRow; row < rowCnt; row++)
	{
		CString temp = GetCellString(row, col);
		if (temp.IsEmpty())	
		{
			if (!bValid)	data.push_back(temp);
		}
		else
		{
			data.push_back(temp);
		}
	}
}

int CExcelManager::GetRowCount()
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(m_worksheet.get_UsedRange(), TRUE);
	range.AttachDispatch(usedRange.get_Rows(), TRUE);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

CString CExcelManager::GetCellString(int row, int col)
{
	if (!m_bOpenSheet) return _T(""); // _T("warning: haven't loaded sheet!");
	if (row <= 0 || col <= 0)	return _T(""); // _T("warning: index out of row or column!");

	COleVariant oleVar = GetCell(row, col);
	CString info;
	if(oleVar.vt == VT_BSTR)       // 字符串
	{
		info = oleVar.bstrVal;
	}
	else if (oleVar.vt == VT_INT)
	{
		info.Format(_T("%d"), oleVar.pintVal);
	}
	else if (oleVar.vt == VT_R8)     // 8字节的数字 
	{
		info.Format(_T("%0.0f"), oleVar.dblVal);
	}
	else if(oleVar.vt == VT_DATE)    // 时间格式
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(oleVar.date, &st);
		CTime tm(st); 
		info = tm.Format(_T("%Y-%m-%d"));
  
	}
	else if(oleVar.vt == VT_EMPTY)   // 单元格空的
	{
		info = _T("");
	} 

	return info;
}

int CExcelManager::GetColIndex(int row, CString val)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	int maxCol = GetColumnCount();
	for (int col = 1; col < maxCol; col++)
	{
		if (GetCellString(row, col) == val)
			return col;
	}
	return 0;	// can find col with val
}

int CExcelManager::GetRowIndex(int col, CString val)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	int maxRow = GetRowCount();
	for (int row = 1; row < maxRow; row++)
	{
		if (GetCellString(row, col) == val)
			return row;
	}
	return 0;	// can find row with val
}

void CExcelManager::GetCellIndex(CString val, int& row, int& col)
{
	if (!m_bOpenExcel) return ;
	if (!m_bOpenSheet) return ;

	int maxRow = GetRowCount();
	int maxCol = GetColumnCount();
	int iRow = 1;
	int iCol = 1;
	for (; iRow < maxRow; iRow++)
	{
		for (; iCol < maxCol; iCol++)
		{
			if (GetRowIndex(iCol, val) > 0)
				break;		// find row
		}
		if (GetColIndex(iRow, val) > 0)
			break;	// find column
	}
	
	row = iRow;
	col = iCol;
}

int CExcelManager::GetNextRowIndex(int startRow, int col, BOOL bValid)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	int row = startRow + 1;
	int maxRow = GetRowCount();

	for (; row < maxRow; row++)
	{
		if (GetCellString(row, col).IsEmpty())
		{
			if (bValid) continue;	// bValid == TRUE, find next valid value
			else		break;		// bValid == FALSE, find this invalid value
		}
		else
			break;
	}
	return row;

}

int CExcelManager::GetNextColIndex(int row, int startCol, BOOL bValid)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	int col = startCol + 1;
	int maxCol = GetColumnCount();
	for (; col < maxCol; col++)
	{
		if (GetCellString(row, col).IsEmpty())
		{
			if (bValid) continue;	// bValid == TRUE, find next valid value
			else		break;		// bValid == FALSE, find this invalid value
		}
		else
			break;
	}
	return col;
}

int CExcelManager::GetCellInt(int row, int col)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	COleVariant oleVar = GetCell(row, col);
	return (int)oleVar.date;;
}

double CExcelManager::GetCellDouble(int row, int col)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	COleVariant oleVar = GetCell(row, col);
	if (oleVar.vt == VT_R8)
		return oleVar.dblVal;
	else
		return 0.0;
}

int CExcelManager::SetCellInt(int row, int col, int val)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	COleVariant oleVar((long)val);
	CRange startRange = m_worksheet.get_Range(COleVariant(_T("A1")), covOptional);
	CRange writeRange = startRange.get_Offset(COleVariant((long)row -1),COleVariant((long)col -1) );
	writeRange.put_Value2(oleVar);
	startRange.ReleaseDispatch();
	writeRange.ReleaseDispatch();

	return RET_success_set;
}

void CExcelManager::SetColumnWidth(int col, int width)
{	
	if (!m_bOpenSheet) return ;
	// 不完善，改变了所有列宽
	CString colName = GetCellName(1, col);
	CRange range = m_worksheet.get_Range(COleVariant(colName), covOptional);
	m_range.put_ColumnWidth(_variant_t((long)width));  // 设置列宽
}

int CExcelManager::SetCellString(int row, int col, CString val)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	COleVariant oleVar(val);
	CRange startRange = m_worksheet.get_Range(COleVariant(_T("A1")), covOptional);
	CRange writeRange = startRange.get_Offset(COleVariant((long)row-1),COleVariant((long)col-1) );
	writeRange.put_Value2(oleVar);
	startRange.ReleaseDispatch();
	writeRange.ReleaseDispatch();

	return RET_success_set;
}

// 检查一个CELL是否是字符串
int CExcelManager::IsCellString(long row, long col)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	COleVariant vResult = GetCell(row, col);
	//VT_BSTR标示字符串
	if(vResult.vt == VT_BSTR)       
	{
		return RET_is_string;
	}
	return RET_is_not_string;
}

//检查一个CELL是否是数值
int CExcelManager::IsCellInt(long row, long col)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;

	COleVariant vResult = GetCell(row, col);
	//好像一般都是VT_R8
	if(vResult.vt == VT_INT || vResult.vt == VT_R8)       
	{
		return RET_is_int;
	}
	return RET_is_not_int;
}

COleVariant CExcelManager::GetCell(int row, int col)
{
	CRange range;
	range.AttachDispatch(m_range.get_Item (COleVariant((long)row), COleVariant((long)col)).pdispVal, TRUE);
	COleVariant vRet = range.get_Value2();
	range.ReleaseDispatch();
	return vRet;
}

void CExcelManager::SaveAs(CString pathName)
{
	if (!m_bOpenExcel) return;

	m_workbook.SaveAs(COleVariant(pathName), covOptional, covOptional, covOptional, covOptional, covOptional,
					  NOT_USED_ACCESS, covOptional, covOptional, covOptional,	covOptional, covOptional);


}

CString CExcelManager::GetCellName(int row, int col)
{
	if (!m_bOpenExcel) return _T("");
	if (!m_bOpenSheet) return _T(""); //_T("warning: haven't loaded sheet!");
	
	CString temp;
	temp.Format(_T("%S%d"), GetColumnName(col), row);
	return temp;
}

// 取得列的名称，比如27->AA
char* CExcelManager::GetColumnName(int col)
{   
	static char colName[64];
	size_t len = 0;

	while(col > 0)
	{
		int alphabet = col % 26;
		col /= 26;
		if (alphabet == 0)
		{
			alphabet = 26;
			col--;
		}
		colName[len] = (char)((alphabet-1) + 'A' );
		len ++;
	}
	colName[len] = '\0';
	//反转
	_strrev(colName);

	return colName;
}

// 合并单元格-陈刚添加
int CExcelManager::MergeRange(int startRow, int startCol, int endRow, int endCol)
{
	if (!m_bOpenExcel) return RET_excel_is_not_open;
	if (!m_bOpenSheet) return RET_sheet_is_not_open;
	CString startCellName = GetCellName(startRow, startCol);
	CString endCellName = GetCellName(endRow, endCol);
	m_range.AttachDispatch(m_worksheet.get_Range(COleVariant(startCellName), COleVariant(endCellName)));
	m_range.Merge(COleVariant((long)0));

	return 0;
}

void CExcelManager::ShowExcel(BOOL bShow)
{
	m_app.put_Visible(bShow);
}

wchar_t* CExcelManager::AnsiToUnicode(char *szTemp)
{
	int cnt = MultiByteToWideChar(CP_ACP, 0, szTemp, -1, NULL, 0);
	wchar_t *wStr;
	wStr = new wchar_t[cnt];

	MultiByteToWideChar (CP_ACP, 0, szTemp, -1, wStr, cnt);
	return wStr;
}

int CExcelManager::AnsiToUnicode(char *szTemp, wchar_t* wStr, int size)
{
	int cnt = MultiByteToWideChar (CP_ACP, 0, szTemp, -1, NULL, 0);

	if (cnt > size) return -1;

	MultiByteToWideChar (CP_ACP, 0, szTemp, -1, wStr, size);

	return 0;
}

// Unicode转ansi
int CExcelManager::UnicodeToAnsi(wchar_t *wStr, char* szTemp, int size)
{
	int cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);

	if (cnt > size) return -1;

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, szTemp, size, NULL, FALSE);
	return 0;
}

// Unicode转ansi
char* CExcelManager::UnicodeToAnsi(wchar_t *wStr)
{
	int Cnt = WideCharToMultiByte(CP_OEMCP, NULL, wStr, -1, NULL, 0, NULL, FALSE);
	char *szTemp;
	szTemp = new char[Cnt];

	WideCharToMultiByte (CP_OEMCP, NULL, wStr, -1, szTemp, Cnt, NULL, FALSE);
	return szTemp;
}