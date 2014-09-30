#pragma once
#include "../sprite/sprite_collection.h"
#include <list>

namespace Sencha {
/*
 * �X�v���C�g�Ǘ����s���B
 * �V�[�����Ń��[�h�Ɣj�����ȒP�ɍs����悤�ɐ݌v����B
 * �R���N�V�����Ƃ����P�ʂŕ����̃X�v���C�g���Ǘ����A���O��U�蕪����B
 * �A�v���w�͖��O���w�肵�ăX�v���C�g�R���N�V�������擾���Ă�������K�v�ȃX�v���C�g���g�p����B
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
