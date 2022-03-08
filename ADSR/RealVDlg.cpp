// RealVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "RealVDlg.h"
#include "MainFrm.h"


// CRealVDlg dialog

IMPLEMENT_DYNAMIC(CRealVDlg, CDialog)

CRealVDlg::CRealVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealVDlg::IDD, pParent)
	, m_nTypeGraph(0)
{
}

CRealVDlg::~CRealVDlg()
{
}

void CRealVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_RESP_TH, m_nTypeGraph);
	DDX_Control(pDX, IDC_LIST_EXC1, m_CtrlEList);
	DDX_Control(pDX, IDC_LIST_RES1, m_CtrlRList);
	DDX_Control(pDX, IDC_CHECK_Exc, m_CtrlExcCheck);
	DDX_Control(pDX, IDC_CHECK_Res, m_CtrlResCheck);
}

BEGIN_MESSAGE_MAP(CRealVDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GRACRE, &CRealVDlg::OnBnClickedButtonGraphCreate)
	ON_BN_CLICKED(IDOK, &CRealVDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_RESP_TH, &CRealVDlg::OnBnClickedRadioRespTh)
	ON_BN_CLICKED(IDC_RADIO_SI_FRF, &CRealVDlg::OnBnClickedRadioSiFrf)
	ON_BN_CLICKED(IDC_RADIO_TS_PSD, &CRealVDlg::OnBnClickedRadioTsPsd)
	ON_BN_CLICKED(IDC_CHECK_Res, &CRealVDlg::OnBnClickedCheckRes)
	ON_BN_CLICKED(IDC_CHECK_Exc, &CRealVDlg::OnBnClickedCheckExc)
END_MESSAGE_MAP()


// CRealVDlg message handlers

BOOL CRealVDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect t_Rect;
	m_CtrlRList.GetClientRect(t_Rect);
	m_CtrlRList.SetColumnWidth(t_Rect.Width()/2);
	m_CtrlEList.SetColumnWidth(t_Rect.Width()/2);

	for ( int iIndex=0 ; iIndex < System.SS.FRONTEND.m_nInput_Channels ; iIndex++)
		m_CtrlEList.AddString(System.SS.FRONTEND.m_arrstrInputChannelName[iIndex].c_str());
	for ( int iIndex=0 ; iIndex < System.SS.FRONTEND.m_nResponse_Channels ; iIndex++)
		m_CtrlRList.AddString(System.SS.FRONTEND.m_arrstrResponseChannelName[iIndex].c_str());

	m_CtrlEList.EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRealVDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CRealVDlg::OnBnClickedRadioRespTh()
{
	// TODO: Add your control notification handler code here

		m_CtrlEList.EnableWindow(FALSE);
}

void CRealVDlg::OnBnClickedRadioSiFrf()
{
	// TODO: Add your control notification handler code here

		m_CtrlEList.EnableWindow(TRUE);
}

void CRealVDlg::OnBnClickedRadioTsPsd()
{
	// TODO: Add your control notification handler code here

		m_CtrlEList.EnableWindow(FALSE);
}


void CRealVDlg::OnBnClickedCheckRes()
{
	// TODO: Add your control notification handler code here
	// 나온상태가 CHECK된 상태이다.

	//눌러서 나왔을때
	if ( m_CtrlResCheck.GetCheck() == BST_UNCHECKED)
	{
		if ( System.SS.FRONTEND.m_nResponse_Channels > 0 
			&& System.SS.FRONTEND.m_nResponse_Channels < 2 ) m_CtrlRList.SetSel(0,FALSE);
		else m_CtrlRList.SelItemRange(FALSE,0,System.SS.FRONTEND.m_nResponse_Channels-1);
		m_CtrlResCheck.SetCheck(BST_UNCHECKED);
	}
	//눌러서 들어갔을때
	else
	{
		if ( System.SS.FRONTEND.m_nResponse_Channels >0 
			&& System.SS.FRONTEND.m_nResponse_Channels < 2 ) m_CtrlRList.SetSel(0,TRUE);
		else m_CtrlRList.SelItemRange(TRUE,0,System.SS.FRONTEND.m_nResponse_Channels-1);
		m_CtrlResCheck.SetCheck(BST_CHECKED);
	}
}

void CRealVDlg::OnBnClickedCheckExc()
{
	// TODO: Add your control notification handler code here
	// 나온상태가 CHECK된 상태이다.

    //눌러서 나왔을때
 	if ( m_CtrlExcCheck.GetCheck() == BST_UNCHECKED)
 	{
		if ( System.SS.FRONTEND.m_nInput_Channels >0 
			&& System.SS.FRONTEND.m_nInput_Channels < 2 ) m_CtrlEList.SetSel(0,FALSE);
 		else m_CtrlEList.SelItemRange(FALSE,0,System.SS.FRONTEND.m_nInput_Channels-1);
 		m_CtrlExcCheck.SetCheck(BST_UNCHECKED);
 	}
	//눌러서 들어갔을때
 	else
 	{
		if ( System.SS.FRONTEND.m_nInput_Channels >0 
			&& System.SS.FRONTEND.m_nInput_Channels < 2 ) m_CtrlEList.SetSel(0,TRUE);
 		else m_CtrlEList.SelItemRange(TRUE,0,System.SS.FRONTEND.m_nInput_Channels-1);
 		m_CtrlExcCheck.SetCheck(BST_CHECKED);
 	}
}


void CRealVDlg::OnBnClickedButtonGraphCreate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CMainFrame* pMainFrame=(CMainFrame*)AfxGetMainWnd();

	int nECount = m_CtrlEList.GetSelCount();
	int nRCount = m_CtrlRList.GetSelCount();

	// 리스트에서 선택된 것이 없다면 리턴.
	if ( nRCount == 0 ) return;
	if ( m_nTypeGraph==1 && nECount == 0) return;

	m_aryEListSel.SetSize(nECount);
	m_aryRListSel.SetSize(nRCount);
	pMainFrame->m_aryEListSel.SetSize(nECount);
	pMainFrame->m_aryRListSel.SetSize(nRCount);

	m_CtrlEList.GetSelItems(nECount,m_aryEListSel.GetData());
	m_CtrlRList.GetSelItems(nRCount,m_aryRListSel.GetData());

	for (int i=0 ; i < nECount ; i++ )
	pMainFrame->m_aryEListSel[i]=m_aryEListSel[i];
	for (int i=0 ; i < nRCount ; i++ )
	pMainFrame->m_aryRListSel[i]=m_aryRListSel[i];

	CString tt;
 	tt.Format("가진 갯수%d 응답 갯수%d  타입 %d",nECount,nRCount,m_nTypeGraph);
// 	tt.Format("선택된 데이타 %d %d",m_nTypeGraph,m_arrRListSel[0]);
  	AfxMessageBox(tt);


	// 메세지를 메인 프레임에 전달
	pMainFrame->SendMessage( WM_HD_TF
						    ,MAKEWPARAM(m_nTypeGraph,1)
							,MAKELPARAM(nECount,nRCount) );


	// Dump the selection array
	AFXDUMP(m_aryEListSel);
	AFXDUMP(m_aryRListSel);

}