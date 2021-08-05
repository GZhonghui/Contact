#define _CRT_SECURE_NO_WARNINGS
#include"support.h"
#pragma comment(lib,"libzint.lib")
vector<Data> gData;
INT32 gSize;
FILE* gFile;
int main() {
	//puts("Hello World....");

	bool lastIn = false;
	MOUSEMSG msg;
	initData();
	initgraph(540, 820);
	SetWindowTextA(GetHWnd(), _T("C语言通讯录 V1.2.7"));

	back_3D88WD:
	printBG_main();

	while (true) {
		msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x, y = msg.y;
			if (in(x, y, 130, 170, 410, 210)) {
				if (lastIn) continue;
				showAddContact();
				lastIn = true;
			}
			else if (in(x, y, 130, 240, 410, 280)) {
				if (lastIn) continue;
				showNowContact();
				goto back_3D88WD;
				lastIn = true;
			}
			else if (in(x, y, 158, 310, 382, 350)) {
				if (lastIn) continue;
				showBackup();
				goto back_3D88WD;
				lastIn = true;
			}
			else if (in(x, y, 158, 380, 382, 420)) {
				if (lastIn) continue;
				showRecover();
				goto back_3D88WD;
				lastIn = true;
			}
			else if (in(x, y, 214, 450, 326, 490)) {
				if (lastIn) continue;
				showAbout();
				goto back_3D88WD;
				lastIn = true;
			}
			else lastIn = false;
		}
		else lastIn = false;
	}
	return 0;
}
