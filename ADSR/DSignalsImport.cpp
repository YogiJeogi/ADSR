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
	// �ӽ� ���� ����
	char N[3];

	// ���� ��ȭ���� ����
	CFileDialog FileDlg(TRUE,"asc",NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NONETWORKBUTTON
					,"ASC Files(*.asc)|*.asc|All Files(*.*)|*.*||",this);
	if( FileDlg.DoModal()==IDOK)
	{
		CStatic * pStaticDesiredSignalFrom=(CStatic*)GetDlgItem(IDC_STATIC_EXST_FROM);
		CStatic * pStaticDesiredSignalTo=(CStatic*)GetDlgItem(IDC_STATIC_EXST_TO);
		CEdit * pEditDesiredSignalPath=(CEdit*)GetDlgItem(IDC_EDIT_FILE_PATH);

		// ����Ÿ ������!!
		if( !System.DataImportExport.LoadDesiredSignals
				((LPTSTR)(const char*)FileDlg.GetPathName(),m_sDesiredSignalFileInformation))
		{
			return;
		}
// 
		// �ð� ����ƽ, ����Ʈ �� ����
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

		// ��ǥ ��ȣ ������ ����Ÿ ����.
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
	// �ӽ� ���� ����
	char N[3];

	// ��Ÿ�� ����
	m_CtrlListDesiredSignals.ModifyStyle(LVS_TYPEMASK|LVS_SINGLESEL,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListRespSignals.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListDesiredSignals.SetExtendedStyle
		(m_CtrlListDesiredSignals.GetExtendedStyle()|LVS_EX_FULLROWSELECT
				|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);
	m_CtrlListRespSignals.SetExtendedStyle
		(m_CtrlListRespSignals.GetExtendedStyle()|LVS_EX_FULLROWSELECT
				|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);	

	// �÷� ����
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

	// ä�� �̸� ����
	for (int i=0; i<nRespC ;i++)
	{
		_itoa_s(i+1,N,10);
		m_CtrlListRespSignals.InsertItem(i,N);
		m_CtrlListRespSignals.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
	}

	// �迭 ������ ����
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
		AfxMessageBox("��ǥ ��ȣ�� ���õ��� �ʾҽ��ϴ�."); return;
	}
	else if ( posR==NULL)
	{
		AfxMessageBox("���� ��ȣ�� ���õ��� �ʾҽ��ϴ�."); return;
	}
	else if (m_CtrlListRespSignals.GetSelectedCount()
				!= m_CtrlListDesiredSignals.GetSelectedCount())
	{
		AfxMessageBox("���õ� ������ ��ġ���� �ʽ��ϴ�."); return;
	}
	else
	{
		// �� ����Ʈ�� ��Ī�ϴ� ���
		while (posD)
		{
			int nItemD = m_CtrlListDesiredSignals.GetNextSelectedItem(posD);
			int nItemR = m_CtrlListRespSignals.GetNextSelectedItem(posR);
			
			// �����
// 			CString a;
// 			a.Format("R %d D %d",nItemR,nItemD);
// 			AfxMessageBox(a);
				
			// ������ ����
			m_CtrlListRespSignals.SetItemText(nItemR,2
							,m_CtrlListDesiredSignals.GetItemText(nItemD,1));
			m_CtrlListRespSignals.SetItemText(nItemR,3,"+");

			// �� �ε����� �迭�� ����.
			m_anMatchedDesiredSignals[nItemR]=nItemD;
			m_anSignDesiredSignals[nItemR]= 1;

		}// while (posD)
	}
}

void CDSignalsImport::OnBnClickedButtonDsignSign()
{
	// TODO: Add your control notification handler code here

	// ��ġ 
	POSITION posR = m_CtrlListRespSignals.GetFirstSelectedItemPosition();
	if (posR == NULL)
	{
		AfxMessageBox("���õ� ���� ��ȣ�� �����ϴ�."); return;
	}
	else
	{
		while(posR)
		{
			int nItemR = m_CtrlListRespSignals.GetNextSelectedItem(posR);
			// ��ȣ �Ǻ�
			if ( m_CtrlListRespSignals.GetItemText(nItemR,3) == "+")
			{
				// ��ȣ ����
				m_CtrlListRespSignals.SetItemText(nItemR,3,"-");
				// ��ȣ�� ���� Plus Minus����
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

	// �ӽ����� ����
	double t_dbDesiredTime=(m_sDesiredSignalFileInformation.LENGTH[0]-1)
					*m_sDesiredSignalFileInformation.DELTA[0];

	// �ð� ���� �Ǵ�
	if ( m_dbDesiredFrom == m_dbDesiredTo || m_dbDesiredFrom> m_dbDesiredTo)
	{AfxMessageBox("�ð��� �߸� �����Ͽ����ϴ�."); return;}
	else if ( m_dbDesiredTo > t_dbDesiredTime )
	{AfxMessageBox("�ð��� �߸� �����Ͽ����ϴ�.");return;}
	else if ( m_dbDesiredTo < double(System.SS.SPECTRAL.m_nBlocksize*5)
				         	   /double(System.SS.FRONTEND.m_nSamplingFreq) )  
	{AfxMessageBox("������ ��ǥ �ð��� �����ϴ�.");  return;}
	
	// ��Ī ���� �Ǵ�.
	// ���� ä���� �Ϸ�Ǿ��°� ����ä�� ����.
	for (int i=0 ; i < System.SS.FRONTEND.m_nResponse_Channels ; i++)
	{
		if ( m_CtrlListRespSignals.GetItemText(i,2)=="")
		{
			AfxMessageBox("���� ä���� ��� ��Ī���� �ʾҽ��ϴ�.");
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
