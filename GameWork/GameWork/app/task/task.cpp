#include "task.h"
#include "../../lib/object/list.h"
#include <memory>
#include "../../DxLib/DxLib.h"


namespace Sencha {
namespace Task {
namespace {
/* ************************************************************* *
 * �^�X�N�̃�������`
 * ************************************************************* */
#include "../../lib/memory/memory_pool.h"
} // namespace 

// ************************************************************** *
// �^�X�N�}�l�[�W����������
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

// �g�p���Ȃ��Ȃ����^�X�N���`�F�b�N�ɓ����
void TaskManager::checkUnuse( GameTask* ptask ){
	*p = ptask;
	*p++;
}

// ������s��
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

// �^�X�N���L�������v�[��
// �Œ蒷�v�[���Ȃ̂Ń^�X�N�̗̈�ɂ͒��ӂ��邱��
static Sencha::Task::MemoryPool task_pool;

// �^�X�N�p�������m�ۊ֐�
void* DefaultNew( size_t size ){
	return (GameTask*)task_pool.Malloc();
}
// �^�X�N�p����������֐�
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

// �O���[�o���^�X�N���쐬����B
GlobalTask* CreateGlobalTask(){
	GlobalTask* task = new GlobalTask();
	return task;
}

// ���݂̃^�X�N�p�������u���b�N��
int GetTaskMemoryCount(){
	return task_pool.count();
}

} // namespace Task
} // namespace Sencha
