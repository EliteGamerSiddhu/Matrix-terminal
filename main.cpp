#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
	LPDWORD lpNumberOfCharactesWritten = NULL;
	COORD origin = { 0, 0 };

	HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetStdHandle(STD_OUTPUT_HANDLE,hHandle);

	HANDLE nHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	const wchar_t* inp = L"Hello World!";
	FillConsoleOutputCharacter(nHandle, 'S', 5, origin, lpNumberOfCharactesWritten);

	SetConsoleActiveScreenBuffer(nHandle);


	return 0;
}