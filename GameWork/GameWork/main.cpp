#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include "app\manager\resource_manager.h"
#include <cassert>
#include <crtdbg.h>
#include <vector>

struct Vec2 {
	float x;
	float y;
	void set( float x , float y ){
		this->x = x;
		this->y = y;
	}
};

class GameObject : public Sencha::Task::GameTask {
protected :
	Vec2 m_pos;
public  :
	GameObject(){
		this->m_pos.set( 0 , 0 );
	}
	void pos( float x , float y ){
		this->m_pos.set( x , y );
	}
	void pos( Vec2 pos ){
		this->m_pos = pos;
	}
};

// レイアウト管理を行う。
// GameObjectクラスが子階層に存在する場合、座標を
// デフォルトのupdateではなく
class Layout : public GameObject {
};

class Picture : public GameObject {
private :
	const Sencha::Sprite* m_sprite;
	float m_ExRate;
	float m_Angle;
	unsigned char m_Turn;
	unsigned char m_Trans;
	unsigned char m_Reserve1;
	unsigned char m_Reserve2;
public  :
	virtual void onInit() override {
		this->m_ExRate = 1.0f;
		this->m_Angle = 0.0f;
		this->m_Turn = FALSE;
		this->m_Trans = TRUE;
	}
	void setSprite( const Sencha::Sprite* sprite ){
		assert( sprite );
		this->m_sprite = sprite;
	}
	virtual void onDraw() override {
		if( !this->m_sprite ){
			return;
		}
		DrawRotaGraphF( this->m_pos.x , this->m_pos.y , this->m_ExRate , this->m_Angle , this->m_sprite->handle() , this->m_Trans , this->m_Turn );
	}
};

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
		const Sencha::SpriteCollection* coll = Sencha::ResourceManager::getInstance()->getSprite()->findCollection( "collection1" );
		this->insertTaskChild<Picture>()->setSprite( coll->findName( "test1" ) );
		this->insertTaskChild<Picture>()->setSprite( coll->findId( 2 ) );
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

