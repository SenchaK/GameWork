#include "task.h"
#include "../lib/object/list.h"
#include <memory>


namespace Sencha {
namespace Task {
namespace {
/* ************************************************************* *
 * タスクのメモリ定義
 * ************************************************************* */
static const int MEMORY_BLOCK_SIZE = 256;
static const int MEMORY_POOL_SIZE = 512;
#include "../lib/memory/memory_pool.h"
} // namespace 


// タスク共有メモリプール
// 固定長プールなのでタスクの領域には注意すること
static Sencha::Task::MemoryPool task_pool;

// リスト構造のルート部分
// 子階層にグローバルタスクが存在する
static Sencha::List<Sencha::Container> global_task;

// static
void* GameTask::operator new( size_t size ){
	return (GameTask*)task_pool.Malloc();
}
// static
void GameTask::operator delete( void* p ){
	task_pool.Free( p );
}


// override
void GlobalTask::update(){
	if( global_task.isExist() ){
		GameTask::UpdateTask( this );
	}
}

// override
void GlobalTask::draw(){
	if( global_task.isExist() ){
		GameTask::DrawTask( this );
	}
}

// override
void GlobalTask::finish(){
	if( global_task.isExist() ){
		GameTask::DestroyTask( this );
	}
}

// グローバルタスクを作成する。
GlobalTask* CreateGlobalTask(){
	return GameTask::InsertTask<GlobalTask>( &global_task );
}

// 現在のタスク用メモリブロック個数
int GetTaskMemoryCount(){
	return task_pool.count();
}

} // namespace Task
} // namespace Sencha
