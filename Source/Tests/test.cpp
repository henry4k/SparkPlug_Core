#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <SparkPlug/Vector.h>
TEST_CASE("Math/Vector", "Wuff")
{
	using namespace SparkPlug;

	vec3f a(1,2,3);
	vec3f b = a;

	CHECK( a == b );
}

#include <SparkPlug/Math.h>
TEST_CASE("Math/PowerOfTwo", "Wuff")
{
	using namespace SparkPlug;
	
	CHECK( IsPowerOfTwo(2048) );
	CHECK( !IsPowerOfTwo(2049) );
	
	CHECK( PowerOfTwoAbove(2047) == 2048 );
	CHECK( PowerOfTwoAbove(2048) == 2048 );
	CHECK( PowerOfTwoAbove(2049) != 2048 );
	
	CHECK( PowerOfTwoBelow(2047) != 2048 );
	CHECK( PowerOfTwoBelow(2048) == 2048 );
	CHECK( PowerOfTwoBelow(2049) == 2048 );
}

#include <SparkPlug/Reference.h>
class Foo : public SparkPlug::ReferenceCounted
{
public:
	Foo( int x_ ) : x(x_)
	{
	}
	
	virtual ~Foo() {}
	
	virtual int getValue()
	{
		return x;
	}
	
	int x;
};

class Bar : public Foo
{
public:
	Bar( int x_, int y_ ) : Foo(x_), y(y_)
	{
	}
	
	virtual ~Bar() {}
	
	int getValue()
	{
		return y;
	}
	
	int y;
};

TEST_CASE("Reference/Core", "Wuff")
{
	using namespace SparkPlug;
	
	StrongRef<Foo> a( new Foo(42) );
	CHECK(a->x == 42);
	CHECK(a == a);
	
	StrongRef<Bar> b;
	REQUIRE(!b);
	b = a;
	CHECK(b);
	
	a = b;
	CHECK(a == b);
	
	StrongRef<Bar> c(a);
	CHECK(c);
	
	WeakRef<Foo> d;
	REQUIRE(!d);
	d = a;
	REQUIRE(d);
	CHECK(d->x == 42);
}

TEST_CASE("Reference/Inheritance", "Wuff")
{
	using namespace SparkPlug;
	
	StrongRef<Bar> a( new Bar(1,2) );
	StrongRef<Foo> b = a;
	
	CHECK(a->x == b->x);
	CHECK(a == b);
	CHECK(a->getValue() == 2);
	CHECK(b->getValue() == 2);
	
	a = b;
}

#include <SparkPlug/Unicode.h>
TEST_CASE("Char/Unicode", "Wuff")
{
	using namespace SparkPlug;
	
	Utf8 input[] = { 0xF0, 0x9D, 0x84, 0x9E }; // Clef
	int inputLength = sizeof(input)/sizeof(Utf8);

	Utf8 zeroTerminated[] = { 0xF0, 0x9D, 0x84, 0x9E, 0 }; // Clef
	REQUIRE( StringLength(zeroTerminated, 4) == 1 );

	std::vector<Utf8>  bufUtf8(input, input+inputLength);
	std::vector<Utf16> bufUtf16;
	std::vector<Utf32> bufUtf32;

#define CHECK_CONV(F, T) \
	bufUtf##T.clear(); \
	REQUIRE( ConvertChars( CharConversionFlags_Strict, bufUtf##F.size(), &bufUtf##F[0], &bufUtf##T ) == CharConversionResult_Success ); \
	Log("%d => %d", bufUtf##F.size(), bufUtf##T.size() );
	
	CHECK_CONV(8,16);
	CHECK_CONV(8,32);

	CHECK_CONV(16,8);
	CHECK_CONV(16,32);

	CHECK_CONV(32,8);
	CHECK_CONV(32,16);
#undef CHECK_CONV

	REQUIRE( StringLength(&bufUtf8[0], bufUtf8.size()) == 1 );
	REQUIRE( StringLength(&bufUtf16[0], bufUtf16.size()) == 1 );
	REQUIRE( StringLength(&bufUtf32[0], bufUtf32.size()) == 1 );
}

#include <SparkPlug/String.h>
TEST_CASE("Char/String", "Wuff")
{
	using namespace SparkPlug;

	String a = "Hello";
	REQUIRE(a.size() == 5);

	String b = a;
	REQUIRE(b.size() == 5);
}

