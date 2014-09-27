/* ******************************************************************************* *
 * このファイルはメモリプールを定義したいスコープにインクルードする。
 * 必ずインクルード先のソースファイルでは
 * ・MEMORY_BLOACK_SIZE ... メモリブロックの大きさ
 * ・MEMORY_POOL_SIZE   ... メモリプールの大きさ
 * の2つの定数を定義する
 * <memory>
 * "list.h"
 * は必ずインクルードすること
 * ******************************************************************************* */
class MemoryPool {
private :
	// メモリブロック
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
