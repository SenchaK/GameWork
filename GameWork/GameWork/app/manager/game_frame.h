#pragma once
#include "../task/task.h"

namespace Sencha {
class GameFrame {
private :
	bool m_isActive;
public :
	GameFrame(){
		this->m_isActive = true;
	}
	bool isActive(){
		return this->m_isActive;
	}
	void Exit(){
		this->m_isActive = false;
	}
};

class GameFrameTask : public Sencha::Task::GameTask {
private :
	GameFrame* m_gameFrame;
public :
	virtual void onFrameInit( GameFrame* gmFrame ){
		this->m_gameFrame = gmFrame;
	}
	void Exit(){
		this->m_gameFrame->Exit();
	}
	bool isActive(){
		return this->m_gameFrame->isActive();
	}
};


class DxGameFrame : GameFrame {
public :
	DxGameFrame( GameFrameTask* task );
};

}
