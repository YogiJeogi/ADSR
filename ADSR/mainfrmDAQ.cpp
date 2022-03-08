#include "StdAfx.h"
#include "MainFrm.h"
#include "ADSR.h"

void CMainFrame::DAQSIInitialization(void)
{

	CleanUpIfNecessary();
	
	// 타스크 준비 및 시작.
	if(!m_taskRunning)
	{
		if ( pRealVDoc !=NULL )
		{
			pRealVDoc->m_bType0dataReady=false;
			pRealVDoc->m_bType1dataReady=false;
			pRealVDoc->m_bType2dataReady=false;
		}
		System.SI.m_bIsRespDataFull=false;	
		m_bIsFirstResp=true;
		// 변수 선언 및 초기화
		
// 		CNiString ExcChanHWs
// 					=theApp.m_strvNIExciterChannel
// 					[System.SS.FRONTEND.m_arrindexInputChannelHW[0]];
// 		for ( int i=1 ; i < System.SS.FRONTEND.m_nInput_Channels ; i++ )
// 			ExcChanHWs = ExcChanHWs + ", " 
// 			+ theApp.m_strvNIExciterChannel[System.SS.FRONTEND.m_arrindexInputChannelHW[i]];

		CNiString ExcChanPerDevice[3];
		int       ExcNumPerDevice[3]={1,0,0};

		ExcChanPerDevice[0] = theApp.m_strvNIExciterChannel
			 					[System.SS.FRONTEND.m_arrindexInputChannelHW[0]];

		CNiString TempExc,TempExcNext;
		int DeviceNumber=1;

		for ( int i=1 ; i < System.SS.FRONTEND.m_nInput_Channels ; i++ )
		{
			TempExc=theApp.m_strvNIExciterChannel[System.SS.FRONTEND.m_arrindexInputChannelHW[i-1]];
			TempExcNext=theApp.m_strvNIExciterChannel[System.SS.FRONTEND.m_arrindexInputChannelHW[i]];
			
			if (TempExcNext.Mid(3,1)!=TempExc.Mid(3,1)) 
			{
				DeviceNumber++;
				ExcChanPerDevice[DeviceNumber-1]=TempExcNext;
				ExcNumPerDevice[DeviceNumber-1]=1;
				
			}
			else
			{
				ExcChanPerDevice[DeviceNumber-1]=ExcChanPerDevice[DeviceNumber-1]+", "+TempExcNext;
				ExcNumPerDevice[DeviceNumber-1]+=1;
			}
		}	
		for (int i=0; i< DeviceNumber ; i++ )
			m_writedata[i].SetSize(ExcNumPerDevice[i],theApp.m_SIWhiteNoiseTimeDataNI.GetCols());

		for ( int j=0; j< ExcNumPerDevice[0]; j++)
			for ( int k=0; k < theApp.m_SIWhiteNoiseTimeDataNI.GetCols() ; k++)
				m_writedata[0](j,k)=theApp.m_SIWhiteNoiseTimeDataNI(j,k);

		for ( int j=0; j< ExcNumPerDevice[1]; j++)
			for ( int k=0; k < theApp.m_SIWhiteNoiseTimeDataNI.GetCols() ; k++)
				m_writedata[1](j,k)=theApp.m_SIWhiteNoiseTimeDataNI(j+ExcNumPerDevice[0],k);

		for ( int j=0; j< ExcNumPerDevice[2]; j++)
			for ( int k=0; k < theApp.m_SIWhiteNoiseTimeDataNI.GetCols() ; k++)
				m_writedata[2](j,k)=theApp.m_SIWhiteNoiseTimeDataNI(j+ExcNumPerDevice[0]+ExcNumPerDevice[1],k);

		CNiString ResChanHWs
					=theApp.m_strvNIResponseChannel
					[System.SS.FRONTEND.m_arrindexResponseChannelHW[0]];
 
   		for ( int i=1 ; i < System.SS.FRONTEND.m_nResponse_Channels; i++ )
   			ResChanHWs = ResChanHWs + "," 
				+ theApp.m_strvNIResponseChannel[System.SS.FRONTEND.m_arrindexResponseChannelHW[i]];

		// 입출력  Max/Min
		double Readmin=-10;
		double Readmax=10;
		double minimum=-10;
		double maximum=10;

		// 입출력 샘플링 Rate
		double rate=System.SS.FRONTEND.m_nSamplingFreq;
		m_samples=static_cast<long> (rate);
		int m_npoint=System.SS.SI.m_nSignalsPoints;

		m_taskRunning = true;
	
		try
		{
			// Create the task
			m_readtask = std::auto_ptr<CNiDAQmxTask>(new CNiDAQmxTask());

			for (int i=0; i< DeviceNumber ; i++){
				m_writetask[i] = std::auto_ptr<CNiDAQmxTask>(new CNiDAQmxTask());
			}


			// Create the input voltage channel
			m_readtask->AIChannels.CreateVoltageChannel(ResChanHWs,
							"",
							DAQmxAITerminalConfigurationRse,
							Readmin, 
							Readmax, 
							DAQmxAIVoltageUnitsVolts);

			for ( int i=0 ; i < DeviceNumber; i++)
			{
			m_writetask[i]->AOChannels.CreateVoltageChannel(ExcChanPerDevice[i],
							_T(""),
							minimum, 
							maximum, 
 							DAQmxAOVoltageUnitsVolts);
			}
			// Configure the internal clock
			m_readtask->Timing.ConfigureSampleClock("", 
							rate, 
							DAQmxSampleClockActiveEdgeRising, 
							DAQmxSampleQuantityModeFiniteSamples, 
							m_npoint);

			for ( int i=0 ; i < DeviceNumber; i++)
			{
				m_writetask[i]->Timing.ConfigureSampleClock("", 
					rate, 
					DAQmxSampleClockActiveEdgeRising, 
					DAQmxSampleQuantityModeFiniteSamples, 
					m_npoint+1);

				m_writetask[i]->Triggers.StartTrigger.ConfigureDigitalEdgeTrigger
					("/Dev1/ai/SampleClock",DAQmxDigitalEdgeStartTriggerEdgeRising);
			}

			m_readtask->Control(DAQmxTaskVerify);
			for ( int i=0 ; i < DeviceNumber; i++)
				m_writetask[i]->Control(DAQmxTaskVerify);

			m_reader = std::auto_ptr<CNiDAQmxAnalogMultiChannelReader>
				(new CNiDAQmxAnalogMultiChannelReader(m_readtask->Stream));

			for ( int i=0 ; i < DeviceNumber; i++)
			m_writer[i] = std::auto_ptr<CNiDAQmxAnalogMultiChannelWriter>
				(new CNiDAQmxAnalogMultiChannelWriter(m_writetask[i]->Stream));

			m_reader->InstallEventHandler(*this, &CMainFrame::OnEventSI);

 			
			for ( int i=0 ; i < DeviceNumber; i++)
				m_writer[i]->WriteMultiSample(true,m_writedata[i]);

			m_reader->ReadMultiSampleAsync(m_samples, m_readdata, NULL);

		}
		catch(CException* e)
		{
			KillTimer(1);
			e->ReportError();
			e->Delete();
			delete m_readtask.release();
			delete m_writetask[0].release();
			delete m_writetask[1].release();
			delete m_writetask[2].release();

			m_taskRunning = false;
		}
	}
}
void CMainFrame::DAQTSInitialization(void)
{
	CleanUpIfNecessary();

	// 타스크 준비 및 시작.
	if(!m_taskRunning)
	{
		if ( pRealVDoc !=NULL )
		{
			pRealVDoc->m_bType0dataReady=false;
			pRealVDoc->m_bType1dataReady=false;
			pRealVDoc->m_bType2dataReady=false;
		}
		System.TS.m_fIsRespDataFull=false;	
		m_bIsFirstResp=true;

		CNiString ExcChanPerDevice[3];
		int       ExcNumPerDevice[3]={1,0,0};

		ExcChanPerDevice[0]= theApp.m_strvNIExciterChannel
			[System.SS.FRONTEND.m_arrindexInputChannelHW[0]];

		CNiString TempExc,TempExcNext;
		int DeviceNumber=1;

		for ( int i=1 ; i < System.SS.FRONTEND.m_nInput_Channels ; i++ )
		{
			TempExc=theApp.m_strvNIExciterChannel[System.SS.FRONTEND.m_arrindexInputChannelHW[i-1]];
			TempExcNext=theApp.m_strvNIExciterChannel[System.SS.FRONTEND.m_arrindexInputChannelHW[i]];

			if (TempExcNext.Mid(3,1)!=TempExc.Mid(3,1)) 
			{
				DeviceNumber++;
				ExcChanPerDevice[DeviceNumber-1]=TempExcNext;
				ExcNumPerDevice[DeviceNumber-1]=1;

			}
			else
			{
				ExcChanPerDevice[DeviceNumber-1]=ExcChanPerDevice[DeviceNumber-1]+", "+TempExcNext;
				ExcNumPerDevice[DeviceNumber-1]+=1;
			}
		}	

	//	m_writedata=theApp.m_TSDriveTimeDataNI;
		for (int i=0; i< DeviceNumber ; i++ )
			m_writedata[i].SetSize(ExcNumPerDevice[i],theApp.m_TSDriveTimeDataNI.GetCols());

		for ( int j=0; j< ExcNumPerDevice[0]; j++)
			for ( int k=0; k < theApp.m_TSDriveTimeDataNI.GetCols() ; k++)
				m_writedata[0](j,k)=theApp.m_TSDriveTimeDataNI(j,k);

		for ( int j=0; j< ExcNumPerDevice[1]; j++)
			for ( int k=0; k < theApp.m_TSDriveTimeDataNI.GetCols() ; k++)
				m_writedata[1](j,k)=theApp.m_TSDriveTimeDataNI(j+ExcNumPerDevice[0],k);

		for ( int j=0; j< ExcNumPerDevice[2]; j++)
			for ( int k=0; k < theApp.m_TSDriveTimeDataNI.GetCols() ; k++)
				m_writedata[2](j,k)=theApp.m_TSDriveTimeDataNI(j+ExcNumPerDevice[0]+ExcNumPerDevice[1],k);

		CNiString ResChanHWs
			=theApp.m_strvNIResponseChannel
			[System.SS.FRONTEND.m_arrindexResponseChannelHW[0]];

		for ( int i=1 ; i < System.SS.FRONTEND.m_nResponse_Channels+System.SS.FRONTEND.m_nSlave_Channels; i++ )
			ResChanHWs = ResChanHWs + "," 
			+ theApp.m_strvNIResponseChannel[System.SS.FRONTEND.m_arrindexResponseChannelHW[i]];

		// 입출력  Max/Min
		double readminimum=-10;
		double readmaximum=10;
		double writeminimum=-10;
		double writemaximum=10;

		// 입출력 샘플링 Rate
		double rate=System.SS.FRONTEND.m_nSamplingFreq;
		m_samples=static_cast<long> (rate);
		int m_npoint=System.SS.TS.m_nNumberOfPointinDesiredSignals;

		m_taskRunning = true;

		try
		{
			// Create the task
			m_readtask = std::auto_ptr<CNiDAQmxTask>(new CNiDAQmxTask());
/*			m_writetask = std::auto_ptr<CNiDAQmxTask>(new CNiDAQmxTask());*/
			for (int i=0; i< DeviceNumber ; i++){
				m_writetask[i] = std::auto_ptr<CNiDAQmxTask>(new CNiDAQmxTask());
			}


			// Create the input voltage channel
			m_readtask->AIChannels.CreateVoltageChannel(ResChanHWs,
				"",
				DAQmxAITerminalConfigurationRse,
				readminimum, 
				readmaximum, 
				DAQmxAIVoltageUnitsVolts);

// 			m_writetask->AOChannels.CreateVoltageChannel(ExcChanHWs,
// 				_T(""),
// 				writeminimum, 
// 				writemaximum, 
// 				DAQmxAOVoltageUnitsVolts);
			for ( int i=0 ; i < DeviceNumber; i++)
			{
				m_writetask[i]->AOChannels.CreateVoltageChannel(ExcChanPerDevice[i],
					_T(""),
					writeminimum, 
					writemaximum, 
					DAQmxAOVoltageUnitsVolts);
			}
			
			// Configure the internal clock
			m_readtask->Timing.ConfigureSampleClock("", 
				rate, 
				DAQmxSampleClockActiveEdgeRising, 
				DAQmxSampleQuantityModeFiniteSamples, 
				m_npoint);
// 
// 			m_writetask->Timing.ConfigureSampleClock("", 
// 				rate, 
// 				DAQmxSampleClockActiveEdgeRising, 
// 				DAQmxSampleQuantityModeFiniteSamples, 
// 				m_npoint+1);// 마지막을 0으로 내보내기 위해서.

			for ( int i=0 ; i < DeviceNumber; i++)
			{
				m_writetask[i]->Timing.ConfigureSampleClock("", 
					rate, 
					DAQmxSampleClockActiveEdgeRising, 
					DAQmxSampleQuantityModeFiniteSamples, 
					m_npoint+1);

				m_writetask[i]->Triggers.StartTrigger.ConfigureDigitalEdgeTrigger
					("/Dev1/ai/SampleClock",DAQmxDigitalEdgeStartTriggerEdgeRising);
			}

/* 			m_writetask->Triggers.StartTrigger.ConfigureDigitalEdgeTrigger("/Dev1/ai/SampleClock",DAQmxDigitalEdgeStartTriggerEdgeRising);*/

			m_readtask->Control(DAQmxTaskVerify);
/*			m_writetask->Control(DAQmxTaskVerify);*/
			for ( int i=0 ; i < DeviceNumber; i++)
				m_writetask[i]->Control(DAQmxTaskVerify);

			m_reader = std::auto_ptr<CNiDAQmxAnalogMultiChannelReader>
				(new CNiDAQmxAnalogMultiChannelReader(m_readtask->Stream));

// 			m_writer = std::auto_ptr<CNiDAQmxAnalogMultiChannelWriter>
// 				(new CNiDAQmxAnalogMultiChannelWriter(m_writetask->Stream));
			for ( int i=0 ; i < DeviceNumber; i++)
				m_writer[i] = std::auto_ptr<CNiDAQmxAnalogMultiChannelWriter>
					(new CNiDAQmxAnalogMultiChannelWriter(m_writetask[i]->Stream));

			m_reader->InstallEventHandler(*this, &CMainFrame::OnEventTS);

/*			m_writer->WriteMultiSample(true,m_writedata);*/
			for ( int i=0 ; i < DeviceNumber; i++)
				m_writer[i]->WriteMultiSample(true,m_writedata[i]);

			m_reader->ReadMultiSampleAsync(m_samples, m_readdata, NULL);

		}
		catch(CException* e)
		{
			KillTimer(1);
			e->ReportError();
			e->Delete();
			delete m_readtask.release();
			delete m_writetask[0].release();
			delete m_writetask[1].release();
			delete m_writetask[2].release();
			m_taskRunning = false;
		}
	}
}


