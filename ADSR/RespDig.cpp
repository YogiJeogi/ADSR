// RespDig.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "RespDig.h"


// CRespDig dialog

IMPLEMENT_DYNAMIC(CRespDig, CDialog)

CRespDig::CRespDig(CWnd* pParent /*=NULL*/)
	: CDialog(CRespDig::IDD, pParent)
{
}

CRespDig::~CRespDig()
{
}

void CRespDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRespDig, CDialog)
	ON_BN_CLICKED(IDOK, &CRespDig::OnBnClickedOk)
END_MESSAGE_MAP()


// CRespDig message handlers

void CRespDig::LoadStringVectorInComboBox( CComboBox& comboBox, const CNiStringVector& strings )
{
	for (unsigned long i = 0; i < strings.GetSize(); ++i)
		comboBox.AddString(strings[i]);
}



BOOL CRespDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	/* =========================================================
	 *	하드웨어 콤보 박스에 하드웨어 채널을 넣어준다.
	 *	채널은 theApp.m_strvNIResponseChannel에 이미 넣어져 있다.
	 * =========================================================*/

	// 콤보 박스와 에디트 박스 콘트롤 변수 생성
	CComboBox* tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Res_HW_1);
	CEdit* tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Res_Name_1);
	CEdit* tedHWScale=(CEdit*)GetDlgItem(IDC_EDIT_Res_Scale_1);
	
	
	// 콤보 박스에 채널을 넣는다.
	LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIResponseChannel);

	if(tcbHW->GetCount()>0) tcbHW->SetCurSel(0);
	else { tcbHW->EnableWindow(FALSE); 
			 tedHW->EnableWindow(FALSE);	
			 tedHWScale->EnableWindow(FALSE);}

	for( int i=1; i < MAX_OUT ; i++ )
	{
		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Res_HW_1+i);
		tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Res_Name_1+i);
		tedHWScale=(CEdit*)GetDlgItem(IDC_EDIT_Res_Scale_1+i);

		// 설정된 가진 채널 수보다 많다면 비활성화 
		if ( i+1 > System.SS.FRONTEND.m_nResponse_Channels ) 
		{ tcbHW->EnableWindow(FALSE); tedHW->EnableWindow(FALSE); tedHWScale->EnableWindow(FALSE);}

		LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIResponseChannel);
		if(tcbHW->GetCount()>i) tcbHW->SetCurSel(i);
		else 
		{ tcbHW->EnableWindow(FALSE); tedHW->EnableWindow(FALSE); tedHWScale->EnableWindow(FALSE);}
	}

	/* =========================================================
	 *	채널 이름과 하드웨어 이름 설정이다.
	 *	기존에 저장된것이 있으면 불러온다.
	 * =========================================================*/
	// 채널 네임 설정
	// 초기화가 이미 되었다면 읽어드린다.
	CString tstr;
	if ( m_bIsRespDigInitialized==TRUE )
	{
		// 에디트 채널 네임 박스를 배열에서 불러옴
		for ( int i=0 ; i < MAX_OUT ; i++ )
		{
			SetDlgItemText(IDC_EDIT_Res_Name_1+i,
					System.SS.FRONTEND.m_arrstrResponseChannelName[i].data());
			SetDlgItemText(IDC_EDIT_Res_Scale_1+i,
					System.SS.FRONTEND.m_astrResponseChannelScale[i].data());

			 tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Res_HW_1+i);
			 tcbHW->SetCurSel(System.SS.FRONTEND.m_arrindexResponseChannelHW[i]);
			
		}
	}
	// 초기화가 안되어있다면 기본 포멧 스트링을 출력한다.
	else
	{
		for ( int i=0 ; i < MAX_OUT ; i++ )
		{
			tstr.Format("Response #%d",i+1);
			SetDlgItemText(IDC_EDIT_Res_Name_1+i,tstr);
			tstr.Format("1");
			SetDlgItemText(IDC_EDIT_Res_Scale_1+i,tstr);
		}	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRespDig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	// HW설정에서 상호 동일하게 설정된 채널이 없는지 판단
	// Public 맴버 변수에 저장한다.
	UpdateData(TRUE);

	CString tstr;
	// 콤보 박스 컨트롤러
	CComboBox* tcbHW;

	// 에디트 채널 네임 박스를 배열에 저장.
	for ( int i=0 ; i < MAX_OUT ; i++ )
	{
		GetDlgItemText(IDC_EDIT_Res_Name_1+i,tstr);
		System.SS.FRONTEND.m_arrstrResponseChannelName[i]=tstr;

		GetDlgItemText(IDC_EDIT_Res_Scale_1+i,tstr);
		System.SS.FRONTEND.m_astrResponseChannelScale[i]=tstr;
		System.SS.FRONTEND.m_adbResponseChannelScale[i]=_tstof(tstr);

		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Res_HW_1+i);
		System.SS.FRONTEND.m_arrindexResponseChannelHW[i]=tcbHW->GetCurSel();

		// 동일한 하드웨어가 있는지 검사
		for ( int j=0 ; j < i ; j++)
		{
			if ( i < System.SS.FRONTEND.m_nResponse_Channels 
				&&  System.SS.FRONTEND.m_arrindexResponseChannelHW[i] 
			== System.SS.FRONTEND.m_arrindexResponseChannelHW[j] )
			{
				AfxMessageBox("동시에 두개의 하드웨어가 설정되었습니다.");
				tcbHW->SetFocus();
				return;
			}
		}
	}

	m_bIsRespDigInitialized=TRUE;

	OnOK();
}

BOOL CRespDig::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		// 여기에 엔터키 기능 작성
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

bool CRespDig::m_bIsRespDigInitialized=FALSE;