#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <future>
#include <conio.h>

int nScreenWidth = 80;
int nScreenHeight = 80;
int MAX_OFFSET = 50;
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
int nOffset() {
	return (-1) * (rand() % MAX_OFFSET);
}

//GENERATES A RANDOM LENGTH BASED ON THE MAXIMUM LENGTH
int nLength() {
	return rand() % MAX_LENGTH;
}

int terminal() {

	//CREATING BUFFER AND HANDLE TO MODIFY	
	HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO sBufferInfo;

	//CHANGING DIMENSIONS BASED ON THE CURRENT CONSOLE
	GetConsoleScreenBufferInfo(hHandle, &sBufferInfo);
	nScreenWidth = sBufferInfo.dwSize.X;
	nScreenHeight = sBufferInfo.dwSize.Y;

	//CREATING POINTERS TO CHAR_INFO
	CHAR_INFO* sBuffer = nullptr;
	sBuffer = new CHAR_INFO[nScreenWidth * nScreenHeight];
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

	//MAKING STING "PRESS Q TO QUIT...." FOR BOTTOM OF SCREEN
	wchar_t lastLine[20] = L"Press q to quit...";

	for (int i = 0; i < ((nScreenWidth >= 18) ? 18 : nScreenWidth); i++) {
		sBuffer[(nScreenHeight - 1) * nScreenWidth + i].Char.UnicodeChar = lastLine[i];
		sBuffer[(nScreenHeight - 1) * nScreenWidth + i].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	}

	srand((unsigned int)time(0)); //SETS SEED SO THAT WE DON'T GET SAME RANDOM VALUE

	bool qConsole = false;
	while (!qConsole) {

		//CHECKS IF CONSOLE SIZE HAS BEEN CHANGED
		GetConsoleScreenBufferInfo(hHandle, &sBufferInfo);
		if (sBufferInfo.dwSize.X != nScreenWidth || sBufferInfo.dwSize.Y != nScreenHeight) {
			break;
		}

		//CHECKS IF THE UPPER AND LOWER CELL ARE SAME AND PROPOGATES COLOUR
		for (int i = 0; i < nScreenWidth; i++) {
			for (int j = nScreenHeight - 3; j > 0; j--) {
				if (sBuffer[(j - 1) * nScreenWidth + i].Attributes != sBuffer[(j + 1) * nScreenWidth + i].Attributes) {
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
			arrOffset[i] += 1;
		}


		//CHANGE OUTPUT BUFFER AND MAKING IT 0.5 FRAME PER SECOND
		WriteConsoleOutput(hHandle, sBuffer, { (SHORT)nScreenWidth, (SHORT)nScreenHeight }, { 0,0 }, &sRect);
		this_thread::sleep_for(chrono::milliseconds(100));

		if (GetKeyState('Q') & 0x8000) {
			qConsole = true;
		}
	}


	//CLOSING AND DELETING HANDLE AND ALLOCATED MEMORY

	delete[] sBuffer;
	delete[] arrOffset;
	delete[] arrLength;
	return 0;
}

int main() {
	bool quit = true;
	while (quit) {
		terminal();

		if (GetKeyState('Q') & 0x8000) {
			quit = false;
		}
	}
	return 0;
}