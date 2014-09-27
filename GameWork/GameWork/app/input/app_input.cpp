#include <cassert>
#include "app_input.h"
#include "../keyboard/DxJoypad.h"
#include "../keyboard/DxKey.h"
#include "../../DxLib/DxLib.h"

namespace {
	struct keytableS {
		unsigned int keycode;
		unsigned int eventcode;
	};

	struct padtableS {
		unsigned int padcode;
		unsigned int eventcode;
	};


	/*
	 * �L�[�{�[�h�Ή��\
	 */
	static const keytableS KEY_TABLE[] = {
		{ KEY_INPUT_Z     , EVENT_A     } , 
		{ KEY_INPUT_X     , EVENT_B     } , 
		{ KEY_INPUT_C     , EVENT_C     } , 
		{ KEY_INPUT_A     , EVENT_D     } , 
		{ KEY_INPUT_UP    , EVENT_UP    } , 
		{ KEY_INPUT_RIGHT , EVENT_RIGHT } , 
		{ KEY_INPUT_DOWN  , EVENT_DOWN  } , 
		{ KEY_INPUT_LEFT  , EVENT_LEFT  } , 
	};

	/*
	 * �W���C�p�b�h�Ή��\
	 */
	static const padtableS PAD_TABLE[] = {
		{ PAD_INPUT_A     , EVENT_A     } ,
		{ PAD_INPUT_B     , EVENT_B     } , 
		{ PAD_INPUT_C     , EVENT_C     } ,
		{ PAD_INPUT_X     , EVENT_D     } , 
		{ PAD_INPUT_UP    , EVENT_UP    } , 
		{ PAD_INPUT_RIGHT , EVENT_RIGHT } , 
		{ PAD_INPUT_DOWN  , EVENT_DOWN  } , 
		{ PAD_INPUT_LEFT  , EVENT_LEFT  } , 
	};
} // namespace

/*
 * �L�[�{�[�h�̏����ݒ�
 * DX���C�u�����񋟂̓��͑��u���g�p���Ă���
 */
AppInput::AppInput( int playerId ){
	this->m_playerId = playerId;
	this->m_joypad = new DxJoypad( this->m_playerId );
	this->m_keyboard = new DxKeyboard();
	this->m_vkey = new VirtualKeyboard();
	this->m_vkey->insertKeyboard( this->m_keyboard );
}

AppInput::~AppInput(){
	delete this->m_vkey;
	delete this->m_keyboard;
	delete this->m_joypad;
}

/*
 * �Ή�����L�[�R�[�h�̓��͂�����ΑΉ�����C�x���g�R�[�h��Ԃ��B
 * �L�[�{�[�h����ł��낤�ƃW���C�p�b�h����ł��낤�ƃA�v���P�[�V�������͈ӎ�����K�v�Ȃ��A
 * �����ꂼ��̓��͑��u����̓������͂��������ꍇ���A�v������1��̓��͂Ƃ��Ă������Ȃ��Ȃ��悤�ɍ��B
 *
 * ��)
 * 1.�L�[�{�[�h��Z�L�[�������ꂽ
 * �@��EVENT_A�𔭍s����
 * 2.�L�[�{�[�h��Z�L�[��������A�W���C�p�b�h��1�{�^�������͂��ꂽ
 * �@��EVENT_A�𔭍s����
 * 3.�L�[�{�[�h��Z�L�[��������A�W���C�p�b�h��2�{�^�������͂��ꂽ
 * �@��EVENT_A | EVENT_B�𔭍s����B
 */
unsigned int AppInput::getEvent(){
	unsigned int result = EVENT_NONE;
	for( int i = 0 ; i < sizeof( KEY_TABLE ) / sizeof( *KEY_TABLE ) ; i++ ){
		if( this->m_vkey->getKeyState( KEY_TABLE[i].keycode ) ){
			result |= KEY_TABLE[i].eventcode;
		}
	}
	for( int i = 0 ; i < sizeof( PAD_TABLE ) / sizeof( *PAD_TABLE ) ; i++ ){
		if( this->m_joypad->getPadState( PAD_TABLE[i].padcode ) ){
			result |= PAD_TABLE[i].eventcode;
		}
	}
	return result;
}


AppInputListener::AppInputListener( AppInput* input ){
	this->m_prevEventCode = 0;
	this->m_currentEventCode = 0;
	memset( this->m_holdtime , 0 , sizeof( this->m_holdtime ) );
	this->m_input = input;
}

void AppInputListener::checkHold(){
	for( int i = 0 ; i < EVENT_SIZE ; i++ ){
		int item = EVENT_TABLE[i];
		if( !( this->m_currentEventCode & item ) ){
			this->m_holdtime[i] = 0;
			continue;
		}
		if( this->m_holdtime[i] >= HOLD_TIME ){
			this->onHold( item );
		}
		this->m_holdtime[i]++;
	}
}

void AppInputListener::update(){
	this->m_prevEventCode = this->m_input->getEvent();
	if( this->m_prevEventCode != this->m_currentEventCode ){
		unsigned int presscode = 0;
		unsigned int relcode = 0;
		for( int i = 0 ; i < EVENT_SIZE ; i++ ){
			int item = EVENT_TABLE[i];
			if( !( this->m_currentEventCode & item ) && ( this->m_prevEventCode & item ) ){
				this->onTrg( item );
			}
			if( ( this->m_currentEventCode & item ) && !( this->m_prevEventCode & item ) ){
				this->onRel( item );
			}
		}

		this->m_currentEventCode = this->m_prevEventCode;
	}

	this->checkHold();
	this->onUpdate();
}