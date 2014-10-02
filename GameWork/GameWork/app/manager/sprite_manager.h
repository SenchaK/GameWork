#pragma once
#include "../graph/graph2d.h"
#include <list>

namespace Sencha {
/*
 * �X�v���C�g�Ǘ����s���B
 * �V�[�����Ń��[�h�Ɣj�����ȒP�ɍs����悤�ɐ݌v����B
 * �R���N�V�����Ƃ����P�ʂŕ����̃X�v���C�g���Ǘ����A���O��U�蕪����B
 * �A�v���w�͖��O���w�肵�ăX�v���C�g�R���N�V�������擾���Ă�������K�v�ȃX�v���C�g���g�p����B
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
	GraphManager(){
	}
	const Graph2DCollection* findCollection( const char* name ){
		return this->getCollection( name );
	}
	void insertGraph2DCollection( const char* name , Graph2DCollectionData::tabledefineS* tabledefine , int length ){
		this->m_collection.push_back( new Graph2DCollection( name , tabledefine , length ) );
	}
	void insertGraph2DCollection( const char* fileName ){
		this->m_collection.push_back( new Graph2DCollection( fileName ) );
	}
	void releaseGraph2DCollection( const char* name ){
		Graph2DCollection* c = this->getCollection( name );
		if( c ){
			this->m_collection.remove( c );
			c->release();
			delete c;
		}
	}
	void clearGraph2DCollection(){
		std::list<Graph2DCollection*>::iterator iter = this->m_collection.begin();
		while( iter != this->m_collection.end() ){
			(*iter)->release();
			delete (*iter);
			iter++;
		}
		this->m_collection.clear();
	}
};


}
