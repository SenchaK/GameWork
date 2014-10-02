#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include "app\manager\resource_manager.h"
#include "app\xml\layout_xml_loader.h"

class GameMainTask : public Sencha::Task::GameTask {
private :
	size_t frametime;
	Layout* object1;
public  :
	virtual void onInit(){
		Sencha::ResourceManager::getInstance()->getSprite()->insertGraph2DCollection( "resource/img/xml/menu_ui_00.xml" );
		frametime = 0;
		LayoutXmlLoader loader;
		GameObject* root = loader.loadFile( "resource/xml/sample_xml.xml" );
		this->entryTask( root );
		object1 = (Layout*)root->findObjectByName( "object1" );
	}
	virtual void onUpdate(){
		frametime++;
		Vec2 pos = object1->localPos();
		object1->localPos( pos.x + 5 , pos.y );
	}
	virtual void onDraw(){
		DrawFormatString( 0 , 20 , GetColor(255,0,255) , "GameMain %d" , frametime );
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
	using namespace Sencha::Task;

	ChangeWindowMode( TRUE );
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );
	GameTask* mainTask = new GameMainTask();
	mainTask->onInit();
	while( ProcessMessage() == 0 ){
		if( CheckHitKey( KEY_INPUT_ESCAPE ) ){
			break;
		}
		ClsDrawScreen();
		mainTask->update();
		mainTask->draw();
		DrawFormatString( 0 , 0 , 0xFF00FF , "MemoryPool %d" , Sencha::Task::GetTaskMemoryCount() );
		ScreenFlip();
	}
	mainTask->onFinish();
	GameTask::DestroyTask( mainTask );
	Sencha::ResourceManager::finalize();
	DxLib_End();
	return 0;
}

