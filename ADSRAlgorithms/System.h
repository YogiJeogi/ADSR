#pragma once
#include "SI_class.h"
#include "TS_class.h"
#include "preferST.h"
#include <cstring>
#include "DataImportExport.h"

class CSystem
{
public:
	CSystem(void);
	~CSystem(void);

public: 
	// 알고리즘
	SYSTEMSETTING			    SS;
	SYSTEMIDENTIFICATION	SI;
	TARGETSIMULATIONEX		TS;
	CDataImportExport       DataImportExport;

public:
	// 파일 경로
	string m_strPathName;
	string m_strProjectName;
	string m_strProjectExt;
	string m_strTempFileName;
	string m_strTempFileExt;

public:
	// 알고리즘 현 상태
	bool   m_bIsPjModified;
public:
	// 임시 저장 객체
	MatDoub m_DesiredSignals;

public:
	// 프로젝트 생성 및 저장 
	void NewProject(string PathName, string FileName, string FileExt);
	void OpenProject(string PathName, string FileName, string FileExt);
	void SaveProject(void);
	void SaveAsProject(string PathName, string FileName, string FileExt);
	void CloseProject(void);

public:
	// TS 이터레이션 중간 결과 저장
	void SaveTemIterationResult(void);
};
