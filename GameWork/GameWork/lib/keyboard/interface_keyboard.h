#pragma once

class IKeyboard {
public :
	virtual bool getKeyState( int KeyCode ) = 0;
};

class IJoypad {
public :
	/*
	 * �w�肵��padCode�����͂���Ă��邩�̏�Ԃ��擾����B
	 * ���͂���Ă����true�A���͂���Ă��Ȃ��Ȃ��false��Ԃ�
	 */
	virtual bool getPadState( int padCode ) = 0;

	/*
	 * �A�i���O���͂̏��𓾂�B
	 * ���l��-1000�`1000�܂ł̒l�ɐ��K���������̂�Ԃ��B
	 * (����-1.0�`1.0�̂悤�Ȏd�l�̏ꍇ�������ɒ����ēn���悤�Ɏ������邱��)
	 */
	virtual void getAnalog( int* x , int* y ) = 0;
};
