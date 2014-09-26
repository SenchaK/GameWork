#pragma once
#include <cassert>
#include <cstdio>
#include "../object/list.h"


namespace Sencha {

class MemoryPool {
private :
	class memory : public Sencha::Container {
	public :
		char buf[128];
	};

	memory m_memory[256];
	List<memory> m_list;
public :
	MemoryPool(){
		memset( m_memory , 0 , sizeof( m_memory ) );
		for( size_t i = 0 ; i < 256 ; i++ ){
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


} // namespace Sencha