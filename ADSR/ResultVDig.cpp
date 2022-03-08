// TimeVDig.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "ResultVDig.h"
#include "WhiteDig.h"


// ����� ���� �޼���
#define WM_HD_RTV  WM_USER+2


// ���� �ɹ� �ʱ�ȭ
BOOL CResultVDig::m_bIsDesiredSignalTimeView=FALSE;
BOOL CResultVDig::m_bIsWhiteNoiseTimeView=FALSE;

// CResultVDig dialog

IMPLEMENT_DYNAMIC(CResultVDig, CDialog)

CResultVDig::CResultVDig(CWnd* pParent /*=NULL*/)
	: CDialog(CResultVDig::IDD, pParent)
{
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CResultVDig::IDD;
}

CResultVDig::CResultVDig( int Type,CWnd* pParent /*= NULL*/ )
	: CDialog(CResultVDig::IDD, pParent)

{
	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CResultVDig::IDD;
	TYPE=Type;
}

CResultVDig::~CResultVDig()
{
}

void CResultVDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RV_SS, m_CtrlStaticSS);
	DDX_Control(pDX, IDC_STATIC_RV_VM, m_CtrlStaticVM);
	DDX_Control(pDX, IDC_LIST_Channel, m_CtrlListCh);
	DDX_Control(pDX, IDC_BUTTON_WIN_HOR, m_CtrlButtonHor);
	DDX_Control(pDX, IDC_BUTTON_WIN_CAS, m_CtrlButtonCas);
	DDX_Control(pDX, IDC_BUTTON_WIN_ALLCLOSE, m_CtrlButtonAllClose);
	DDX_Control(pDX, IDC_CHECK_TIME, m_CtrlCheckTime);
	DDX_Control(pDX, IDC_CHECK_PSD, m_CtrlCheckPSD);
	DDX_Control(pDX, IDC_CHECK_XY, m_CtrlCheckXY);
	DDX_Control(pDX, IDC_CHECK_STATIS, m_CtrlCheckStatistics);
	DDX_Control(pDX, IDOK, m_CtrlButtonView);
	DDX_Control(pDX, IDC_LIST_Channel2, m_CtrlListCh2);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_CtrlButtonClose);
	DDX_Control(pDX, IDC_STATIC_RV_H, m_CtrlStaticH);
	DDX_Control(pDX, IDC_STATIC_RV_SSCOM, m_CtrlStaticSSComp);
}


BEGIN_MESSAGE_MAP(CResultVDig, CDialog)
/*	ON_BN_CLICKED(IDC_BUTTON_WN_TimeV, &CResultVDig::OnBnClickedButtonWnTimev)*/
	ON_BN_CLICKED(IDOK, &CResultVDig::OnBnClickedView)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CResultVDig::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_RADIO_SS_WhiteNoise, &CResultVDig::OnBnClickedRadio0)
	ON_BN_CLICKED(IDC_RADIO12, &CResultVDig::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO13, &CResultVDig::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO14, &CResultVDig::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO15, &CResultVDig::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON_WIN_HOR, &CResultVDig::OnBnClickedButtonWinHor)
	ON_BN_CLICKED(IDC_BUTTON_WIN_CAS, &CResultVDig::OnBnClickedButtonWinCas)
	ON_BN_CLICKED(IDC_BUTTON_WIN_ALLCLOSE, &CResultVDig::OnBnClickedButtonWinAllclose)
END_MESSAGE_MAP()


// CResultVDig message handlers

void CResultVDig::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CResultVDig::Create()
{
	return CDialog::Create(m_nID, m_pParent);
}

void CResultVDig::OnBnClickedView()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// �����
	
	short nSelectedSignal;
	int nCheckedRadio=GetCheckedRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5)
											-IDC_RADIO_SS_WhiteNoise;
