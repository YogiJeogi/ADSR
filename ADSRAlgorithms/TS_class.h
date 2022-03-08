#pragma once
#ifndef __TS_CLASS_H__
#define __TS_CLASS_H__

// Define Header
#include "BaseHD.h"
#include "SI_class.h"


#define PI 3.141592653589793238

// Define Class
class TargetSimulationBase : protected ADSRbase
{
// protected:
// 	int			nNExc;					// ����ä�� ���� ���Ѵ�.
// 	int			nNPriResp;				// ������ä�� ���� ���Ѵ�.
// 	int			nNSlavResp;				// ������ä�� ���� ���Ѵ�.
// 	int			nNTarSignals;			// ��ǥ��ȣ�� �ִ� ä�� ���� ���Ѵ�. ������ + (������ �� ��ǥ��ȣ�� �ִ°�)
// 	int			nNTarSignalPoint;		// ��ǥ��ȣ�� ���μ� ���̴�. �ٸ� BS�� ����� �ƴϴ�.
// 	int			nNRespC;				// ��ü ���� ä���� ���Ѵ�.
// 	int			nLBlockSize;			// ��ϻ����� ���̸� ���Ѵ�.
// 	int			nNBlockSize;			// ��ϻ����� ������ ������
// 	int			nNSegment;				// BS���� BS������ ���̴�. �� BS�� �������� ���̴�. Target��ȣ�� �����е��� �� ��ȣ.	
// 	int			nLowBandPoint;			// ���ļ��� ���ϴ� ����Ʈ��.
// 	int			nUpperBandPoint;		// ���ļ��� ��Ÿ���� ���ε�.
// 	int			nPointShift;
// 	int			nSamplingFreq;
// 
protected:
	bool*		pWhetherSubTargetExist; // ������ä�ο� ��ǥ��ȣ�� ��Ī�Ǿ������� ���� BOOL
		 
private:
	// ���� ����
	int m_nkdelay;
	int m_nFIRQtap;
	int m_nFIRMpoint;
	int m_nINVHQtap;
	int m_nINVHMpoint;

	VecDoub		 m_vecFilter;
	VecDoub      m_vecFilterFre;

	SpectOlap2D* m_pInstUnfiltRespPSD;
	SpectOlap2D* m_pInstFiltRespPSD;

	SpectOlap2D* m_pInstFiltDesiredPSD;
	SpectOlap2D* m_pInstUnfiltDesiredPSD;

protected: 

	MatDoub     m_matDriveSignals;			// [ Input  X (NumberofBS*LengthBS) ]
	Mat3DDoub	m_matDesiredSignals;		// [ DesiredOutput X (NumberofBS*LengthBS) ]
	Mat3DDoub   m_matResponseSignals;		// [ Pr+Slave Output X (NumberofBS*LengthBS) ]
	Mat3DDoub	m_matErrorSignals;			// [ DesiredOutput X (NumberofBS*LengthBS) ]

	// Desired Signal
	Mat3DDoub	m_matDesiredPSD;	// 0 Index -> Unfilt , 1 Index -> Filt
	
	// Response Signal
	Mat3DDoub	m_matRespPSD;

	VecDoub		m_vecPSDFreq;

	Mat3DDoub	m_matConvInvH;     // [ Input X Output X BS*2 ]
	

public  :

	// FLAG
	// GUI���� RESP�� �����ϴ� �� ������ m_flag_is_takeresposesignals_initialized Set up ������Ѵ�.
	// ù��° ����̹� ������ ���� �� ������Ʈ �Ұ�� m_flag_is_firstTimeTS Set Off
	// GUI���� m_flag_is_responsedata_fullȮ���ؾ��Ѵ�.
	// GUI���� m_flag_PSDReady�� Ȯ���ؾ��Ѵ�.
	// GUI���� m_flag_overMaximumVoltage Ȯ���ؾ��Ѵ�.
	bool         m_fIsTSInitialized;
	bool		 m_fIsFirstDrive;			// ����̹� ������ ó���ΰ�.
	bool         m_fIsDriveSignalsReady;	// ����̹� ��ȣ�� �����Ǿ��°�.

public:
	//METHOD
	TargetSimulationBase ();

	virtual void TSInitialization( const SYSTEMSETTING & SS); // �ʼ��� ���� �δܰ�	ImportInverseModel(matInvH);ImportDesiredSignals(matDesiredSignals);
	virtual void TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals );

	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals );  //�� +PSD
	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals, int nPaddingpoints);
	void ImportSystemsetting(SYSTEMSETTING & SS);		  //��
	void ImportInverseModel(const Mat3DDoub & matInvH);  //��	
	void ImportResponseSignals(MatDoub & matRespSignals );
	void ImportResponseSignals(MatDoub & matRespSignals , int nPaddingpoints);


