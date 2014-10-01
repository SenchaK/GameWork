#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include "app\manager\resource_manager.h"
#include "app\xml\layout_xml_loader.h"

class GameMainTask : public Sencha::Task::GameTask {
private :
	size_t frametime;
public  :
	virtual void onInit(){
		Sencha::SpriteCollectionData::tabledefineS dataDefine[] = {
			{ 1 , "test1" , "resource/img/test1.bmp" , 1 , 1 } , 
			{ 5 , "right" , "scr_right.png"          , 1 , 1 } , 
		};
		frametime = 0;
		Sencha::ResourceManager::getInstance()->getSprite()->insertCollection( "collection1" , dataDefine , sizeof( dataDefine ) / sizeof( *dataDefine ) );
		LayoutXmlLoader loader;
		GameObject* root = loader.loadFile( "resource/xml/sample_xml.xml" );
		this->entryTask( root );
	}
	virtual void onUpdate(){
		frametime++;
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

