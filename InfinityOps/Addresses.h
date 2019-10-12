#pragma once

enum dvarType_t {
	DVAR_TYPE_BOOL = 0x0,
	DVAR_TYPE_FLOAT = 0x1,
	DVAR_TYPE_FLOAT_2 = 0x2,
	DVAR_TYPE_FLOAT_3 = 0x3,
	DVAR_TYPE_FLOAT_4 = 0x4,
	DVAR_TYPE_INT = 0x5,
	DVAR_TYPE_ENUM = 0x6,
	DVAR_TYPE_STRING = 0x7,
	DVAR_TYPE_COLOR = 0x8,
	DVAR_TYPE_INT64 = 0x9,
	DVAR_TYPE_LINEAR_COLOR_RGB = 0xA,
	DVAR_TYPE_COLOR_XYZ = 0xB,
	DVAR_TYPE_COUNT = 0xC,
};

union DvarValue {
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	__int64 integer64;
	unsigned __int64 unsignedInt64;
	float value;
	float vector[4];
	const char *string;
	char color[4];
};

union DvarLimits {
	struct {
		int stringCount;
		char * * strings;
	} enumeration;
	struct {
		int min;
		int max;
	} integer;
	struct {
		float min;
		float max;
	} decimal;
};

struct dvar_s {
	const char *name;
	const char *description;
	int hash;
	unsigned int flags;
	dvarType_t type;
	bool modified;
	bool loadedFromSaveGame;
	DvarValue current;
	DvarValue latched;
	DvarValue reset;
	DvarValue saved;
	DvarLimits domain;
	dvar_s *hashNext;
};

union Pointer {
	char ByteValue;
	short ShortValue;
	unsigned int IntValue;
	unsigned __int64 Int64Value;
	float FloatValue;
	char* StringValue;
	dvar_s* DvarValue;
};

struct RawFile {
	const char *name;
	int len;
	char *buffer;
};

union XAssetHeader {
	RawFile* rawFile;
};

extern Pointer* ui_mapname;
extern Pointer* g_gametype;

bool IsMultiplayer();

extern XAssetHeader(__cdecl * DB_FindXAssetHeader)(int assetType, const char* assetName, bool errorIfMissing, int waitTime);
extern const char* (__cdecl * Com_GetExtensionSubString)(const char* fileName);
extern void*(__cdecl * Hunk_AllocateTempMemoryHigh)(unsigned int length);