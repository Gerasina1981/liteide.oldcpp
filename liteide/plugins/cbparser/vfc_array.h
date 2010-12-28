#ifndef _VFC_ARRAY_H_
#define _VFC_ARRAY_H_

#include "vfc_token.h"

#include <vector>

template <typename T>
struct vector_vfc_array_t : i_vfc_array_t<T>
{
	virtual ~vector_vfc_array_t()
	{
		clear();
	}
	virtual void push_back(T t)
	{
		array.push_back(t);
	}
	virtual T at(int index)
	{
		return array.at(index);
	}
	virtual size_t size()
	{
		return array.size();
	}
	virtual void clear()
	{
		array.clear();
	}
	T & operator[](int index)
	{
		return array[index];
	}
	std::vector<T>	array;
};

template <typename T>
struct simple_vfc_array_t : i_vfc_array_t<T>
{
	virtual ~simple_vfc_array_t()
	{
		clear();
	}
	virtual void push_back(T t)
	{
		array.Add(t);
	}
	virtual T at(int index)
	{
		return array[index];
	}
	virtual size_t size()
	{
		return array.GetSize();
	}
	virtual void clear()
	{
		array.RemoveAll();
	}
	T & operator[](int index)
	{
		return array[index];
	}
	CSimpleArray<T>	array;
};

#ifdef _VFC_USE_SIMPLE
#define vfc_array						simple_vfc_array_t
typedef simple_vfc_array_t<int>			vfc_int_array;
typedef simple_vfc_array_t<vfc_token*>	vfc_token_array;
#else
#define vfc_array						vector_vfc_array_t
typedef vector_vfc_array_t<int>			vfc_int_array;
typedef vector_vfc_array_t<vfc_token*>	vfc_token_array;
#endif	//_VFC_USE_SIMPLE

inline void string_to_array(LPCTSTR text, vfc_array<CString> & array, LPCTSTR separator = _T(""), bool trimSpaces = true)
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
                array.push_back(search);
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
            array.push_back(part);
    }
}

#endif