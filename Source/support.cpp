#include"support.h"
extern INT32 gSize;
extern vector<Data> gData;
extern FILE* gFile;
std::string ASCIIToUTF8(const std::string& str)
{
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t *pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wrt = (wchar_t*)pUnicode;
	delete  pUnicode;

	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, wrt.c_str(), -1, NULL, 0, NULL, NULL);
	char *pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, wrt.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	delete pElementText;

	return strText;
}
std::string UnicodeToUTF8(const std::wstring& str)
{
	char *pElementText;
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	delete[] pElementText;
	return strText;
}
bool updateQR(const TCHAR* update)
{
	struct zint_symbol *my_symbol = ZBarcode_Create();

	my_symbol->symbology = BARCODE_QRCODE;
	my_symbol->height = 100;
	my_symbol->border_width = 2;
	my_symbol->input_mode = DATA_MODE;
	my_symbol->scale = 4;
	strcpy_s(my_symbol->outfile, _T("tempQR/temp.bmp"));

	if (NULL == my_symbol)
	{
		MessageBox(GetHWnd(), _T("二维码生成组件错误"), _T("错误"), MB_OK|MB_ICONERROR);
		return false;
	}
	
	//std::string StrSource = UnicodeToUTF8(update);
	std::string StrSource = ASCIIToUTF8(update);
	int error = ZBarcode_Encode(my_symbol, (const unsigned char*)StrSource.c_str(), StrSource.length());
	if (0 != error)
	{
		MessageBox(GetHWnd(), my_symbol->errtxt, _T("二维码生成失败"), MB_OK|MB_ICONERROR);
		return false;
	}
	ZBarcode_Print(my_symbol, 0);
	ZBarcode_Delete(my_symbol);

	return true;
}
void standFont(LOGFONT* style)
{
	style->lfHeight = 36;
	style->lfWidth = 16;
}
void loadBG(const char* file)
{
	showError();
	IMAGE img;
	loadimage(&img, file);
	putimage(0, 0, &img);
	return;
}
int in(int x, int y, int ax, int ay, int bx, int by)
{
	return ax <= x && x <= bx && ay <= y && y <= by;
}
void showError()
{
	LOGFONT style;
	settextcolor(WHITE);
	style.lfHeight = 10;
	style.lfWidth = 8;
	outtextxy(0, 0, _T("程序文件被破坏..."));
	return;
}
bool findName(const char* name)
{
	int i = -1;
	while (++i < gSize) {
		if (!strcmp(name, gData[i].name)) return true;
	}
	return false;
}
void del(const int x)
{
	Data ins;
	vector<Data> forEx;
	if (gSize <= 0) return;
	if (x + 1 > gData.size()) return;
	int i = -1;
	gFile = fopen("data/data.con", "wb");
	gSize--;
	while (++i < gSize) {
		if (x == i) continue;
		forEx.push_back(gData[i]);
	}
	gData.clear();
	i = -1;
	while (++i < gSize) {
		gData.push_back(forEx[i]);
	}
	fwrite(&gSize, sizeof(INT32), 1, gFile);
	i = -1;
	while (++i < gSize) {
		ins = gData[i];
		fwrite(&in, sizeof(Data), 1, gFile);
	}
	fclose(gFile);
	return;
}
void insert(const Data& da)
{
	Data ins;
	int i = -1;
	if (findName(da.name)) return;
	gSize++;
	gData.push_back(da);
	gFile = fopen("data/data.con", "wb");
	fwrite(&gSize, sizeof(INT32), 1, gFile);
	while (++i < gSize) {
		ins = gData[i];
		fwrite(&in, sizeof(Data), 1, gFile);
	}
	fclose(gFile);
}
void printBG_main()
{
	cleardevice();
	loadBG("images/backGround_main");
	LOGFONT style;
	gettextstyle(&style);
	style.lfHeight = 10;
	style.lfWidth = 8;
	settextstyle(&style);
	settextcolor(WHITE);
	outtextxy(186, 790, "Copyright@ GZhonghui.");
	return;
}
void showAbout()
{
	MOUSEMSG msg;
	cleardevice();
	loadBG("images/backGround_about");
	while (true) {
		msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x, y = msg.y;
			if (in(x, y, 214, 740, 326, 780)) return;
			if(in(x,y,158,520,382,560)) 
				ShellExecute(NULL, "open", "http://gzher.com/Contact/index.html", NULL, NULL, SW_SHOWNORMAL);
		}
	}
}
void showBackup()
{
	bool lastIn = false;
	Data ins;
	int i = -1;
	MOUSEMSG msg;
	TCHAR path[1024];
	cleardevice();
	loadBG("images/backGround_backup");
	while (true) {
		msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x, y = msg.y;
			if (in(x, y, 214, 740, 326, 780)) return;
			else if (in(x, y, 130, 170, 410, 210)) {
				if (lastIn) continue;
				lastIn = true;
				InputBox(path, 1024, _T("请输入备份文件的位置"), _T("备份为文件"), _T("桌面"));
				gFile = fopen(path, "wb");
				if (gFile == NULL) {
					MessageBox(GetHWnd(), _T("文件存入失败，请检查路径是否正确"), _T("错误"), MB_OK|MB_ICONERROR);
					continue;
				}
				//backup the data
				fwrite(&gFile, sizeof(INT32), 1, gFile);
				while (++i < gSize)
				{
					ins = gData[i];
					fwrite(&ins, sizeof(Data), 1, gFile);
				}
				fclose(gFile);
			}
			else if (in(x, y, 130, 240, 410, 280)) {
				if (lastIn) continue;
				lastIn = true;
				InputBox(path, 1024, _T("请输入备份文件的位置"), _T("备份为文本"), _T("桌面"));
				gFile = fopen(path, "w");
				if (gFile == NULL) {
					MessageBox(GetHWnd(), _T("文件存入失败，请检查路径是否正确"), _T("错误"), MB_OK|MB_ICONERROR);
					continue;
				}
				//backup the txt
				TCHAR out[1024 * 1024];
				while (++i < gSize) {
					fprintf(gFile, _T("姓名：%s 电话：%s\n邮箱：%s\n"), gData[i].name, gData[i].phone, gData[i].mail);
				}
				fclose(gFile);
			}
			else if (in(x, y, 130, 310, 410, 350)) {
				if (lastIn) continue;
				lastIn = true;
				MessageBox(GetHWnd(), _T("网络备份正在Buliding..."), _T("网络备份"), MB_OK|MB_ICONINFORMATION);
			}
			else lastIn = false;
		}
		else lastIn = false;
	}
}
void showRecover()
{
	MOUSEMSG msg;
	Data da;
	INT32 recoverSize;
	bool lastIn = false;
	TCHAR path[1024];
	cleardevice();
	loadBG("images/backGround_recover");
	while (true) {
		msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x, y = msg.y;
			if (in(x, y, 214, 740, 326, 780)) return;
			else if (in(x, y, 130, 170, 410, 210)) {
				if (lastIn) continue;
				lastIn = true;
				InputBox(path, 1024, _T("请输入文件路径"), _T("从文件恢复数据"), _T("桌面"));
				gFile=fopen(path, "rb");
				if (gFile == NULL) {
					MessageBox(GetHWnd(), _T("无法打开指定文件，请检查路径是否正确\n文件是否正被占用。"), _T("恢复错误"), MB_OK | MB_ICONERROR);
					continue;
				}
				//read the data file
				fread(&recoverSize, sizeof(INT32), 1, gFile);
				int i = 0;
				while (++i <= recoverSize) {
					fread(&da, sizeof(Data), 1, gFile);
					if (findName(da.name)) continue;
					insert(da);
				}
				fclose(gFile);
				MessageBox(GetHWnd(), _T("文件数据已恢复"), _T("恢复文件"), MB_OK | MB_ICONINFORMATION);
			}
			else if (in(x, y, 130, 240, 410, 280)) {
				if (lastIn) continue;
				lastIn = true;
				MessageBox(GetHWnd(), _T("网络备份功能正在Bulidling..."), _T("网络备份"), MB_OK|MB_ICONINFORMATION);
			}
			else lastIn = false;
		}
		else lastIn = false;
	}
}
bool isNum(const TCHAR* str)
{
	int i = -1;
	while (++i < lstrlen(str)) {
		if (str[i] < _T('0') || str[i]>_T('9')) return false;
	}
	return true;
}
int toNum(const TCHAR* str)
{
	int num = 0;
	int i = -1;
	while (++i < lstrlen(str))
	{
		num = num * 10 + str[i] - _T('0');
	}
	return num;
}
void showAddContact()
{
	Data da;
	TCHAR tran[32];
	TCHAR inputFinal[1024];
	againName_327HDS:
	InputBox(da.name, 32, _T("请输入联系人姓名"),_T("创建联系人（第1步，共4步）"));
	if (lstrlenA(da.name) == 0) {
		int res=MessageBoxA(GetHWnd(), _T("您未输入姓名！\n选择 “是” 重新输入姓名。选择 “否” 放弃本次创建。"),_T("格式错误"), MB_YESNO);
		if (res == IDYES) goto againName_327HDS;
		return;
	}
	if (findName(da.name)){
		auto res = MessageBoxA(GetHWnd(), _T("该联系人已经存在！\n选择 “是” 重新输入姓名。选择 “否” 放弃本次创建。"), _T("格式错误"), MB_YESNO);
		if (res == IDYES) goto againName_327HDS;
		return;
	}
	againPhone_ENWCW2:
	InputBox(da.phone, 12, _T("联系电话"), _T("创建联系人（第2步，共4步）"));
	if (lstrlenA(da.phone)==0||!isNum(da.phone)) {
		auto res = MessageBoxA(GetHWnd(), _T("您输入的电话号码有误，请重新输入！\n选择 “是” 重新输入姓名。选择 “否” 放弃本次创建。"),_T("格式错误"),MB_YESNO);
		if (res == IDYES) goto againPhone_ENWCW2;
		return;
	}
	againMail_3EDEDE:
	InputBox(da.mail, 64, _T("邮箱"), _T("创建联系人（第3步，共4步）"));
	if (lstrlenA(da.mail) == 0) {
		auto res = MessageBoxA(GetHWnd(), _T("您未输入邮箱地址，请重新输入！\n选择 “是” 重新输入姓名。选择 “否” 放弃本次创建。"), _T("格式错误"), MB_YESNO);
		if (res == IDYES) goto againMail_3EDEDE;
		return;
	}
	_stprintf(inputFinal, _T("以下是您输入的联系人信息，请核对!\n姓名：%s\n电话：%s\n邮箱：%s\n输入(Y/y)确定。输入(N/n)取消本次输入。"), da.name, da.phone, da.mail);
	againCon_1D4HD3:
	InputBox(tran, 2, inputFinal, _T("第4步，共4步"));
	if (lstrlenA(tran) == 0) goto againCon_1D4HD3;
	if (tran[0] == _T('N') || tran[0] == _T('n')) return;
	if (tran[0] != _T('y') && tran[0] != _T('Y')) goto againCon_1D4HD3;
	insert(da);
}
void showNowContact()
{
	bool lastIn = false;
	int page = 1,endPage;
	int num,i,pos;
	MOUSEMSG msg;
	TCHAR info[2048];
	TCHAR forQr[16],forDel[16];
	TCHAR name[1024];
	TCHAR index[1024],showPhone[1024],showMail[2048];
	back_1D3D4A:
	//reprint
	pos = 1;
	i = (page - 1) * 4 + 1;
	endPage = gSize / 4 + (gSize % 4 ? 1 : 0);
	LOGFONT style;
	gettextstyle(&style);
	standFont(&style);
	settextstyle(&style);
	settextcolor(WHITE);
	cleardevice();
	loadBG("images/backGround_show");
	while (true) {
		if (i > gSize || i > page * 4) break;
		_stprintf(index, _T("编号：%d  姓名：%s"), i, gData[i - 1].name);
		_stprintf(showPhone, _T("电话：%s"), gData[i - 1].phone);
		_stprintf(showMail, _T("邮箱：%s"), gData[i - 1].mail);
		switch (pos) {
		case 1:
			outtextxy(20,140, index);
			outtextxy(20, 180, showPhone);
			outtextxy(20, 220, showMail);
			break;
		case 2:
			outtextxy(20, 260, index);
			outtextxy(20, 300, showPhone);
			outtextxy(20, 340, showMail);
			break;
		case 3:
			outtextxy(20, 380, index);
			outtextxy(20, 420, showPhone);
			outtextxy(20, 460, showMail);
			break;
		case 4:
			outtextxy(20, 500, index);
			outtextxy(20, 540, showPhone);
			outtextxy(20, 580, showMail);
			break;
		}
		pos++; i++;
	}
	while (true) {
		msg = GetMouseMsg();
		if (msg.mkLButton) {
			int x = msg.x, y = msg.y;
			if (in(x, y, 214, 760, 326, 800)) return;
			else if (in(x, y, 200, 700, 340, 740)) {
				if (lastIn) continue;
				lastIn = true;
				InputBox(forQr, 16, _T("您需要为哪位联系人生成二维码？请输入编号"),_T("生成二维码"));
				if (lstrlen(forQr) == 0) continue;
				if (!isNum(forQr)) {
					MessageBox(GetHWnd(), _T("请输入数字！"), _T("输入错误"), MB_OK | MB_ICONERROR);
					continue;
				}
				num = toNum(forQr);
				if (num > gSize) {
					MessageBox(GetHWnd(), _T("输入的数字超出范围！"), _T("输入错误"), MB_OK | MB_ICONERROR);
					continue;
				}
				_stprintf(info, _T("BEGIN:VCARD\nVERSION:3.0\nFN:%s\nTEL;CELL;VOICE:%s\nEMAIL:%s\nEND:VCARD"), gData[num-1].name, gData[num-1].phone, gData[num-1].mail);
				showCode(info);
				goto back_1D3D4A;
			}
			else if (in(x,y,36,640,136,680)) {
				if (lastIn) continue;
				lastIn = true;
				if (page == 1) continue;
				page--;
				goto back_1D3D4A;
			}
			else if (in(x,y,172,640,252,680)){
				if (lastIn) continue;
				lastIn = true;
				InputBox(name, 1024, _T("请输入要查找的联系人的姓名："), _T("查找联系人"));
				if (lstrlen(name) == 0) continue;
				if (!findName(name)) {
					MessageBox(GetHWnd(), _T("没有该联系人！"), _T("未找到记录"), MB_OK | MB_ICONINFORMATION);
					continue;
				}
				i = -1;
				while (++i < gSize) {
					if (!lstrcmp(gData[i].name, name)) {
						_stprintf(info, _T("姓名：%s\n电话：%s\n邮箱：%s"), gData[i].name, gData[i].phone, gData[i].mail);
						MessageBox(GetHWnd(), info, _T("已为您找到联系人"), MB_OK|MB_ICONINFORMATION);
						break;
					}
				}

			}
			else if (in(x, y, 288, 640, 368, 680)){
				if (lastIn) continue;
				lastIn = true;
				InputBox(forDel, 16, _T("您需要删除哪位联系人？请输入编号"), _T("删除联系人"));
				if (lstrlen(forDel) == 0) continue;
				if (!isNum(forDel)) {
					MessageBox(GetHWnd(), _T("请输入数字！"), _T("输入错误"), MB_OK | MB_ICONERROR);
					continue;
				}
				num = toNum(forDel);
				if (num > gSize) {
					MessageBox(GetHWnd(), _T("输入的数字超出范围！"), _T("输入错误"), MB_OK | MB_ICONERROR);
					continue;
				}
				del(num - 1);
				goto back_1D3D4A;
			}
			else if (in(x, y, 404, 640, 504, 680)) {
				if (lastIn) continue;
				lastIn = true;
				if (page == endPage) continue;
				page++;
				goto back_1D3D4A;
			}
			else lastIn = false;
		}
		else lastIn = false;
	}

}
void showCode(const TCHAR* info)
{
	IMAGE code;
	MOUSEMSG msg;
	updateQR(info);
	cleardevice();
	loadBG("images/backGround_code");
	loadimage(&code, "tempQR/temp.bmp", 500, 500, true);
	putimage(20, 180, &code);

	while (true)
	{
		msg = GetMouseMsg();
		if (msg.mkLButton)
		{
			int x = msg.x, y = msg.y;
			if (in(x, y, 214, 740, 326, 780)) return;
		}
	}
}
void initData()
{
	Data da;
	gData.clear();
	gFile = fopen("data/data.con", "rb");
	if(gFile==NULL){
		MessageBoxA(GetHWnd(), _T("文件打开失败"), _T("数据初始化"), MB_OK|MB_ICONERROR);
		exit(0);
	}
	fread(&gSize, sizeof(INT32), 1, gFile);
	int i=0;
	while (++i <= gSize) {
		//Read data here
		fread(&da, sizeof(Data), 1, gFile);
		gData.push_back(da);
	}
	fclose(gFile);
}
