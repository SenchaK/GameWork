#pragma once
#include "../sprite/sprite_collection.h"
#include <list>

namespace Sencha {
/*
 * スプライト管理を行う。
 * シーン内でロードと破棄が簡単に行えるように設計する。
 * コレクションという単位で複数のスプライトを管理し、名前を振り分ける。
 * アプリ層は名前を指定してスプライトコレクションを取得してそこから必要なスプライトを使用する。
 */
class SpriteManager {
private :
	std::list<SpriteCollection*> m_collection;
private :
	SpriteCollection* getCollection( const char* name ){
		for( std::list<SpriteCollection*>::iterator iter = this->m_collection.begin() ; iter != this->m_collection.end() ; iter++ ){
			if( strcmp( (*iter)->name() , name ) == 0 ){
				return (*iter);
			}
		}
		return NULL;
	}
public  :
	SpriteManager(){
	}
	const SpriteCollection* findCollection( const char* name ){
		return this->getCollection( name );
	}
	void insertCollection( const char* name , SpriteCollectionData::tabledefineS* tabledefine , int length ){
		this->m_collection.push_back( new SpriteCollection( name , tabledefine , length ) );
	}
	void releaseCollection( const char* name ){
		SpriteCollection* c = this->getCollection( name );
		if( c ){
			this->m_collection.remove( c );
			c->release();
			delete c;
		}
	}
	void clearCollection(){
		std::list<SpriteCollection*>::iterator iter = this->m_collection.begin();
		while( iter != this->m_collection.end() ){
			(*iter)->release();
			delete (*iter);
			iter++;
		}
		this->m_collection.clear();
	}
};


}
