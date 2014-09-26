#pragma once

class IKeyboard {
public :
	virtual bool getKeyState( int KeyCode ) = 0;
};

class IJoypad {
public :
	/*
	 * 指定したpadCodeが入力されているかの状態を取得する。
	 * 入力されていればtrue、入力されていないならばfalseを返す
	 */
	virtual bool getPadState( int padCode ) = 0;

	/*
	 * アナログ入力の情報を得る。
	 * 数値は-1000～1000までの値に正規化したものを返す。
	 * (仮に-1.0～1.0のような仕様の場合も整数に直して渡すように実装すること)
	 */
	virtual void getAnalog( int* x , int* y ) = 0;
};
