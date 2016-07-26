
// RS232Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRS232Dlg dialog
class CRS232Dlg : public CDialogEx
{
// Construction
public:
	CRS232Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RS232_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonCloseport();
	afx_msg void OnBnClickedButtonSend();
	CString m_TextboxData;
	afx_msg void OnBnClickedButtonOpenport();
	CString m_StatictxtNotif;
	CString m_temp;
	CStatic m_temp1;
	CStatic m_TxtNotif;
	CStatic m_TxtStatus;
	afx_msg void OnBnClickedButtonRand();
	CString m_TextboxRandData;
	CStatic m_TxtRandStatus;
	CStatic m_TxtSuccess;
	CSpinButtonCtrl m_Spin_Baudrate;
	CString m_TxtboxBaudrate;
	CString m_TextboxBaudrate;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CStatic m_FileStatus;
	CStatic m_Title;
	afx_msg void OnStnClickedTitle();
};
