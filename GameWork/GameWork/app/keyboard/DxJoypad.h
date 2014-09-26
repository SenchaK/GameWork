#pragma once
#include "../../lib/keyboard/interface_keyboard.h"

/*
 * DXライブラリを使用したJoypad監視
 */
class DxJoypad : public IJoypad {
private :
	int m_playerId;
public :
	DxJoypad( int playerId );
	virtual bool getPadState( int padCode ) override;
	virtual void getAnalog( int* x , int* y ) override;
public :
	static void Update();
};
