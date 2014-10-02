#include "sprite_manager.h"
#include "../xml/graph_xml_loader.h"

namespace Sencha {

GraphManager::GraphManager(){
}

const Graph2DCollection* GraphManager::findCollection( const char* name ){
	return this->getCollection( name );
}

void GraphManager::insertGraph2DCollection( const char* name , Graph2DCollectionData::tabledefineS* tabledefine , int length ){
	this->m_collection.push_back( new Graph2DCollection( name , tabledefine , length ) );
}

void GraphManager::insertGraph2DCollection( const char* fileName ){
	GraphXmlLoader loader( fileName );
	this->m_collection.push_back( loader.load() );
}

void GraphManager::releaseGraph2DCollection( const char* name ){
	Graph2DCollection* c = this->getCollection( name );
	if( c ){
		this->m_collection.remove( c );
		c->release();
		delete c;
	}
}

void GraphManager::clearGraph2DCollection(){
	std::list<Graph2DCollection*>::iterator iter = this->m_collection.begin();
	while( iter != this->m_collection.end() ){
		(*iter)->release();
		delete (*iter);
		iter++;
	}
	this->m_collection.clear();
}

} // namespace Sencha
