#ifndef _DSPPARSER_H__
#define _DSPPARSER_H__

//time 2007-4-24
//copyright visualfc@gmail.com
#include "wtlfileex.h"

class dspparser
{
public:
	CSimpleArray<CString>	source;
	CString			root_path;
public:
	bool load_dsp(LPCTSTR lpszFileName);
	bool load_vcproj(LPCTSTR lpszFileName);
};

inline bool dspparser::load_vcproj(LPCTSTR lpszFileName)
{
	source.RemoveAll();
	root_path = get_file_root(lpszFileName);
	CTextFile file;
	if (file.Open(lpszFileName,CFile::modeRead | CFile::shareDenyNone) == FALSE)
	{
		return false;
	}
	CString line;
	CString tmp;
	const int len = lstrlen(_T("RelativePath="));///sizeof(char);
	while(file.ReadLine(line))
	{
		line.TrimLeft();
		line.TrimRight();
		if (line.Left(len) == _T("RelativePath=")) 
		{
			tmp = line.Right(line.GetLength()-len);
			int left = tmp.Find(_T('\"'));
			int right = tmp.ReverseFind(_T('\"'));
			if (left != -1 && right != -1)
			{
				tmp = tmp.Mid(left+1,right-left-1);
				source.Add(root_path+tmp);
			}
		}
	}
	file.Close();
	return true;
}

inline bool dspparser::load_dsp(LPCTSTR lpszFileName)
{
	source.RemoveAll();
	root_path = get_file_root(lpszFileName);
	CTextFile file;
	if (file.Open(lpszFileName,CFile::modeRead | CFile::shareDenyNone) == FALSE)
	{
		return false;
	}
	CString line;
	CString tmp;
	while(file.ReadLine(line))
	{
		if (line.Left(7) == _T("SOURCE=")) 
		{
			tmp = line.Right(line.GetLength()-7);
			if (tmp.Left(1) == _T("\""))
			{
				tmp.TrimLeft(_T("\""));
				tmp.TrimRight(_T("\""));
			}
			else 
			{
				tmp = root_path+tmp;
			}
			source.Add(tmp);
		}
	}
	file.Close();
	return true;
}

#endif //_DSPPARSER_H__