
#include "cgUtil.h"
#include "cgMemory.h"
#include "cgLexer.h"
#include "cgParser.h"

void cgTest();

int main(int argc, char** argv) {
	
	cgLexer lexer("a(1,2.3);");
	cgTest();
}