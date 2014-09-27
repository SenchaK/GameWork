#pragma once

namespace Sencha {
template<typename T>
class SharedPtr {
private :
	int ref;
	T* ptr;
public  :
	SharedPtr( T* t = 0 ){
		if( t ){
			ref = 1;
		}
		ptr = t;
	}
	SharedPtr( const SharedPtr<T>& o ){
		if( o.ptr ){
			ref = o.ref + 1;
		}
		ptr = o.ptr;
	}
	void release(){
		ref--;
		if( ref <= 0 ){
			delete ptr;
		}
	}
	~SharedPtr(){
		release();
	}
};

} // namespace Sencha