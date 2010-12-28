#define TPL_USE_AUTO_ALLOC
#include <cppmodel/Lex.h>
#include <cppmodel/Lex.cpp>
#include <stdext/FileBuf.h>

// -------------------------------------------------------------------------

# /* aaa */ if 1

/*
class A { public: static void f() {} } g()
{
	A a;
	return a;
}
*/

template <class T> class TTT {};

class Base1 {};
class Base2 {};
class Base3 {};

template <template <class T> class A, typename B, bool f = (2 < 0), typename B* (*g)() = NULL>
struct Foo // Foo comment
	: private Base1, protected Base2, Base3
{
	/* no body */
};

class Foo2;

int* ggg() { return NULL; }

Foo<TTT, int, false, ggg> foo;

#endif

// -------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <C++ Source File>\n", argv[0]);
		return 0;
	}

	const NS_STDEXT::FileBuf source(argv[1]);
	if (!source.good())
	{
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		return -1;
	}
	
	dom::Allocator alloc;
	dom::Document doc(alloc);

	source >> cppm_skip_[ cppm_globals() ]/doc;

	std::OutputLog log;
	json_print(alloc, log, doc);
	return 0;
}

// -------------------------------------------------------------------------

