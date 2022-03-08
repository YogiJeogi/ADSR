// DSignalsImport.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "DSignalsImport.h"


// CDSignalsImport dialog

IMPLEMENT_DYNAMIC(CDSignalsImport, CDialog)

CDSignalsImport::CDSignalsImport(CWnd* pParent /*=NULL*/)
	: CDialog(CDSignalsImport::IDD, pParent)
	, m_dbDesiredFrom(0)
	, m_dbDesiredTo(0)
	, m_nSubMatchedDesiredSignals(0)
{

}

CDSignalsImport::~CDSignalsImport()
{
}

void CDSignalsImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DESIRED_SIGN, m_CtrlListDesiredSignals);
	DDX_Control(pDX, IDC_LIST_RESP_SIGN, m_CtrlListRespSignals);
	DDX_Text(pDX, IDC_EDIT_Dsing_Fp, m_dbDesiredFrom);
	DDX_Text(pDX, IDC_EDIT_Dsign_Lp, m_dbDesiredTo);
}


BEGIN_MESSAGE_MAP(CDSignalsImport, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SEL, &CDSignalsImport::OnBnClickedButtonFileSel)
	ON_BN_CLICKED(IDC_BUTTON_Insert_Dsign, &CDSignalsImport::OnBnClickedButtonInsertDsign)
	ON_BN_CLICKED(IDC_BUTTON_Dsign_Sign, &CDSignalsImport::OnBnClickedButtonDsignSign)
	ON_BN_CLICKED(IDOK, &CDSignalsImport::OnBnClickedOk)
END_MESSAGE_MAP()


// CDSignalsImport message handlers

void CDSignalsImport::OnBnClickedButtonFileSel()
{
	// TODO: Add your control notification handler code here
	// 임시 변수 선언
	char N[3];

	// 파일 대화상자 생성
	CFileDialog FileDlg(TRUE,"asc",NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON
					,"ASC Files(*.asc)|*.asc|All Files(*.*)|*.*||",this);
	if( FileDlg.DoModal()==IDOK)
	{
		CStatic * pStaticDesiredSignalFrom=(CStatic*)GetDlgItem(IDC_STATIC_EXST_FROM);
		CStatic * pStaticDesiredSignalTo=(CStatic*)GetDlgItem(IDC_STATIC_EXST_TO);
		CEdit * pEditDesiredSignalPath=(CEdit*)GetDlgItem(IDC_EDIT_FILE_PATH);

		// 데이타 가져옴!!
		if( !System.DataImportExport.LoadDesiredSignals
				((LPTSTR)(const char*)FileDlg.GetPathName(),m_sDesiredSignalFileInformation))
		{
			return;
		}
// 
		// 시간 스태틱, 에디트 값 설정
		double t_nTimeTo=(m_sDesiredSignalFileInformation.LENGTH[0]-1)
						*m_sDesiredSignalFileInformation.DELTA[0];

		CString t_strTimeTo;
		t_strTimeTo.Format("%f sec",t_nTimeTo);
	
		pEditDesiredSignalPath->SetWindowText(FileDlg.GetPathName());
		pStaticDesiredSignalFrom->SetWindowText("0 sec");
		pStaticDesiredSignalTo->SetWindowText(t_strTimeTo);
		m_dbDesiredTo=t_nTimeTo;
		m_dbDesiredFrom=0;
		UpdateData(false);

		// 목표 신호 아이템 데이타 삽입.
		for (unsigned int i=0; i<m_sDesiredSignalFileInformation.NumberofSignals;i++)
		{
			CString t_str;
			_itoa_s(i+1,N,10);
			m_CtrlListDesiredSignals.InsertItem(i,N);
			m_CtrlListDesiredSignals.SetItemText(i,1,m_sDesiredSignalFileInformation.CHANNELNAME[i].data());
			m_CtrlListDesiredSignals.SetItemText(i,2,m_sDesiredSignalFileInformation.UNIT[i].data());
			t_str.Format("%f",m_sDesiredSignalFileInformation.DELTA[i]);
			m_CtrlListDesiredSignals.SetItemText(i,3,t_str);
			t_str.Format("%f",m_sDesiredSignalFileInformation.DELTA[i]
			*m_sDesiredSignalFileInformation.LENGTH[i]);
			m_CtrlListDesiredSignals.SetItemText(i,4,t_str);
			/*		m_CtrlListRespSignals.SetItemData(i,i);*/
		}
	}
}

