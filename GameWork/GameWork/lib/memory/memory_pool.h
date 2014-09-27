/* ******************************************************************************* *
 * ���̃t�@�C���̓������v�[�����`�������X�R�[�v�ɃC���N���[�h����B
 * �K���C���N���[�h��̃\�[�X�t�@�C���ł�
 * �EMEMORY_BLOACK_SIZE ... �������u���b�N�̑傫��
 * �EMEMORY_POOL_SIZE   ... �������v�[���̑傫��
 * ��2�̒萔���`����
 * <memory>
 * "list.h"
 * �͕K���C���N���[�h���邱��
 * ******************************************************************************* */
class MemoryPool {
private :
	// �������u���b�N
	class memory : public Sencha::Container {
	public :
		char buf[MEMORY_BLOCK_SIZE];
	};
	memory m_memory[MEMORY_POOL_SIZE];
	List<memory> m_list;
public :
	MemoryPool(){
		memset( m_memory , 0 , sizeof( m_memory ) );
		for( size_t i = 0 ; i < MEMORY_POOL_SIZE ; i++ ){
			m_list.add( &m_memory[i] );
		}
	}

	size_t count(){
		return m_list.count();
	}

	void* Malloc(){
		memory* m = m_list.pop();
		return m;
	}

	void Free( void* p ){
		m_list.add( (memory*)p );
	}
};
