#include "graph2d.h"
#include "../xml/graph_xml_loader.h"


namespace Sencha {

// ------------------------------------------------------------------------------
// class Graph2DCollectionData
// ------------------------------------------------------------------------------

// 解放
void Graph2DCollectionData::release(){
	if( this->m_lock ){
		delete this->m_lock;
		this->m_lock = NULL;
		for( size_t i = 0 ; i < this->m_graph2d.size() ; i++ ){
			this->m_graph2d[i].release();
		}
	}
}

// コンストラクタ
Graph2DCollectionData::Graph2DCollectionData(){
	this->m_lock = 0;
}

// デストラクタ
Graph2DCollectionData::~Graph2DCollectionData(){
}

// テーブルデータからコレクションデータを作成する。
void Graph2DCollectionData::loadData( tabledefineS* tabledefine , int length ){
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

// テクスチャアトラスの処理
void Graph2DCollectionData::loadMulti( tabledefineS& def ){
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
		char newName[Graph2D::NAME_SIZE];
		sprintf_s<Graph2D::NAME_SIZE>( newName , "%s_%d" , def.name , i );
		this->m_graph2d.push_back( Graph2D( def.gid + i , newName , img[i] , w , h ) );
	}
	delete[] img;
}

// 通常テクスチャの処理
void Graph2DCollectionData::loadSingle( tabledefineS& def ){
	int img = LoadGraph( def.path );
	if( img < 0 ){
		printfDx( "ロード失敗:%s\n" , def.path );
	}
	int xSize = 0;
	int ySize = 0;
	GetGraphSize( img , &xSize , &ySize );
	this->m_graph2d.push_back( Graph2D( def.gid , def.name , img , xSize , ySize ) );
}

const Graph2D* Graph2DCollectionData::findIndex( int index ) const {
	assert( index >= 0 && index < (int)this->m_graph2d.size() );
	return &this->m_graph2d[index];
}

const Graph2D* Graph2DCollectionData::findName( const char* name ) const {
	for( size_t i = 0 ; i < this->m_graph2d.size() ; i++ ){
		if( strcmp( this->m_graph2d[i].name() , name ) == 0 ){
			return &this->m_graph2d[i];
		}
	}
	return &this->m_graph2d[0];
}

const Graph2D* Graph2DCollectionData::findId( int gid ) const {
	for( size_t i = 0 ; i < this->m_graph2d.size() ; i++ ){
		if( this->m_graph2d[i].gid() == gid ){
			return &this->m_graph2d[i];
		}
	}
	return &this->m_graph2d[0];
}



// ------------------------------------------------------------------------------
// class Graph2DCollection
// ------------------------------------------------------------------------------
Graph2DCollection::Graph2DCollection(){
	memset( this->m_name , 0 , sizeof( this->m_name ) );
}
Graph2DCollection::Graph2DCollection( const char* name , Graph2DCollectionData::tabledefineS* tabledefine , int length ){
	this->setName( name );
	this->m_graphData2D.loadData( tabledefine , length );
}

void Graph2DCollection::insertTable( Graph2DCollectionData::tabledefineS& tabledefine ){
	if( tabledefine.xNum * tabledefine.yNum > 1 ){
		this->m_graphData2D.loadMulti( tabledefine );
		return;
	}
	this->m_graphData2D.loadSingle( tabledefine );
}
const Graph2D* Graph2DCollection::findIndex( int index ) const {
	return this->m_graphData2D.findIndex( index );
}
const Graph2D* Graph2DCollection::findName( const char* name ) const {
	return this->m_graphData2D.findName( name );
}
const Graph2D* Graph2DCollection::findId( int gid ) const {
	return this->m_graphData2D.findId( gid );
}




} // namespace Sencha