private:
	void FilterDesign();
	void PadingDesiredSignals(int point);
	void PadingRespSignals(int point);
	//======================//
	// ���� ����Ÿ ó�� �Լ�//
	//======================//
	void ProcessRawDesiredSignals();  //��
	void ProcessRawResponseSignals(); //��
	void ProcessRawErrorSignals();	  //��

public:
	// �� �� / vecExcWeight �غ� �ȵ�.
	virtual void GenerateFirstDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);
	virtual void GenerateUpdatedDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);

	// ����� ��û�� ���� Export
	//  Get Data
	MatDoub GetDriveSignals();		     
	Mat3DDoub GetPSD3DDesiredSignals();      
	Mat3DDoub GetPSD3DResponseSignals();     
	Mat3DDoub GetResponseSignals();
	Mat3DDoub GetDesiredSignals();

	void Debugging();
	~TargetSimulationBase();

};

// �ǽð� ���� ó���� ���� �ڽ� Ŭ����
class TARGETSIMULATION : public TargetSimulationBase
{

private:

	// ����Ÿ�� ����Ÿ ����� Ȯ�� ����Ÿ  
	int				m_nTempLgthSavedRespData;
	int				m_nNBlockPerformedInPSD;
	// ����Ÿ�� ����Ÿ ���� ����Ÿ 
	MatDoub			m_matRealTimeRespSignals;
	MatDoub			m_matRealTimePSD;
	MatDoub			m_matDriveSignalPre;

	SpectOlap2D*	m_pInstRealTimePSD; 

	// ���ͷ��̼� ������� ����� ���Ÿ� ���� ��� ���� �迭.
	// 1Index ��ȣ����, 2Index Iteration��, 3Index Data
	
	Mat3DDoub		m_matIterData;
	
	bool			m_fIsIterationComplete;
public:
	// ���� ���ͷ��̼� ���� �۾��� ��Ÿ���� ���°�
	int				m_nIterationNumber;

public:
	//�ܺο��� Ȯ���ϱ� ���� ���°�
	// ����Ÿ�� �� �޾Ҵ�������
	bool			m_fIsRespDataFull;
	// PSD�� �غ� �Ǿ����� ���� 
	bool			m_fIsRespPSDReady;	

private:
	// ���ͷ��̼� ��� ����.
	void IterationResultSave(); 
	//������Ʈ����̺��Լ������� ������Ʈ ���� ����.
public:
	// ���� ���ͷ��̼� ��� ���� �ҷ���. ����̺�, ����, ���� PSD
	void IterationResultLoad(int IterationNumber); // ����� ��û�� ���� �ѹ�� �� �־ �ۺ�!

public:
	virtual void TSInitialization( const SYSTEMSETTING & SS);
	virtual void TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals );

	// ����̺� �����Ӹ� �ƴ϶� �� ���� ���� ����Ÿ�� ��Ʈ������ ����ȴ�.
	virtual void GenerateFirstDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);
	virtual void GenerateUpdatedDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);

	//=============================//
	// �ǽð� ���� ����Ÿ ó�� �Լ�//
	//=============================//
	virtual void TakeResponseSignals( MatDoub & matResponse ,bool IsFirst/*false*/ );
	void RealResponseProcessing();//��ġ���Ͽ����� NI �̺�Ʈ ó�� �ӵ��� ����Ͽ� �и�.

public:
	// �ǽð� ����� ��û�� ���� Export 
	// Get Data
	MatDoub GetRealTimePSDRespSignals();     //�̿�
	VecDoub GetFrequency();              //�̱���
	int     GetIterationNumber();

public:
	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals ) abstract;
	
	// ����
	void ResamplingTargetSignals();//�̿�

	~TARGETSIMULATION();
};


// ��� ó���� �� �ڽ� Ŭ����
class TARGETSIMULATIONEX : public TARGETSIMULATION
{
private: 
	int				m_nElement; // ��� ��� ����;
private:
	MatDoub			m_matStatisticDesiredSignal;
	Mat3DDoub		m_matStatisticData;
					// �� ����Ÿ�� ���ͷ��̼ǿ� ���� ��ȭ�� ��� ������ . ��ǥ��ȣ ����;
private:
	VecDoub CalculateStatistic(VecDoub & Data);
public:
	Mat3DDoub & GetStatisticData();
	MatDoub   & GetDesiredStatisticData();
public:
	TARGETSIMULATIONEX();
	virtual void TSInitialization( const SYSTEMSETTING & SS);
	virtual void TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals );

	virtual void GenerateFirstDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);
	virtual void GenerateUpdatedDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);

	virtual void TakeResponseSignals( MatDoub & matResponse ,bool IsFirst=false );
	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals );

};
#endif