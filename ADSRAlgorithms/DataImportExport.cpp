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

	//�ӽ� ���� ���� ����.
	int       IndexPre=0;
	char	  tempChar[1000];

	m_sDesiredSignalStatus.NumberofSignals=0;

	// ���� �ڵ� 
	ifstream fin(PathName.data());

	// ù���� �д´�.
	fin.getline(tempChar,1000,'\n');

	// ù���� ���� �������� �Ǻ�
	if(strcmp(tempChar,"BEGIN")== 0)
	{
		// END�� ������ ������ ���� ����
		while( true )
		{

			// 100�� �̻� �Ǻ��ϰ� ������ ������� ����. 
			// �� END�� �����ϴ��� �Ǻ��ϴ� ���̴�.
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
					if ( k>900 ) throw("���� �����ʰ�");
					fin.getline(tempChar,1000,'\'');// ���ſ�			
					fin.getline(tempChar,1000,'\'');// ���� ����
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
					if ( k>900 ) throw("���� �����ʰ�");
					fin.getline(tempChar,1000,'\'');// ���ſ�			
					fin.getline(tempChar,1000,'\'');// ���� ����
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
			// Ż����̴�. �� ������ ������ IndexPre�� ���� ������ �߻��Ѵ�.
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

	// ����Ÿ �ӽ� ����
	m_matDesiredSignals.Assign(m_sDesiredSignalStatus.NumberofSignals
						  ,int(m_sDesiredSignalStatus.LENGTH[0]),0.);
	for (unsigned int j=0 ; j < int(m_sDesiredSignalStatus.LENGTH[0]) ; j++)
		for (unsigned int i=0 ; i < m_sDesiredSignalStatus.NumberofSignals ; i++)
		{
			fin>>tempChar;
			m_matDesiredSignals[i][j]=strtod(tempChar,NULL);
		}

	// ���� ����
	FileSystem=m_sDesiredSignalStatus;
	fin.close();
	return true;
}

void CDataImportExport::GetDesiredSignals(unsigned int FirstLine,unsigned int LastLine,MatDoub & DesiredSignal)
{
	// ���� ó��
	if ( FirstLine >= LastLine ) return; 
	if ( LastLine > m_sDesiredSignalStatus.LENGTH[0]) return; 
	
	// �Է� �Ķ���� ��Ʈ���� �ʱ�ȭ
	DesiredSignal.Assign(m_sDesiredSignalStatus.NumberofSignals,LastLine-FirstLine+1,0.);


	// ����
	for (unsigned int j=FirstLine ; j < (LastLine+1) ; j++)
		for (unsigned int i=0 ; i < m_sDesiredSignalStatus.NumberofSignals ; i++)
		{
			DesiredSignal[i][j-FirstLine]=m_matDesiredSignals[i][j];
		}
}

bool CDataImportExport::SaveResponseSignals( Mat3DDoub Response )
{
	// ���� �ڵ� 
	ofstream fout;
	fout.open("datalog.txt", ios_base::out  | ios_base::trunc );

	for ( int i=0 ; i < Response.dim3() ; i++ )
		fout << Response[0][0][i] << "\n";

	fout.close();
	return true;

}

bool CDataImportExport::SaveSIData( MatDoub WhiteNoise, MatDoub Response )
{
	// ���� �ڵ� 
	ofstream fout;
	fout.open("response.txt", ios_base::out  | ios_base::trunc );

	for ( int j=0 ; j < Response.ncols() ; j++ )
	{
		for ( int i=0 ; i < Response.nrows() ; i++ )
			fout << Response[i][j] << "     "; // "\n";
		fout << "\n";
	}
	fout.close();

	// ���� �ڵ� 
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
	// ���� �ڵ� 
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
	// ���� �ڵ� 
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
	// ���� �ڵ� 
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

