#pragma once
#include "typedef.h"

class CProjectManager
{
private:

	string		m_sfilename; // Ȯ���� ����
	int			m_nProgress; // 0: ������� ����, 1 SI, 2 TS
	int         m_nMode;	 // 0: ����, 1 ���̺�, 2 ���̺� as

public:

	CProjectManager();
	CProjectManager(string &filename, int Mode);

};