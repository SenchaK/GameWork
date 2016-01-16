#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include "app\manager\resource_manager.h"
#include "app\xml\layout_xml_loader.h"
#include "lib\memory\memory_pool.h"
#include "app\manager\game_frame.h"

class GameMainTask : public Sencha::GameFrameTask {
private :
	size_t m_frametime;
public  :
	virtual void onInit(){
		Sencha::ResourceManager::getInstance()->getGraph()->insertGraph2DCollection( "resource/img/xml/menu_ui_00.xml" );
		LayoutXmlLoader loader;
		GameObject* lyt = loader.loadAndEntryFile( "resource/xml/sample_xml.xml" , this );
		this->m_frametime = 0;
	}
	virtual void onUpdate(){
		if( CheckHitKey( KEY_INPUT_ESCAPE ) ){
			this->Exit();
		}
		this->m_frametime++;
	}
	virtual void onDraw(){
		DrawFormatString( 0 , 0 , 0xFF00FF , "MemoryPool %d" , Sencha::Task::GetTaskMemoryCount() );
		DrawFormatString( 0 , 20 , GetColor(255,0,255) , "GameMain %d" , this->m_frametime );
	}
	virtual void onFinish(){
	}
	static void* operator new( size_t size ){
		return Sencha::Task::DefaultNew( size );
	}
	static void operator delete( void* p ){
		Sencha::Task::DefaultDelete( p );
	}
};


int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nShowCmd ){
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	Sencha::DxGameFrame( new GameMainTask() );
	return 0;
}

