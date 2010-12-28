/* -------------------------------------------------------------------------
// CppModel: C++ DOM Model
// 
// Module: cppmodel/Lex.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Lex.h 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/
#ifndef CPPMODEL_LEX_H
#define CPPMODEL_LEX_H

#ifndef TPL_C_LEX_H
#include <tpl/c/Lex.h>
#endif

#ifndef TPL_REGEX_DOM_H
#include <tpl/regex/DOM.h>
#endif

// =========================================================================

using namespace tpl;

typedef DOM<> dom;

extern dom::NodeMark tagGlobals;

extern dom::NodeMark tagTemplate;
	extern dom::NodeMark tagParams;
		extern dom::NodeMark tagTypename;
			extern dom::Mark tagName;
			extern dom::Mark tagDefault;
		extern dom::NodeMark tagVariable;
			extern dom::NodeMark tagType;
			extern dom::Mark tagName;
			extern dom::Mark tagDefault;
		extern dom::NodeMark tagTemplateTypename;
			extern dom::NodeMark tagTemplate;
			extern dom::Mark tagName;
			extern dom::Mark tagDefault;

extern dom::NodeMark tagClass;
	extern dom::Mark tagClassKeyword;
	extern dom::Mark tagName;
	extern dom::NodeMark tagBaseClasses;
		extern dom::Mark tagAccess;
		extern dom::Mark tagName;
	extern dom::NodeMark tagBody;

// -------------------------------------------------------------------------
// basic

#define cppm_eq2(s1, s2)			(eq(s1) || eq(s2))
#define cppm_eq3(s1, s2, s3)		(eq(s1) || eq(s2) || eq(s3))

// -------------------------------------------------------------------------
// pp - preprocessor

#define cppm_keyword_define()		gr(c_symbol()/eq("define"))

#define cppm_pp_skip()				(*gr(c_token()))

#define cppm_pp_define_body()		
#define cppm_pp_define()			(cppm_keyword_define() + cppm_pp_define_body())

#define cppm_pp_body()				c_pp_skip_[cppm_pp_skip()]
#define cppm_pp()					('#' + cppm_pp_body())

// -------------------------------------------------------------------------
// skipper

#define cppm_skip_					(cpp_skip() % cppm_pp())

// -------------------------------------------------------------------------
// template

#define cppm_keyword_template()		gr(c_symbol()/eq("template"))
#define cppm_keyword_typename()		gr(c_symbol()/eq("typename"))
#define cppm_keyword_typename2()	gr(c_symbol()/cppm_eq2("class", "typename"))

#define cppm_templpara_typename()	((cppm_keyword_typename2() + c_symbol()/tagName)/tagTypename)
#define cppm_templpara()			cppm_templpara_typename()
#define cppm_templparas()			('<' + cppm_templpara()/tagParams % ',' + '>')
#define cppm_template()				((cppm_keyword_template() + cppm_templparas())/tagTemplate)

// -------------------------------------------------------------------------
// class

#define cppm_keyword_class()		gr(c_symbol()/cppm_eq2("class", "struct")/tagClassKeyword)
#define cppm_access()				gr(c_symbol()/cppm_eq3("private", "public", "protected")/tagAccess)

#define cppm_class_header()			(cppm_keyword_class() + c_symbol()/tagName)

#define cppm_baseclass()			(!cppm_access() + c_symbol()/tagName)
#define cppm_baseclasses()			(':' + cppm_baseclass()/tagBaseClasses % ',')

#define cppm_class_body()			((gr('{') + '}')/tagBody)
#define cppm_class_imp()			(!cppm_baseclasses() + cppm_class_body())

#define cppm_class()				((cppm_class_header() + !cppm_class_imp())/tagClass)

// -------------------------------------------------------------------------
// global sentence

#define cppm_global()				(!cppm_template() + cppm_class() +  ';')
#define cppm_globals()				(*(cppm_global()/tagGlobals))

// =========================================================================
// $Log: $

#endif /* CPPMODEL_LEX_H */
