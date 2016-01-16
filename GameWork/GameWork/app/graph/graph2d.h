#pragma once
#include "../../DxLib/DxLib.h"
#include "../../lib/tinyxml/tinyxml2.h"
#include <vector>
#include <cassert>

/*
 * グラフィック管理を行う
 * DXライブラリの機能を使用してるがアプリは意識しなくても良いように隠蔽する。
 */
namespace Sencha {

// 2Dグラフィック管理クラス
// 画像のハンドル番号を持ち、コレクションに管理される。
// 1スプライトに付き1インスタンスとして管理する。(アトラスの場合は1枚1枚分離する)
class Graph2D {
public :
	enum {
		NAME_SIZE = 32 ,
	};
private :
	int  m_handle         ; // ハンドル番号
	int  m_gid            ; // グラフィックID。ハンドル番号ではなく、1始まりの値となる
	char m_name[NAME_SIZE]; // 名前は32文字までで定義可能
	int  m_width          ; // 画像幅
	int  m_height         ; // 画像高さ
public  :
	Graph2D( int id , char* name , int handle , int width , int height ){
		this->m_gid = id;
		strcpy_s<NAME_SIZE>( this->m_name , name );
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
	int gid() const {
		return this->m_gid;
	}
};


class Graph2DCollectionData {
	friend class Graph2DCollection;
public :
	typedef int lockobject;
	struct tabledefineS {
		int gid;
		char name[Graph2D::NAME_SIZE];
		const char* path;
		int xNum;
		int yNum;

		tabledefineS(){	
			this->gid = 0;
			memset( this->name , 0 , sizeof( this->name ) );
			this->path = NULL;
			this->xNum = 1;
			this->yNum = 1;
		}
	};
private :
	lockobject* m_lock;
	std::vector<Graph2D> m_graph2d;
private :
	// ロードは一回しか行わないよチェック
	void lock(){
		assert( !this->m_lock );
		this->m_lock = new lockobject();
	}
	// 複数画像読み込み
	void loadMulti( tabledefineS& def );
	// 1枚画像読み込み
	void loadSingle( tabledefineS& def );
	// 解放
	void release();
	// コンストラクタ
	Graph2DCollectionData();
	// デストラクタ
	~Graph2DCollectionData();

	// テーブルデータからコレクションデータを作成する。
	void loadData( tabledefineS* tabledefine , int length );
	const Graph2D* findIndex( int index ) const;
	const Graph2D* findName( const char* name ) const;
	const Graph2D* findId( int gid ) const;
};


class Graph2DCollection {
	friend class GraphXmlLoader;
public :
	enum {
		NAME_SIZE = 32 , 
	};
private :
	Graph2DCollectionData m_graphData2D;
	char m_name[NAME_SIZE];
private :
	void setName( const char* name ){
		strcpy_s<NAME_SIZE>( m_name , name );
	}
	void insertTable( Graph2DCollectionData::tabledefineS& tabledefine );
public  :
	void release(){
		this->m_graphData2D.release();
	}
	const char* name(){
		return this->m_name;
	}
	Graph2DCollection();
	Graph2DCollection( const char* name , Graph2DCollectionData::tabledefineS* tabledefine , int length );
	const Graph2D* findIndex( int index )       const;
	const Graph2D* findName( const char* name ) const;
	const Graph2D* findId( int gid )            const;
};

}
