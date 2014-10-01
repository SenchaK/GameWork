#pragma once
#include <cassert>


namespace Sencha{

/*
 * ���X�g�ɍ����R���e�i�I�u�W�F�N�g
 * ���X�g�̗v�f�ɂ���I�u�W�F�N�g�͕K�����̃N���X���p������K�v������
 */
class Container {
public :
	typedef Container* container;
	container prev;
	container next;
public :
	Container(){
	}

	void add( container item ){
		assert( item );
		item->prev = this;
		item->next = this->next;
		if( item->next ){
			item->next->prev = item;
		}
		this->next = item;
	}

	container remove(){
		if( prev ){ 
			prev->next = next;
		}
		if( next ){
			next->prev = prev;
		}
		this->prev = NULL;
		this->next = NULL;
		return this;
	}
};


/*
 * ���X�g�I�u�W�F�N�g
 */
class List {
public :
	typedef Container* container;

	class Iterator {
	private :
		container m_current;
		container m_next;
	public :
		template<typename T>
		T* current(){
			return (T*)m_current;
		}

		Iterator( container c ){
			m_current = c;
			if( m_current ){
				m_next = m_current->next;
			}
		}
		bool hasMore(){
			if( m_current ){
				return true;
			}
			return false;
		}
		void next(){
			m_current = m_next;
			if( m_current ){
				m_next = m_current->next;
			}
		}
	};
private :
	container m_top;
	container m_bottom;
public :
	List(){
		m_top = NULL;
		m_bottom = NULL;
	}

	/*
	 * �A������Ă���v�f�����擾����B
	 */
	size_t count(){
		size_t c = 0;
		container iter = m_top;
		while( iter != NULL ){
			c++;
			iter = (container)iter->next;
		}
		return c;
	}

	bool isExist(){
		if( this->m_top )    return true;
		if( this->m_bottom ) return true;
		return false;
	}

	/*
	 * �R���e�i��ǉ�����B
	 * ���ɒǉ������
	 */
	void add( container item ){
		if( !m_bottom ){
			m_bottom = item;
			m_top = m_bottom;
			return;
		}
		assert( m_bottom );
		m_bottom->add( item );
		m_bottom = item;
	}

	/*
	 * �w��̈ʒu�ɂ���R���e�i�����X�g����폜
	 * �擪�ʒu�ƏI���ʒu�̃|�C���^�͕K�v�ɉ����ĕύX������
	 */
	void remove( container item ){
		if( m_top == item ){
			pop_front();
			return;
		}
		if( m_bottom == item ){
			pop();
			return;
		}

		container iter = m_top;
		while( iter != NULL ){
			if( iter == item ){
				iter->remove();
				return;
			}
			iter = (container)iter->next;
		}
	}

	/*
	 * �擪�ʒu�����̈ʒu�ɕύX����B
	 * �擪�̘A���͉�������B
	 */
	container pop_front(){
		container result = m_top;
		container next_top = (container)m_top->next;
		m_top->remove();
		m_top = next_top;
		if( !m_top ){
			m_bottom = m_top;
		}
		return result;
	}

	/*
	 * �����̈ʒu����O�ɕύX�B
	 * �����̘A���͉�������B
	 */
	container pop(){
		container result = m_bottom;
		container next_bottom = (container)m_bottom->prev;
		m_bottom->remove();
		m_bottom = next_bottom;
		if( !m_bottom ){
			m_top = m_bottom;
		}
		return result;
	}

	container top(){
		return m_top;
	}

	void clear(){
		while( m_top ){
			pop_front();
		}
	}
};

}