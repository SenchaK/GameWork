#pragma once

#include "../../lib/tinyxml/tinyxml2.h"
#include "../graph/graph2d.h"
#include <vector>


namespace Sencha {

class GraphXmlLoader{
private :
	const char* m_fileName;
public :
	GraphXmlLoader( const char* fileName );
	Graph2DCollection* load();
private :
	void exec( Graph2DCollection* data , tinyxml2::XMLElement* element );
	void graphic( Graph2DCollection* data , tinyxml2::XMLElement* element );
};

}