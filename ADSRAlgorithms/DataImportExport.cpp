#include "DataImportExport.h"

using namespace std;

CDataImportExport::CDataImportExport(void)
{

}

CDataImportExport::~CDataImportExport(void)
{

}

bool CDataImportExport::LoadDesiredSignals( string PathName,_FileSystem &FileSystem )
{

	//임시 저장 변수 선언.
	int       IndexPre=0;
	char	  tempChar[1000];

	m_sDesiredSignalStatus.NumberofSignals=0;

	// 파일 핸들 
	ifstream fin(PathName.data());

	// 첫줄을 읽는다.
	fin.getline(tempChar,1000,'\n');

	// 첫줄이 시작 문구인지 판별
	if(strcmp(tempChar,"BEGIN")== 0)
	{
		// END를 만나기 전까지 무한 루프
		while( true )
		{

			// 100줄 이상 판별하고 있으면 에러라고 본다. 
			// 즉 END가 존재하는지 판별하는 것이다.
			if (IndexPre++ > 1000 ) {fin.close(); return false;}
			
			fin >> tempChar;

			// 			if (strcmp(tempChar,"COLUMNWIDTH")== 0)
			// 			{
			// 				while( fin.get()!= '\n') continue;
			// 			}
			// 
			// 			else if (strcmp(tempChar,"COLUMNOFFSET")== 0)
			// 			{
			// 				while( fin.get()!= '\n') continue;
			// 			}
			// 			else if (strcmp(tempChar,"MINIMUM")== 0)
			// 			{
			// 				while( fin.get()!= '\n') continue;
			// 			}
			// 			else if (strcmp(tempChar,"MAXIMUM")== 0)
			// 			{
			// 				while( fin.get()!= '\n') continue;
			// 			}
			// 			else if (strcmp(tempChar,"START")== 0)
			// 			{
			// 				while( fin.get()!= '\n') continue;
			// 			}
			// 
			// 			else if (strcmp(tempChar,"#")== 0)
			// 			{
			// 				while( fin.get()!= '\n') continue;
			// 			}
			// 			
			// 			else if ( strcmp(tempChar,"CHANNELNAME")== 0)

			if ( strcmp(tempChar,"CHANNELNAME")== 0)  // CHANNELNAME = ['aa','bb','cc']
			{
				for ( int k=0 ; k<1000 ; k++)
				{
					if ( fin.peek() == ']') break;
					if ( k>900 ) throw("에러 길이초과");
					fin.getline(tempChar,1000,'\'');// 제거용			
					fin.getline(tempChar,1000,'\'');// 얻을 문장
					m_sDesiredSignalStatus.CHANNELNAME[k]=tempChar;
					m_sDesiredSignalStatus.NumberofSignals=k+1;
				}
				while( fin.get()!= '\n') continue;

			}
			else if ( strcmp(tempChar,"LENGTH")== 0) // LENGTH = [1,1,1]
			{
				if (m_sDesiredSignalStatus.NumberofSignals < 1 ) {fin.close(); return false;}
				fin.getline(tempChar,1000,'[');
				for (unsigned int k=0 ; k< m_sDesiredSignalStatus.NumberofSignals-1 ;k++)
				{			
					fin.getline(tempChar,1000,',');
					m_sDesiredSignalStatus.LENGTH[k]
									=static_cast<int>(strtod(tempChar,NULL));

				}
				fin.getline(tempChar,1000,']');
				m_sDesiredSignalStatus.LENGTH[m_sDesiredSignalStatus.NumberofSignals-1]
									=static_cast<int>(strtod(tempChar,NULL));
				while( fin.get()!= '\n') continue;
			}
			else if ( strcmp(tempChar,"UNIT")== 0) // UNIT = 'm/s2','m/s2'
			{
				for ( unsigned int k=0 ; k<1000 ; k++)
				{
					if ( fin.peek() == ']') break;
					if ( k>900 ) throw("에러 길이초과");
					fin.getline(tempChar,1000,'\'');// 제거용			
					fin.getline(tempChar,1000,'\'');// 얻을 문장
					m_sDesiredSignalStatus.UNIT[k]=tempChar;
				}
				while( fin.get()!= '\n') continue;
			}
			else if ( strcmp(tempChar,"DELTA")== 0) // DELTA = [1,1,1]
			{
				if (m_sDesiredSignalStatus.NumberofSignals < 1 ){fin.close(); return false;}
				fin.getline(tempChar,1000,'[');
				for (unsigned int k=0 ; k< m_sDesiredSignalStatus.NumberofSignals-1 ;k++)
				{			
					fin.getline(tempChar,1000,',');
					m_sDesiredSignalStatus.DELTA[k]=strtod(tempChar,NULL);

				}
				fin.getline(tempChar,1000,']');
				m_sDesiredSignalStatus.DELTA[m_sDesiredSignalStatus.NumberofSignals-1]
													=strtod(tempChar,NULL);
				while( fin.get()!= '\n') continue;
			}
			// 탈출용이다. 이 문구가 없으면 IndexPre에 의해 오류가 발생한다.
			else if (strcmp(tempChar,"END")== 0)
			{
				break; 
			}
			else
			{
				while( fin.get()!= '\n') continue;
			}
		}
	}
	else
	{
		fin.close();
		return false;
	}

	// 데이타 임시 저장
	m_matDesiredSignals.Assign(m_sDesiredSignalStatus.NumberofSignals
						  ,int(m_sDesiredSignalStatus.LENGTH[0]),0.);
	for (unsigned int j=0 ; j < int(m_sDesiredSignalStatus.LENGTH[0]) ; j++)
		for (unsigned int i=0 ; i < m_sDesiredSignalStatus.NumberofSignals ; i++)
		{
			fin>>tempChar;
			m_matDesiredSignals[i][j]=strtod(tempChar,NULL);
		}

	// 상태 저장
	FileSystem=m_sDesiredSignalStatus;
	fin.close();
	return true;
}

