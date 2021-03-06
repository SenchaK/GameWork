#pragma once
#include "../object/list.h"
#include <memory>

namespace Sencha {


template<size_t memory_block_size,size_t memory_pool_size>
class MemoryPool {
private :
	// メモリブロック
	class memory : public Sencha::Container {
	public :
		char buf[memory_block_size];
	};
	memory m_memory[memory_pool_size];
	List m_list;
public :
	MemoryPool(){
		memset( m_memory , 0 , sizeof( m_memory ) );
		for( size_t i = 0 ; i < memory_pool_size ; i++ ){
			m_list.add( &m_memory[i] );
		}
	}

	size_t count(){
		return m_list.count();
	}

	void* Malloc(){
		memory* m = reinterpret_cast<memory*>( m_list.pop() );
		return m;
	}

	void Free( void* p ){
		m_list.add( reinterpret_cast<memory*>( p ) );
	}
};

} // namespace Sencha