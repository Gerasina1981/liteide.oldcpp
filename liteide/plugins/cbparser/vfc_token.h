#ifndef _VFC_TOKEN_H_
#define _VFC_TOKEN_H_

struct vfc_token;
struct vfc_token_tree;

enum vfc_token_enum
{
	vt_Type = 1000,
	vt_ActualType,
	vt_Id,
	vt_Name,
	vt_Args,
	vt_RealArgs,
	vt_AncestorsString,
	vt_File,
	vt_Line,
	vt_ImplFile,
	vt_ImplLine,
	vt_ImplLineStart,
	vt_ImplLineEnd,
	vt_Scope,
	vt_TokenKind,
	vt_IsOperator,
	vt_IsLocal,
	vt_IsTemp,
	vt_IsConst,
	vt_ParentIndex,
	vt_Index,
	vt_Children,
	vt_Ancestors,
	vt_DirectAncestors,
	vt_Descendants,
	vt_FileName,
	vt_ImplFileName
};

enum vfc_token_scope
{
    vs_Undefined = 0,
    vs_Private,
    vs_Protected,
    vs_Public
};

enum vfc_token_update
{
	vu_Normal = 0,
	vu_Addnew,
	vu_Remove,
	vu_Update
};

enum vfc_token_kind
{
    vk_Class         = 0x0001,
    vk_Namespace     = 0x0002,
    vk_Typedef       = 0x0004, // typedefs are stored as classes inheriting from the typedef'd type (taking advantage of existing inheritance code)
    vk_Constructor   = 0x0008,
    vk_Destructor    = 0x0010,
    vk_Function      = 0x0020,
    vk_Variable      = 0x0040,
    vk_Enum          = 0x0080,
    vk_Enumerator    = 0x0100,
    vk_Preprocessor  = 0x0200,
	vk_Mapping	    = 0x0400, 	//vfc add the enum

    // convenient masks
    vk_AnyContainer  = vk_Class | vk_Namespace | vk_Typedef,
    vk_AnyFunction   = vk_Function | vk_Constructor | vk_Destructor,

    vk_Undefined     = 0xFFFF,
};

template <typename T>
struct i_vfc_array_t
{
	virtual ~i_vfc_array_t() {}
	virtual void push_back(T t) = 0;
	virtual T at(int index) = 0;
	virtual size_t size() = 0;
	virtual void clear() = 0;
};

typedef i_vfc_array_t<int>			i_vfc_int_array;
typedef i_vfc_array_t<vfc_token*>	i_vfc_token_array;

struct vfc_token
{
	virtual ~vfc_token() {}
	virtual size_t	get_string(int vt, wchar_t * text, size_t cch) = 0;
	virtual const unsigned int	get_uint(int vt) = 0;
	virtual bool get_token_idx(int vt, i_vfc_int_array * pidx) = 0;
	virtual vfc_token_tree *	get_tree() = 0;
	virtual vfc_token	*	get_parent() = 0;
	virtual int get_update() = 0;
	virtual void set_update(int value) = 0;
	virtual void add_child(int id) = 0;
	virtual void remove_child(int id) = 0;
};

struct vfc_token_tree
{
	virtual ~vfc_token_tree() {}
	virtual vfc_token *		 get_token(int id) = 0;
	virtual int add_token(int parent_id, const wchar_t * name, const wchar_t * type, const wchar_t * args, const wchar_t * body, int scope, int kind) = 0;
	virtual bool get_token_array(i_vfc_token_array * par) = 0;
	virtual bool get_token_files(i_vfc_int_array * par) = 0;
	virtual bool get_token_file_idx(int idx, i_vfc_int_array * par) = 0;
	virtual const wchar_t * get_token_file_name(int idx) = 0;
};

#endif