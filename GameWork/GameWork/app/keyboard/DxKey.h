#pragma once
#include "../../lib/keyboard/interface_keyboard.h"


/*
 * DXライブラリを使用したキー入力監視
 */
class DxKeyboard : public IKeyboard {
private :
public  :
	static void Update();
	virtual bool getKeyState( int KeyCode ) override;
};
