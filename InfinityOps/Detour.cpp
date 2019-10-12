#include "Detour.h"

byte Detour::HookSection[0x10000] = { 0 };
DWORD Detour::HookCount = 0;

DWORD Detour::AllocStub() {
	PDWORD dwStartStub = (PDWORD)(&this->HookSection[this->HookCount * 0x80]);
	this->HookCount++;
	return (DWORD)dwStartStub;
}

DWORD Detour::ResolveBranch(DWORD dwInstruction, DWORD dwBranchAddress) {
	DWORD dwCurrentOffset = dwInstruction & 0x3FFFFFC;
	if (dwCurrentOffset & (1 << 25)) dwCurrentOffset |= 0xFC000000;
	return dwBranchAddress + dwCurrentOffset;
}

void Detour::PatchInJump(DWORD dwAddress, DWORD dwDestination, bool Linked) {
	DWORD* dwInstruction = (DWORD*)dwAddress;
	dwInstruction[0] = 0x3C000000 + ((dwDestination >> 16) & 0xFFFF);
	dwInstruction[1] = 0x60000000 + (dwDestination & 0xFFFF);
	dwInstruction[2] = 0x7C0903A6;
	dwInstruction[3] = 0x4E800420 + (Linked ? 1 : 0);
}

Detour::Detour(DWORD dwAddress, DWORD dwDestination) {
	if (dwAddress == NULL) return;
	this->dwAddress = dwAddress;
	this->pAddress = (DWORD*)dwAddress;
	this->dwDestination = dwDestination;
	this->dwStubAddress = (DWORD*)this->AllocStub();
	this->CallOriginal = (void*(*)(...))this->dwStubAddress;
	memcpy(this->dwRestoreInstructions, (void*)(this->dwAddress), 0x10);

	DWORD dwAddressRelocation = (DWORD)(&this->pAddress[4]);
	this->dwStubAddress[0] = (0x3D600000 + (((dwAddressRelocation >> 16) & 0xFFFF) + ((dwAddressRelocation & 0x8000) ? 1 : 0)));
	this->dwStubAddress[1] = 0x396B0000 + (dwAddressRelocation & 0xFFFF);
	this->dwStubAddress[2] = 0x7D6903A6;

	unsigned int InstructionWrittenCount = 0;
	for (int i = 0; i < 4; i++) {
		if ((pAddress[i] & 0x48000003) == 0x48000001) {
			PatchInJump((unsigned int)&this->dwStubAddress[InstructionWrittenCount], (unsigned int)this->ResolveBranch(this->pAddress[i], (int)&this->pAddress[i]), true);
			InstructionWrittenCount += 4;
		}
		else {
			this->dwStubAddress[InstructionWrittenCount] = this->pAddress[i];
			InstructionWrittenCount++;
		}
	}

	PatchInJump((unsigned int)&this->dwStubAddress[InstructionWrittenCount], this->dwAddress + 0x10, false);
	PatchInJump(this->dwAddress, this->dwDestination, false);

	memcpy(this->dwCurrentInstructions, (void*)(this->dwAddress), 0x10);
}

Detour::~Detour() {
	if (MmIsAddressValid((void*)this->dwAddress))
		memcpy((void*)this->dwAddress, this->dwRestoreInstructions, 0x10);
}
