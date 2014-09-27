#include "task.h"
#include "../../lib/object/list.h"
#include <memory>
#include "../../DxLib/DxLib.h"


namespace Sencha {
namespace Task {
namespace {
/* ************************************************************* *
 * タスクのメモリ定義
 * ************************************************************* */
#include "../../lib/memory/memory_pool.h"
} // namespace 

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

// タスク共有メモリプール
// 固定長プールなのでタスクの領域には注意すること
static Sencha::Task::MemoryPool task_pool;

// タスク用メモリ確保関数
void* DefaultNew( size_t size ){
	return (GameTask*)task_pool.Malloc();
}
// タスク用メモリ解放関数
void DefaultDelete( void* p ){
	task_pool.Free( p );
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
