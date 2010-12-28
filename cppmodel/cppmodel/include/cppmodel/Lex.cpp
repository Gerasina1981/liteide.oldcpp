/* -------------------------------------------------------------------------
// CppModel: C++ DOM Model
// 
// Module: cppmodel/Lex.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Lex.cpp 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/

#ifndef CPPMODEL_LEX_H
#include "Lex.h"
#endif

// =========================================================================

dom::NodeMark tagGlobals("globals", true);

dom::NodeMark tagTemplate("template");
dom::NodeMark tagTypename("typename");
dom::NodeMark tagParams("params", true);

dom::NodeMark tagClass("class");
dom::NodeMark tagBaseClasses("bases", true);
dom::NodeMark tagBody("body");

dom::Mark tagName("name");
dom::Mark tagAccess("access");
dom::Mark tagClassKeyword("type");

// =========================================================================
// $Log: $
