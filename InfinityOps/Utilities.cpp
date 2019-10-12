#include "Utilities.h"

void CreateFilename(char* Output, const char* filename) {
	sprintf(Output, "game:\\raw\\%s", filename);
	for (size_t i = 0; i < strlen(Output); i++) Output[i] = (Output[i] == '/' ? '\\' : Output[i]);
}

bool FileExists(const char* filename) {
	return std::ifstream(filename).good();
}