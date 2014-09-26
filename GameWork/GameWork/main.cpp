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
public :
	Sencha::List<Sencha::Container>* m_parent;
	Sencha::List<Sencha::Container> m_child;
	Task* m_parent_task;
public :
	Task(){
		m_parent_task = NULL;
		m_parent = NULL;
	}
	virtual ~Task(){
		FinishTask( this );
	}
	virtual void onInit(){
	}
	virtual void onUpdate(){
	}
	virtual void onDraw(){
	}
	virtual void onFinish(){
	}
	void update(){
		UpdateTask( this );
	}
	void draw(){
		DrawTask( this );
	}
	template<typename T>
	T* insertTask(){
		T* task = new T();
		this->m_child.add( task );
		task->m_parent = &this->m_child;
		task->m_parent_task = this;
		task->onInit();
		return task;
	}
public :
	/* ***********************************************************************
	 * Static Method
	 * *********************************************************************** */
	static void FinishTask( Task* task ){
		if( !task ){
			return;
		}
		Sencha::List<Sencha::Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			Task* task = (Task*)iter;
			Task* next = (Task*)iter->next;
			Task::DestroyTask( task );
			iter = next;
		}
	}
	static void UpdateTask( Task* task ){
		if( !task ){
			return;
		}
		task->onUpdate();
		Sencha::List<Sencha::Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			Task* task = (Task*)iter;
			Task* next = (Task*)task->next;
			UpdateTask( task );
			iter = next;
		}
	}
	static void DrawTask( Task* task ){
		if( !task ){
			return;
		}
		task->onDraw();
		Sencha::List<Sencha::Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			Task* task = (Task*)iter;
			Task* next = (Task*)task->next;
			DrawTask( task );
			iter = next;
		}
	}
	static void DestroyTask( Task* task ){
		assert( task );
		task->onFinish();
		task->m_parent->remove( task );
		delete task;
	}
	template<typename T>
	static T* InsertTask( Sencha::List<Sencha::Container>* tasklist ){
		assert( tasklist );
		T* task = new T();
		task->m_parent = tasklist;
		tasklist->add( task );
		return task;
	}
private :
	static void* operator new( size_t size ){
		return (Task*)task_pool.Malloc();
	}
	static void operator delete( void* p ){
		task_pool.Free( p );
	}
};

class TaskB : public Task {
private :
	int x;
	int y;
public :
	void setPos( int x , int y ){
		this->x = x;
		this->y = y;
	}
	virtual void onInit(){
		this->x = 0;
		this->y = 0;
	}
	virtual void onUpdate(){
		this->y -= 1;
		if( this->y < 0 ){
			DestroyTask( this );
		}
	}
	virtual void onDraw(){
		DrawCircle( x , y , 10 , 0x0000FF );
	}
};

class TaskA : public Task {
private :
	int x;
	int y;
	int vx;
	int vy;
	int frametime;
public :
	void setPos( int x , int y ){
		this->x = x;
		this->y = y;
	}
	virtual void onInit(){
		x = 0;
		y = 0;
		vx = 8;
		vy = 1;
		frametime = 0;
	}
	virtual void onUpdate(){
		x += vx;
		y += vy;
		if( frametime == 30 ){
			this->m_parent_task->insertTask<TaskB>()->setPos( x , y );
		}
		if( x >= 640 ){
			Task::DestroyTask( this );
		}
		frametime++;
	}
	virtual void onDraw(){
		DrawCircle( x , y , 5 , 0x00FFFF );
	}
};


class GlobalTask : public Task {
private :
	int frametime;
public :
	GlobalTask(){
		frametime = 0;
	}
	~GlobalTask(){
	}
	virtual void onUpdate(){
		if( frametime > 0  && frametime % 5 == 0 ){
			if( frametime < 120 ){
				insertTask<TaskA>()->setPos( 0 , 240 );
			}
		}
		frametime++;
		if( CheckHitKey( KEY_INPUT_ESCAPE ) ){
			DestroyTask( this );
		}
	}
	virtual void onDraw(){
		DrawFormatString( 0 , 20 , 0xFFFFFF , "GlobalTask %d" , this->m_child.count() );
	}
};


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nShowCmd ){
	ChangeWindowMode( TRUE );
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );


	Task* global = Task::InsertTask<GlobalTask>( &global_task );
	
	while( ProcessMessage() == 0 ){
		ClsDrawScreen();
		DxKeyboard::Update();
		DxJoypad::Update();

		if( global_task.isExist() ){
			Task::UpdateTask( global );
		}
		if( global_task.isExist() ){
			Task::DrawTask( global );
		}
		DrawFormatString( 0 , 0 , 0xFF00FF , "MemoryPool %d" , task_pool.count() );
		ScreenFlip();
	}

	if( global_task.isExist() ){
		Task::DestroyTask( global );
	}
	DxLib_End();
	return 0;
}