//  CString tt;
// 	tt.Format("%d",nCheckedRadio);
// 	AfxMessageBox(tt);
	int nCheckedViewMode=0;

	// �ñ׳� �Ǻ�
	if		(TYPE==1) // Signal
	{
		nSelectedSignal=static_cast<short>(nCheckedRadio+100);
	}
	else if (TYPE==2) // H Model
	{
		nSelectedSignal=static_cast<short>(nCheckedRadio+200);
	}
	else if (TYPE==3) // Signal Comparison
	{
		nSelectedSignal=static_cast<short>(nCheckedRadio+300);
	}

	// üũ��ư �Ǻ�
	if(m_CtrlCheckTime.GetCheck()==BST_CHECKED) 
												nCheckedViewMode=nCheckedViewMode|1;
	if(m_CtrlCheckPSD.GetCheck()==BST_CHECKED)	
												nCheckedViewMode=nCheckedViewMode|2;
	if(m_CtrlCheckXY.GetCheck()==BST_CHECKED)	
												nCheckedViewMode=nCheckedViewMode|4;
	if(m_CtrlCheckStatistics.GetCheck()==BST_CHECKED) 
												nCheckedViewMode=nCheckedViewMode|8;
	
	// ����Ʈ �Ǻ�
	POSITION pos2 = m_CtrlListCh2.GetFirstSelectedItemPosition();
	POSITION pos1 = m_CtrlListCh.GetFirstSelectedItemPosition();
	if (pos1 == NULL)
	{
		AfxMessageBox("���õ� ��ȣ�� �����ϴ�."); return;
	}
	if ( TYPE==2 && pos2==NULL )
	{
		AfxMessageBox("���õ� ��ȣ�� �����ϴ�."); return;
	}
	else
	{
		unsigned int nCount=m_CtrlListCh.GetSelectedCount();
		g_sVUserData.RTV.m_nNSelectedCh1=nCount;
		g_sVUserData.RTV.m_anSelectedCh1.SetSize(nCount);
		for (unsigned int i=0; i < m_CtrlListCh.GetSelectedCount() ; i++ )
		{
			g_sVUserData.RTV.m_anSelectedCh1[i]=m_CtrlListCh.GetNextSelectedItem(pos1);
		}
		nCount=m_CtrlListCh2.GetSelectedCount();
		g_sVUserData.RTV.m_nNSelectedCh2=nCount;
		g_sVUserData.RTV.m_anSelectedCh2.SetSize(nCount);
		g_sVUserData.RTV.m_anSelectedCh2.SetSize(m_CtrlListCh2.GetSelectedCount());	
		for (unsigned int i=0; i < m_CtrlListCh2.GetSelectedCount() ; i++ )
		{
			g_sVUserData.RTV.m_anSelectedCh2[i]=m_CtrlListCh2.GetNextSelectedItem(pos2);
		}
	}
	// 		CString tt;
	// 		tt.Format("%d",g_sVUserData.RTV.m_anSelectedCh1[0]);
	// 		AfxMessageBox(tt);
	


	// �޼��� ����
	// WPARAM LOWORD : ���õǴ� ��ȣ Ÿ���� ��Ÿ����.
	//				 : 1 -> ���� ��ȣ
	//				 : 2 -> ���� �Լ�
	//				 : 3 -> ��ȣ ��
	// WPARAM HIWORD : ���õ� ��ȣ�� ��Ÿ����.
	//				 : 100���� 
	//				 : 200����
	//				 : 300����
	// LPARAM LOWORD : CheckBox���� ���õ� ���� Bit����Ǿ��ִ�.
	// LPARAM HIWORD : 0 -> â �ݱ�
	//				 : 1 -> â �����ϱ�
	//				 : 2 -> ����Ʈ ���� �����ϰ� ��ȣ�� ���õ� ���â ����
	//				 : 3 -> üũ ��ư ��ȭ�� ���� â������Ʈ �Ҷ�
	AfxGetMainWnd()->SendMessage(WM_HD_RTV
								,MAKEWPARAM(TYPE,nSelectedSignal)
								,MAKELPARAM(nCheckedViewMode,1));

	//OnOK();
}


void CResultVDig::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
//	CDialog::OnCancel();
}

