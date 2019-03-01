#pragma once


// ExListBox



class ExListBox : public CListBox
{
	DECLARE_DYNAMIC(ExListBox)

public:
	ExListBox();
	virtual ~ExListBox();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

protected:

	DECLARE_MESSAGE_MAP()


public:
	int AddString(LPCTSTR lpszItem);
	void ResetContent();
	int DeleteString(UINT nIndex);
private:
	int m_nMaxWidth;

};


