#pragma once
#include "interface_keyboard.h"
#include <list>

/*
 * �����̃L�[�{�[�h�C���X�^���X�𓝊����Đ��䂷�鉼�z���L�[�{�[�h
 * �ȉ��̂悤�ȋ������N�����悤�Ɏ�������B 
 * ��)
 * 1.�L�[�{�[�h1��Z�L�[����͂����ꍇ
 * �@�L�[�{�[�h1,�L�[�{�[�h2��Z�L�[����͂����ƌ��Ȃ�
 * 2.�L�[�{�[�h1��Z�L�[����́A�L�[�{�[�h2��X�L�[�����
 * �@�L�[�{�[�h1,�L�[�{�[�h2��Z,X�L�[����͂����ƌ��Ȃ�
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