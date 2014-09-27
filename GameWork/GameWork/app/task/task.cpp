#include "task.h"
#include "../../lib/object/list.h"
#include <memory>
#include "../../DxLib/DxLib.h"
#include "../../lib/memory/memory_pool.h"

namespace Sencha {
namespace Task {
// �^�X�N���L�������v�[��
// �Œ蒷�v�[���Ȃ̂Ń^�X�N�̗̈�ɂ͒��ӂ��邱��
static Sencha::MemoryPool<MEMORY_BLOCK_SIZE,MEMORY_POOL_SIZE> task_pool;

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



// �^�X�N�p�������m�ۊ֐�
void* DefaultNew( size_t size ){
	return (GameTask*)task_pool.Malloc();
}
// �^�X�N�p����������֐�
void DefaultDelete( void* p ){
	task_pool.Free( p );
}

// virtual
// �X�V
void GameTask::update(){
	UpdateTask( this );
	TaskManager::Instance().collect();
}
// virtual
// �`��
void GameTask::draw(){
	DrawTask( this );
	TaskManager::Instance().collect();
}
// virtual
// �q�K�w�̑S�Ẵ^�X�N���폜����
void GameTask::finish(){
	FinishTask( this );
}

// static
// �f�X�g���N�^���ɌĂяo���B
// ��������q�K�w�̃^�X�N��S�č폜����
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
// �^�X�N�ւ̍X�V�����𑖂点��
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
// �^�X�N�ւ̕`�揈���𑖂点��
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
// �^�X�N�̔j��
// �A������������������B
// ���̒i�K�ł̓^�X�N�ɑ΂���delete�͌Ă΂ꂸ
// update,draw�̍Ō�ɉ���������Ăяo��
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
