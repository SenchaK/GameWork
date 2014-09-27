#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include <cassert>

class Task3 : public Sencha::Task::GameTask {
private :
	int x;
	int y;
	int frametime;
public :
	Task3(){
		x = 0;
		y = 0;
		frametime = 0;
	}
	void set( int _x , int _y ){
		x = _x;
		y = _y;
	}
	virtual void onUpdate(){
		if( frametime == 30 ){
			DestroyTask( this );
		}
		frametime ++;
	}
	virtual void onDraw(){
		DrawFormatString( x , y , GetColor(128,255,255) , "Task3" );
	}
};

class Task2 : public Sencha::Task::GameTask {
private :
	int x;
	int y;
	int frametime;
public :
	void set( int _x , int _y ){
		x = _x;
		y = _y;
	}
	virtual void onInit(){
		x = 0;
		y = 0;
		frametime = 0;
	}

	virtual void onUpdate(){
		if( frametime == 10 ){
			this->insertTaskChild<Task3>()->set( x + 20 , y + 20 );
		}
		if( frametime == 60 ){
			this->DestroyTask( this );
		}
		frametime++;
	}

	virtual void onDraw(){
		DrawFormatString( x , y , GetColor(128,128,255) , "Task2" );
	}

	virtual void onFinish(){
		printfDx( "onFinish\n" );
	}
};

class MainTask : public Sencha::Task::GameTask {
private :
	int frametime;
public :
	virtual void onInit(){
		frametime = 0;
		printfDx( "MainTask Init\n" );
	}

	virtual void onUpdate(){
		if( frametime == 30 ){
			this->insertTaskChild<Task2>()->set(   0 , 40 );
			this->insertTaskChild<Task2>()->set(  50 , 40 );
			this->insertTaskChild<Task2>()->set( 100 , 40 );
			this->insertTaskChild<Task2>()->set( 150 , 40 );
		}
		if( frametime == 200 ){
			this->insertTaskChild<Task2>()->set(   0 , 40 );
			this->insertTaskChild<Task2>()->set(  50 , 40 );
			this->insertTaskChild<Task2>()->set( 100 , 40 );
			this->insertTaskChild<Task2>()->set( 150 , 40 );
		}
		if( frametime == 250 ){
			DestroyTask( this );
		}
		frametime++;
	}

	virtual void onDraw(){
		DrawFormatString( 0 , 20 , GetColor(255,0,255) , "Main Task" );
	}

	virtual void onFinish(){
		printfDx( "onFinish\n" );
	}
};



int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nShowCmd ){
	using namespace Sencha::Task;

	ChangeWindowMode( TRUE );
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );
	GameTask* global = CreateGlobalTask();
	GameTask* task = global->insertTaskChild<MainTask>();
	while( ProcessMessage() == 0 ){
		ClsDrawScreen();
		global->update();
		global->draw();
		DrawFormatString( 0 , 0 , 0xFF00FF , "MemoryPool %d" , Sencha::Task::GetTaskMemoryCount() );
		ScreenFlip();
	}

	GameTask::DestroyTask( global );
	DxLib_End();
	return 0;
}

