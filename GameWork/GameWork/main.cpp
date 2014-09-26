#include "DxLib\DxLib.h"
#include "app\keyboard\DxKey.h"
#include "app\keyboard\DxJoypad.h"
#include "app\app_input.h"
#include "lib\object\list.h"
#include "lib\memory\memory_pool.h"
#include <cassert>
Sencha::MemoryPool task_pool;
Sencha::List<Sencha::Container> global_task;


class Task : public Sencha::Container {
protected :
	Sencha::List<Sencha::Container>* m_parent;
	Sencha::List<Sencha::Container> m_child;
public :
	Task(){
	}

	virtual ~Task(){
	}

	virtual void onInit(){
	}
	virtual void onUpdate(){
	}
	virtual void onDraw(){
	}

	virtual void update(){
		onUpdate();
		Sencha::List<Sencha::Container>::iterator iter = m_child.top();
		while( iter != NULL ){
			Task* task = (Task*)iter;
			Task* next = (Task*)task->next;
			task->update();
			iter = next;
		}
	}

	virtual void draw(){
		onDraw();
		Sencha::List<Sencha::Container>::iterator iter = m_child.top();
		while( iter != NULL ){
			Task* task = (Task*)iter;
			Task* next = (Task*)task->next;
			task->draw();
			iter = next;
		}
	}


private :
	static void* operator new( size_t size ){
		return NULL;
	}
	static void operator delete( void* p ){
	}
};

class GlobalTask : public Task {
public :
	GlobalTask(){
		this->m_parent = &global_task;
		global_task.add( this );
	}
	~GlobalTask(){
		global_task.remove( this );	
	}
	virtual void onUpdate(){
		printfDx( "global::onUpdate\n" );
	}
	static void* operator new( size_t size ){
		return (Task*)task_pool.Malloc();
	}
	static void operator delete( void* p ){
		task_pool.Free( p );
	}
};


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nShowCmd ){
	ChangeWindowMode( TRUE );
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );


	GlobalTask* global = new GlobalTask();
	while( ProcessMessage() == 0 ){
		ClsDrawScreen();
		DxKeyboard::Update();
		DxJoypad::Update();

		Sencha::List<Sencha::Container>::iterator iter = global_task.top();
		while( iter != NULL ){
			Task* task = (Task*)iter;
			Task* next = (Task*)task->next;
			task->update();
			task->draw();
			iter = next;
		}
		ScreenFlip();
	}

	delete global;
	DxLib_End();
	return 0;
}

