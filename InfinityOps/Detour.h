#pragma once
#include <stdio.h>
#include <xtl.h>
extern "C" {
	BOOL MmIsAddressValid(void* pAddress);

	void * _ReturnAddress(void);
}

class Detour {
private:
	static byte HookSection[0x10000];
	static DWORD HookCount;

	DWORD dwAddress,
		*pAddress,
		*dwStubAddress,
		dwDestination,
		dwRestoreInstructions[4],
		dwCurrentInstructions[4];

	DWORD AllocStub();
	DWORD ResolveBranch(DWORD dwInstruction, DWORD dwBranchAddress);
	void PatchInJump(DWORD dwAddress, DWORD dwDestination, bool Linked);

public:
	Detour(DWORD dwAddress, DWORD dwDestination);
	~Detour();

	void* (*CallOriginal)(...);
};