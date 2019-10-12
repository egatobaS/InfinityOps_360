#include "Addresses.h"

bool IsMultiplayer() {
	return *(unsigned int*)0x822E248C == 0x48183E65;
}

Pointer* ui_mapname = (Pointer*)(IsMultiplayer() ? 0x83911050 : 0x832D3EF8);
Pointer* g_gametype = (Pointer*)(IsMultiplayer() ? 0x83036CD8 : 0x8376A63C);

XAssetHeader(__cdecl * DB_FindXAssetHeader)(int assetType, const char* assetName, bool errorIfMissing, int waitTime) = (XAssetHeader(__cdecl *)(int, const char*, bool, int))(IsMultiplayer() ? 0x82286370 : 0x821F0050);

const char* (__cdecl * Com_GetExtensionSubString)(const char* fileName) = (const char*(__cdecl *)(const char*))(IsMultiplayer() ? 0x823EE110 : 0x823851A0);

void*(__cdecl * Hunk_AllocateTempMemoryHigh)(unsigned int length) = (void*(__cdecl *)(unsigned int))(IsMultiplayer() ? 0x823DCEA8 : 0x823733E8);