#include "task.h"
#include "../lib/object/list.h"
#include <memory>


namespace Sencha {
namespace Task {
namespace {
/* ************************************************************* *
 * �^�X�N�̃�������`
 * ************************************************************* */
static const int MEMORY_BLOCK_SIZE = 256;
static const int MEMORY_POOL_SIZE = 512;
#include "../lib/memory/memory_pool.h"
} // namespace 


// �^�X�N���L�������v�[��
// �Œ蒷�v�[���Ȃ̂Ń^�X�N�̗̈�ɂ͒��ӂ��邱��
static Sencha::Task::MemoryPool task_pool;

// ���X�g�\���̃��[�g����
// �q�K�w�ɃO���[�o���^�X�N�����݂���
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

// �O���[�o���^�X�N���쐬����B
GlobalTask* CreateGlobalTask(){
	return GameTask::InsertTask<GlobalTask>( &global_task );
}

// ���݂̃^�X�N�p�������u���b�N��
int GetTaskMemoryCount(){
	return task_pool.count();
}

} // namespace Task
} // namespace Sencha