void CDataImportExport::GetDesiredSignals(unsigned int FirstLine,unsigned int LastLine,MatDoub & DesiredSignal)
{
	// 에러 처리
	if ( FirstLine >= LastLine ) return; 
	if ( LastLine > m_sDesiredSignalStatus.LENGTH[0]) return; 
	
	// 입력 파라미터 매트릭스 초기화
	DesiredSignal.Assign(m_sDesiredSignalStatus.NumberofSignals,LastLine-FirstLine+1,0.);


	// 대입
	for (unsigned int j=FirstLine ; j < (LastLine+1) ; j++)
		for (unsigned int i=0 ; i < m_sDesiredSignalStatus.NumberofSignals ; i++)
		{
			DesiredSignal[i][j-FirstLine]=m_matDesiredSignals[i][j];
		}
}

bool CDataImportExport::SaveResponseSignals( Mat3DDoub Response )
{
	// 파일 핸들 
	ofstream fout;
	fout.open("datalog.txt", ios_base::out  | ios_base::trunc );

	for ( int i=0 ; i < Response.dim3() ; i++ )
		fout << Response[0][0][i] << "\n";

	fout.close();
	return true;

}

bool CDataImportExport::SaveSIData( MatDoub WhiteNoise, MatDoub Response )
{
	// 파일 핸들 
	ofstream fout;
	fout.open("response.txt", ios_base::out  | ios_base::trunc );

	for ( int j=0 ; j < Response.ncols() ; j++ )
	{
		for ( int i=0 ; i < Response.nrows() ; i++ )
			fout << Response[i][j] << "     "; // "\n";
		fout << "\n";
	}
	fout.close();

	// 파일 핸들 
	fout.open("white.txt", ios_base::out  | ios_base::trunc );

	for ( int j=0 ; j < WhiteNoise.ncols() ; j++ )
	{
		for ( int i=0 ; i < WhiteNoise.nrows() ; i++ )
			fout << WhiteNoise[i][j] << "     "; // "\n";
		fout << "\n";
	}
	fout.close();
	return true;
}

bool CDataImportExport::SaveSIResp( MatDoub Response)
{
	// 파일 핸들 
	ofstream fout;
	fout.open("SIResp.txt", ios_base::out  | ios_base::trunc );

	for ( int j=0 ; j < Response.ncols() ; j++ )
	{
		for ( int i=0 ; i < Response.nrows() ; i++ )
			fout << Response[i][j] << "     "; // "\n";
		fout << "\n";
	}
	fout.close();
	return true;
}

bool CDataImportExport::SaveTsResp( Mat3DDoub Response)
{
	// 파일 핸들 
	ofstream fout;
	fout.open("TSresp.txt", ios_base::out  | ios_base::trunc );

	for ( int j=0 ; j < Response.dim3() ; j++ )
	{
		for ( int i=0 ; i < Response.dim2() ; i++ )
			fout << Response[0][i][j] << "     "; // "\n";
		fout << "\n";
	}
	fout.close();
	return true;
}

bool CDataImportExport::SaveTsDrive( MatDoub Drive )
{
	// 파일 핸들 
	ofstream fout;
	fout.open("TSDrive.txt", ios_base::out  | ios_base::trunc );

	for ( int j=0 ; j < Drive.ncols() ; j++ )
	{
		for ( int i=0 ; i < Drive.nrows() ; i++ )
			fout << Drive[i][j] << "     "; // "\n";
		fout << "\n";
	}
	fout.close();
	return true;
}

