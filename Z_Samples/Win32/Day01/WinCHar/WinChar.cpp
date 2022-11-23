#define UNICODE // 要放在windowh前 因为后者包含下面那个宏示例
#include <Windows.h>
#include <stdio.h>
void C_char() {
	const char* pszText = "hello";
	printf("%s\n", pszText);
}

void W_char() {
	const wchar_t* pszText = L"HELLO 你好";
	int len = wcslen(pszText); // 字符个数 5 , 内存 10
	wprintf(L"%s %d\n", pszText, len);


	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsole(
		hOut,// 标准输出句柄, 显示器
		pszText,
		wcslen(pszText),
		NULL,
		NULL);
}


void T_char()
{
	const TCHAR* pszText = __TEXT("hello");
	// 如果定义宏就走双 没定义就走单
#ifdef UNICODE
	wprintf(L"%s\n", pszText);
#else
	printf("单:%s\n", pszText);
#endif
}

int main() {
	//C_char();
	W_char();
	//T_char();
	getchar();


	return 0;
}

/*
* 如果定义该UNICODE宏 就加L 没定义就正常char
*
* ifdef UNICODE
* typedef wchar_t TCHAR;
*  #define __TEXT(quote) L##quote
* #else
*  typedef char TCHAR;
*  #define __TEXT(quote) quote
* #endif


*/