#pragma once
#include "../../lib/keyboard/virtual_keyboard.h"
#include "../../lib/keyboard/interface_keyboard.h"


const unsigned int MAX_PLAYER  = 4;

const unsigned int PLAYER_ID_1 = 0;
const unsigned int PLAYER_ID_2 = 1;
const unsigned int PLAYER_ID_3 = 2;
const unsigned int PLAYER_ID_4 = 3;

const unsigned int EVENT_NONE  = 0x00;
const unsigned int EVENT_A     = 0x01;
const unsigned int EVENT_B     = 0x02;
const unsigned int EVENT_C     = 0x04;
const unsigned int EVENT_D     = 0x08;
const unsigned int EVENT_UP    = 0x10; 
const unsigned int EVENT_RIGHT = 0x20;
const unsigned int EVENT_DOWN  = 0x40;
const unsigned int EVENT_LEFT  = 0x80;

const unsigned int HOLD_TIME = 60;
const unsigned int EVENT_TABLE[] = {
	EVENT_A     ,
	EVENT_B     ,
	EVENT_C     ,
	EVENT_D     ,
	EVENT_UP    ,
	EVENT_RIGHT ,
	EVENT_DOWN  ,
	EVENT_LEFT  ,
};
const size_t EVENT_SIZE = sizeof( EVENT_TABLE ) / sizeof( *EVENT_TABLE );

/*
 * �A�v���P�[�V�������ɂ�������͏���舵���B
 * �L�[�{�[�h�A�W���C�p�b�h�Ȃǂɂ����͂͂����ł܂Ƃ߂�1�̓��̓R�}���h�Ƃ��Ē��ۉ�����B
 * �A�v���P�[�V���������ł͂��̓��͏������ɓ��͐�����s���B
 */
class AppInput {
private :
	IJoypad* m_joypad;
	IKeyboard* m_keyboard;
	VirtualKeyboard* m_vkey;
	int m_playerId;
public  :
	AppInput( int playerId );
	~AppInput();
	unsigned int getEvent();
};


/*
 * ���̓C�x���g���X�i�[
 * ���̓C�x���g���K�v�ȏꍇ�p�����Ď�������
 */
class AppInputListener {
private :
	unsigned int m_prevEventCode;
	unsigned int m_currentEventCode;
	unsigned int m_holdtime[EVENT_SIZE];
	AppInput* m_input;
private :
	void checkHold();
public  :
	AppInputListener( AppInput* input );
	unsigned int currentEventCode(){
		return m_currentEventCode;
	}
	virtual void onTrg( unsigned int cmd )  = 0;
	virtual void onHold( unsigned int cmd ) = 0;
	virtual void onRel( unsigned int cmd )  = 0;
	virtual void onUpdate() = 0;
	void update();
};
