#include "DxLib\DxLib.h"
#include "app\task\task.h"
#include "app\manager\resource_manager.h"
#include "app\xml\layout_xml_loader.h"
#include "lib\memory\memory_pool.h"

namespace Collision {
	Sencha::MemoryPool<64,2048> collision_memory;
}

namespace Affine {
	//À•W‰ñ“]
	//(x0,y0)‚©‚ç(mx,my)‚ğŠî€‚ÉangŠp‰ñ“]‚µ‚½Šp“x‚ğ(x,y)‚É‚¢‚ê‚é
	void conv_pos0(double *x, double *y, double x0, double y0, double mx, double my,double ang){
		double ox=x0-mx,oy=y0-my;
		*x=ox*cos(ang) +oy*sin(ang);
		*y=-ox*sin(ang)+oy*cos(ang);
		*x+=mx;
		*y+=my;
	}
};

class Collider {
public :
	class EventHandler abstract {
	public :
		virtual void onHit( Collider* collider ) = 0;
		virtual void onHitTrg( Collider* collider ) = 0;
	};
public :
	static void* operator new( size_t size ){
		assert( size < 128 );
		return Collision::collision_memory.Malloc();
	}
	static void operator delete( void* p ){
		Collision::collision_memory.Free( p );
	}
};

class Sphere2D : public Collider {
private :
	Vec2 m_pos;
	float m_r;
public  :
	// O•½•û‚Ì’è—‚É‚æ‚é‰~Œ`‚ÌÚG”»’è
	// “–‚½‚Á‚Ä‚¢‚½‚çtrue
	static bool Judge( Sphere2D* sphere1 , Sphere2D* sphere2 ){
		float dx,dy,r;
		dx = sphere1->m_pos.x - sphere2->m_pos.x; //…•½•ûŒü‚Ì‹——£
		dy = sphere1->m_pos.y - sphere2->m_pos.y; //‰”’¼•ûŒü‚Ì‹——£
		r  = sphere1->m_r     + sphere2->m_r    ; //”¼Œa‚Ì˜a
		return ( ( dx * dx ) + ( dy * dy ) < ( r*r ) );
	}
};

class Box2D : public Collider {
private :
	Vec2 m_point[4];
public  :
};

class GameMainTask : public Sencha::Task::GameTask {
private :
	size_t frametime;
public  :
	virtual void onInit(){
		Sencha::ResourceManager::getInstance()->getSprite()->insertGraph2DCollection( "resource/img/xml/menu_ui_00.xml" );
		frametime = 0;
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

