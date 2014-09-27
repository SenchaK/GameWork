#pragma once
#include <stdio.h>
#include "../../lib/object/list.h"

namespace Sencha {
namespace Task {


// �^�X�N���N���X
// �Q�[�����̃I�u�W�F�N�g�͑S�Ă��̃^�X�N��ʂ��ď���������B
class GameTask : public Sencha::Container {
private :
	List<Container>* m_parent;
	List<Container> m_child;
	GameTask* m_parent_task;
public :
	// �q�^�X�N�̐����擾
	int childTaskCount(){
		return this->m_child.count();
	}
	// ������
	GameTask(){
		m_parent_task = NULL;
		m_parent = NULL;
	}
	// �q�K�w�̑S�Ẵ^�X�N���폜����
	virtual ~GameTask(){
		FinishTask( this );
	}

	// ---------------------------------------------------------
	// �p�������
	// ---------------------------------------------------------
	virtual void onInit(){
	}
	virtual void onUpdate(){
	}
	virtual void onDraw(){
	}
	virtual void onFinish(){
	}

	// ---------------------------------------------------------
	// �񋟋@�\
	// ---------------------------------------------------------
	virtual void update(){
		UpdateTask( this );
	}
	virtual void draw(){
		DrawTask( this );
	}
	virtual void finish(){
		DestroyTask( this );
	}

	// �^�X�N�̓o�^
	// ���݂̃^�X�N�̎q�K�w�Ƀ^�X�N�𐶐�����
	// �������ꂽ�^�X�N�͐e�^�X�N�̏��Ɛe���X�g�̏��̓�����悤�ɍ��B
	// �����ɐ��������ꍇ�AonInit�֐����Ăяo��
	// �^�X�N�͑S�Ċ�{�I�ɂ��̊֐����炵���������Ă͂����Ȃ�
	template<typename T>
	T* insertTask(){
		T* task = new T();
		this->m_child.add( task );
		task->m_parent = &this->m_child;
		task->m_parent_task = this;
		task->onInit();
		return task;
	}

	// �^�X�N�̓o�^
	// ���݂̃^�X�N�̐e�K�w�Ƀ^�X�N�𐶐�����B
	template<typename T>
	T* insertTaskFromParent(){
		assert( this->m_parent_task );
		return this->m_parent_task->insertTask<T>();
	}
public :
	/* ***********************************************************************
	 * Static Method
	 * *********************************************************************** */

	// �f�X�g���N�^���ɌĂяo���B
	// ��������q�K�w�̃^�X�N��S�č폜����
	static void FinishTask( GameTask* task ){
		if( !task ){
			return;
		}
		List<Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			GameTask* task = (GameTask*)iter;
			GameTask* next = (GameTask*)iter->next;
			GameTask::DestroyTask( task );
			iter = next;
		}
	}

	// �^�X�N�ւ̍X�V�����𑖂点��
	static void UpdateTask( GameTask* task ){
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

	// �^�X�N�ւ̕`�揈���𑖂点��
	static void DrawTask( GameTask* task ){
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

	// �^�X�N�̔j��
	// ��{�I�ɂ��̊֐����炵���^�X�N�̍폜���s���Ă͂����Ȃ�
	static void DestroyTask( GameTask* task ){
		assert( task );
		task->onFinish();
		task->m_parent->remove( task );
		delete task;
	}

	// �^�X�N�̑}��
	// �w�肵�����X�g�̎q�K�w�Ƀ^�X�N��}������B
	template<typename T>
	static T* InsertTask( Sencha::List<Sencha::Container>* tasklist ){
		assert( tasklist );
		T* task = new T();
		task->m_parent = tasklist;
		tasklist->add( task );
		return task;
	}
private :
	/* ************************************************** *
	 * new/delete���Z�q�̓A�N�Z�X�ł��Ȃ�����B
	 * �C���X�^���X�����������ꍇ��insertTask���\�b�h���g���B
	 * �j���������ꍇ��DestroyTask���\�b�h���g���B
	 * �ʂ̃������v�[�����K�v�ȃ^�X�N�̂݃I�[�o�[���[�h���Ă��ǂ����A
	 * �K��private�ɂ��邱��
	 * ************************************************** */
	static void* operator new   ( size_t size );
	static void  operator delete( void* p );
};


// �O���[�o���^�X�N
// �ÓI�ȃ^�X�N���X�g��e�Ƃ����ŏ�ʃ^�X�N
class GlobalTask : public GameTask {
public :
	virtual void update() override;
	virtual void draw() override;
	virtual void finish() override;
};

// �O���[�o���^�X�N�𐶐�����B
GlobalTask* CreateGlobalTask();

// ���݂̃^�X�N�p�������u���b�N��
int GetTaskMemoryCount();

} // namespace Task
} // namespace Sencha