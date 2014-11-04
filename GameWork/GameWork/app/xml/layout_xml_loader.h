#pragma once
#include "../manager/resource_manager.h"
#include "../object/game_object.h"
#include "../../lib/tinyxml/tinyxml2.h"


class LayoutXmlLoader {
private :
	tinyxml2::XMLDocument m_doc;
public :
	GameObject* loadFile( const char* fileName ){
		this->m_doc.LoadFile( fileName );
		GameObject* root = new Layout();
		return this->exec( root , this->m_doc.FirstChildElement() );
	}
	GameObject* loadAndEntryFile( const char* fileName , Sencha::Task::GameTask* parent ){
		assert( parent );
		GameObject* obj = this->loadFile( fileName );
		parent->entryTask( obj );
		return obj;
	}
private :
	GameObject* exec( GameObject* parent , tinyxml2::XMLElement* element ){
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
				Picture* pict = parent->insertTaskChild<Picture>();
				double x = 0;
				double y = 0;
				const char* collectionName = NULL;	
				const char* name = NULL;
				int gid = 0;
				const char* gname = NULL;
				if( e->Attribute( "x"          ) ){ x              = e->DoubleAttribute( "x" );          }
				if( e->Attribute( "y"          ) ){ y              = e->DoubleAttribute( "y" );          }
				if( e->Attribute( "name"       ) ){ name           = e->Attribute( "name" );             }
				if( e->Attribute( "gname" ) )     { gname          = e->Attribute( "gname" );            }
				if( e->Attribute( "gid"   ) )     { gid            = e->IntAttribute( "gid" );           }
				if( e->Attribute( "collection" ) ){ collectionName = e->Attribute( "collection" );       }

				collectionName = e->Attribute( "collection" );
				const Sencha::Graph2DCollection* collection = Sencha::ResourceManager::getInstance()->getGraph()->findCollection( collectionName );
				assert( collection );
				const Sencha::Graph2D* sprite = collection->findId( gid );
				if( !gid ){
					assert( gname );
					sprite = collection->findName( gname );
				}
				assert( sprite );
				pict->name( name );
				pict->localPos( (float)x , (float)y );
				pict->setSprite( sprite );
				this->exec( pict , e );
			}
		}
		return parent;
	}

#undef CREATE_IF_NOT_EXIST
};