BOOL CDSignalsImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// 임시 변수 정의
	char N[3];

	// 스타일 정의
	m_CtrlListDesiredSignals.ModifyStyle(LVS_TYPEMASK|LVS_SINGLESEL,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListRespSignals.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListDesiredSignals.SetExtendedStyle
		(m_CtrlListDesiredSignals.GetExtendedStyle()|LVS_EX_FULLROWSELECT
				|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);
	m_CtrlListRespSignals.SetExtendedStyle
		(m_CtrlListRespSignals.GetExtendedStyle()|LVS_EX_FULLROWSELECT
				|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);	

	// 컬룸 생성
	m_CtrlListDesiredSignals.InsertColumn(0,"N",LVCFMT_CENTER);
	m_CtrlListDesiredSignals.InsertColumn(1,"Channel Name",LVCFMT_CENTER);
	m_CtrlListDesiredSignals.InsertColumn(2,"Unit",LVCFMT_CENTER);
	m_CtrlListDesiredSignals.InsertColumn(3,"Delta",LVCFMT_CENTER);
	m_CtrlListDesiredSignals.InsertColumn(4,"Time",LVCFMT_CENTER);

	m_CtrlListRespSignals.InsertColumn(0,"N",LVCFMT_CENTER);
	m_CtrlListRespSignals.InsertColumn(1,"Channel Name",LVCFMT_CENTER);
	m_CtrlListRespSignals.InsertColumn(2,"Seleted Desired Signal",LVCFMT_CENTER);
	m_CtrlListRespSignals.InsertColumn(3,"Sign",LVCFMT_CENTER);

	m_CtrlListDesiredSignals.SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListDesiredSignals.SetColumnWidth(1,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListDesiredSignals.SetColumnWidth(2,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListDesiredSignals.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListDesiredSignals.SetColumnWidth(4,LVSCW_AUTOSIZE_USEHEADER);

	m_CtrlListRespSignals.SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListRespSignals.SetColumnWidth(1,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListRespSignals.SetColumnWidth(2,LVSCW_AUTOSIZE_USEHEADER);
	m_CtrlListRespSignals.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);

	int nRespC=System.SS.FRONTEND.m_nResponse_Channels
				+System.SS.FRONTEND.m_nSlave_Channels;

	// 채널 이름 대입
	for (int i=0; i<nRespC ;i++)
	{
		_itoa_s(i+1,N,10);
		m_CtrlListRespSignals.InsertItem(i,N);
		m_CtrlListRespSignals.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
	}

	// 배열 사이즈 선정
	m_anMatchedDesiredSignals.SetSize(nRespC);
	m_anSignDesiredSignals.SetSize(nRespC);
	m_aryIsDesiredSignalExistInReationWithSubResponseChannel.SetSize(System.SS.FRONTEND.m_nSlave_Channels);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDSignalsImport::OnBnClickedButtonInsertDsign()
{
	// TODO: Add your control notification handler code here
	POSITION posD = m_CtrlListDesiredSignals.GetFirstSelectedItemPosition();
	POSITION posR = m_CtrlListRespSignals.GetFirstSelectedItemPosition();

	if (posD == NULL)
	{
		AfxMessageBox("목표 신호가 선택되지 않았습니다."); return;
	}
	else if ( posR==NULL)
	{
		AfxMessageBox("응답 신호가 선택되지 않았습니다."); return;
	}
	else if (m_CtrlListRespSignals.GetSelectedCount()
				!= m_CtrlListDesiredSignals.GetSelectedCount())
	{
		AfxMessageBox("선택된 갯수가 일치하지 않습니다."); return;
	}
	else
	{
		// 두 리스트간 매칭하는 잡업
		while (posD)
		{
			int nItemD = m_CtrlListDesiredSignals.GetNextSelectedItem(posD);
			int nItemR = m_CtrlListRespSignals.GetNextSelectedItem(posR);
			
			// 디버깅
// 			CString a;
// 			a.Format("R %d D %d",nItemR,nItemD);
// 			AfxMessageBox(a);
				
			// 아이템 대입
			m_CtrlListRespSignals.SetItemText(nItemR,2
							,m_CtrlListDesiredSignals.GetItemText(nItemD,1));
			m_CtrlListRespSignals.SetItemText(nItemR,3,"+");

			// 각 인덱스를 배열에 저장.
			m_anMatchedDesiredSignals[nItemR]=nItemD;
			m_anSignDesiredSignals[nItemR]= 1;

		}// while (posD)
	}
}

void CDSignalsImport::OnBnClickedButtonDsignSign()
{
	// TODO: Add your control notification handler code here

	// 위치 
	POSITION posR = m_CtrlListRespSignals.GetFirstSelectedItemPosition();
	if (posR == NULL)
	{
		AfxMessageBox("선택된 응답 신호가 없습니다."); return;
	}
	else
	{
		while(posR)
		{
			int nItemR = m_CtrlListRespSignals.GetNextSelectedItem(posR);
			// 부호 판별
			if ( m_CtrlListRespSignals.GetItemText(nItemR,3) == "+")
			{
				// 부호 변경
				m_CtrlListRespSignals.SetItemText(nItemR,3,"-");
				// 부호에 따른 Plus Minus변경
				m_anSignDesiredSignals[nItemR]=-1;
			}
			else
			{
				m_CtrlListRespSignals.SetItemText(nItemR,3,"+");
				m_anSignDesiredSignals[nItemR]=1;
			}
		}// while(posR)
	}
}

void CDSignalsImport::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// 임시저장 변수
	double t_dbDesiredTime=(m_sDesiredSignalFileInformation.LENGTH[0]-1)
					*m_sDesiredSignalFileInformation.DELTA[0];

	// 시간 오류 판단
	if ( m_dbDesiredFrom == m_dbDesiredTo || m_dbDesiredFrom> m_dbDesiredTo)
	{AfxMessageBox("시간을 잘못 설정하였습니다."); return;}
	else if ( m_dbDesiredTo > t_dbDesiredTime )
	{AfxMessageBox("시간을 잘못 설정하였습니다.");return;}
	else if ( m_dbDesiredTo < double(System.SS.SPECTRAL.m_nBlocksize*5)
				         	   /double(System.SS.FRONTEND.m_nSamplingFreq) )  
	{AfxMessageBox("설정된 목표 시간이 적습니다.");  return;}
	
	// 매칭 오류 판단.
	// 응답 채널이 완료되었는가 서브채널 제외.
	for (int i=0 ; i < System.SS.FRONTEND.m_nResponse_Channels ; i++)
	{
		if ( m_CtrlListRespSignals.GetItemText(i,2)=="")
		{
			AfxMessageBox("응답 채널이 모두 매칭되지 않았습니다.");
			return;
		}
	}
	for ( int i=0 ; i < System.SS.FRONTEND.m_nSlave_Channels ; i++ )
	{
		if ( m_CtrlListRespSignals.GetItemText(i+System.SS.FRONTEND.m_nResponse_Channels,2)==""){
			m_aryIsDesiredSignalExistInReationWithSubResponseChannel[i]=false;
		}
		else{
			m_aryIsDesiredSignalExistInReationWithSubResponseChannel[i]=true;
			m_nSubMatchedDesiredSignals += 1;
		}
	}
	OnOK();
}
