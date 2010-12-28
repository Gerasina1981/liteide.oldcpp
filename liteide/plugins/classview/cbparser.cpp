// cbparser.cpp: implementation of the cbparser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cbparser.h"
#include <public/wtlfile.h>
#include <public/wtlutil.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
class Loader : public LoaderBase
{
public:
	Loader()
	{
		buf = NULL;
	}
	~Loader()
	{
		Release();
	}
	
	void Release()
	{
		if (buf)
		{
			delete[] buf;
			buf = NULL;
		}
	}

	char * GetData()
	{
		return buf;
	}
	unsigned int GetLength()
	{
		return len;
	}
	void Sync()
	{
	}
	BOOL LoadFile(const TCHAR * filename)
	{
		CFile file;
		if (!file.Open(filename,CFile::modeRead ,NULL))
			return FALSE;
		len = file.GetLength();
		buf = new char[len];
		file.Read(buf,len);
		file.Close();
		return TRUE;
	}
	char * buf;
	unsigned int	   len;
};



cbparser::cbparser()
{
	m_pTokens = new TokensTree();
}

cbparser::~cbparser()
{
	if (m_pTokens)
		delete m_pTokens;
}

bool cbparser::load_sources(std::vector<CString> &source)
{
	m_pTokens->Clear();
	
	CString file;
	std::vector<CString> ar1,ar2;
	size_t i = 0;
	for (i = 0; i < source.size(); i++) 
	{
		CString & tmp = source[i];
		if (tmp.Left(1) == _T("\""))
		{
			file = tmp;
			file.TrimLeft(_T("\""));
			file.TrimRight(_T("\""));
		}
		else
			file = tmp;
		CString & ext = get_file_ext(file);
		if (ext.CompareNoCase(_T(".rc")) == 0) {
			//load_res(file,true);
		}
		else if (ext.CompareNoCase(_T(".h")) == 0 ||
			ext.CompareNoCase(_T(".hxx")) == 0 ||
			ext.CompareNoCase(_T(".hpp")) == 0)
		{
			ar1.push_back(file);
		}
		else if ( ext.CompareNoCase(_T(".cpp")) == 0 ||
			ext.CompareNoCase(_T(".cxx")) == 0 ||
			ext.CompareNoCase(_T(".c")) == 0 ) 
		{
			ar2.push_back(file);
		}
	}
	//frist parser .h
	for (i = 0; i < ar1.size(); i++)
	{
		load_code(ar1[i]);
	}
	//then parser .cpp
	for (i = 0; i < ar2.size(); i++)
	{
		load_code(ar2[i]);
	}
	return true;
}

void cbparser::clear()
{
	m_pTokens->clear();
}

bool cbparser::load_code(const wchar_t *path)
{
	Loader load;
	load.LoadFile(path);
	
	bool isLocal = true;
	Parser parent;
	ParserThreadOptions opts;
	opts.wantPreprocessor = false;
	opts.useBuffer = false;
	opts.bufferSkipBlocks = false;
	opts.bufferSkipOuterBlocks = false;
	opts.followLocalIncludes = false;
	opts.followGlobalIncludes = false;
	opts.loader = &load;
	
	ParserThread * pt = new ParserThread(&parent,path,isLocal,opts,m_pTokens);
	bool bret = pt->Parse();
	delete pt;
	return bret;	
}
