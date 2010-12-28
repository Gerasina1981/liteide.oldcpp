#ifndef _WXHELPER_H_
#define _WXHELPER_H_

#include <map>

class vfc_string
{
public:
	enum { npos = -1};

	vfc_string()
	{
	}
	vfc_string(const vfc_string & str)
	{
		m_str = str.m_str;
	}
	vfc_string(TCHAR ch) : m_str(ch)
	{
	}
	vfc_string(const TCHAR * str) : m_str(str)
	{
	}
	vfc_string(const TCHAR * str, int len) : m_str(str,len)
	{
	}
	vfc_string(const CString & str) : m_str(str)
	{
	}

	operator LPCTSTR() const   // as a C string
	{
		return m_str;
	}

	vfc_string substr(int start, int len) const
	{
		return m_str.Mid(start,len);
	}
	vfc_string substr(int start) const
	{
		return m_str.Right(m_str.GetLength()-start);
	}
	const TCHAR operator [] (int i) const
	{
		return m_str[i];
	}
	const TCHAR operator [] (unsigned int i) const
	{
		return m_str[(int)i];
	}

	TCHAR GetChar (int i) const
	{
		if (i >= m_str.GetLength())
			return _T('');
		return m_str[i];
	}
	size_t size()
	{
		return m_str.GetLength();
	}
	vfc_string& operator =(const vfc_string& stringSrc)
	{
		m_str = stringSrc.m_str;
		return *this;
	}
	// string concatenation
	vfc_string& operator +=(const vfc_string& string)
	{
		m_str += string.m_str;
		return *this;
	}

	vfc_string& operator +=(TCHAR ch)
	{
		m_str += ch;
		return *this;
	}
	size_t length() const
	{
		return m_str.GetLength();
	}
	size_t Length() const
	{
		return m_str.GetLength();
	}
	vfc_string & operator << (const vfc_string & str)
	{
		m_str += str.m_str;
		return *this;
	}
	vfc_string & operator << (TCHAR sz)
	{
		m_str += sz;
		return *this;
	}
	vfc_string & append(int size,TCHAR ch)
	{
		m_str += CString(ch,size);
		return *this;
	}
	vfc_string & Prepend(const vfc_string & str)
	{
		m_str.Insert(0,str.m_str);
		return *this;
	}
	BOOL empty() const
	{
		return m_str.IsEmpty();
	}
	void SetAt(int index, TCHAR ch)
	{
		m_str.SetAt(index,ch);
	}
	void Shrink()
	{
	}
	vfc_string Lower() const
	{
		CString str = m_str;
		str.MakeLower();
		return str;
	}
	bool StartsWith(const vfc_string & str) const
	{
		return m_str.Find(str.m_str) == 0;
	}
	void Clear()
	{
		m_str.Empty();
	}
	bool IsEmpty() const
	{
		return m_str.IsEmpty() == TRUE;
	}
	int	Find(const TCHAR * str) const
	{
		return m_str.Find(str);
	}

	int Find(TCHAR ch, bool fromEnd)
	{
		if (fromEnd)
			return m_str.ReverseFind(ch);
		else
			return m_str.Find(ch);
	}

	vfc_string& RemoveLast()
	{
		m_str = m_str.Left(m_str.GetLength()-1);
		return *this;
	}

	int	Find(TCHAR ch) const
	{
		return m_str.Find(ch);
	}

	vfc_string Mid(int start, int len) const
	{
		return m_str.Mid(start,len);
	}
	bool IsSameAs(const TCHAR * str)
	{
		return m_str.Compare(str) == 0;
	}
	void assign(const vfc_string & str)
	{
		m_str = str;
	}
	int	find_first_of(const TCHAR * str)
	{
		return m_str.Find(str);
	}
	int Compare(LPCTSTR lpsz) const   
	{
		return m_str.Compare(lpsz);
	}
	bool Contains(const TCHAR * str)
	{
		return m_str.Find(str) != -1;
	}
	void Remove(int index, int count)
	{
		m_str.Delete(index,count);
	}
	void Trim()
	{
		m_str.TrimLeft();
		m_str.TrimRight();
	}
	BOOL __cdecl Printf(LPCTSTR lpszFormat, ...)
	{
		va_list argList;
		va_start(argList, lpszFormat);
		return m_str.Format(lpszFormat,argList);
	}
	friend vfc_string __stdcall operator +(const vfc_string& string1, const vfc_string& string2);
	friend vfc_string __stdcall operator +(const vfc_string& string, TCHAR ch);
	friend vfc_string __stdcall operator +(TCHAR ch, const vfc_string& string);
#ifdef _UNICODE
	friend vfc_string __stdcall operator +(const vfc_string& string, char ch);
	friend vfc_string __stdcall operator +(char ch, const vfc_string& string);
#endif
	friend vfc_string __stdcall operator +(const vfc_string& string, LPCTSTR lpsz);
	friend vfc_string __stdcall operator +(LPCTSTR lpsz, const vfc_string& string);
public:
	CString	 m_str;
};

