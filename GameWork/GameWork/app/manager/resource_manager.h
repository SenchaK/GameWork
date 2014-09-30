#pragma once
#include "sprite_manager.h"


namespace Sencha {
class ResourceManager {
private :
	static ResourceManager* instance;
	SpriteManager* m_sprite;

	ResourceManager(){
		m_sprite = new SpriteManager();
	}
public  :
	SpriteManager* getSprite(){
		return m_sprite;
	}
	static ResourceManager* getInstance(){
		if( !instance ){
			instance = new ResourceManager();
		}
		return instance;
	}
};

}
