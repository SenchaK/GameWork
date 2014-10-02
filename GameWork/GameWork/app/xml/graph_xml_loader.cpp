#include "graph_xml_loader.h"

namespace Sencha {
GraphXmlLoader::GraphXmlLoader( const char* fileName ){
	this->m_fileName = fileName;
}

Graph2DCollection* GraphXmlLoader::load(){
	assert( this->m_fileName );
	tinyxml2::XMLDocument doc;
	doc.LoadFile( this->m_fileName );
	Graph2DCollection* result = new Graph2DCollection();
	exec( result , doc.FirstChildElement() );
	return result;
}

void GraphXmlLoader::exec( Graph2DCollection* data , tinyxml2::XMLElement* element ){
	if( !element ) return;
	if( strcmp( element->Name() , "graphic_collection" ) == 0 ){
		if( element->Attribute( "name" ) ){
			data->setName( element->Attribute( "name" ) );
		}
		for( tinyxml2::XMLElement* e = element->FirstChildElement() ; e ; e = e->NextSiblingElement() ){
			if( strcmp( e->Name() , "graphic" ) == 0 ){
				this->graphic( data , e );
			}
		}
	}
}

void GraphXmlLoader::graphic( Graph2DCollection* data , tinyxml2::XMLElement* element ){
	if( !element ) return;
	Graph2DCollectionData::tabledefineS table;
	if( element->Attribute( "name" ) ){
		strcpy_s<Graph2D::NAME_SIZE>( table.name , element->Attribute( "name" ) );
	}
	if( element->Attribute( "path" ) ){
		table.path = element->Attribute( "path" );
	}
	if( element->Attribute( "gid" ) ){
		table.gid = element->IntAttribute( "gid" );
	}
	if( element->Attribute( "x" ) ){
		table.xNum = element->IntAttribute( "x" );
	}
	if( element->Attribute( "y" ) ){
		table.yNum = element->IntAttribute( "y" );
	}
	data->insertTable( table );
}

} // namespace Sencha
