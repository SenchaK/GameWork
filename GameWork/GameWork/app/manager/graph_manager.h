#pragma once
#include "../graph/graph2d.h"
#include <list>

namespace Sencha {
/*
 * スプライト管理を行う。
 * シーン内でロードと破棄が簡単に行えるように設計する。
 * コレクションという単位で複数のスプライトを管理し、名前を振り分ける。
 * アプリ層は名前を指定してスプライトコレクションを取得してそこから必要なスプライトを使用する。
 */
class GraphManager {
private :
	std::list<Graph2DCollection*> m_collection;
private :
	Graph2DCollection* getCollection( const char* name ){
		for( std::list<Graph2DCollection*>::iterator iter = this->m_collection.begin() ; iter != this->m_collection.end() ; iter++ ){
			if( strcmp( (*iter)->name() , name ) == 0 ){
				return (*iter);
			}
		}
		return NULL;
	}
public  :
	GraphManager();
	const Graph2DCollection* findCollection( const char* name );
	void insertGraph2DCollection( const char* name , Graph2DCollectionData::tabledefineS* tabledefine , int length );
	void insertGraph2DCollection( const char* fileName );
	void releaseGraph2DCollection( const char* name );
	void clearGraph2DCollection();
};


}
