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
	 *	�ϵ���� �޺� �ڽ��� �ϵ���� ä���� �־��ش�.
	 *	ä���� theApp.m_strvNIResponseChannel�� �̹� �־��� �ִ�.
	 * =========================================================*/

	// �޺� �ڽ��� ����Ʈ �ڽ� ��Ʈ�� ���� ����
	CComboBox* tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Res_HW_1);
	CEdit* tedHW=(CEdit*)GetDlgItem(IDC_EDIT_Res_Name_1);
	CEdit* tedHWScale=(CEdit*)GetDlgItem(IDC_EDIT_Res_Scale_1);
	
	
	// �޺� �ڽ��� ä���� �ִ´�.
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

		// ������ ���� ä�� ������ ���ٸ� ��Ȱ��ȭ 
		if ( i+1 > System.SS.FRONTEND.m_nResponse_Channels ) 
		{ tcbHW->EnableWindow(FALSE); tedHW->EnableWindow(FALSE); tedHWScale->EnableWindow(FALSE);}

		LoadStringVectorInComboBox(*(tcbHW), theApp.m_strvNIResponseChannel);
		if(tcbHW->GetCount()>i) tcbHW->SetCurSel(i);
		else 
		{ tcbHW->EnableWindow(FALSE); tedHW->EnableWindow(FALSE); tedHWScale->EnableWindow(FALSE);}
	}

	/* =========================================================
	 *	ä�� �̸��� �ϵ���� �̸� �����̴�.
	 *	������ ����Ȱ��� ������ �ҷ��´�.
	 * =========================================================*/
	// ä�� ���� ����
	// �ʱ�ȭ�� �̹� �Ǿ��ٸ� �о�帰��.
	CString tstr;
	if ( m_bIsRespDigInitialized==TRUE )
	{
		// ����Ʈ ä�� ���� �ڽ��� �迭���� �ҷ���
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
	// �ʱ�ȭ�� �ȵǾ��ִٸ� �⺻ ���� ��Ʈ���� ����Ѵ�.
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

	// HW�������� ��ȣ �����ϰ� ������ ä���� ������ �Ǵ�
	// Public �ɹ� ������ �����Ѵ�.
	UpdateData(TRUE);

	CString tstr;
	// �޺� �ڽ� ��Ʈ�ѷ�
	CComboBox* tcbHW;

	// ����Ʈ ä�� ���� �ڽ��� �迭�� ����.
	for ( int i=0 ; i < MAX_OUT ; i++ )
	{
		GetDlgItemText(IDC_EDIT_Res_Name_1+i,tstr);
		System.SS.FRONTEND.m_arrstrResponseChannelName[i]=tstr;

		GetDlgItemText(IDC_EDIT_Res_Scale_1+i,tstr);
		System.SS.FRONTEND.m_astrResponseChannelScale[i]=tstr;
		System.SS.FRONTEND.m_adbResponseChannelScale[i]=_tstof(tstr);

		tcbHW=(CComboBox*)GetDlgItem(IDC_COMBO_Res_HW_1+i);
		System.SS.FRONTEND.m_arrindexResponseChannelHW[i]=tcbHW->GetCurSel();

		// ������ �ϵ��� �ִ��� �˻�
		for ( int j=0 ; j < i ; j++)
		{
			if ( i < System.SS.FRONTEND.m_nResponse_Channels 
				&&  System.SS.FRONTEND.m_arrindexResponseChannelHW[i] 
			== System.SS.FRONTEND.m_arrindexResponseChannelHW[j] )
			{
				AfxMessageBox("���ÿ� �ΰ��� �ϵ��� �����Ǿ����ϴ�.");
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
		// ���⿡ ����Ű ��� �ۼ�
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

bool CRespDig::m_bIsRespDigInitialized=FALSE;