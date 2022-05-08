// RetsudenEditor.cpp : メイン プロジェクト ファイルです。
#include <windows.h>
#include <string>
#include <locale> 

#include "LS11ArchieverForm.h"

using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	LS11ArchieverForm^ laf = gcnew LS11ArchieverForm();

	// メイン ウィンドウを作成して、実行します
	Application::Run(laf);
	return 0;
}
