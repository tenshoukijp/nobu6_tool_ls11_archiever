#pragma once;


// RetsudenEditor.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B
#include <windows.h>
#include <string>

// �}�[�V�������O�p�w�b�_
#include <msclr/marshal_cppstd.h>
#include "ls11_mod.h"

using namespace msclr::interop;

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;


ref class LS11ArchieverForm : public Form {

protected:
	Label^ lvlInputFile;		// "���̓t�@�C��"�Ƃ��������񃉃x��
	TextBox^ tbInputFile;		// ���̓t�@�C�����p�̃e�L�X�g�{�b�N�X
	Button^ btnInputDialog;		// �_�C�A���O�Ăяo���p�̃{�^��
	GroupBox^ gbProcessType;	// ���k���𓚂��̃O���[�v�g
	RadioButton^ rbCompress;	// ���W�I�{�^���B���k�p
	RadioButton^ rbDecompress;	// ���W�I�{�^���B�𓀗p
	Button^ btnExecute;			// ���s�{�^��

	OpenFileDialog^ ofdForCompress;  	// ���k�p�̃_�C�A���O
	OpenFileDialog^ ofdForDeCompress;	// �𓀗p�̃_�C�A���O

	// �N�����Ƀf�B���N�g��
	String^ objCurDirName;

	// �R�}���h���C������
	String^ objCmdLine;

public:
	LS11ArchieverForm() {

		// �N�����̏���ێ�
		SaveLaunchInfomation();

		// ���C�A�E�g�`��~��
		this->SuspendLayout();

		// �t�H�[���v���p�e�B�ݒ�
		SetFormProperty();

		// GUI�̔z�u
		AddGUIComponents();

		// GUI�̂����C�x���g�n���h����������̂ɕR�t��
		SetEventHandler();

		// ���C�A�E�g�`��ĊJ
		this->ResumeLayout();
	}

protected:

	// �N�����̏���ێ�
	void SaveLaunchInfomation() {

		// ���̋L�q�Ɩ��O��ԓ��Ŗ����Փ˂��Ă��Ďז��ɂȂ邽�ߒ�`���̏���
#undef GetCurrentDirectory

		// �N�����ɃJ�����g�f�B���N�g����ۑ����Ă����B
		objCurDirName = System::IO::Directory::GetCurrentDirectory();

		//�R�}���h���C��������z��Ŏ擾����
		cli::array<String^>^ cmds = System::Environment::GetCommandLineArgs();

		// �����ɉ�������Ȃ�
		if ( cmds->Length > 1 ) {
			objCmdLine = cmds[1];
		}
	}

	void SetFormProperty() {
		this->Text = "LS11 Archiever";

		// ���g�̑傫��
		this->Width = 280;
		this->Height = 140;

		// ���\�[�X�A�Z���u�����烊�\�[�X�}�l�[�W���C���X�^���X�擾
		System::Reflection::Assembly^ asmbly = System::Reflection::Assembly::GetExecutingAssembly();
		System::Resources::ResourceManager^ rm = gcnew System::Resources::ResourceManager(String::Format("{0}.LS11ArchieverRes", asmbly->GetName()->Name), asmbly);

		this->Icon = (System::Drawing::Icon^)rm->GetObject("FormICO");


		 // ���T�C�Y�s�\
		this->FormBorderStyle = ::FormBorderStyle::FixedSingle;


	}

	void AddGUIComponents() {

		// "���̓t�@�C��"�Ƃ��������񃉃x��
		lvlInputFile = gcnew Label();
		lvlInputFile->Text = "���̓t�@�C��";
		lvlInputFile->TextAlign = ContentAlignment::MiddleCenter;
		lvlInputFile->Width = 80;
		lvlInputFile->Height = 20;
		lvlInputFile->Left = 10;
		lvlInputFile->Top  = 15;

		// ���̓t�@�C�����p�̃e�L�X�g�{�b�N�X
		tbInputFile = gcnew TextBox();
		tbInputFile->Width = 150;
		tbInputFile->Height = 20;
		tbInputFile->Left = lvlInputFile->Left + lvlInputFile->Width;
		tbInputFile->Top  = 15;
		tbInputFile->AllowDrop = true; // �h���b�O���h���b�v�\

		// �R�}���h���C������(�������͋N���O�̃A�v���ɒ��ڃt�@�C�����h���b�N�j�����B
		if ( objCmdLine ) {
			tbInputFile->Text = objCmdLine;
		}


		// �_�C�A���O�Ăяo���p�̃{�^��
		btnInputDialog = gcnew Button();
		btnInputDialog->Text = "...";
		btnInputDialog->Left = tbInputFile->Left + tbInputFile->Width;
		btnInputDialog->Top  = 15;
		btnInputDialog->Width  = 20;
		btnInputDialog->Height = 20;

		// ���k���𓚂��̃O���[�v�g
		gbProcessType = gcnew GroupBox();
		gbProcessType->Top = 50;
		gbProcessType->Left = 90;
		gbProcessType->Width = tbInputFile->Width + btnInputDialog->Width;
		gbProcessType->Height = 50;
		gbProcessType->Text = "����";

		// ���k�̃��W�I�{�^��
		rbCompress = gcnew RadioButton();
		rbCompress->Text = "���k";
		rbCompress->Width = 60;
		rbCompress->Left = 25;
		rbCompress->Top = 15;

		// �𓀂̃��W�I�{�^��
		rbDecompress = gcnew RadioButton();
		rbDecompress->Text = "��";
		rbDecompress->Width = 60;
		rbDecompress->Left = 100;
		rbDecompress->Top = 15;
		rbDecompress->Checked = true;

		// �O���[�v�ɔz�u
		gbProcessType->Controls->Add(rbCompress);
		gbProcessType->Controls->Add(rbDecompress);

		// ���s�{�^��
		btnExecute = gcnew Button();
		btnExecute->Text = "���s";
		btnExecute->Top = gbProcessType->Top + 15;
		btnExecute->Left = lvlInputFile->Left;
		btnExecute->Width = 70;

		// �t�H�[���ɔz�u
		this->Controls->Add(lvlInputFile);
		this->Controls->Add(tbInputFile);
		this->Controls->Add(btnInputDialog);
		this->Controls->Add(gbProcessType);
		this->Controls->Add(btnExecute);
	}

	void SetEventHandler() {
		btnInputDialog->Click += gcnew EventHandler( this, &LS11ArchieverForm::btnInputDialog_Click );
		btnExecute->Click += gcnew EventHandler( this, &LS11ArchieverForm::btnExecute_Click );
		tbInputFile->DragEnter += gcnew DragEventHandler( this, &LS11ArchieverForm::tbInputFile_DragEnter );
		tbInputFile->DragDrop += gcnew DragEventHandler( this, &LS11ArchieverForm::tbInputFile_DragDrop );
	}

	void btnInputDialog_Click( Object^ sender, EventArgs^ e ) {
		// �𓀗p�Ƀ`�F�b�N���t���Ă���Ȃ�΁A�𓀗p�̃_�C�A���O��\��
		if ( rbCompress->Checked ) {
			OpenFileDialogForCompress();
		} else if ( rbDecompress->Checked ) {
			OpenFileDialogForDeCompress();
		}
	}

	void tbInputFile_DragEnter( Object^ sender, DragEventArgs^ e ) {
		//�t�@�C�����h���b�O����Ă���ꍇ�A�J�[�\����ύX����
		if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
			e->Effect = ::DragDropEffects::Copy;
		}
	}
	 
	void tbInputFile_DragDrop( Object^ sender, DragEventArgs^ e ) {
         //�h���b�v���ꂽ�t�@�C������p�X���܂ރt�@�C�������擾����
		 cli::array<String^>^ files = (cli::array<String^>^)e->Data->GetData(DataFormats::FileDrop);
 
         //�P�����e�L�X�g�{�b�N�X��
         tbInputFile->Text = files[0];
	}


	// �p�b�N(Compress)���ƂȂ�Decompress���ꂽ�t�@�C�����������f�B���N�g���̐������t�@�C�����p�^�[���Ƃ��đ��݂��邩�ǂ����B
	bool IsExistDecompressFiles( String^ objFilePattern ) {
		// �ΏۂƂȂ�t�@�C���̑��݃J�E���g
		int iExistCnt = 0;

		// �p�b�N���ƂȂ�t�@�C�������邩�ǂ����B
		String^ objCurFileName = "";

		// �p�b�N���ƂȂ�t�@�C����T���B3000���炢�܂łŎ�ł��B
		for ( int iNo=0; iNo<3000; iNo++ ) {
			objCurFileName = String::Format( "{0}/{1}.{2:D3}", objFilePattern, objFilePattern, iNo );
			if ( System::IO::File::Exists(objCurFileName) ) {
				iExistCnt++;
			}
		}

		// �ΏۂƂȂ�t�@�C�����������B
		if ( iExistCnt ) {
			return true;
		}

		return false;
	}

	void btnExecute_Click( Object^ sender, EventArgs^ e ) {

		// �����́B
		if ( tbInputFile->Text == nullptr ) {
			System::Windows::Forms::MessageBox::Show( "���̓t�@�C���������͂ł�", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return;

		// ���݂����B�Ώۂ��f�B���N�g���������ꍇ�������ɊY������B
		} else if ( !System::IO::File::Exists(tbInputFile->Text) ) {
			System::Windows::Forms::MessageBox::Show( "���̓t�@�C���͑��݂��܂���", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return;
		}

		// ���΃p�X�����΃p�X��
		tbInputFile->Text = System::IO::Path::GetFullPath(tbInputFile->Text);

		// �������牺�́u�t�@�C���v�Ƃ��Ă����Ƒ��݂���B
		// �t�@�C���������𒊏o����B
		String^ objFilePattern = System::IO::Path::GetFileNameWithoutExtension( tbInputFile->Text );

		// �ړI�̃t�@�C����������f�B���N�g���ւƈړ�
		String^ objAimDirName = System::IO::Path::GetDirectoryName( tbInputFile->Text );
		System::IO::Directory::SetCurrentDirectory( objAimDirName );

		// .NET�^�C�vString��C++�^�C�vstd::string��
		std::string strFullPathName = marshal_as<std::string>(tbInputFile->Text);
		std::string strFilePattern = marshal_as<std::string>(objFilePattern);


		// ���k�p�Ƀ`�F�b�N���t���Ă���Ȃ�΁A���k�p�̃_�C�A���O��\��
		int result = 0;

		if ( rbCompress->Checked ) {

			if ( IsExistDecompressFiles( objFilePattern ) ) {
				// �t�@�C���Ɉ��k
				result = ls11_EncodePack((char *)strFullPathName.c_str(), (char *)strFilePattern.c_str(), NULL );

			} else {
				result = -3;
			}

		// �𓀗p�Ȃ�΁c

		} else if ( rbDecompress->Checked ) {

			//�t�@�C�����J��
			System::IO::FileStream^ fs = gcnew System::IO::FileStream( tbInputFile->Text, System::IO::FileMode::Open, System::IO::FileAccess::Read);
			//�t�@�C����ǂݍ��ރo�C�g�^�z����쐬����
			cli::array<byte>^ bs = gcnew cli::array<byte>(4);
			//�t�@�C���̓��e��4�o�C�g�����ǂݍ���
			fs->Read(bs, 0, 4);
			//����
			fs->Close();

			if ( bs[0] == 'L' && bs[1] == 'S' && bs[2] == '1' && bs[3] == '1' ) {

				// �t�@�C���ɕ���
				result = ls11_DecodePack((char *)strFullPathName.c_str(), (char *)strFilePattern.c_str(), NULL );
			} else {
				result = -2;
			}
		}

		// ���ʂɂ��������ă��b�Z�[�W
		ShowResultMessageDialog(result);

		// �J�����g�f�B���N�g�������ւƖ߂��B
		System::IO::Directory::SetCurrentDirectory( objAimDirName );
	}

	// ���ʂɂ��������ă��b�Z�[�W
	void ShowResultMessageDialog(int result ) {
		// ����Ȃ�O��Ԃ��Ă���B
		if ( result == 0) {
			System::Windows::Forms::MessageBox::Show( "�������������܂����B", "����", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		} else if ( result == -2 ) {
			System::Windows::Forms::MessageBox::Show( "LS11�`���̃t�@�C���ł͂���܂���B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		} else if ( result == -3 ) {
			System::Windows::Forms::MessageBox::Show( "���k��̃t�@�C���ւƊi�[���邽�߂�\n�t�@�C���Q�����݂��܂���B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		} else {
			System::Windows::Forms::MessageBox::Show( "�\�����ʃG���[���������܂����B", "�G���[", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}
	}

	// �𓀗p�̃_�C�A���O��\��
	void OpenFileDialogForDeCompress() {
		ofdForDeCompress = gcnew OpenFileDialog();

		ofdForDeCompress->InitialDirectory = "."; // �c�[���Ɠ����f�B���N�g�����f�t�H���g

		ofdForDeCompress->Filter = "�V�ċLLS11���k�t�@�C��(*.nb6;*.n6p)|*.nb6;*.n6p";

		ofdForDeCompress->FilterIndex = 1;

		ofdForDeCompress->Title = "���𓀌����̃t�@�C����I�����Ă�������";

		//�_�C�A���O�{�b�N�X�����O�Ɍ��݂̃f�B���N�g���𕜌�����悤�ɂ���
		ofdForDeCompress->RestoreDirectory = true;

		//�P��I��
		ofdForDeCompress->Multiselect = false;

		//�_�C�A���O��\������
		if (ofdForDeCompress->ShowDialog() == ::DialogResult::OK) {
			//OK�{�^�����N���b�N���ꂽ�Ƃ�
			if ( ofdForDeCompress->FileName ) {
				tbInputFile->Text = gcnew String(ofdForDeCompress->FileName); // �t�@�C�����R�s�[
			}
		}
	}


	void OpenFileDialogForCompress() {
		ofdForCompress = gcnew OpenFileDialog();

		ofdForCompress->InitialDirectory = "."; // �c�[���Ɠ����f�B���N�g�����f�t�H���g

		ofdForCompress->Filter = "�V�ċLLS11���k�t�@�C��(*.nb6;*.n6p)|*.nb6;*.n6p";

		ofdForCompress->FilterIndex = 1;

		ofdForCompress->Title = "�����k�恚�̃t�@�C����I�����Ă�������";

		//�_�C�A���O�{�b�N�X�����O�Ɍ��݂̃f�B���N�g���𕜌�����悤�ɂ���
		ofdForCompress->RestoreDirectory = true;

		//�P��I��
		ofdForCompress->Multiselect = false;

		//�_�C�A���O��\������
		if (ofdForCompress->ShowDialog() == ::DialogResult::OK) {
			//OK�{�^�����N���b�N���ꂽ�Ƃ�
			if ( ofdForCompress->FileName ) {
				tbInputFile->Text = gcnew String(ofdForCompress->FileName); // �t�@�C�����R�s�[
			}
		}

	}

};

