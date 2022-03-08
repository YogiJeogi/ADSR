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
	// ���� ����
	// ������Ʈ�� �ҷ������� �ҷ��� ����Ÿ���� ������ ����
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
	 *	�ϵ���� �޺� �ڽ��� �ϵ���� ä���� �־��ش�.
	 *	ä���� theApp.m_strNIInChannel�� �̹� �־��� �ִ�.
	 * =========================================================*/

	// �޺� �ڽ��� ����Ʈ �ڽ� ��Ʈ�� ���� ����
	CComboBox* tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1);
	CEdit* tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Exc_Name_1);

	// �޺� �ڽ��� ä���� �ִ´�.
	LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIExciterChannel);
	
	if( tcbHW->GetCount() > 0 ) tcbHW->SetCurSel(0);
	else {	tcbHW->EnableWindow(FALSE);	tedHW->EnableWindow(FALSE);	}

	for( int i=1; i < MAX_INC ; i++ )
	{
		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1+i);
		tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Exc_Name_1+i);

		// ������ ���� ä�� ������ ���ٸ� ��Ȱ��ȭ 
		if ( i+1 > System.SS.FRONTEND.m_nInput_Channels ) 
		{ tcbHW->EnableWindow(false); tedHW->EnableWindow(false); }

		LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIExciterChannel);
		if( tcbHW->GetCount() > i ) tcbHW->SetCurSel(i);
		else
		{ tcbHW->EnableWindow(false); tedHW->EnableWindow(false); }
	}

	/* =========================================================
	 *	ä�� �̸��� �ϵ���� �̸� �����̴�.
	 *	������ ����Ȱ��� ������ �ҷ��´�.
	 * =========================================================*/
	// ä�� ���� ����
	// �ʱ�ȭ�� �̹� �Ǿ��ٸ� �о�帰��.
	CString tstr;
	if ( m_bIsExcDigInitialized==TRUE )
	{
		// ����Ʈ ä�� ���� �ڽ��� �迭���� �ҷ���
		CString tstr;
		for ( int i=0 ; i < MAX_INC ; i++ )
		{
			SetDlgItemText(IDC_EDIT_Exc_Name_1+i,
					System.SS.FRONTEND.m_arrstrInputChannelName[i].c_str());

			 tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1+i);
			 tcbHW->SetCurSel(System.SS.FRONTEND.m_arrindexInputChannelHW[i]);
			
		}
	}
	// �ʱ�ȭ�� �ȵǾ��ִٸ� �⺻ ���� ��Ʈ���� ����Ѵ�.
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
	// HW�� ��ȣ �����ϰ� �����Ǿ� �ִ��� �Ǵ�
	// Public �ɹ��� �����Ѵ�.
	UpdateData(TRUE);

	CString tstr;
	// �޺� �ڽ� ��Ʈ�ѷ�
	CComboBox* tcbHW;


	// ����Ʈ ä�� ���� �ڽ��� �迭�� ����
	for ( int i=0 ; i < MAX_INC ; i++ )
	{
		// ����Ʈ���� �ϳ��� �����´�.
		GetDlgItemText(IDC_EDIT_Exc_Name_1+i,tstr);
		// SS�� �����Ѵ�.
		System.SS.FRONTEND.m_arrstrInputChannelName[i]=tstr;

		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Exc_HW_1+i);
		System.SS.FRONTEND.m_arrindexInputChannelHW[i]=tcbHW->GetCurSel();

		// ������ �ϵ��� �ִ��� �˻�
		for ( int j=0 ; j < i ; j++)
		{
			if ( i < System.SS.FRONTEND.m_nInput_Channels 
				      &&  System.SS.FRONTEND.m_arrindexInputChannelHW[i] 
							== System.SS.FRONTEND.m_arrindexInputChannelHW[j] )
			{
				AfxMessageBox("���ÿ� �ΰ��� �ϵ��� �����Ǿ����ϴ�.");
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
		// ���⿡ ����Ű ��� �ۼ�
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

bool CExcDig::m_bIsExcDigInitialized=FALSE;

