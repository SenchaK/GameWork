#pragma once
#include "../../lib/keyboard/interface_keyboard.h"


/*
 * DX���C�u�������g�p�����L�[���͊Ď�
 */
class DxKeyboard : public IKeyboard {
private :
public  :
	static void Update();
	virtual bool getKeyState( int KeyCode ) override;
};