// 힙 메모리 삭제 
void CMainFrame::CleanUpIfNecessary(void)
{
	delete m_readtask.release();
	delete m_writetask[0].release();
	delete m_writetask[1].release();
	delete m_writetask[2].release();

	delete m_reader.release();
	delete m_writer[0].release();
	delete m_writer[1].release();
	delete m_writer[2].release();
}

// 이벤트
void CMainFrame::OnEventSI(CNiDAQmxAsyncOperation asyncHandle, void* userData)
{
	CNiComInitialize com(CNiComInitialize::Apartment);
	try
	{
		asyncHandle.CheckForAsyncException();

// 		if ( m_writetask->IsDone)
// 		{
// 			AfxMessageBox("타스크 돈");
// 		}
// 		
		// 데이타가 꽉찼는지 확인.
		if ( System.SI.m_bIsRespDataFull) 
		{
			
			m_taskRunning=false;
			// 최종계산!

		}

		if (m_taskRunning) 
		{
			// 데이타 변환

			unsigned int DATArow,DATAcol;
			m_readdata.GetSize(DATArow,DATAcol);

			// SI에 넣기 위한 데이타 가공부분
			m_SIreaddata.resize(DATArow,DATAcol);
			for ( unsigned int i=0; i < DATArow ; i++)
				for (unsigned int j=0; j < DATAcol ; j++ )
				{
					m_readdata(i,j)=m_readdata(i,j)*System.SS.FRONTEND.m_adbResponseChannelScale[i];
					m_SIreaddata[i][j]=m_readdata(i,j);
				}
			

			// 데이타 대입
			System.SI.InsertResponseSignals(m_SIreaddata,m_bIsFirstResp);
			m_bIsFirstResp=false;

			// 뷰 업데이트 구간이다.
			if ( pRealVDoc !=NULL)
			{
				int TypeOfSYSTEM=0; //SI
				::PostMessageA(GetSafeHwnd(),WM_HD_UP,MAKEWPARAM(TypeOfSYSTEM,0),MAKELPARAM(0,0));
			}

			// 데이타 읽음
			g_CriticalSection.Lock();
			m_reader->ReadMultiSampleAsync(m_samples, m_readdata, NULL);
			g_CriticalSection.Unlock();
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
// 		m_start.EnableWindow(TRUE);
//  	m_stop.EnableWindow(FALSE);
//  	m_start.SetFocus();
	}

}
void CMainFrame::OnEventTS(CNiDAQmxAsyncOperation asyncHandle, void* userData)
{
	CNiComInitialize com(CNiComInitialize::Apartment);
	try
	{
		asyncHandle.CheckForAsyncException();

		// 		if ( m_writetask->IsDone)
		// 		{
		// 			AfxMessageBox("타스크 돈");
		// 		}
		// 		
		// 데이타가 꽉찼는지 확인.
		if ( System.TS.m_fIsRespDataFull) 
		{

			m_taskRunning=false;
			// 최종계산!
		}

		if (m_taskRunning) 
		{
			// 데이타 변환

			unsigned int DATArow,DATAcol;
			m_readdata.GetSize(DATArow,DATAcol);

			// SI에 넣기 위한 데이타 가공부분
			m_TSreaddata.resize(DATArow,DATAcol);
			for ( unsigned int i=0; i < DATArow ; i++)
				for (unsigned int j=0; j < DATAcol ; j++ )
				{
					m_readdata(i,j)=m_readdata(i,j)*System.SS.FRONTEND.m_adbResponseChannelScale[i];
					m_TSreaddata[i][j]=m_readdata(i,j);
				}

			// 데이타 대입
			System.TS.TakeResponseSignals(m_TSreaddata,m_bIsFirstResp);
			m_bIsFirstResp=false;

			// 뷰 업데이트 구간이다.
			if ( pRealVDoc !=NULL)
			{
				int TypeOfSYSTEM=1; //TS
				::PostMessageA(GetSafeHwnd(),WM_HD_UP,MAKEWPARAM(TypeOfSYSTEM,0),MAKELPARAM(0,0));
			}

			// 데이타 읽음
			if ( System.TS.m_fIsRespDataFull != true)
			{
				g_CriticalSection.Lock();
				m_reader->ReadMultiSampleAsync(m_samples, m_readdata, NULL);
				g_CriticalSection.Unlock();
			}

		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}

}
void CMainFrame::RemoveEventHandler(void)
{
	if(m_reader.get())
	{
		m_taskRunning = false;
		m_bIsFirstResp=true;

		// Remove our event handler
		while(!m_reader->RemoveAllEventHandlers())
		{
			::Sleep(100);

			MSG msg;
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
}

void CMainFrame::DAQStop(void)
{
	CWaitCursor wait;

	//m_stop.EnableWindow(FALSE);

	RemoveEventHandler();

	try
	{
		if (m_readtask.get() != NULL)
		{
		m_readtask->Stop();
		}
		if (m_writetask[0].get() != NULL)
		{
			m_writetask[0]->Stop();
		}
		if (m_writetask[1].get() != NULL)
		{
			m_writetask[1]->Stop();
		}
		if (m_writetask[2].get() != NULL)
		{
			m_writetask[2]->Stop();
		}
	}
	catch (CException* e)
	{
		e->ReportError();
		e->Delete();
	}

	//m_start.EnableWindow(TRUE);
	//m_start.SetFocus();
}

void CMainFrame::DAQCleanUp(void)
{
	if (m_readtask.get() != NULL)
	{
		m_readtask->Control(DAQmxTaskAbort);
	}
	if (m_writetask[0].get() != NULL)
	{
		m_writetask[0]->Control(DAQmxTaskAbort);
	}
	if (m_writetask[1].get() != NULL)
	{
		m_writetask[1]->Control(DAQmxTaskAbort);
	}
	if (m_writetask[2].get() != NULL)
	{
		m_writetask[2]->Control(DAQmxTaskAbort);
	}
	CleanUpIfNecessary();	
}
