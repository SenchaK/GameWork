#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include <cassert>


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nShowCmd ){
	ChangeWindowMode( TRUE );
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );


	Sencha::Task::GameTask* global = Sencha::Task::CreateGlobalTask();
	while( ProcessMessage() == 0 ){
		ClsDrawScreen();
		global->update();
		global->draw();
		DrawFormatString( 0 , 0 , 0xFF00FF , "MemoryPool %d" , Sencha::Task::GetTaskMemoryCount() );
		ScreenFlip();
	}

	global->finish();
	DxLib_End();
	return 0;
}

