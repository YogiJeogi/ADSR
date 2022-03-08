#pragma once
#include "typedef.h"
#include "preferST.h"

class CDataImportExport
{
public:
	CDataImportExport(void);
	~CDataImportExport(void);
public:

	// STATUS 
	string			m_strDesiredSignalPathName;
	string			m_strModelPathName;
	string			m_strDriveSignalPathName;

	_FileSystem		m_sDesiredSignalStatus;
	_FileSystem		m_sModelStatus;
	_FileSystem		m_sDriveSignalStatus;

	// ���� ����Ÿ
	MatDoub			m_matDesiredSignals;
	MatDoub			m_matDriveSignals;
	Mat3DDoub		m_matModel;

	// �����̾� �ñ׳� ó�� �κ�.
	bool LoadDesiredSignals(string PathName,_FileSystem &FileSystem);
	void GetDesiredSignals(unsigned int FirstLine,unsigned int LastLine,MatDoub & DesiredSignal);
	void GetDesiredSignals(double FirstTime,double LastTime,MatDoub & DesiredSignal);

	// �ϴ� ������ ������.
	bool SaveResponseSignals(Mat3DDoub Response );
	bool SaveSIData(MatDoub WhiteNoise, MatDoub Response);
	bool SaveTsResp( Mat3DDoub Response);
	bool SaveTsDrive(MatDoub Drive);
	bool SaveSIResp( MatDoub Response);
	// �� ó�� �κ�.

};
