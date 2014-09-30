#pragma once
#include "../sprite/sprite_collection.h"

namespace Sencha {
/*
 * �X�v���C�g�Ǘ����s���B
 * �V�[�����Ń��[�h�Ɣj�����ȒP�ɍs����悤�ɐ݌v����B
 * �R���N�V�����Ƃ����P�ʂŕ����̃X�v���C�g���Ǘ����A���O��U�蕪����B
 * �A�v���w�͖��O���w�肵�ăX�v���C�g�R���N�V�������擾���Ă�������K�v�ȃX�v���C�g���g�p����B
 */
class SpriteManager {
private :
	std::vector<SpriteCollection> m_collection;
	SpriteCollection m_null_collection;
public  :
	SpriteManager(){
	}
	const SpriteCollection* findCollection( const char* name ){
		for( size_t i = 0 ; i < this->m_collection.size() ; i++ ){
			if( strcmp( this->m_collection[i].name() , name ) == 0 ){
				return &this->m_collection[i];
			}
		}
		return &m_null_collection;
	}
	void insertCollection( const char* name , SpriteCollectionData::tabledefineS* tabledefine , int length ){
		m_collection.push_back( SpriteCollection( name , tabledefine , length ) );
	}
};


}
