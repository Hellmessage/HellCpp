#ifndef _HELL_UTIL_HEADER_
#define _HELL_UTIL_HEADER_

#include <string>

class HUtil {
public:
	HUtil(LPCWSTR module);
	~HUtil();
	DWORD Offset(DWORD offset);
	void OpenConsole(const char* name);
	void CloseConsole();

	void LoadHook(DWORD runAddress, DWORD hookAddress, LPVOID backup);
	void UnloadHook(DWORD hookAddress, LPVOID backup);
private:
	DWORD m_Base = NULL;
	FILE* m_ConsoleOut;
	std::streambuf* m_ConsoleOutBackup;
	BOOL m_ConsoleOpen = FALSE;

};

#endif // !_HELL_UTIL_HEADER_