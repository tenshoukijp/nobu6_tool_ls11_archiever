// RetsudenEditor.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B
#include <windows.h>
#include <string>
#include <locale> 

#include "LS11ArchieverForm.h"

using namespace System;
using namespace System::Windows::Forms;


[STAThreadAttribute]
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	// �R���g���[�����쐬�����O�ɁAWindows XP �r�W���A�����ʂ�L���ɂ��܂�
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	LS11ArchieverForm^ laf = gcnew LS11ArchieverForm();

	// ���C�� �E�B���h�E���쐬���āA���s���܂�
	Application::Run(laf);
	return 0;
}
