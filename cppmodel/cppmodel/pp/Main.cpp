/* -------------------------------------------------------------------------
// CppModel: C++ DOM Model
// 
// Module: pp/Main.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2009-3-26 19:41:58
// 
// $Id: Main.cpp 619 2008-06-01 16:00:35Z xushiweizh $
// -----------------------------------------------------------------------*/

#define TPL_USE_AUTO_ALLOC
#include <tpl/c/Lex.h>
#include <stdext/FileBuf.h>

// =========================================================================

using namespace NS_STDEXT;
using namespace NS_TPL;

// -------------------------------------------------------------------------

#define cppm_pp_skip()				(*gr(c_token()))

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <C++ Source File>\n", argv[0]);
		return 0;
	}

	const FileBuf source(argv[1]);
	if (!source.good())
	{
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		return -1;
	}

	String cmd, token;
	
	source >> cpp_skip_
		[
			*gr(
				'#' + c_pp_skip_
				[
					(c_symbol()/assign(cmd)/TPL_INFO("CMD"))
					[
						case_("define")
						[
							cppm_pp_skip()/TPL_INFO("DEFINE")
						],
						case_("include")
						[
							cppm_pp_skip()/TPL_INFO("INCLUDE")
						],
						default_()
						[
							cppm_pp_skip()/TPL_INFO("UNKNOWN_PP")
						]
					]
				]
			|
				(
					(c_token()/assign(token)/TPL_INFO("TOKEN"))
					[
						case_((int)CTokens::symbol)
						[
							eps()/TPL_INFO("  <-- SYMBOL")
						],
						default_()
						[
							eps()/TPL_INFO("  <-- UNKNOWN_TOKEN")
						]
					]
				)
			)
		];

	return 0;
}

// =========================================================================
