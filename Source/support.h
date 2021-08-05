#ifndef SUPPROT_H_
#define SUPPORT_H_
#define _CRT_SECURE_NO_WARNINGS
#define NO_PNG
#include<stdio.h>
#include<graphics.h>
#include<string.h>
#include<math.h>
#include<Windows.h>
#include<commdlg.h>
#include<ShlObj.h>
#include<atlconv.h>
#include<tchar.h>
#include<vector>
#include<iostream>
#include"zint.h"
using namespace std;
struct Data{
	TCHAR name[32];
	TCHAR phone[16];
	TCHAR mail[64];
};
string UnicodeToUTF8(const std::wstring& str);
string ASCIIToUTF8(const std::string& str);
int in(int x, int y, int ax, int ay, int bx, int by);
bool updateQR(const TCHAR* update);
void printBG_main();
void standFont(LOGFONT* style);
void loadBG(const char* file);
void showAddContact();
void showNowContact();
void initData();
void showError();
void showRecover();
void showAbout();
void showBackup();
bool findName(const char* name);
void del(const int x);
void insert(const Data& da);
bool isNum(const TCHAR* str);
int toNum(const TCHAR* str);
void showCode(const TCHAR* info);

#endif
