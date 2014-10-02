#pragma once
#include "../manager/resource_manager.h"
#include "../object/game_object.h"
#include "../../lib/tinyxml/tinyxml2.h"


class LayoutXmlLoader {
private :
	tinyxml2::XMLDocument m_doc;
	GameObject* m_lyt;
public :
	GameObject* loadFile( const char* fileName ){
		this->m_lyt = new Layout();
		this->m_doc.LoadFile( fileName );
		this->exec( this->m_lyt , this->m_doc.FirstChildElement() );
		return m_lyt;
	}
private :
	void exec( GameObject* parent , tinyxml2::XMLElement* element ){
		for( tinyxml2::XMLElement* e = element->FirstChildElement() ; e ; e = e->NextSiblingElement() ){
			if( strcmp( e->Name() , "layout" ) == 0 ){
				Layout* lyt = parent->insertTaskChild<Layout>();
				double x = 0;
				double y = 0;
				const char* name = NULL;
				if( e->Attribute( "x"          ) ){ x              = e->DoubleAttribute( "x" );          }
				if( e->Attribute( "y"          ) ){ y              = e->DoubleAttribute( "y" );          }
				if( e->Attribute( "name"       ) ){ name           = e->Attribute( "name" );             }
				lyt->name( name );
				lyt->localPos( (float)x , (float)y );
				this->exec( lyt , e );
			}
			else if( strcmp( e->Name() , "graphic" ) == 0 ){
				double x = 0;
				double y = 0;
				const char* collectionName = NULL;	
				const char* name = NULL;
				int gid = 0;
				const char* spriteName = NULL;
				if( e->Attribute( "x"          ) ){ x              = e->DoubleAttribute( "x" );          }
				if( e->Attribute( "y"          ) ){ y              = e->DoubleAttribute( "y" );          }
				if( e->Attribute( "name"       ) ){ name           = e->Attribute( "name" );             }
				if( e->Attribute( "spriteName" ) ){ spriteName     = e->Attribute( "spriteName" );       }
				if( e->Attribute( "gid"   ) ){ gid       = e->IntAttribute( "gid" );      }
				if( e->Attribute( "collection" ) ){ collectionName = e->Attribute( "collection" );       }

				collectionName = e->Attribute( "collection" );
				const Sencha::Graph2DCollection* collection = Sencha::ResourceManager::getInstance()->getSprite()->findCollection( collectionName );
				assert( collection );
				const Sencha::Graph2D* sprite = collection->findId( gid );
				if( !sprite ){
					assert( spriteName );
					sprite = collection->findName( spriteName );
				}
				assert( sprite );

				Picture* pict = parent->insertTaskChild<Picture>();
				pict->name( name );
				pict->localPos( (float)x , (float)y );
				pict->setSprite( sprite );
				this->exec( pict , e );
			}
		}
	}
};
