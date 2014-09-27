#include "task.h"
#include "../../lib/object/list.h"
#include <memory>
#include "../../DxLib/DxLib.h"
#include "../../lib/memory/memory_pool.h"

namespace Sencha {
namespace Task {
// タスク共有メモリプール
// 固定長プールなのでタスクの領域には注意すること
static Sencha::MemoryPool<MEMORY_BLOCK_SIZE,MEMORY_POOL_SIZE> task_pool;

// ************************************************************** *
// タスクマネージャ処理部分
// ************************************************************** *
// static
TaskManager& TaskManager::Instance(){
	static TaskManager instance;
	return instance;
}

TaskManager::TaskManager(){
	memset( collector , 0 , sizeof( collector ) );
	p = collector;
}

// 使用しなくなったタスクをチェックに入れる
void TaskManager::checkUnuse( GameTask* ptask ){
	*p = ptask;
	*p++;
}

// 解放を行う
void TaskManager::collect(){
	int count = 0;
	for( task_p* pos = collector ; pos != p ; *pos++ ){
		count++;
		GameTask* task = *pos;
		assert( task );
		delete task;
		*pos = NULL;
	}
	p = collector;
}



// タスク用メモリ確保関数
void* DefaultNew( size_t size ){
	return (GameTask*)task_pool.Malloc();
}
// タスク用メモリ解放関数
void DefaultDelete( void* p ){
	task_pool.Free( p );
}

// virtual
// 更新
void GameTask::update(){
	UpdateTask( this );
	TaskManager::Instance().collect();
}
// virtual
// 描画
void GameTask::draw(){
	DrawTask( this );
	TaskManager::Instance().collect();
}
// virtual
// 子階層の全てのタスクを削除する
void GameTask::finish(){
	FinishTask( this );
}

// static
// デストラクタ時に呼び出す。
// 所持する子階層のタスクを全て削除する
void GameTask::FinishTask( GameTask* task ){
	if( !task ){
		return;
	}
	task->onFinish();
	List<Container>::iterator iter = task->m_child.top();
	while( iter != NULL ){
		GameTask* task = (GameTask*)iter;
		GameTask* next = (GameTask*)iter->next;
		GameTask::DestroyTask( task );
		iter = next;
	}
}

// static
// タスクへの更新処理を走らせる
void GameTask::UpdateTask( GameTask* task ){
	if( !task ){
		return;
	}
	task->onUpdate();
	List<Container>::iterator iter = task->m_child.top();
	while( iter != NULL ){
		GameTask* task = (GameTask*)iter;
		GameTask* next = (GameTask*)task->next;
		UpdateTask( task );
		iter = next;
	}
}

// static
// タスクへの描画処理を走らせる
void GameTask::DrawTask( GameTask* task ){
	if( !task ){
		return;
	}
	task->onDraw();
	List<Container>::iterator iter = task->m_child.top();
	while( iter != NULL ){
		GameTask* task = (GameTask*)iter;
		GameTask* next = (GameTask*)task->next;
		DrawTask( task );
		iter = next;
	}
}

// static
// タスクの破棄
// 連結から解除し回収する。
// この段階ではタスクに対してdeleteは呼ばれず
// update,drawの最後に解放処理を呼び出す
void GameTask::DestroyTask( GameTask* task ){
	assert( task );
	assert( task->m_delete_check == 0 );
	task->m_delete_check = 1;
	task->finish();
	task->m_parent->remove( task );
	task->m_parent_task = NULL;
	TaskManager::Instance().checkUnuse( task );
}

// static
void* GameTask::operator new( size_t size ){
	return DefaultNew( size );
}
// static
void GameTask::operator delete( void* p ){
	DefaultDelete( p );
}


// static
void* GlobalTask::operator new( size_t size ){
	return DefaultNew( size );
}
// static
void GlobalTask::operator delete( void* p ){
	DefaultDelete( p );
}

// グローバルタスクを作成する。
GlobalTask* CreateGlobalTask(){
	GlobalTask* task = new GlobalTask();
	return task;
}

// 現在のタスク用メモリブロック個数
int GetTaskMemoryCount(){
	return task_pool.count();
}

} // namespace Task
} // namespace Sencha
