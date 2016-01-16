#pragma once
#include "../../DxLib/DxLib.h"
#include "../../lib/tinyxml/tinyxml2.h"
#include <vector>
#include <cassert>

/*
 * �O���t�B�b�N�Ǘ����s��
 * DX���C�u�����̋@�\���g�p���Ă邪�A�v���͈ӎ����Ȃ��Ă��ǂ��悤�ɉB������B
 */
namespace Sencha {

// 2D�O���t�B�b�N�Ǘ��N���X
// �摜�̃n���h���ԍ��������A�R���N�V�����ɊǗ������B
// 1�X�v���C�g�ɕt��1�C���X�^���X�Ƃ��ĊǗ�����B(�A�g���X�̏ꍇ��1��1����������)
class Graph2D {
public :
	enum {
		NAME_SIZE = 32 ,
	};
private :
	int  m_handle         ; // �n���h���ԍ�
	int  m_gid            ; // �O���t�B�b�NID�B�n���h���ԍ��ł͂Ȃ��A1�n�܂�̒l�ƂȂ�
	char m_name[NAME_SIZE]; // ���O��32�����܂łŒ�`�\
	int  m_width          ; // �摜��
	int  m_height         ; // �摜����
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
	// ���[�h�͈�񂵂��s��Ȃ���`�F�b�N
	void lock(){
		assert( !this->m_lock );
		this->m_lock = new lockobject();
	}
	// �����摜�ǂݍ���
	void loadMulti( tabledefineS& def );
	// 1���摜�ǂݍ���
	void loadSingle( tabledefineS& def );
	// ���
	void release();
	// �R���X�g���N�^
	Graph2DCollectionData();
	// �f�X�g���N�^
	~Graph2DCollectionData();

	// �e�[�u���f�[�^����R���N�V�����f�[�^���쐬����B
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
