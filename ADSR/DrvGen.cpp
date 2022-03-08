// DrvGen.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "DrvGen.h"


// CDrvGen dialog

IMPLEMENT_DYNAMIC(CDrvGen, CDialog)


CDrvGen::CDrvGen( bool IsFirstDrive/*=FALSE*/,CWnd* pParent /*= NULL*/ )
	: CDialog(CDrvGen::IDD, pParent)
	, m_nComboRespElement(0)
	, m_nEditRespValue(0)
	, m_bIsFirstDrive(IsFirstDrive)
{

}


CDrvGen::~CDrvGen()
{
}

void CDrvGen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ErrorW, m_ListCtrlErrorW);
	DDX_Control(pDX, IDC_COMBO_RESP_ELE, m_ComCtrlRespElement);
	DDX_CBIndex(pDX, IDC_COMBO_RESP_ELE, m_nComboRespElement);
	DDX_Text(pDX, IDC_EDIT_RESP_Value, m_nEditRespValue);
}


BEGIN_MESSAGE_MAP(CDrvGen, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESP_Apply, &CDrvGen::OnBnClickedButtonRespApply)
	ON_BN_CLICKED(IDOK, &CDrvGen::OnBnClickedOk)
END_MESSAGE_MAP()


// CDrvGen message handlers

BOOL CDrvGen::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	// 임시 변수
	char N[4];
	int nNPriResp=System.SS.FRONTEND.m_nResponse_Channels;

	// LIST CTRL 
	m_ListCtrlErrorW.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_ListCtrlErrorW.SetExtendedStyle
		(m_ListCtrlErrorW.GetExtendedStyle()|LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);

	CRect rect;
	m_ListCtrlErrorW.GetClientRect(&rect);
	m_ListCtrlErrorW.InsertColumn(0,"N",LVCFMT_CENTER,rect.Width()*8/100);
	m_ListCtrlErrorW.InsertColumn(1,"Channel Name",LVCFMT_CENTER,rect.Width()*23/100);
	m_ListCtrlErrorW.InsertColumn(2,"Target Level",LVCFMT_CENTER,rect.Width()*23/100);
	m_ListCtrlErrorW.InsertColumn(3,"First Drive",LVCFMT_CENTER,rect.Width()*23/100);
	m_ListCtrlErrorW.InsertColumn(4,"Error Weight",LVCFMT_CENTER,rect.Width()*23/100);

	for (int i=0; i< nNPriResp ;i++)
	{
		_itoa_s(i+1,N,10);
		m_ListCtrlErrorW.InsertItem(i,N,0);
		m_ListCtrlErrorW.SetItemText(i,1
			,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());

		_itoa_s(100,N,10);
		m_ListCtrlErrorW.SetItemText(i,2,N);
		_itoa_s(50,N,10);
		m_ListCtrlErrorW.SetItemText(i,3,N);
		_itoa_s(50,N,10);
		m_ListCtrlErrorW.SetItemText(i,4,N);
	}

	// Combo Box
	if (m_bIsFirstDrive==TRUE)
	{
		m_ComCtrlRespElement.AddString("First Drive Level");
		m_ComCtrlRespElement.AddString("Target Level");
	}
	else
	{
		m_ComCtrlRespElement.AddString("Error Weighting Level");
		m_ComCtrlRespElement.AddString("Target Level");
	}

	m_nEditRespValue=50;
	UpdateData(FALSE);

	m_vecFirstDriveLevel.resize(System.SS.FRONTEND.m_nResponse_Channels);
	m_vecErrorLevel.resize(System.SS.FRONTEND.m_nResponse_Channels);
	m_vecOutWeightLevel.assign(System.SS.FRONTEND.m_nInput_Channels,1.);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



// 적용 버튼 클릭
void CDrvGen::OnBnClickedButtonRespApply()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	POSITION pos=m_ListCtrlErrorW.GetFirstSelectedItemPosition();
	CEdit *pEditResp=(CEdit*)GetDlgItem(IDC_EDIT_RESP_Value);
	CString text;
	pEditResp->GetWindowText(text);
	if ( m_nComboRespElement==0/* First Drive Level or Error LEVEL */)
	{
		if ( m_bIsFirstDrive==TRUE)
		{
			while(pos != NULL)
			{
				int nItem=m_ListCtrlErrorW.GetNextSelectedItem(pos);
				m_ListCtrlErrorW.SetItemText(nItem,3,(LPTSTR)(const char*)text);
			}
		}
		else 
		{
			while(pos != NULL)
			{
				int nItem=m_ListCtrlErrorW.GetNextSelectedItem(pos);
				m_ListCtrlErrorW.SetItemText(nItem,4,(LPTSTR)(const char*)text);
			}
		}

	}
	
}

void CDrvGen::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString text;
	int nNPriResp=System.SS.FRONTEND.m_nResponse_Channels;
	int number;
	for ( int i=0 ; i < nNPriResp ; i++)
	{
		if(m_bIsFirstDrive==TRUE)
		{
			text=m_ListCtrlErrorW.GetItemText(i,3);
			number=static_cast<unsigned int>(_tstoi((const char*)text));
			m_vecFirstDriveLevel[i]=static_cast<double>(number/100.);
// 			CString a;
// 			a.Format("%f",m_aiFirstDriveLevel[i]);
// 			AfxMessageBox(a);
		}
		else
		{
			text=m_ListCtrlErrorW.GetItemText(i,4);
			number=static_cast<unsigned int>(_tstoi((const char*)text));
			m_vecErrorLevel[i]=static_cast<double>(number/100.);
		}
	}
	OnOK();
}
