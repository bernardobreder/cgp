#include "cgTest.h"

#define CGASSERT_RETRAY 32 * 1024 * 1024
#define CGASSERT_FAIL(S) { std::cout << "FAIL at " << __FILE__ << ":" << __LINE__ << ": " << S << "\n"; return 1; }
#define CGASSERT_NOTNULL(V) if (!(V)) { CGASSERT_FAIL("Expected not null"); }
#define CGASSERT_TRUE(V) if (!(V)) { CGASSERT_FAIL("Expected true"); }
#define CGASSERT_FALSE(V) if (V) { CGASSERT_FAIL("Expected false"); }
#define CGASSERT_EQUAL(E,V) if ((E)!=(V)) { CGASSERT_FAIL("Expected equal"); }
#define CGASSERT_EQUAL_STR(E,V) if (strcmp(E,V)) { char msg[1024]; sprintf(msg, "Expected '%s' but found '%s'", E, V); CGASSERT_FAIL(msg); }
#define CGASSERT_EQUAL_STRN(E,V,N) if (strlen(E) != N || strncmp(E,V,N)) { char msg[1024]; sprintf(msg, "Expected '%s' but found '%s'", E, V); CGASSERT_FAIL(msg); }
#define cgTestExecute(F) { printf(""#F"..."); testTime = clock(); CULTest_BeforeTest(); if (!F()) { int i, max = CGASSERT_RETRAY; for (i = 0 ; i < max; i++) CULTest_BeforeTest(); printf("OK in %.3lf milisegs\n", ((double)(clock() - testTime) / max)); } CULTest_AfterTest(); }

//
//static bool testLexicalEmpty() {
//	CULLexical lexical("");
//	lexical.next();
//	CGASSERT_TRUE(lexical.eof());
//	for (int n = 0; n < 10; n++) {
//		lexical.next();
//		CGASSERT_TRUE(lexical.eof());
//	}
//	return 0;
//}
//
//static bool testLexicalA() {
//	CULLexical lexical("a");
//	CGASSERT_EQUAL(CUL_LEXICAL_ID, lexical.look());
//	CGASSERT_EQUAL_STR("a", lexical.token()->_word);
//	lexical.next();
//	CGASSERT_TRUE(lexical.eof());
//	for (int n = 0; n < 10; n++) {
//		lexical.next();
//		CGASSERT_TRUE(lexical.eof());
//	}
//	return 0;
//}
//
//static bool testLexicalFull() {
//	CULLexical lexical("\ta(1,2.3);\n\na\n\t+\t\n1\t");
//	CGASSERT_EQUAL(CUL_LEXICAL_ID, lexical.look());
//	CGASSERT_EQUAL_STRN("a", lexical.token()->_word, lexical.token()->_len);
//	lexical.next();
//	CGASSERT_EQUAL('(', lexical.look());
//	lexical.next();
//	CGASSERT_EQUAL(CUL_LEXICAL_NUMBER, lexical.look());
//	CGASSERT_EQUAL_STRN("1", lexical.token()->_word, lexical.token()->_len);
//	lexical.next();
//	CGASSERT_EQUAL(',', lexical.look());
//	lexical.next();
//	CGASSERT_EQUAL(CUL_LEXICAL_NUMBER, lexical.look());
//	CGASSERT_EQUAL_STRN("2.3", lexical.token()->_word, lexical.token()->_len);
//	lexical.next();
//	CGASSERT_EQUAL(')', lexical.look());
//	lexical.next();
//	CGASSERT_EQUAL(';', lexical.look());
//	lexical.next();
//	CGASSERT_EQUAL(CUL_LEXICAL_ID, lexical.look());
//	CGASSERT_EQUAL_STRN("a", lexical.token()->_word, lexical.token()->_len);
//	lexical.next();
//	CGASSERT_EQUAL('+', lexical.look());
//	lexical.next();
//	CGASSERT_EQUAL(CUL_LEXICAL_NUMBER, lexical.look());
//	CGASSERT_EQUAL_STRN("1", lexical.token()->_word, lexical.token()->_len);
//	lexical.next();
//	CGASSERT_TRUE(lexical.eof());
//	for (int n = 0; n < 10; n++) {
//		lexical.next();
//		CGASSERT_TRUE(lexical.eof());
//	}
//	return 0;
//}
//
//static clock_t testTime;
//
//void cgTest_BeforeTests() {
//	std::cout << "Initing all Tests!\n";
//}
//
//void cgTest_AfterTests() {
//	std::cout << "Finished all Tests!\n";
//}
//
//void cgTest_BeforeTest() {
//
//}
//
//void cgTest_AfterTest() {
//
//}
//
//void cgTest::execute() {
//	cgTestExecute(testLexicalEmpty);
//	cgTestExecute(testLexicalA);
//	cgTestExecute(testLexicalFull);
//}
//#else
//void CULTest() {}
//#endif

cgTest::cgTest()
{
}

cgTest::~cgTest()
{
}
