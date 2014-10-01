#pragma once
#include "../../DxLib/DxLib.h"
#include "../../lib/tinyxml/tinyxml2.h"
#include <vector>
#include <cassert>

/*
 * スプライト管理を行う
 * DXライブラリの機能を使用してるがアプリは意識しなくても良いように隠蔽する。
 */
namespace Sencha {

// スプライト管理クラス
// 画像のハンドル番号を持ち、コレクションに管理される。
// 1スプライトに付き1インスタンスとして管理する。(アトラスの場合は1枚1枚分離する)
class Sprite {
private :
	int  m_handle  ; // ハンドル番号
	int  m_spriteId; // スプライトID。ハンドル番号ではなく、1始まりの値となる
	char m_name[32]; // 名前は32文字までで定義可能
	int  m_width   ; // 画像幅
	int  m_height  ; // 画像高さ
public  :
	Sprite( int id , char* name , int handle , int width , int height ){
		this->m_spriteId = id;
		strcpy_s<32>( this->m_name , name );
		this->m_handle = handle;
		this->m_width = width;
		this->m_height = height;
	}
	void release(){
		DeleteGraph( this->m_handle );
	}
	const int& handle() const {
		return this->m_handle;
	}
	const char* name() const {
		return this->m_name;
	}
	int spriteId() const {
		return this->m_spriteId;
	}
};


class SpriteCollectionData {
public :
	typedef int lockobject;
	struct tabledefineS {
		int spriteId;
		char name[32];
		const char* path;
		int xNum;
		int yNum;
	};
private :
	lockobject* m_lock;
	std::vector<Sprite> m_sprite;
private :
	// ロードは一回しか行わないよチェック
	void lock(){
		assert( !this->m_lock );
		this->m_lock = new lockobject();
	}
public :
	// 解放
	void release(){
		if( this->m_lock ){
			delete this->m_lock;
			this->m_lock = NULL;
			for( size_t i = 0 ; i < this->m_sprite.size() ; i++ ){
				this->m_sprite[i].release();
			}
		}
	}
	// コンストラクタ
	SpriteCollectionData(){
		this->m_lock = 0;
	}
	// デストラクタ
	~SpriteCollectionData(){
	}

	// 設定ファイルからコレクションデータを作成する。
	// 形式はxml形式想定
	void loadFile( const char* fileName );

	// テーブルデータからコレクションデータを作成する。
	void loadData( tabledefineS* tabledefine , int length ){
		assert( length > 0 );
		this->lock();
		for( int i = 0 ; i < length ; i++ ){
			tabledefineS& def = tabledefine[i];
			int allnum = def.xNum * def.yNum;
			bool isMulti = false;
			if( allnum >= 2 ){
				isMulti = true;
			}
			if( isMulti ){
				loadMulti( def );
				continue;
			}
			loadSingle( def );
		}
	}
	void loadMulti( tabledefineS& def ){
		int allnum = def.xNum * def.yNum;
		int src = LoadGraph( def.path );
		if( src < 0 ){
			printfDx( "ロード失敗:%s\n" , def.path );
		}
		int xSize = 0;
		int ySize = 0;
		int* img = new int[allnum];
		GetGraphSize( src , &xSize , &ySize );
		int w = xSize / def.xNum;
		int h = ySize / def.yNum;
		DeleteGraph( src );
		LoadDivGraph( def.path , allnum , def.xNum , def.yNum , w , h , img );
		for( int i = 0 ; i < allnum ; i++ ){
			char newName[32];
			sprintf_s<32>( newName , "%s_%d" , def.name , i );
			this->m_sprite.push_back( Sprite( def.spriteId + i , newName , img[i] , w , h ) );
		}
		delete[] img;
	}
	void loadSingle( tabledefineS& def ){
		int img = LoadGraph( def.path );
		if( img < 0 ){
			printfDx( "ロード失敗:%s\n" , def.path );
		}
		int xSize = 0;
		int ySize = 0;
		GetGraphSize( img , &xSize , &ySize );
		this->m_sprite.push_back( Sprite( def.spriteId , def.name , img , xSize , ySize ) );
	}
	const Sprite* findIndex( int index ) const {
		assert( index >= 0 && index < (int)this->m_sprite.size() );
		return &this->m_sprite[index];
	}
	const Sprite* findName( const char* name ) const {
		for( size_t i = 0 ; i < this->m_sprite.size() ; i++ ){
			if( strcmp( this->m_sprite[i].name() , name ) == 0 ){
				return &this->m_sprite[i];
			}
		}
		return &this->m_sprite[0];
	}
	const Sprite* findId( int spriteId ) const {
		for( size_t i = 0 ; i < this->m_sprite.size() ; i++ ){
			if( this->m_sprite[i].spriteId() == spriteId ){
				return &this->m_sprite[i];
			}
		}
		return &this->m_sprite[0];
	}
};


class SpriteCollection {
private :
	SpriteCollectionData m_spriteData;
	char m_name[32];
private :
	void setName( const char* name ){
		strcpy_s<32>( m_name , name );
	}
public  :
	void release(){
		this->m_spriteData.release();
	}
	const char* name(){
		return this->m_name;
	}
	SpriteCollection(){
		memset( this->m_name , 0 , sizeof( this->m_name ) );
	}
	SpriteCollection( const char* name , SpriteCollectionData::tabledefineS* tabledefine , int length ){
		this->init( name , tabledefine , length );
	}
	void init( const char* name , SpriteCollectionData::tabledefineS* tabledefine , int length ){
		this->setName( name );
		this->m_spriteData.loadData( tabledefine , length );
	}
	const Sprite* findIndex( int index ) const {
		return this->m_spriteData.findIndex( index );
	}
	const Sprite* findName( const char* name ) const {
		return this->m_spriteData.findName( name );
	}
	const Sprite* findId( int spriteId ) const {
		return this->m_spriteData.findId( spriteId );
	}
};

}
