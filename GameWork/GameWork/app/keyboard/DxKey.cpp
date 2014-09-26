#include "DxKey.h"
#include "../../DxLib/DxLib.h"
#include <cassert>

namespace {
static const int MAXKEY = 256;
static char keybuf[MAXKEY];
} // namespace

/* static */
void DxKeyboard::Update(){
	GetHitKeyStateAll( keybuf );
}


/* override */
bool DxKeyboard::getKeyState( int KeyCode ){
	assert( KeyCode >= 0 && KeyCode < MAXKEY );
	if( keybuf[KeyCode] ) return true;
	return false;
}