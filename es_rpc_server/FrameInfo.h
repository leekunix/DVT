#pragma once

#include <list>

using namespace std;

class CFrmInfo
{
public:

	CFrmInfo()
	{
		st = -1;
		et = -1;
		data_len = -1;
		bKeyFrame = -1;
	}

	__int64 st,et;
	int		data_len;
	int		bKeyFrame;
};

class CTransServer
{
public:
	CTransServer()
	{
		m_VideoWidth = 1280;
		m_VideoHeight = 720;

		hLocalFile = INVALID_HANDLE_VALUE;
		CompressedhFile = INVALID_HANDLE_VALUE;
	}

	~CTransServer()
	{
		if( CompressedhFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle(CompressedhFile);
		}

		if( hLocalFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle(hLocalFile);
		}

		hLocalFile = INVALID_HANDLE_VALUE;
		CompressedhFile = INVALID_HANDLE_VALUE;
	}

	HANDLE		hLocalFile;
	HANDLE		CompressedhFile;

	GUID		videoType;
	char		filename[256];
	char		compress_filename[256];
	__int64		AvgTimePerFrame;
	int			m_VideoWidth;
	int			m_VideoHeight;

	list<CFrmInfo*> frmInfoList;
};

