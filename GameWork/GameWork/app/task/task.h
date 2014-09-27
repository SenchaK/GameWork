#pragma once
#include <stdio.h>
#include "../../lib/object/list.h"

namespace Sencha {
namespace Task {
static const int MEMORY_BLOCK_SIZE = 256;
static const int MEMORY_POOL_SIZE  = 512;

class GameTask;
class TaskManager {
	typedef GameTask* task_p;
private :
	task_p collector[MEMORY_POOL_SIZE];
	task_p* p;
	TaskManager();
public :
	// インスタンス取得
	static TaskManager& Instance();
	// 使用しなくなったタスクをチェックに入れる
	void checkUnuse( GameTask* ptask );
	// 解放を行う
	void collect();
};

// タスク基底クラス
// ゲーム内のオブジェクトは全てこのタスクを通して処理をする。
class GameTask : public Sencha::Container {
	friend class TaskManager;
private :
	List<Container>* m_parent;
	List<Container> m_child;
	GameTask* m_parent_task;
	int m_delete_check;
public :
	// 子タスクの数を取得
	int childTaskCount(){
		return this->m_child.count();
	}
	// 初期化
	GameTask(){
		m_parent_task = NULL;
		m_parent = NULL;
		m_delete_check = 0;
	}

	// デストラクタ
	virtual ~GameTask(){
	}

	// ---------------------------------------------------------
	// 継承先実装
	// ---------------------------------------------------------
	virtual void onInit(){
	}
	virtual void onUpdate(){
	}
	virtual void onDraw(){
	}
	virtual void onFinish(){
	}

	// ---------------------------------------------------------
	// 提供機能
	// ---------------------------------------------------------
	// 更新
	virtual void update();
	// 描画
	virtual void draw();
	// 子階層の全てのタスクを削除する
	virtual void finish();
	// タスクの登録
	// 現在のタスクの子階層にタスクを生成する
	// 生成されたタスクは親タスクの情報と親リストの情報の二つを持つように作る。
	// 生成に成功した場合、onInit関数を呼び出す
	// タスクは全て基本的にこの関数からしか生成してはいけない
	template<typename T>
	T* insertTaskChild(){
		T* task = new T();
		this->m_child.add( task );
		task->m_parent = &this->m_child;
		task->m_parent_task = this;
		task->onInit();
		return task;
	}

	// タスクの登録
	// 呼び出し元の階層と同じ階層にタスクを生成する。
	template<typename T>
	T* insertTask(){
		assert( this->m_parent_task );
		return this->m_parent_task->insertTaskChild<T>();
	}

	// タスクの登録
	// 呼び出し元の親階層にタスクを生成する。
	template<typename T>
	T* insertTaskParent(){
		assert( this->m_parent_task );
		return this->m_parent_task->insertTask<T>();
	}
public :
	/* ***********************************************************************
	 * Static Method
	 * *********************************************************************** */
	// デストラクタ時に呼び出す。
	// 所持する子階層のタスクを全て削除する
	static void FinishTask( GameTask* task );
	// タスクへの更新処理を走らせる
	static void UpdateTask( GameTask* task );
	// タスクへの描画処理を走らせる
	static void DrawTask( GameTask* task );
	// ************************************************** *
	// タスクの破棄
	// 連結から解除し回収する。
	// この段階ではタスクに対してdeleteは呼ばれず
	// update,drawの最後に呼び出す
	// ************************************************** *
	static void DestroyTask( GameTask* task );
private :
	/* ************************************************** *
	 * new/delete演算子はアクセスできなくする。
	 * インスタンス生成したい場合はinsertTaskメソッドを使う。
	 * 破棄したい場合はDestroyTaskメソッドを使う。
	 * 別のメモリプールが必要な場合やログを仕込みたい場合はオーバーロードする
	 * (friend指定しないでprivateにすると継承先で怒られるので注意)
	 * ************************************************** */
	static void* operator new   ( size_t size );
	static void  operator delete( void* p );
};


// グローバルタスク
// 静的なタスクリストを親とした最上位タスク
class GlobalTask : public GameTask {
public :
	static void* operator new   ( size_t size );
	static void  operator delete( void* p );
};

// グローバルタスクを生成する。
GlobalTask* CreateGlobalTask();

// 現在のタスク用メモリブロック個数
int GetTaskMemoryCount();

// タスクシステムで使用するデフォルトのメモリ確保処理
void* DefaultNew( size_t size );

// タスクシステムで使用されるデフォルトの解放処理
void DefaultDelete( void* p );

} // namespace Task
} // namespace Sencha