BOOL CResultVDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int nPriResp=System.SS.FRONTEND.m_nResponse_Channels;
	int nRespC=nPriResp+System.SS.FRONTEND.m_nSlave_Channels;
	int nExc=System.SS.FRONTEND.m_nInput_Channels;
	char N[50];
	CWnd * pRadiobutton;

	/*============================
	*	��ǥ 
	/*============================*/
	// ��ǥ ���ϱ�
	CRect rect,MWndRect;
	CRect CtrlStaticSSRect,CtrlStaticHRect,CtrlListRect,CtrlList2Rect;
	CRect CtrlStaticSSCompRect,CtrlStaticVMRect;
	int x,dx1,dx2,dx;
	int y,dy;
	int cx;
	int cy;
	// ��ȭ���� ��ǥ
	GetClientRect(&rect);
	// ���������� ��ǥ
	AfxGetMainWnd()->GetClientRect(&MWndRect);
	// ���ϵ� ������ ��ǥ
	m_CtrlStaticSS.GetClientRect(&CtrlStaticSSRect);
	m_CtrlStaticH.GetClientRect(&CtrlStaticHRect);
	m_CtrlListCh.GetClientRect(&CtrlListRect);
	m_CtrlListCh2.GetClientRect(&CtrlList2Rect);
	m_CtrlStaticSSComp.GetClientRect(&CtrlStaticSSCompRect);
	m_CtrlStaticVM.GetClientRect(&CtrlStaticVMRect);

	/*============================
	*	����Ʈ
	/*============================*/
	// ����Ʈ ����
	m_CtrlListCh.InsertColumn(0,"No",0);
	m_CtrlListCh.InsertColumn(1,"Excitation",0);
	m_CtrlListCh2.InsertColumn(0,"No",0);
	m_CtrlListCh2.InsertColumn(1,"Response",0);
		// ��Ÿ�� ����
	m_CtrlListCh.ModifyStyle(LVS_TYPEMASK|LVS_SINGLESEL,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListCh2.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|LVS_SHOWSELALWAYS );
	m_CtrlListCh.SetExtendedStyle(m_CtrlListCh.GetExtendedStyle()|LVS_EX_FULLROWSELECT
							|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);
	m_CtrlListCh2.SetExtendedStyle(m_CtrlListCh2.GetExtendedStyle()|LVS_EX_FULLROWSELECT
							|LVS_EX_GRIDLINES|LVS_EX_LABELTIP|LVS_EX_TRACKSELECT);	

	/*============================
	*	üũ Ÿ�� �ʱ�ġ üũ��
	/*============================*/
	m_CtrlCheckTime.SetCheck(BST_CHECKED);

	// Signals
	if (TYPE==1)
	{
		// ����Ʈ
		// ����Ʈ �ݷ� �̸� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// ä�� �̸� ����
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
		// ����Ʈ Į�� ����.
		m_CtrlListCh.SetColumnWidth(0,CtrlStaticSSRect.Width()*25/100);
		m_CtrlListCh.SetColumnWidth(1,CtrlStaticSSRect.Width()*72/100);


		// ���̾�α� â ũ�� �� ��ġ ����
		cx=CtrlStaticSSRect.right-CtrlStaticSSRect.left+CtrlStaticSSRect.left*2+35;
		cy=rect.Height()+35;
		SetWindowPos(&wndTop,MWndRect.Width()-(cx+50),150,cx,cy,0);

		//���� ��ư üũ 0��
		CheckRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5,IDC_RADIO_SS_WhiteNoise);

		////////////////////////////////////////////////////////////////////////
		if ( ProgStates.bWNGen!=true) {pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise); pRadiobutton->EnableWindow(false); m_CtrlButtonView.EnableWindow(FALSE);}
		if ( ProgStates.bFirstDrive!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+1); pRadiobutton->EnableWindow(false);}
		if ( ProgStates.bSIComplete!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+2); pRadiobutton->EnableWindow(false);}
		if ( ProgStates.bTSComplete!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+3); pRadiobutton->EnableWindow(false);}
		if ( ProgStates.bTargetImport!=true){pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+4); pRadiobutton->EnableWindow(false);}
		////////////////////////////////////////////////////////////////////////
	}
	// H Model
	else if (TYPE==2)
	{
		// ����Ʈ
		// ����Ʈ �ݷ� �̸� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		m_CtrlListCh2.GetColumn(1,&col);
		col.pszText="Response";
		m_CtrlListCh2.SetColumn(1,&col);
		// ä�� �̸� ����
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
		for (int i=0; i<nPriResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh2.InsertItem(i,N);
			m_CtrlListCh2.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
		// ����Ʈ Į�� ����.
		m_CtrlListCh.SetColumnWidth(0,CtrlStaticHRect.Width()*18/100);
		m_CtrlListCh.SetColumnWidth(1,CtrlStaticHRect.Width()*80/100);
		m_CtrlListCh2.SetColumnWidth(0,CtrlStaticHRect.Width()*18/100);
		m_CtrlListCh2.SetColumnWidth(1,CtrlStaticHRect.Width()*80/100);

		m_CtrlStaticVM.ShowWindow(FALSE);
		m_CtrlCheckStatistics.ShowWindow(FALSE);
		m_CtrlCheckTime.ShowWindow(FALSE);
		m_CtrlCheckXY.ShowWindow(FALSE);
		m_CtrlCheckPSD.ShowWindow(FALSE);
		m_CtrlStaticSS.ShowWindow(FALSE);

		// ���̾�α� â ũ�� �� ��ġ ����
		cx=CtrlStaticHRect.right+35;
		cy=rect.Height()+35;
		SetWindowPos(&wndTop,MWndRect.Width()-(cx+50),150,cx,cy,0);

		
		x=15;
		dx=15;
		y=15;
		dy=30;

		// ���� ������ ����.
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise);
		pRadiobutton->SetWindowText("System Model H");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+1);
		pRadiobutton->SetWindowText("Respective Coherence");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+2*dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+2);
		pRadiobutton->SetWindowText("Partial Coherence");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+3*dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+3);
		pRadiobutton->SetWindowText("Multiple Coherence");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+4*dy,CtrlStaticHRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+4);
		pRadiobutton->ShowWindow(FALSE);
		pRadiobutton->EnableWindow(FALSE);

		// ����ƽ ������ ��ġ ����.
		m_CtrlStaticH.SetWindowPos(&wndTop,x,y,0,0,SWP_NOSIZE);

		//���� ��ư üũ 0��
		CheckRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5,IDC_RADIO_SS_WhiteNoise);

		// ����Ʈ ������ ��ġ ����.
		m_CtrlListCh.SetWindowPos(&wndTop,15,15+CtrlStaticHRect.Height()+15
								,CtrlStaticHRect.right,CtrlListRect.Height(),0);
		m_CtrlListCh2.SetWindowPos(&wndTop,15,15+CtrlStaticHRect.Height()+15+CtrlListRect.Height()+15
								,CtrlStaticHRect.right,CtrlList2Rect.Height(),0);
		if ( ProgStates.bSIComplete!=true)	m_CtrlButtonView.EnableWindow(FALSE);
	}
	// Signal Comparison

	//////////////////////////////////////////////
	///////////        3                     ///////
	else if (TYPE==3)
	{
		// ����Ʈ
		// �÷� �̸� ����
		m_CtrlCheckXY.EnableWindow(true);
		m_CtrlCheckPSD.EnableWindow(TRUE);

		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// ä�� �̸� ����
		for (int i=0; i<nRespC ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
		// ����Ʈ Į�� ����.
		m_CtrlListCh.SetColumnWidth(0,CtrlStaticSSCompRect.Width()*18/100);
		m_CtrlListCh.SetColumnWidth(1,CtrlStaticSSCompRect.Width()*80/100);


		// ���� ������ �ʿ���� â �����
		m_CtrlStaticSS.ShowWindow(FALSE);

		// ���̾�α� â ũ�� �� ��ġ ����
		int cx=CtrlStaticSSCompRect.right+35;
		int cy=rect.Height()+35;
		SetWindowPos(&wndTop,MWndRect.Width()-(cx+50),150,cx,cy,0);

		// ����ƽ ������ ��ġ ����.
		x=15;
		dx=15;
		y=15;
		dy=30;
		m_CtrlStaticSSComp.SetWindowPos(&wndTop,x,y,0,0,SWP_NOSIZE);
		// ���� ������ ��ġ ����.
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise);
		pRadiobutton->SetWindowText("Unfiltered and Filtered Desired Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+1);
		pRadiobutton->SetWindowText("Unfiltered and Filtered Response Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+2*dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+2);
		pRadiobutton->SetWindowText("Unfiltered Desired and Response Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+3*dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+3);
		pRadiobutton->SetWindowText("Filtered Desired and Response Signals");	
		pRadiobutton->SetWindowPos(&wndTop,x+dx,y+4*dy,CtrlStaticSSCompRect.right-dx-x,15,SWP_NOOWNERZORDER);
		pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+4);
		pRadiobutton->ShowWindow(FALSE);
		pRadiobutton->EnableWindow(FALSE);

		if (ProgStates.bTSComplete!=true)
		{
			for (int i=0; i<4; i++)
			{
				pRadiobutton=GetDlgItem(IDC_RADIO_SS_WhiteNoise+i);
				pRadiobutton->EnableWindow(FALSE);
			}
		}

		//���� ��ư üũ 0��
		CheckRadioButton(IDC_RADIO_SS_WhiteNoise,IDC_RADIO_SS_WhiteNoise+5,IDC_RADIO_SS_WhiteNoise);

		// ����ƽ ������ ��ġ ����.
		x=15;
		dx1=15;
		dx2=CtrlStaticSSCompRect.right/2+20;
		y=15+CtrlStaticSSCompRect.Height()+15;
		dy=15;
		cx=CtrlStaticSSCompRect.right;
		cy=CtrlStaticVMRect.Height()*2/3;
		m_CtrlStaticVM.SetWindowPos(&wndTop,x,y,cx,cy,0);
		// üũ ��ư ��ġ ����.
		m_CtrlCheckTime.SetWindowPos(&wndTop,x+dx1,y+5+dy,0,0,SWP_NOSIZE);
		m_CtrlCheckPSD.SetWindowPos(&wndTop,x+dx1,y+5+2*dy,0,0,SWP_NOSIZE);
		m_CtrlCheckXY.SetWindowPos(&wndTop,x+dx2,y+5+dy,0,0,SWP_NOSIZE);
		m_CtrlCheckStatistics.SetWindowPos(&wndTop,15+dx2,y+5+2*dy,0,0,SWP_NOSIZE);

		// ��Ʈ�� ����Ʈ ��ġ ����
		x=15;
		y=15+CtrlStaticSSCompRect.Height()+15+CtrlStaticVMRect.Height()*2/3+15;
		cx=CtrlStaticSSCompRect.right;
		cy=CtrlListRect.Height();
		m_CtrlListCh.SetWindowPos(&wndTop,x,y,cx,cy,0);

	    if ( ProgStates.bTSComplete!=true)	m_CtrlButtonView.EnableWindow(FALSE);
	}
	else
	{
		return false;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CResultVDig::OnBnClickedButtonClose()
{
	// TODO: Add your control notification handler code here
	theApp.Modalessdone();
	AfxGetMainWnd()->SendMessage(WM_HD_RTV,0,0);
	DestroyWindow();
}


void CResultVDig::OnBnClickedRadio0()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// �ݷ� ä�� �� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);

		// �ݷ� ������ ����
		int nExc=System.SS.FRONTEND.m_nInput_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
	}
}

void CResultVDig::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// �ݷ� ä�� �� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Exc. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		int nExc=System.SS.FRONTEND.m_nInput_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		// ����Ʈ
		// ä�� �̸� ����
		for (int i=0; i<nExc ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrInputChannelName[i].data());
		}
	}

}

