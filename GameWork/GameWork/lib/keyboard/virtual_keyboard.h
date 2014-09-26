#pragma once
#include "interface_keyboard.h"
#include <list>

/*
 * 複数のキーボードインスタンスを統括して制御する仮想化キーボード
 * 以下のような挙動を起こすように実装する。 
 * 例)
 * 1.キーボード1がZキーを入力した場合
 * 　キーボード1,キーボード2がZキーを入力したと見なす
 * 2.キーボード1がZキーを入力、キーボード2がXキーを入力
 * 　キーボード1,キーボード2がZ,Xキーを入力したと見なす
 */
class VirtualKeyboard {
private :
	std::list<IKeyboard*> m_keylist;
public  :
	void insertKeyboard( IKeyboard* keyboard ){
		this->m_keylist.push_back( keyboard );
	}
	bool getKeyState( int keyCode ){
		for( std::list<IKeyboard*>::iterator iter = this->m_keylist.begin() ; iter != this->m_keylist.end() ; iter++ ){
			if( (*iter)->getKeyState( keyCode ) ){
				return true;
			}
		}
		return false;
	}
};