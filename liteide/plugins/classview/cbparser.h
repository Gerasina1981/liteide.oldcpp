// cbparser.h: interface for the cbparser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CBPARSER_H__76164096_6E45_407E_9225_29DB165525A4__INCLUDED_)
#define AFX_CBPARSER_H__76164096_6E45_407E_9225_29DB165525A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../cbparser/cppparser.h"
#include "../cbparser/vfc_array.h"

struct IProject;
class cbparser  
{
public:
	bool load_code(const wchar_t * path);
	cbparser();
	virtual ~cbparser();

	void clear();
	bool load_sources(std::vector<CString> &source);
	vfc_token_tree * get_tree()
	{
		return m_pTokens;
	}
public:
	TokensTree * m_pTokens;
	IProject   * m_proj;
};

#endif // !defined(AFX_CBPARSER_H__76164096_6E45_407E_9225_29DB165525A4__INCLUDED_)
