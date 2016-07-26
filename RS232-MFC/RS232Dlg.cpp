
// RS232Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "RS232.h"
#include "RS232Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//My ####
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "afxwin.h"
#include <windows.h>
#include <fstream>
using namespace std;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRS232Dlg dialog



CRS232Dlg::CRS232Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRS232Dlg::IDD, pParent)
	, m_TextboxData(_T("0"))
	, m_StatictxtNotif(_T("ali hejazi"))
	, m_TextboxRandData(_T(""))
	, m_TextboxBaudrate(_T("1200"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRS232Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Data, m_TextboxData);
	DDX_Control(pDX, IDC_NOTIF, m_TxtNotif);
	DDX_Control(pDX, IDC_STATUS, m_TxtStatus);
	DDX_Text(pDX, IDC_EDIT_RAND, m_TextboxRandData);
	DDX_Control(pDX, IDC_DATASTATUS, m_TxtSuccess);
	DDX_Control(pDX, IDC_SPIN_BAUDRATE, m_Spin_Baudrate);
	DDX_Text(pDX, IDC_EDIT_BAUDRATE, m_TextboxBaudrate);
	DDX_Control(pDX, IDC_FILE, m_FileStatus);
	DDX_Control(pDX, IDC_TITLE, m_Title);
}

BEGIN_MESSAGE_MAP(CRS232Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &CRS232Dlg::OnBnClickedButtonOpenport)
	ON_BN_CLICKED(IDC_BUTTON_CLOSEPORT, &CRS232Dlg::OnBnClickedButtonCloseport)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CRS232Dlg::OnBnClickedButtonSend)	
	ON_BN_CLICKED(IDC_BUTTON_RAND, &CRS232Dlg::OnBnClickedButtonRand)
	ON_WM_VSCROLL()
	ON_STN_CLICKED(IDC_TITLE, &CRS232Dlg::OnStnClickedTitle)
END_MESSAGE_MAP()


// CRS232Dlg message handlers

BOOL CRS232Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Spin_Baudrate.SetRange(0, 5000);

	CStatic * m_pTitle = &m_Title;
	CFont *m_Font = new CFont;
	m_Font->CreatePointFont(90, (LPCTSTR)"MS Shell Dlg 2 Bold");
	m_pTitle = (CStatic *)GetDlgItem(IDC_TITLE);
	m_pTitle->SetFont(m_Font);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRS232Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRS232Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRS232Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// CRS232Dialog message handlers
ofstream myfile;
HANDLE hSerial;
char Buff[200];
bool P_OpenStatus = FALSE;
DCB dcb = { 0 };
DCB config = { 0 };

void CRS232Dlg::OnBnClickedButtonOpenport()
{
	if (P_OpenStatus == FALSE){
		UpdateData(1);
		hSerial = ::CreateFile(L"COM1",
			GENERIC_READ | GENERIC_WRITE,//access ( read and write)
			0,    //(share) 0:cannot share the COM port                        
			0,    //security  (None)                
			OPEN_EXISTING,// creation : open_existing
			0,    // we dont want overlapped operation
			0// no templates file for COM port...
			);
		
		config.DCBlength = sizeof(config);
		if ((GetCommState(hSerial, &config) == 0))
			printf("Get configuration port has a problem.");

		int Baudrate = _wtoi(m_TextboxBaudrate.GetBuffer());
		config.BaudRate = Baudrate;
		config.StopBits = ONESTOPBIT;
		config.Parity = PARITY_NONE;
		config.ByteSize = DATABITS_8;
		config.fDtrControl = 0;
		config.fRtsControl = 0;

		if (!SetCommState(hSerial, &config))
			printf("Failed to Set Comm State Reason: %d\n", GetLastError());

		printf("Current Settings\n Baud Rate %d\n Parity %d\n Byte Size %d\n Stop Bits %d", config.BaudRate,
			config.Parity, config.ByteSize, config.StopBits);

		myfile.open("c++Output.txt");
		m_FileStatus.SetWindowText(L"Output txt Opened as a Raw File!");
		P_OpenStatus = TRUE;
		m_TxtNotif.SetWindowText(L"Port COM1 opened");
		m_TxtStatus.SetWindowText(L"Ready to send Data...");
	}
	else
		m_TxtNotif.SetWindowText(L"Port is already open!");
}

void CRS232Dlg::OnBnClickedButtonCloseport()
{
	if (P_OpenStatus == TRUE){
		myfile.close();
		CloseHandle(hSerial);
		P_OpenStatus = FALSE;
		m_TxtNotif.SetWindowText(L"Port COM1 closed");
		m_TxtStatus.SetWindowText(L"");
		m_TextboxRandData.Format(L"%s", "");
		m_TextboxData.Format(L"%s", "");
		m_FileStatus.SetWindowText(L"Output txt Closed!");
	}
	else
		m_TxtNotif.SetWindowText(L"Port is already closed!");

}

void CRS232Dlg::OnBnClickedButtonSend()
{
	if (P_OpenStatus == TRUE){
		UpdateData(1);
		m_TextboxRandData.Format(L"%s", " ");
		UpdateData(0);

		int number = _wtoi(m_TextboxData.GetBuffer());
		_itoa_s(number, Buff, 10);

		int counter{ 1 };
		while (number > 1){
			number /= 10;
			counter++;
		}
		int size = counter * sizeof(Buff[0]);
		DWORD dwBytesRead = 1;
		WriteFile(hSerial, Buff, size, &dwBytesRead, NULL);
		myfile << Buff << flush;

		m_TxtSuccess.SetWindowText(L"Data sent successfully!");
		m_FileStatus.SetWindowText(L"Writing in Output txt File...");
		
		Sleep(200);
		m_TxtSuccess.SetWindowText(L"");
	}
	else
		MessageBox(_T("Unable to send Data! Port is closed!"));
}


void CRS232Dlg::OnBnClickedButtonRand()
{
	if (P_OpenStatus == TRUE){		
		int number = rand() % 1000;
		m_TextboxRandData.Format(L"%d", number);
		m_TextboxData.Format(L"%s", "0");
		UpdateData(0);

		_itoa_s(number, Buff, 10);
		int counter{ 1 };
		while (number > 1){
			number /= 10;
			counter++;
		}
		int size = counter * sizeof(Buff[0]);
		DWORD dwBytesRead = 1;
		WriteFile(hSerial, Buff, size, &dwBytesRead, NULL);
		myfile << Buff << flush;

		m_TxtSuccess.SetWindowText(L"Random Data sent successfully!");
		m_FileStatus.SetWindowText(L"Writing in Output txt File...");
		Sleep(200);
		m_TxtSuccess.SetWindowText(L"");

	}
	else
		MessageBox(_T("Unable to send Data! Port is closed!"));
}

void CRS232Dlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CRS232Dlg::OnStnClickedTitle()
{
	// TODO: Add your control notification handler code here
}
