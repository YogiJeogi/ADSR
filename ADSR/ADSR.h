
// ADSR.h : ADSR ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "../ADSRAlgorithms/BaseHD.h"
#include "../ADSRAlgorithms/\SI_class.h"
#include "ResultVDig.h"
#include "StartDAQ.h"
#include <NiScalarVector.h>
#include "../ADSRAlgorithms/System.h"


/*=========================================
 * ����ü ����
 *  �� ����ü�� �� ��ȭ���ڰ�� ����Ÿ�� 
 *  ���������ν� ��� â�� �����ϱ� ����
 *  ����� ���� ����ü�̴�.
 *  ======================================*/
struct sResultViewUserData
{
	CArray<int,int> m_anSelectedCh1;
	CArray<int,int> m_anSelectedCh2;
	unsigned int	m_nNSelectedCh1;
	unsigned int	m_nNSelectedCh2;
};
struct sRealViewUserData
{

};
struct sRealResultViewUserData
{
	sResultViewUserData RTV;
	sRealViewUserData RV;
};
/* ============================================
 *	���� �ɹ� SS�� ����Ǿ� �ִ�.
 *	
 *	��� ��ȭ���ڿ� ���� ������ �Ǿ��ִ�.
 *	
 *	��� �޴� �ڸǵ忡 ���� ������ �Ǿ��ִ�.
 *
 * ==========================================*/

// ���� �ɹ� ����
extern CSystem					System;
extern CCriticalSection			g_CriticalSection;
extern sRealResultViewUserData  g_sVUserData;


// CADSRApp:
// �� Ŭ������ ������ ���ؼ��� ADSR.cpp�� �����Ͻʽÿ�.
//
 struct ProgressState 
 {
	 bool bSystemSet;
	 bool bExcChSet;
	 bool bRespChSet;
	 bool bWNGen;
	 bool bSIComplete;
	 bool bTargetImport;
	 bool bFirstDrive;
	 bool bUpDrive;
	 bool bTSComplete;
 } ;
 extern ProgressState ProgStates;

class CADSRApp : public CWinAppEx
{
public:
	CADSRApp();

private:


	// �ð�����Ÿ�� �������� �г��̴�.
	CResultVDig			* pResultVDlg;

	// SI�� ���� ����Ÿ ȹ�� �� ���� ������ ���� �г��̴�.
	CStartDAQDlg		* pStartDlg;

// �������Դϴ�.
public:
	virtual BOOL	InitInstance();

// �����Դϴ�.
/////////////////////////////////////////////////////

	MatDoub			m_SIWhiteNoiseTimeData;
	CNiReal64Matrix m_SIWhiteNoiseTimeDataNI;

	MatDoub			m_TSDriveTimeData;
	CNiReal64Matrix m_TSDriveTimeDataNI;

	void			Modalessdone();
	BOOL			m_bHiColorIcons;
	BOOL			m_bIsNewProject;
	BOOL			m_bIsSystemsetInitialized;

	int				m_numExciterChannels;
	int				m_numResponseChannels;
	CNiStringVector m_strvNIResponseChannel;
	CNiStringVector m_strvNIExciterChannel;

/////////////////////////////////////////////////////

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnMenuSysSysset();
	afx_msg void OnMenuSysExc();
	afx_msg void OnMenuSysResp();
	afx_msg void OnMenuSiWn();
	afx_msg void OnMenuSiSiv();
	afx_msg void OnUpdateMenuSysExc(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSysResp(CCmdUI *pCmdUI);
	afx_msg void OnMenuSiWnv();
	afx_msg void OnMenuSiSi();
	afx_msg void OnMenuGraphCreat();
	afx_msg void OnMenuTsDsImport();
	afx_msg void OnMenuTsDrvgen();
	afx_msg void OnMenuTsDrvup();
	afx_msg void OnMenuTsDrivesys();
	afx_msg void OnResultSignals();
	afx_msg void OnResultHmodel();
	afx_msg void OnResultSignalcomparison();
	afx_msg void OnMenuSidata();
	afx_msg void OnTsResp();
	afx_msg void OnMenuTsdrive();
	afx_msg void OnUpdateMenuSiWnv(CCmdUI *pCmdUI);
	afx_msg void OnMenuSiresp();
	afx_msg void OnUpdateMenuSiWn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSiSi(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDsImport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDrvgen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDrvup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDrivesys(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuGraphWin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateResultHmodel(CCmdUI *pCmdUI);
};

extern CADSRApp theApp;
