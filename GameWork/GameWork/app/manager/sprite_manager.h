#pragma once
#include "../sprite/sprite_collection.h"

namespace Sencha {
/*
 * スプライト管理を行う。
 * シーン内でロードと破棄が簡単に行えるように設計する。
 * コレクションという単位で複数のスプライトを管理し、名前を振り分ける。
 * アプリ層は名前を指定してスプライトコレクションを取得してそこから必要なスプライトを使用する。
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
