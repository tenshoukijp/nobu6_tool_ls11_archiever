#pragma once;


// RetsudenEditor.cpp : メイン プロジェクト ファイルです。
#include <windows.h>
#include <string>

// マーシャリング用ヘッダ
#include <msclr/marshal_cppstd.h>
#include "ls11_mod.h"

using namespace msclr::interop;

using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;


ref class LS11ArchieverForm : public Form {

protected:
	Label^ lvlInputFile;		// "入力ファイル"という文字列ラベル
	TextBox^ tbInputFile;		// 入力ファイル名用のテキストボックス
	Button^ btnInputDialog;		// ダイアログ呼び出し用のボタン
	GroupBox^ gbProcessType;	// 圧縮か解答かのグループ枠
	RadioButton^ rbCompress;	// ラジオボタン。圧縮用
	RadioButton^ rbDecompress;	// ラジオボタン。解凍用
	Button^ btnExecute;			// 実行ボタン

	OpenFileDialog^ ofdForCompress;  	// 圧縮用のダイアログ
	OpenFileDialog^ ofdForDeCompress;	// 解凍用のダイアログ

	// 起動時にディレクトリ
	String^ objCurDirName;

	// コマンドライン引数
	String^ objCmdLine;

public:
	LS11ArchieverForm() {

		// 起動時の情報を保持
		SaveLaunchInfomation();

		// レイアウト描画止め
		this->SuspendLayout();

		// フォームプロパティ設定
		SetFormProperty();

		// GUIの配置
		AddGUIComponents();

		// GUIのうちイベントハンドラがあるものに紐付け
		SetEventHandler();

		// レイアウト描画再開
		this->ResumeLayout();
	}

protected:

	// 起動時の情報を保持
	void SaveLaunchInfomation() {

		// 下の記述と名前空間内で命名衝突していて邪魔になるため定義自体消去
#undef GetCurrentDirectory

		// 起動時にカレントディレクトリを保存しておく。
		objCurDirName = System::IO::Directory::GetCurrentDirectory();

		//コマンドライン引数を配列で取得する
		cli::array<String^>^ cmds = System::Environment::GetCommandLineArgs();

		// 引数に何かあるなら
		if ( cmds->Length > 1 ) {
			objCmdLine = cmds[1];
		}
	}

	void SetFormProperty() {
		this->Text = "LS11 Archiever";

		// 自身の大きさ
		this->Width = 280;
		this->Height = 140;

		// リソースアセンブリからリソースマネージャインスタンス取得
		System::Reflection::Assembly^ asmbly = System::Reflection::Assembly::GetExecutingAssembly();
		System::Resources::ResourceManager^ rm = gcnew System::Resources::ResourceManager(String::Format("{0}.LS11ArchieverRes", asmbly->GetName()->Name), asmbly);

		this->Icon = (System::Drawing::Icon^)rm->GetObject("FormICO");


		 // リサイズ不可能
		this->FormBorderStyle = ::FormBorderStyle::FixedSingle;


	}

	void AddGUIComponents() {

		// "入力ファイル"という文字列ラベル
		lvlInputFile = gcnew Label();
		lvlInputFile->Text = "入力ファイル";
		lvlInputFile->TextAlign = ContentAlignment::MiddleCenter;
		lvlInputFile->Width = 80;
		lvlInputFile->Height = 20;
		lvlInputFile->Left = 10;
		lvlInputFile->Top  = 15;

		// 入力ファイル名用のテキストボックス
		tbInputFile = gcnew TextBox();
		tbInputFile->Width = 150;
		tbInputFile->Height = 20;
		tbInputFile->Left = lvlInputFile->Left + lvlInputFile->Width;
		tbInputFile->Top  = 15;
		tbInputFile->AllowDrop = true; // ドラッグ＆ドロップ可能

		// コマンドライン引数(もしくは起動前のアプリに直接ファイルをドラック）した。
		if ( objCmdLine ) {
			tbInputFile->Text = objCmdLine;
		}


		// ダイアログ呼び出し用のボタン
		btnInputDialog = gcnew Button();
		btnInputDialog->Text = "...";
		btnInputDialog->Left = tbInputFile->Left + tbInputFile->Width;
		btnInputDialog->Top  = 15;
		btnInputDialog->Width  = 20;
		btnInputDialog->Height = 20;

		// 圧縮か解答かのグループ枠
		gbProcessType = gcnew GroupBox();
		gbProcessType->Top = 50;
		gbProcessType->Left = 90;
		gbProcessType->Width = tbInputFile->Width + btnInputDialog->Width;
		gbProcessType->Height = 50;
		gbProcessType->Text = "処理";

		// 圧縮のラジオボタン
		rbCompress = gcnew RadioButton();
		rbCompress->Text = "圧縮";
		rbCompress->Width = 60;
		rbCompress->Left = 25;
		rbCompress->Top = 15;

		// 解凍のラジオボタン
		rbDecompress = gcnew RadioButton();
		rbDecompress->Text = "解凍";
		rbDecompress->Width = 60;
		rbDecompress->Left = 100;
		rbDecompress->Top = 15;
		rbDecompress->Checked = true;

		// グループに配置
		gbProcessType->Controls->Add(rbCompress);
		gbProcessType->Controls->Add(rbDecompress);

		// 実行ボタン
		btnExecute = gcnew Button();
		btnExecute->Text = "実行";
		btnExecute->Top = gbProcessType->Top + 15;
		btnExecute->Left = lvlInputFile->Left;
		btnExecute->Width = 70;

		// フォームに配置
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
		// 解凍用にチェックが付いているならば、解凍用のダイアログを表示
		if ( rbCompress->Checked ) {
			OpenFileDialogForCompress();
		} else if ( rbDecompress->Checked ) {
			OpenFileDialogForDeCompress();
		}
	}

	void tbInputFile_DragEnter( Object^ sender, DragEventArgs^ e ) {
		//ファイルがドラッグされている場合、カーソルを変更する
		if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
			e->Effect = ::DragDropEffects::Copy;
		}
	}
	 
	void tbInputFile_DragDrop( Object^ sender, DragEventArgs^ e ) {
         //ドロップされたファイルからパスを含むファイル名を取得する
		 cli::array<String^>^ files = (cli::array<String^>^)e->Data->GetData(DataFormats::FileDrop);
 
         //１つだけテキストボックスへ
         tbInputFile->Text = files[0];
	}


	// パック(Compress)元となるDecompressされたファイルが正しいディレクトリの正しいファイル名パターンとして存在するかどうか。
	bool IsExistDecompressFiles( String^ objFilePattern ) {
		// 対象となるファイルの存在カウント
		int iExistCnt = 0;

		// パック元となるファイルがあるかどうか。
		String^ objCurFileName = "";

		// パック元となるファイルを探す。3000ぐらいまでで手打ち。
		for ( int iNo=0; iNo<3000; iNo++ ) {
			objCurFileName = String::Format( "{0}/{1}.{2:D3}", objFilePattern, objFilePattern, iNo );
			if ( System::IO::File::Exists(objCurFileName) ) {
				iExistCnt++;
			}
		}

		// 対象となるファイルがあった。
		if ( iExistCnt ) {
			return true;
		}

		return false;
	}

	void btnExecute_Click( Object^ sender, EventArgs^ e ) {

		// 未入力。
		if ( tbInputFile->Text == nullptr ) {
			System::Windows::Forms::MessageBox::Show( "入力ファイルが未入力です", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return;

		// 存在せず。対象がディレクトリだった場合もここに該当する。
		} else if ( !System::IO::File::Exists(tbInputFile->Text) ) {
			System::Windows::Forms::MessageBox::Show( "入力ファイルは存在しません", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
			return;
		}

		// 相対パスから絶対パスへ
		tbInputFile->Text = System::IO::Path::GetFullPath(tbInputFile->Text);

		// ここから下は「ファイル」としてちゃんと存在する。
		// ファイル名だけを抽出する。
		String^ objFilePattern = System::IO::Path::GetFileNameWithoutExtension( tbInputFile->Text );

		// 目的のファイル名があるディレクトリへと移動
		String^ objAimDirName = System::IO::Path::GetDirectoryName( tbInputFile->Text );
		System::IO::Directory::SetCurrentDirectory( objAimDirName );

		// .NETタイプString→C++タイプstd::stringへ
		std::string strFullPathName = marshal_as<std::string>(tbInputFile->Text);
		std::string strFilePattern = marshal_as<std::string>(objFilePattern);


		// 圧縮用にチェックが付いているならば、圧縮用のダイアログを表示
		int result = 0;

		if ( rbCompress->Checked ) {

			if ( IsExistDecompressFiles( objFilePattern ) ) {
				// ファイルに圧縮
				result = ls11_EncodePack((char *)strFullPathName.c_str(), (char *)strFilePattern.c_str(), NULL );

			} else {
				result = -3;
			}

		// 解凍用ならば…

		} else if ( rbDecompress->Checked ) {

			//ファイルを開く
			System::IO::FileStream^ fs = gcnew System::IO::FileStream( tbInputFile->Text, System::IO::FileMode::Open, System::IO::FileAccess::Read);
			//ファイルを読み込むバイト型配列を作成する
			cli::array<byte>^ bs = gcnew cli::array<byte>(4);
			//ファイルの内容を4バイトだけ読み込む
			fs->Read(bs, 0, 4);
			//閉じる
			fs->Close();

			if ( bs[0] == 'L' && bs[1] == 'S' && bs[2] == '1' && bs[3] == '1' ) {

				// ファイルに分解
				result = ls11_DecodePack((char *)strFullPathName.c_str(), (char *)strFilePattern.c_str(), NULL );
			} else {
				result = -2;
			}
		}

		// 結果にしたがってメッセージ
		ShowResultMessageDialog(result);

		// カレントディレクトリを元へと戻す。
		System::IO::Directory::SetCurrentDirectory( objAimDirName );
	}

	// 結果にしたがってメッセージ
	void ShowResultMessageDialog(int result ) {
		// 正常なら０を返している。
		if ( result == 0) {
			System::Windows::Forms::MessageBox::Show( "処理が完了しました。", "完了", ::MessageBoxButtons::OK, ::MessageBoxIcon::Information);
		} else if ( result == -2 ) {
			System::Windows::Forms::MessageBox::Show( "LS11形式のファイルではありません。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		} else if ( result == -3 ) {
			System::Windows::Forms::MessageBox::Show( "圧縮先のファイルへと格納するための\nファイル群が存在しません。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		} else {
			System::Windows::Forms::MessageBox::Show( "予期せぬエラーが発生しました。", "エラー", MessageBoxButtons::OK, ::MessageBoxIcon::Error);
		}
	}

	// 解凍用のダイアログを表示
	void OpenFileDialogForDeCompress() {
		ofdForDeCompress = gcnew OpenFileDialog();

		ofdForDeCompress->InitialDirectory = "."; // ツールと同じディレクトリがデフォルト

		ofdForDeCompress->Filter = "天翔記LS11圧縮ファイル(*.nb6;*.n6p)|*.nb6;*.n6p";

		ofdForDeCompress->FilterIndex = 1;

		ofdForDeCompress->Title = "★解凍元★のファイルを選択してください";

		//ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
		ofdForDeCompress->RestoreDirectory = true;

		//単一選択
		ofdForDeCompress->Multiselect = false;

		//ダイアログを表示する
		if (ofdForDeCompress->ShowDialog() == ::DialogResult::OK) {
			//OKボタンがクリックされたとき
			if ( ofdForDeCompress->FileName ) {
				tbInputFile->Text = gcnew String(ofdForDeCompress->FileName); // ファイル名コピー
			}
		}
	}


	void OpenFileDialogForCompress() {
		ofdForCompress = gcnew OpenFileDialog();

		ofdForCompress->InitialDirectory = "."; // ツールと同じディレクトリがデフォルト

		ofdForCompress->Filter = "天翔記LS11圧縮ファイル(*.nb6;*.n6p)|*.nb6;*.n6p";

		ofdForCompress->FilterIndex = 1;

		ofdForCompress->Title = "★圧縮先★のファイルを選択してください";

		//ダイアログボックスを閉じる前に現在のディレクトリを復元するようにする
		ofdForCompress->RestoreDirectory = true;

		//単一選択
		ofdForCompress->Multiselect = false;

		//ダイアログを表示する
		if (ofdForCompress->ShowDialog() == ::DialogResult::OK) {
			//OKボタンがクリックされたとき
			if ( ofdForCompress->FileName ) {
				tbInputFile->Text = gcnew String(ofdForCompress->FileName); // ファイル名コピー
			}
		}

	}

};

