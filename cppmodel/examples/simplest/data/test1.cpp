template <class A, typename B>
struct Foo // Foo comment
  : private Base1, protected Base2, Base3
#if 1
{
#endif
	/* no body */
};

class Foo2;
