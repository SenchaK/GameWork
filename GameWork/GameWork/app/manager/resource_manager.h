#pragma once
#include "sprite_manager.h"


namespace Sencha {
class ResourceManager {
private :
	static ResourceManager* instance;
	GraphManager* m_graph2d;

	ResourceManager(){
		m_graph2d = new GraphManager();
	}
	void release(){
		m_graph2d->clearGraph2DCollection();
		delete m_graph2d;
	}
public  :
	GraphManager* getSprite(){
		return m_graph2d;
	}
public :
	static ResourceManager* getInstance(){
		if( !instance ){
			instance = new ResourceManager();
		}
		return instance;
	}
	static void finalize(){
		getInstance()->release();
		delete instance;
	}
};

}
