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
	// �˰���
	SYSTEMSETTING			    SS;
	SYSTEMIDENTIFICATION	SI;
	TARGETSIMULATIONEX		TS;
	CDataImportExport       DataImportExport;

public:
	// ���� ���
	string m_strPathName;
	string m_strProjectName;
	string m_strProjectExt;
	string m_strTempFileName;
	string m_strTempFileExt;

public:
	// �˰��� �� ����
	bool   m_bIsPjModified;
public:
	// �ӽ� ���� ��ü
	MatDoub m_DesiredSignals;

public:
	// ������Ʈ ���� �� ���� 
	void NewProject(string PathName, string FileName, string FileExt);
	void OpenProject(string PathName, string FileName, string FileExt);
	void SaveProject(void);
	void SaveAsProject(string PathName, string FileName, string FileExt);
	void CloseProject(void);

public:
	// TS ���ͷ��̼� �߰� ��� ����
	void SaveTemIterationResult(void);
};
