#pragma once
#include <stdio.h>
#include "../../lib/object/list.h"

namespace Sencha {
namespace Task {
static const int MEMORY_BLOCK_SIZE = 256;
static const int MEMORY_POOL_SIZE  = 512;

class GameTask;
class TaskManager {
	typedef GameTask* task_p;
private :
	task_p collector[MEMORY_POOL_SIZE];
	task_p* p;
	TaskManager();
public :
	// �C���X�^���X�擾
	static TaskManager& Instance();
	// �g�p���Ȃ��Ȃ����^�X�N���`�F�b�N�ɓ����
	void checkUnuse( GameTask* ptask );
	// ������s��
	void collect();
};

// �^�X�N���N���X
// �Q�[�����̃I�u�W�F�N�g�͑S�Ă��̃^�X�N��ʂ��ď���������B
// super  8byte
// vtable 4byte
class GameTask : public Sencha::Container {
	friend class TaskManager;
protected :
	List* m_parent           ; // 4byte
	List m_child             ; // 8byte
	GameTask* m_parent_task  ; // 4byte
	int m_delete_check       ; // 4byte
public :
	GameTask* getParentTask(){
		return m_parent_task;
	}
	// �q�^�X�N�̐����擾
	int childTaskCount(){
		return this->m_child.count();
	}
	// ������
	GameTask(){
		m_parent_task = NULL;
		m_parent = NULL;
		m_delete_check = 0;
	}

	// �f�X�g���N�^
	virtual ~GameTask(){
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
	// �X�V
	virtual void update();
	// �`��
	virtual void draw();
	// �q�K�w�̑S�Ẵ^�X�N���폜����
	virtual void finish();
	// �e����؂藣��
	void removeTask(){
		this->m_parent->remove( this );
		this->m_parent = NULL;
		this->m_parent_task = NULL;
	}
	// �����ς݂̃^�X�N�����X�g�ɓo�^
	// insert�ƈႢ�������Ȃ��ׁAonInit���Ă΂�Ȃ�
	// �o�^����鑤�̃^�X�N�͐e�q�K�w�������Ă��Ȃ���Ԃł��邱�Ƃ��O��(���O��remove���Ă�������)
	void entryTask( GameTask* task ){
		assert( task );
		assert( !task->m_parent );
		assert( !task->m_parent_task );
		this->m_child.add( task );
		task->m_parent = &this->m_child;
		task->m_parent_task = this;
	}
	// �^�X�N�̓o�^
	// ���݂̃^�X�N�̎q�K�w�Ƀ^�X�N�𐶐�����
	// �������ꂽ�^�X�N�͐e�^�X�N�̏��Ɛe���X�g�̏��̓�����悤�ɍ��B
	// �����ɐ��������ꍇ�AonInit�֐����Ăяo��
	// �^�X�N�͑S�Ċ�{�I�ɂ��̊֐����炵���������Ă͂����Ȃ�
	template<typename T>
	T* insertTaskChild(){
		T* task = new T();
		this->m_child.add( task );
		task->m_parent = &this->m_child;
		task->m_parent_task = this;
		task->onInit();
		return task;
	}

	// �^�X�N�̓o�^
	// �Ăяo�����̊K�w�Ɠ����K�w�Ƀ^�X�N�𐶐�����B
	template<typename T>
	T* insertTask(){
		assert( this->m_parent_task );
		return this->m_parent_task->insertTaskChild<T>();
	}

	// �^�X�N�̓o�^
	// �Ăяo�����̐e�K�w�Ƀ^�X�N�𐶐�����B
	template<typename T>
	T* insertTaskParent(){
		assert( this->m_parent_task );
		return this->m_parent_task->insertTask<T>();
	}
public :
	/* ***********************************************************************
	 * Static Method
	 * *********************************************************************** */
	// �f�X�g���N�^���ɌĂяo���B
	// ��������q�K�w�̃^�X�N��S�č폜����
	static void FinishTask( GameTask* task );
	// �^�X�N�ւ̍X�V�����𑖂点��
	static void UpdateTask( GameTask* task );
	// �^�X�N�ւ̕`�揈���𑖂点��
	static void DrawTask( GameTask* task );
	// ************************************************** *
	// �^�X�N�̔j��
	// �A������������������B
	// ���̒i�K�ł̓^�X�N�ɑ΂���delete�͌Ă΂ꂸ
	// update,draw�̍Ō�ɌĂяo��
	// ************************************************** *
	static void DestroyTask( GameTask* task );
private :
	/* ************************************************** *
	 * new/delete���Z�q�̓A�N�Z�X�ł��Ȃ�����B
	 * �C���X�^���X�����������ꍇ��insertTask���\�b�h���g���B
	 * �j���������ꍇ��DestroyTask���\�b�h���g���B
	 * �ʂ̃������v�[�����K�v�ȏꍇ�⃍�O���d���݂����ꍇ�̓I�[�o�[���[�h����
	 * (friend�w�肵�Ȃ���private�ɂ���ƌp����œ{����̂Œ���)
	 * ************************************************** */
	static void* operator new   ( size_t size );
	static void  operator delete( void* p );
};

// ���݂̃^�X�N�p�������u���b�N��
int GetTaskMemoryCount();

// �^�X�N�V�X�e���Ŏg�p����f�t�H���g�̃������m�ۏ���
void* DefaultNew( size_t size );

// �^�X�N�V�X�e���Ŏg�p�����f�t�H���g�̉������
void DefaultDelete( void* p );

} // namespace Task
} // namespace Sencha