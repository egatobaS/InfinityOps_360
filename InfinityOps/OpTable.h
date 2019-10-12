#pragma once
#include <stdio.h>
#include <xtl.h>
#include <string>

struct OpTable {
	unsigned int MapHash;
	unsigned int GametypeHash;
	unsigned int OpCount;
	unsigned int FunctionCount;
};

extern OpTable*(__cdecl * OP_GetValue)(const char* mapname, const char* gametype);
