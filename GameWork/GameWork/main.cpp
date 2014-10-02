#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include "app\manager\resource_manager.h"
#include "app\xml\layout_xml_loader.h"

class MotionTask : public Sencha::Task::GameTask {
private :
	Picture* pict;
	int frametime;
public :
	void set( Picture* pict ){
		this->pict = pict;
		this->frametime = 0;
	}
	virtual void onUpdate() override {
		Vec2 pos = pict->localPos();
		pos.x -= 5;
		pict->localPos( pos );
		if( frametime == 60 ){
			DestroyTask( this );
			return;
		}
		this->frametime++;
	}
};

class GameMainTask : public Sencha::Task::GameTask {
private :
	size_t frametime;
	Layout* object4;
	Picture* object5;
	Picture* object6;
public  :
	virtual void onInit(){
		Sencha::ResourceManager::getInstance()->getSprite()->insertGraph2DCollection( "resource/img/xml/menu_ui_00.xml" );
		frametime = 0;
		LayoutXmlLoader loader;
		GameObject* root = loader.loadAndEntryFile( "resource/xml/sample_xml.xml" , this );
		object4 = root->findObjectByName<Layout>( "object4" );
		object5 = object4->findObjectByName<Picture>( "object5" );
		object6 = object4->findObjectByName<Picture>( "object6" );
	}
	virtual void onUpdate(){
		if( frametime == 30 ){
			this->insertTaskChild<MotionTask>()->set( object6 );
		}
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
	try
	{
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
	}
	catch( Exception& e )
	{
		MessageBox( NULL , e.getMessage() , "exception!!" , MB_OK );
	}
	
	GameTask::DestroyTask( mainTask );
	Sencha::ResourceManager::finalize();
	DxLib_End();
	return 0;
}