void CResultVDig::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// �ݷ� ä�� �� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// �ݷ� ������ ����
		int nResp=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i < nResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
	}

}

void CResultVDig::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// �ݷ� ä�� �� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// �ݷ� ������ ����
		int nResp=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i < nResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
	}
}

void CResultVDig::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	if ( TYPE==1)
	{
		// �ݷ� ä�� �� ����
		LVCOLUMN col;
		m_CtrlListCh.GetColumn(1,&col);
		col.pszText="Resp. Channel";
		m_CtrlListCh.SetColumn(1,&col);
		// �ݷ� ������ ����
		int nResp=System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels;
		char N[50];
		m_CtrlListCh.DeleteAllItems();
		for (int i=0; i < nResp ;i++)
		{
			_itoa_s(i+1,N,10);
			m_CtrlListCh.InsertItem(i,N);
			m_CtrlListCh.SetItemText(i,1
				,System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
		}
	}
}

void CResultVDig::OnBnClickedButtonWinHor()
{
	// TODO: Add your control notification handler code here
	CMDIFrameWndEx* pmain=(CMDIFrameWndEx*)AfxGetMainWnd();
	pmain->MDITile();
}

void CResultVDig::OnBnClickedButtonWinCas()
{
	// TODO: Add your control notification handler code here
	CMDIFrameWndEx* pmain=(CMDIFrameWndEx*)AfxGetMainWnd();
	pmain->MDICascade();
}

void CResultVDig::OnBnClickedButtonWinAllclose()
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_HD_RTV
								,MAKEWPARAM(0,0)
								,MAKELPARAM(0,4));
}
