#pragma once
#include <cassert>


namespace Sencha{

/*
 * リストに梱包するコンテナオブジェクト
 * リストの要素にするオブジェクトは必ずこのクラスを継承する必要がある
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
 * リストオブジェクト
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
	 * 連結されている要素数を取得する。
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
	 * コンテナを追加する。
	 * 後ろに追加される
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
	 * 指定の位置にあるコンテナをリストから削除
	 * 先頭位置と終了位置のポインタは必要に応じて変更させる
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
	 * 先頭位置を次の位置に変更する。
	 * 先頭の連結は解除する。
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
	 * 末尾の位置を一つ前に変更。
	 * 末尾の連結は解除する。
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