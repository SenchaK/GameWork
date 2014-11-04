#include "game_frame.h"
#include "../../DxLib/DxLib.h"
#include "../exception/exception.h"
#include "../manager/resource_manager.h"

namespace Sencha {
DxGameFrame::DxGameFrame( GameFrameTask* task ) : GameFrame(){
	assert( task );

	ChangeWindowMode( TRUE );
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );
	try
	{
		task->onFrameInit( this );
		task->onInit();
		while( ProcessMessage() == 0 ){
			ClsDrawScreen();
			task->update();
			task->draw();
			if( !this->isActive() ){
				break;
			}
			ScreenFlip();
		}
		task->onFinish();
	}
	catch( Exception& e )
	{
		MessageBox( NULL , e.getMessage() , "exception!!" , MB_OK );
	}

	Sencha::Task::GameTask::DestroyTask( task );
	Sencha::ResourceManager::finalize();
	DxLib_End();
}

} // namespace Sencha
