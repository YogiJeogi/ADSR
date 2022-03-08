// ExcDig.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "ExcDig.h"


// CExcDig dialog

IMPLEMENT_DYNAMIC(CExcDig, CDialog)

CExcDig::CExcDig(CWnd* pParent /*=NULL*/)
	: CDialog(CExcDig::IDD, pParent)
{
	// 구현 예정
	// 프로젝트를 불러왔으면 불러온 데이타에서 변수값 대입
	if (theApp.m_bIsNewProject != TRUE)
	{
	}
}

CExcDig::~CExcDig()
{
}

void CExcDig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExcDig, CDialog)
	ON_BN_CLICKED(IDOK, &CExcDig::OnBnClickedOk)
END_MESSAGE_MAP()

void CExcDig::LoadStringVectorInComboBox( CComboBox& comboBox, const CNiStringVector& strings)
{
	for (unsigned long i = 0; i < strings.GetSize(); ++i)
		comboBox.AddString(strings[i]);
}

// CExcDig message handlers

BOOL CExcDig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	/* =========================================================
	 *	하드웨어 콤보 박스에 하드웨어 채널을 넣어준다.
	 *	채널은 theApp.m_strNIInChannel에 이미 넣어져 있다.
	 * =========================================================*/

	// 콤보 박스와 에디트 박스 콘트롤 변수 생성
	CComboBox* tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1);
	CEdit* tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Exc_Name_1);

	// 콤보 박스에 채널을 넣는다.
	LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIExciterChannel);
	
	if( tcbHW->GetCount() > 0 ) tcbHW->SetCurSel(0);
	else {	tcbHW->EnableWindow(FALSE);	tedHW->EnableWindow(FALSE);	}

	for( int i=1; i < MAX_INC ; i++ )
	{
		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1+i);
		tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Exc_Name_1+i);

		// 설정된 가진 채널 수보다 많다면 비활성화 
		if ( i+1 > System.SS.FRONTEND.m_nInput_Channels ) 
		{ tcbHW->EnableWindow(false); tedHW->EnableWindow(false); }

		LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIExciterChannel);
		if( tcbHW->GetCount() > i ) tcbHW->SetCurSel(i);
		else
		{ tcbHW->EnableWindow(false); tedHW->EnableWindow(false); }
	}

	/* =========================================================
	 *	채널 이름과 하드웨어 이름 설정이다.
	 *	기존에 저장된것이 있으면 불러온다.
	 * =========================================================*/
	// 채널 네임 설정
	// 초기화가 이미 되었다면 읽어드린다.
	CString tstr;
	if ( m_bIsExcDigInitialized==TRUE )
	{
		// 에디트 채널 네임 박스를 배열에서 불러옴
		CString tstr;
		for ( int i=0 ; i < MAX_INC ; i++ )
		{
			SetDlgItemText(IDC_EDIT_Exc_Name_1+i,
					System.SS.FRONTEND.m_arrstrInputChannelName[i].c_str());

			 tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1+i);
			 tcbHW->SetCurSel(System.SS.FRONTEND.m_arrindexInputChannelHW[i]);
			
		}
	}
	// 초기화가 안되어있다면 기본 포멧 스트링을 출력한다.
	else
	{
		for ( int i=0 ; i < MAX_INC ; i++ )
		{
			tstr.Format("EXCITER #%d",i+1);
			SetDlgItemText(IDC_EDIT_Exc_Name_1+i,tstr);
		}	
	}


	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CExcDig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// HW간 상호 동일하게 설정되어 있는지 판단
	// Public 맴버에 저장한다.
	UpdateData(TRUE);

	CString tstr;
	// 콤보 박스 컨트롤러
	CComboBox* tcbHW;


	// 에디트 채널 네임 박스를 배열에 저장
	for ( int i=0 ; i < MAX_INC ; i++ )
	{
		// 에디트에서 하나씩 가져온다.
		GetDlgItemText(IDC_EDIT_Exc_Name_1+i,tstr);
		// SS에 저장한다.
		System.SS.FRONTEND.m_arrstrInputChannelName[i]=tstr;

		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1+i);
		System.SS.FRONTEND.m_arrindexInputChannelHW[i]=tcbHW->GetCurSel();

		// 동일한 하드웨어가 있는지 검사
		for ( int j=0 ; j < i ; j++)
		{
			if ( i < System.SS.FRONTEND.m_nInput_Channels 
				      &&  System.SS.FRONTEND.m_arrindexInputChannelHW[i] 
							== System.SS.FRONTEND.m_arrindexInputChannelHW[j] )
			{
				AfxMessageBox("동시에 두개의 하드웨어가 설정되었습니다.");
				tcbHW->SetFocus();
				return;/////////////
			}
		}
	}
	
	m_bIsExcDigInitialized=TRUE;

	OnOK();
}

BOOL CExcDig::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN){
		// 여기에 엔터키 기능 작성
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

bool CExcDig::m_bIsExcDigInitialized=FALSE;

