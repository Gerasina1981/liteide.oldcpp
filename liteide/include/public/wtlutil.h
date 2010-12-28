#ifndef _WTLUTIL_H_
#define _WTLUTIL_H_

//#include <algorithm>
//#include <vector>

template <typename TVector>
inline void clear(TVector * ar)
{
	for (size_t i = 0; i < ar->size(); i++)
	{
		delete (*ar)[i];
	}
	ar->clear();
}

template<typename TVector, typename T>
inline bool remove_at(TVector * ar, T t)
{
    typedef typename TVector::iterator It;
	for (It it = ar->begin(); it != ar->end(); it++)
	{
		if (*it == t)
		{
			ar->erase(it);
			return true;
		}
	}
	return false;
}

inline size_t copy_string(wchar_t * target, wchar_t * source, size_t cch)
{
	size_t max = cch >= wcslen(source) ? cch : wcslen(source);
	wcsncpy(target,source,max);
	return max;
}

inline size_t copy_string(wchar_t * target, CString & source, size_t cch)
{
	size_t max = cch >= source.GetLength() ? cch : source.GetLength();
	wcsncpy(target,source,max);
	return max;
}

inline CString get_file_root(const CString & filename)
{
	int i = filename.ReverseFind(_T('\\'));
	if (i != -1)
	{
		return filename.Left(i+1);
	}
	return _T("");
}

inline CString get_file_title(const CString & filename)
{
	int i = filename.ReverseFind(_T('\\'));
	if (i != -1)
	{
		return filename.Right(filename.GetLength()-i-1);
	}
	return filename;
}

inline CString get_file_ext(const CString & filename)
{
	int i = filename.ReverseFind(_T('.'));
	if (i != -1)
	{
		return filename.Right(filename.GetLength()-i);
	}
	return _T("");
}

inline CString GetModulePath(HMODULE hMod)
{
	CString path;
	//取得程序所在路径
	TCHAR exeFullPath[MAX_PATH] = {0};
	::GetModuleFileName(hMod,exeFullPath,MAX_PATH);
	CString strPath = exeFullPath;
	int r = strPath.ReverseFind(_T('\\'));
	if (r != -1)
	{
		path = strPath.Left(r);
	}
	return path;
}


#endif
