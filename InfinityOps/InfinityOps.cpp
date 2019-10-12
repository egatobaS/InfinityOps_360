#include "InfinityOps.h"

#pragma comment(lib, "xav")
extern "C" VOID XNotifyQueueUI( DWORD exnq, DWORD dwUserIndex, ULONGLONG qwAreas, PWCHAR displayText, PVOID contextData);

Detour* Scr_LoadRawFile_FastFile = nullptr;
Detour* RawFile_GetOpCodeChecksum = nullptr;

char* Scr_LoadRawFile_FastFileHook(int scriptInstance_t, const char* filename) {
	RawFile* rawFile = DB_FindXAssetHeader(0x25, filename, 1, -1).rawFile;
	char RawFileBuffer[255] = { 0 };
	CreateFilename(RawFileBuffer, filename);

	if (FileExists(RawFileBuffer)) {
		const char* fileExtension = Com_GetExtensionSubString(filename);
		if (strcmp(fileExtension, ".gsc") == 0 || strcmp(fileExtension, ".csc") == 0) {
			HANDLE hFile = CreateFile(RawFileBuffer, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) return (char*)Scr_LoadRawFile_FastFile->CallOriginal(scriptInstance_t, filename);

			DWORD FileLength = GetFileSize(hFile, NULL);
			DWORD BytesRead = 0;
			char* scriptBuffer = (char*)Hunk_AllocateTempMemoryHigh(FileLength + 1);
			memset(scriptBuffer, 0, FileLength + 1);

			ReadFile(hFile, scriptBuffer, FileLength, &BytesRead, NULL);
			CloseHandle(hFile);
			printf("[Infinity Ops]: Loaded Script: %s\n", filename);
			*(int*)((IsMultiplayer() ? 0x83E890F8 : 0x839AEEF8) + (scriptInstance_t * 0x10)) = 0;
			return scriptBuffer;
			return 0;
		}
		else {
			*(int*)((IsMultiplayer() ? 0x83E890F8 : 0x839AEEF8) + (scriptInstance_t * 0x10)) = 0;
			return rawFile->buffer;
		}
	}
	else return (char*)Scr_LoadRawFile_FastFile->CallOriginal(scriptInstance_t, filename);
}

void RawFile_GetOpCodeChecksumHook(int* checksumValues, int scriptInstance_t) {
	OpTable* Value = OP_GetValue(ui_mapname->DvarValue->current.string, g_gametype->DvarValue->current.string);
	
	if (!Value) {
		RawFile_GetOpCodeChecksum->CallOriginal(checksumValues, scriptInstance_t);
		printf("WARNING: The OpTable value for mapname \"%s\" on \"%s\" does not exist!\n\tUsers will not be able to join until the value has been accquired!\n\tThe values currently set are: 0x%X and 0x%X\n", ui_mapname->DvarValue->current.string, g_gametype->DvarValue->current.string, checksumValues[0], checksumValues[1]);
		return;
	}

	printf("[Infinity Ops]: OpTable value loaded!\n");

	checksumValues[0] = Value->OpCount;
	checksumValues[1] = Value->FunctionCount;
}

void ShowWelcomeMessage() {
	XNotifyQueueUI(14, 0, 2, L"Infinity Ops Loaded", 0);
}

BOOL __stdcall DllMain(HANDLE hHandle, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		printf("[Infinity Ops]: Infinity Ops Loaded\n");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShowWelcomeMessage, 0, 0, 0);

		if (!IsMultiplayer()) {
			*(int*)(0x820644B4) = 0x60000000; //Patch Com_Error for EXE_INVALID_GAMERTAG
			*(int*)(0x823E4204) = 0x48000020; //Patch Devkits Matchmaking crash
		}

		*(int*)(IsMultiplayer() ? 0x823E24E8 : 0x823793C8) = 0x38600001; //Anti Cheat Protection
		*(int*)(IsMultiplayer() ? 0x8245D8EC : 0x82426B3C) = 0x7F88E378; //Show Line Number On Script Errors
		*(int*)(IsMultiplayer() ? 0x8245D8A8 : 0x82426AF8) = 0x60000000; //Show The Script Name The Error Occurred in
		*(int*)(IsMultiplayer() ? 0x8245D8C4 : 0x82426B14) = 0x60000000; //Show The Script Name The Error Occurred in

		Scr_LoadRawFile_FastFile = new Detour(IsMultiplayer() ? 0x8245D4C0 : 0x82426710, (DWORD)Scr_LoadRawFile_FastFileHook);
		RawFile_GetOpCodeChecksum = new Detour(IsMultiplayer() ? 0x824662F0 : 0x8242F590, (DWORD)RawFile_GetOpCodeChecksumHook); //SP 
	}
	if (dwReason == DLL_PROCESS_DETACH) {
		printf("[Infinity Ops]: Infinity Ops Stopped\n");
		if (Scr_LoadRawFile_FastFile) delete Scr_LoadRawFile_FastFile;
		if (RawFile_GetOpCodeChecksum) delete RawFile_GetOpCodeChecksum;
	}
	return true;
}