#pragma once
#include "typedef.h"

class CProjectManager
{
private:

	string		m_sfilename; // 확장자 포함
	int			m_nProgress; // 0: 진행없이 생성, 1 SI, 2 TS
	int         m_nMode;	 // 0: 오픈, 1 세이브, 2 세이브 as

public:

	CProjectManager();
	CProjectManager(string &filename, int Mode);

};