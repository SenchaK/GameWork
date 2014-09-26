#include "DxJoypad.h"
#include "../../DxLib/DxLib.h"
#include <cassert>

namespace {
static const int PADMAX = 4;
static int padinputstate[PADMAX];
static const int padinput[PADMAX] = {
	DX_INPUT_PAD1 , 
	DX_INPUT_PAD2 , 
	DX_INPUT_PAD3 ,
	DX_INPUT_PAD4 ,
};


} // namespace



/* static */
void DxJoypad::Update(){
	for( int i = 0 ; i < PADMAX ; i++ ){
		padinputstate[i] = GetJoypadInputState( padinput[i] );
	}
}

DxJoypad::DxJoypad( int playerId ){
	assert( playerId >= 0 && playerId < PADMAX );
	this->m_playerId = playerId;
}

/* virtual */
bool DxJoypad::getPadState( int padCode ){
	if( padinputstate[this->m_playerId] & padCode ) return true;
	return false;
}

/* virtual */
void DxJoypad::getAnalog( int* x , int* y ){
	GetJoypadAnalogInput( x , y , padinput[this->m_playerId] );
}