inline bool __stdcall operator ==(const vfc_string& s1, const vfc_string& s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(const vfc_string& s1, LPCTSTR s2)
{ return s1.Compare(s2) == 0; }

inline bool __stdcall operator ==(LPCTSTR s1, const vfc_string& s2)
{ return s2.Compare(s1) == 0; }

inline bool __stdcall operator !=(const vfc_string& s1, const vfc_string& s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(const vfc_string& s1, LPCTSTR s2)
{ return s1.Compare(s2) != 0; }

inline bool __stdcall operator !=(LPCTSTR s1, const vfc_string& s2)
{ return s2.Compare(s1) != 0; }

inline bool __stdcall operator <(const vfc_string& s1, const vfc_string& s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(const vfc_string& s1, LPCTSTR s2)
{ return s1.Compare(s2) < 0; }

inline bool __stdcall operator <(LPCTSTR s1, const vfc_string& s2)
{ return s2.Compare(s1) > 0; }

inline bool __stdcall operator >(const vfc_string& s1, const vfc_string& s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(const vfc_string& s1, LPCTSTR s2)
{ return s1.Compare(s2) > 0; }

inline bool __stdcall operator >(LPCTSTR s1, const vfc_string& s2)
{ return s2.Compare(s1) < 0; }

inline bool __stdcall operator <=(const vfc_string& s1, const vfc_string& s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(const vfc_string& s1, LPCTSTR s2)
{ return s1.Compare(s2) <= 0; }

inline bool __stdcall operator <=(LPCTSTR s1, const vfc_string& s2)
{ return s2.Compare(s1) >= 0; }

inline bool __stdcall operator >=(const vfc_string& s1, const vfc_string& s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(const vfc_string& s1, LPCTSTR s2)
{ return s1.Compare(s2) >= 0; }

inline bool __stdcall operator >=(LPCTSTR s1, const vfc_string& s2)
{ return s2.Compare(s1) <= 0; }


// CString "operator +" functions

inline vfc_string __stdcall operator +(const vfc_string& string1, const vfc_string& string2)
{
	return string1.m_str+string2.m_str;
}

inline vfc_string __stdcall operator +(const vfc_string& string, TCHAR ch)
{
	return string+ch;
}

inline vfc_string __stdcall operator +(TCHAR ch, const vfc_string& string)
{
	return ch+string;
}

#ifdef _UNICODE
inline vfc_string __stdcall operator +(const vfc_string& string, char ch)
{
	return string + (TCHAR)ch;
}

inline vfc_string __stdcall operator +(char ch, const vfc_string& string)
{
	return (TCHAR)ch + string;
}
#endif // _UNICODE

inline vfc_string __stdcall operator +(const vfc_string& string, LPCTSTR lpsz)
{
	return string+lpsz;
}

inline vfc_string __stdcall operator +(LPCTSTR lpsz, const vfc_string& string)
{
	return lpsz+string;
}


static LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
	while(p1 != NULL && *p1 != NULL)
	{
		LPCTSTR p = p2;
		while(p != NULL && *p != NULL)
		{
			if(*p1 == *p)
				return ::CharNext(p1);
			p = ::CharNext(p);
		}
		p1 = ::CharNext(p1);
	}
	return NULL;
}

template <typename ArrayT>
inline void StringToArray(LPCTSTR text, ArrayT & array, LPCTSTR separator = _T(""), bool trimSpaces = true)
{
    CString search = text;
    int seplen = CString(separator).GetLength();
    while (true)
    {
        int idx = search.Find(separator);
        if (idx == -1)
        {
            if (trimSpaces)
            {
                search.TrimLeft();
                search.TrimRight();
            }
            if (!search.IsEmpty())
                array.Add(search);
            break;
        }
        CString part = search.Left(idx);
        search.Delete(0, idx + seplen);
        if (trimSpaces)
        {
            part.TrimLeft();
            part.TrimRight();
        }
        if (!part.IsEmpty())
            array.Add(part);
    }
}


class wxStringTokenizer
{
public:
	wxStringTokenizer(const vfc_string & str, const vfc_string & szTokens)
	{
		StringToArray(str,m_array,szTokens);
		m_index = 0;
	}
	vfc_string GetNextToken()
	{
		if (m_index < 0 || m_index >= m_array.GetSize())
			return _T("");
		return m_array[m_index++];
	}
	bool HasMoreTokens()
	{
		return m_index < m_array.GetSize();
	}
protected:
	CSimpleArray<CString>	m_array;
	int						m_index;
};


typedef vfc_string	wxString;
typedef TCHAR wxChar;

#define wxEmptyString _T("")
#define wxNOT_FOUND	  -1

inline vfc_string cbC2U(const char * buf)
{
	USES_CONVERSION;
	return vfc_string(A2T((char*)buf));
}
/*
inline int cbC2U(const char * buf, vfc_string * pstr)
{
	unsigned int len = strlen(buf);
	wchar_t * data = pstr->m_str.GetBufferSetLength(len+1);
	data[0] = '\0';
	MultiByteToWideChar(CP_ACP, 0, buf, -1,data, len+1);
	pstr->m_str.ReleaseBuffer(-1);
	return len;
}
*/
inline unsigned int cbC2U(const char * buf, unsigned int buf_len, vfc_string * pstr)
{
	wchar_t * data = pstr->m_str.GetBufferSetLength(buf_len+1);
	data[0] = L'\0';
	buf_len = MultiByteToWideChar(CP_ACP, 0, buf, buf_len,data, buf_len+1);
	data[buf_len] = _T('\0');
	pstr->m_str.ReleaseBuffer(-1);
	return buf_len;
}


class LoaderBase
{
public:
	virtual char * GetData()
	{
		return NULL;
	}
	virtual unsigned int GetLength()
	{
		return 0;
	}
	virtual void Sync()
	{
	}
	virtual void Release()
	{
	}
};

class wxFile
{
public:
	wxFile(const TCHAR * file) : m_file(file)
	{
	}
	CString m_file;
};

#define wxStrlen	_tcsclen
#define wxIsalpha	_istalpha
#define wxIsalnum	_istalnum
#define wxIsdigit	_istdigit

class ConfigManagerContainer
{
public:
	typedef std::map<wxString,wxString> StringToStringMap;
};

inline BOOL wxFileExists(const TCHAR * filename)
{
	return TRUE;
}

inline BOOL cbRead(wxFile & file, vfc_string & str)
{
	return FALSE;
}

inline DWORD wxNewId()
{
	static int i = 2000;
	return i++;
}

class cbThreadedTask
{
public:
	bool TestDestroy()
	{
		return false;
	}
};

class Parser
{
public:
	vfc_string GetFullFileName(const TCHAR * Filename,const TCHAR * filename,bool isGlobal)
	{
		return _T("");
	}
	void OnParseFile(const TCHAR * filename, int flag)
	{
	}
};

class wxInputStream
{
public:
	DWORD Read(TCHAR * str, DWORD len)
	{
		return len;
	}
};

class wxOutputStream
{
public:
	DWORD Write(TCHAR * ch, DWORD len)
	{
		return 0;
	}
};

class wxCriticalSection
{
public:
	void Enter()
	{
	}
	void Leave()
	{
	}
};

class wxCriticalSectionLocker
{
public:
	wxCriticalSectionLocker(wxCriticalSection & cs)
	{
	}
};

inline CString get_realargs(const TCHAR * args)
{
	int index = 0;
	const TCHAR * ptr = args;
	CString str;
	CString word;
	bool skip = false; //next char skip
	bool sym = false;  //cur char symbol
	while(*ptr != _T('\0'))
	{
		switch(*ptr)
		{
		case _T(' '):
			if (word == _T("const") || word == _T("volatile") )
				skip = false;
			else
				skip = true;
			word = _T("");
			sym = false;
			break;
		case _T('*'):
		case _T('&'):
			word = _T("");
			skip = true;
			sym = true;
			break;
		case _T(':'):
			skip = false;
			sym = true;
			break;
		case _T('['):
			while (*ptr != _T('\0') && *(ptr) != _T(']'))
			{
				if (*ptr != _T(' '))
					str += *ptr;
				*ptr++;
			}
			skip = true;
			sym = true;
			break;
		case _T('<'):
			while (*ptr != _T('\0') && *(ptr) != _T('>'))
			{
				if (*ptr != _T(' '))
					str += *ptr;
				*ptr++;
			}
			skip = true;
			sym = true;
			break;
		case _T(','):
		case _T(')'):
		case _T('('):
			word = _T("");
			sym = true;
			skip = false;
			break;
		default:
			sym = false;
		}
		if (!skip || sym)
		{
			str += *ptr;
			if (::_istalnum(*ptr))
			{
				word += *ptr;
			}
		}

		if (sym == true && skip == false)
		{
			while (*ptr != _T('\0') && *(ptr+1) == _T(' '))
			{
				ptr++;
			}
		}
		ptr++;
	}

	return str;
}


#endif