#pragma once
#include "../task/task.h"
#include "../graph/graph2d.h"
#include "../exception/exception.h"
#include "../../lib/object/vec2.h"
#include "../../lib/object/color.h"
#include <memory>



// ゲームで使用するオブジェクトシステム基底
// オブジェクトには32文字までの名前を付けることができる
// super 32byte
class GameObject : public Sencha::Task::GameTask {
	friend class Layout;
private :
	char m_name[32]; // 32byte
	Vec2 m_localPos; // 8byte
	Vec2 m_worldPos; // 8byte
private :
	// 自分より下の階層でオブジェクト検索
	// 該当する名前のオブジェクトを取得する。
	template<typename T>
	T* _findObjectByName( const char* objectName ){
		Sencha::List::Iterator iter( this->m_child.top() );
		while( iter.hasMore() ){
			GameObject* task = iter.current<GameObject>();
			if( strcmp( objectName , task->m_name ) == 0 ){
				return dynamic_cast<T*>( task );
			}
			T* result = task->_findObjectByName<T>( objectName );
			if( result ){
				return result;
			}
			iter.next();
		}
		return NULL;
	}
public  :
	class ObjectNotFoundException : public Exception {
	public :
		const char* message;
		ObjectNotFoundException( const char* objectName , const type_info& info ){ 
sprintf_s<Exception::MESSAGE_LEN>(
buf , 
"findObjectByName Failure!!\n"
"objectName:%s\n"
"Type:%s\n" , 
objectName , 
info.name() ); }
	};

	GameObject(){
		memset( this->m_name , 0 , sizeof( this->m_name ) );
		this->m_localPos.set( 0 , 0 );
		this->m_worldPos.set( 0 , 0 );
	}
	template<typename T>
	T* findObjectByName( const char* objectName ){
		T* result = this->_findObjectByName<T>( objectName );
		if( !result ){
			throw ObjectNotFoundException( objectName , typeid(T*) );
		}
		return result;
	}

	virtual void onUpdate() override {
		this->m_worldPos.set( this->m_localPos.x , this->m_localPos.y );
		GameObject* parentTask = dynamic_cast<GameObject*>( this->getParentTask() );
		if( parentTask ){
			const Vec2& pos = parentTask->worldPos();
			this->m_worldPos.x += pos.x;
			this->m_worldPos.y += pos.y;
		}
	}
	void name( const char* s ){
		if( s ){
			strcpy_s<32>( this->m_name , s );
		}
	}
	const char* name(){
		return this->m_name;
	}
	void localPos( float x , float y ){
		this->m_localPos.set( x , y );
	}
	void localPos( Vec2 pos ){
		this->m_localPos = pos;
	}
	const Vec2& localPos(){
		return this->m_localPos;
	}
	const Vec2& worldPos(){
		return this->m_worldPos;
	}
	static void* operator new( size_t size ){
		return Sencha::Task::DefaultNew( size );
	}
	static void operator delete( void* p ){
		Sencha::Task::DefaultDelete( p );
	}
};


class Layout : public GameObject {
public :
};

class Picture : public GameObject {
private :
	const Sencha::Graph2D* m_graph2d;
	float m_ExRate;
	float m_Angle;
	unsigned char m_Turn;
	unsigned char m_Trans;
	unsigned char m_BlendMode;
	unsigned char m_Alpha;
	colorS m_color;
public  :
	const colorS& color(){
		return this->m_color;
	}
	void color( int r , int g , int b ){
		this->m_color.set( r , g , b );
	}
	void color( colorS& c ){
		this->m_color.set( c );
	}
	virtual void onInit() override {
		this->m_ExRate = 1.0f;
		this->m_Angle = 0.0f;
		this->m_Turn = FALSE;
		this->m_Trans = TRUE;
		this->m_Alpha = 255;
		this->m_color.set( 255 , 255 , 255 );
		this->m_BlendMode = DX_BLENDMODE_ALPHA;
	}
	void setSprite( const Sencha::Graph2D* sprite ){
		assert( sprite );
		this->m_graph2d = sprite;
	}
	virtual void onDraw() override {
		if( this->m_graph2d ){
			SetDrawBlendMode( this->m_BlendMode , this->m_Alpha );
			SetDrawBright( this->m_color.r , this->m_color.g , this->m_color.b );
			DrawRotaGraphF( this->worldPos().x , this->worldPos().y , this->m_ExRate , this->m_Angle , this->m_graph2d->handle() , this->m_Trans , this->m_Turn );
			SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );
			SetDrawBright( 255 , 255 , 255 );
		}
	}
};

class SelectMenuLayout : public Layout {
};