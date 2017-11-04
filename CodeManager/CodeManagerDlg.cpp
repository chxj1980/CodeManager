
// CodeManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CodeManager.h"
#include "CodeManagerDlg.h"
#include "afxdialogex.h"
#include "filenameos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCodeManagerDlg dialog



CCodeManagerDlg::CCodeManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CODEMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCodeManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTFILE, m_ListFile);
	DDX_Control(pDX, IDC_TREEFOLDER, m_TreeFile);
	DDX_Control(pDX, IDC_EDITLIST, m_editList);
}

BEGIN_MESSAGE_MAP(CCodeManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LISTFILE, &CCodeManagerDlg::OnNMClickListfile)
	ON_EN_KILLFOCUS(IDC_EDITLIST, &CCodeManagerDlg::OnEnKillfocusEditlist)
END_MESSAGE_MAP()


// CCodeManagerDlg message handlers

BOOL CCodeManagerDlg::OnInitDialog()
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

	m_MainMenu.LoadMenu(IDR_MAINMENU);
	SetMenu(&m_MainMenu);

	m_Drive[256] = { 0 };
	m_nDriveNum = GetLogicalDriveStrings(sizeof(m_Drive) / sizeof(TCHAR), m_Drive);

	// TODO: Add extra initialization here
	//InsertTreeFile();
	InsertListFile();
	m_editList.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCodeManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCodeManagerDlg::OnPaint()
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
HCURSOR CCodeManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��listctrl�в�������
void CCodeManagerDlg::InsertListFile()
{
	m_ListFile.InsertColumn(0, "�γ�����", LVCFMT_CENTER, 100);
	m_ListFile.InsertColumn(1, "�Ƿ�����", LVCFMT_CENTER, 100);
	m_ListFile.InsertColumn(2, "�ܿ�ʱ", LVCFMT_CENTER, 100);
	m_ListFile.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListFile.SetBkColor(RGB(172, 199, 255));

	for (int n = 0;n < 10;n++)
		m_ListFile.InsertItem(n, "wwqw");

	CRect rect(100, 100, 100, 100);
	m_ListFile.GetClientRect(&rect);
	m_ListFile.SetColumnWidth(0, rect.Width() / 2);
	m_ListFile.SetColumnWidth(1, rect.Width() / 4);
	m_ListFile.SetColumnWidth(2, rect.Width() / 4);
}

//�����������ļ����뵽���νṹ��
void CCodeManagerDlg::InsertTreeFile()
{
	//������ڵ�
	HTREEITEM hItem, hSubItem;
	hItem = m_TreeFile.InsertItem("$", TVI_ROOT);						//�ڸ���������Parent1
	for (int n = 0; n < m_nDriveNum; n+=4)
	{
		hSubItem = m_TreeFile.InsertItem(&m_Drive[n], hItem);					//��Parent1�����һ���ӽ��
		//��������
		std::vector<CString> arrPath;
		char strCPath[256];
		sprintf(strCPath, "%s", &m_Drive[n]);
		std::string strSPath;
		strSPath = strCPath;
		TraversalFile(strSPath, arrPath);

		//�����ݽ�����һ����Ŀ¼��Ȼ����뵽���οؼ���


		//FILE * fp = fopen("E:\\a.txt", "wt");
		//std::vector<CString>::iterator iterFile = arrPath.begin();
		//for (; iterFile != arrPath.end(); iterFile++)
		//	fprintf(fp,"%s\n", *iterFile);

		//fclose(fp);

		//�и��ַ�������C:\360������ʦĿ¼\���ر���Ŀ¼\vga_intel_21.20.16.4627_x64\Graphics\he_w7_32.vp
		//���ַ������뵽���Խṹ�У�
		std::vector<CString>::iterator iterFile = arrPath.begin();
		for (; iterFile != arrPath.end(); iterFile++)
		{
			std::vector<std::string> arrStr;
			std::string strTmp = *iterFile;
			strSplit(strTmp, arrStr, '\\', false);
		}
	}
	
	//hSubItem = m_TreeFile.InsertItem(L"Child1_2", hItem, hSubItem);			//��Parent1�����һ���ӽ�㣬����Child1_1����
	//hSubItem = m_TreeFile.InsertItem(L"Child1_3", hItem, hSubItem);
	//hItem = m_TreeFile.InsertItem(L"Parent2", TVI_ROOT, hItem);
	//hItem = m_TreeFile.InsertItem(L"Parent3", TVI_ROOT, hItem);

	//��ȡ
}

void CCodeManagerDlg::OnNMClickListfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	/*
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	if (pNMListView->iItem == -1)
		return;

	m_row = pNMListView->iItem;
	m_col = pNMListView->iSubItem;

	//if (m_col != 2)
	//	return;
	UpdateData(TRUE);

	m_ListFile.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem, LVIR_LABEL, rc);

	rc.left += 150;
	rc.right += 150;
	rc.bottom += 100;

	MessageBox(_T("aa"), _T("sdsd"), MB_OK);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	*/

	//���ӿɱ༭����
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	if (pNMListView->iItem != -1)
	{
		m_row = pNMListView->iItem;
		m_col = pNMListView->iSubItem;
		m_ListFile.GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem, LVIR_LABEL, rc);		//���С����
		rc.left += 3;
		rc.top += 2;
		rc.right += 3;
		rc.bottom += 2;

		char* ch = new char[128];
		m_ListFile.GetItemText(pNMListView->iItem, pNMListView->iSubItem, ch, 128);//��ȡ��������

		m_editList.SetWindowText(ch);
		m_editList.ShowWindow(SW_SHOW);
		m_editList.MoveWindow(&rc);
		m_editList.SetFocus();
		m_editList.CreateSolidCaret(1, rc.Height() - 5);	//����һ�����
		m_editList.ShowCaret();
		m_editList.SetSel(-1);								//�ǹ���Ƶ������
	}
	*pResult = 0;
}


//editʧȥ�������Ӧ����
void CCodeManagerDlg::OnEnKillfocusEditlist()
{
	// TODO: Add your control notification handler code here
	CString str;
	m_editList.GetWindowText(str);//ȡ�ñ༭�������
	m_ListFile.SetItemText(m_row, m_col, str);//�������ݸ��µ�CListCtrl��
	m_editList.ShowWindow(SW_HIDE);//���ر༭��
}
