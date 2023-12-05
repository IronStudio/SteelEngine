#pragma once

#include <iostream>
#include <map>
#include <string>

static std::map<std::string, bool (*)()> functions;

void addTestToList(const std::string &testName, bool (*func)())
{
	functions[testName] = func;
}


#define SE_TEST_BEGIN(testName) bool testName() {\
	addTestToList(#testName, testName);
#define SE_TEST_END(testName) return true;}

#define SE_TEST(expr, cond) expr; if (!(cond)) {\
	std::cout << "File " << __FILE__ << ", line " << __LINE__ << ", test " << __func__ << " : expression `" << #expr << "` failed condition `" << #cond << "`" << std::endl;\
	return false;}
#define SE_TEST_CONDITION(cond) if (!(cond)) {std::cout << "File " << __FILE__ << ", line " << __LINE__ << ", test " << __func__ << " : failed condition `" << #cond << "`" << std::endl;\
	return false;}


