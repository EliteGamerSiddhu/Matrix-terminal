#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <future>
#include <conio.h>

int nScreenWidth = 80;
int nScreenHeight = 80;
int MAX_OFFSET = 30;
int MAX_LENGTH = 10;

using namespace std;

//RETURNS A RANDOM UNICODE CHARACTER
wchar_t randChar() {
	char fChar = rand() % 95 + 32;
	return (wchar_t)fChar;
}

//CREATES A BUFFER WITH FORECOLOR BLACK AND BACKGROUND BLACK
void newBuffer(CHAR_INFO* buffer) {
	for (int i = 0; i < nScreenWidth; i++) {
		for (int j = 0; j < nScreenHeight; j++) {
			buffer[j * nScreenWidth + i].Char.UnicodeChar = randChar();
			buffer[j * nScreenWidth + i].Attributes = 0;
		}
	}
}

//GENERATES A RANDOM OFFSET BASED ON THE MAXIMUM OFFSET
int nOffset(){
	return (-1) * (rand() % MAX_OFFSET);
}

//GENERATES A RANDOM LENGTH BASED ON THE MAXIMUM LENGTH
int nLength() {
	return rand() % MAX_LENGTH;
}

int main() {

	//CREATING BUFFER AND HANDLE TO MODIFY	
	HANDLE hHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hHandle);
	CHAR_INFO* sBuffer = new CHAR_INFO[nScreenHeight * nScreenWidth];
	newBuffer(sBuffer);

	//OFFSET AND LENGTH ARRAY AND INITIALIZING IT
	int* arrOffset = new int[nScreenWidth];
	int* arrLength = new int[nScreenWidth];
	for (int i = 0; i < nScreenWidth; i++) {
		arrOffset[i] = nOffset();
		arrLength[i] = nLength();
	}

	//CREATING RECT FOR CONSOLE OUTPUT
	SMALL_RECT sRect = { 0, 0, (SHORT)nScreenWidth, (SHORT)nScreenHeight };

	srand((unsigned int)time(0)); //SETS SEED SO THAT WE DON'T GET SAME RANDOM VALUE

	newBuffer(sBuffer); //INITIALIZING NEW BUFFER

	bool qConsole = false;
	while (!qConsole) {

		//CHECKS IF THE UPPER AND LOWER CELL ARE SAME AND PROPOGATES COLOUR
		for (int i = 0; i < nScreenWidth; i++) {
			for (int j = nScreenHeight - 1; j > 0; j--) {
				if (sBuffer[(j-1) * nScreenWidth + i].Attributes != sBuffer[(j+1) * nScreenWidth + i].Attributes) {
					sBuffer[(j + 1) * nScreenWidth + i].Attributes = sBuffer[j * nScreenWidth + i].Attributes;
					sBuffer[j * nScreenWidth + i].Attributes = sBuffer[(j - 1) * nScreenWidth + i].Attributes;
				}
			}
		}

		for (int i = 0; i < nScreenWidth; i++) {
			if (arrOffset[i] > 0) {
				if (arrOffset[i] <= arrLength[i]) sBuffer[i].Attributes = FOREGROUND_GREEN;
				else {
					sBuffer[i].Attributes = 0;
					sBuffer[i].Char.UnicodeChar = randChar();
					arrOffset[i] = nOffset();
					arrLength[i] = nLength();
				}
			}
			arrOffset[i]++;
		}


		//CHANGE OUTPUT BUFFER AND MAKING IT 1 FRAME PER SECOND
		WriteConsoleOutput(hHandle, sBuffer, { (SHORT)nScreenWidth, (SHORT)nScreenHeight }, { 0,0 }, &sRect);
		this_thread::sleep_for(chrono::milliseconds(500));
	}

	(void)getchar();


	//CLOSING AND DELETING HANDLE AND ALLOCATED MEMORY
	CloseHandle(hHandle);
	delete[] sBuffer;
	delete[] arrOffset;
	delete[] arrLength;
	return 0;
}