#pragma once
#include <stdio.h>
#include "../../lib/object/list.h"

namespace Sencha {
namespace Task {


// タスク基底クラス
// ゲーム内のオブジェクトは全てこのタスクを通して処理をする。
class GameTask : public Sencha::Container {
private :
	List<Container>* m_parent;
	List<Container> m_child;
	GameTask* m_parent_task;
public :
	// 子タスクの数を取得
	int childTaskCount(){
		return this->m_child.count();
	}
	// 初期化
	GameTask(){
		m_parent_task = NULL;
		m_parent = NULL;
	}
	// 子階層の全てのタスクを削除する
	virtual ~GameTask(){
		FinishTask( this );
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
	virtual void update(){
		UpdateTask( this );
	}
	virtual void draw(){
		DrawTask( this );
	}
	virtual void finish(){
		DestroyTask( this );
	}

	// タスクの登録
	// 現在のタスクの子階層にタスクを生成する
	// 生成されたタスクは親タスクの情報と親リストの情報の二つを持つように作る。
	// 生成に成功した場合、onInit関数を呼び出す
	// タスクは全て基本的にこの関数からしか生成してはいけない
	template<typename T>
	T* insertTask(){
		T* task = new T();
		this->m_child.add( task );
		task->m_parent = &this->m_child;
		task->m_parent_task = this;
		task->onInit();
		return task;
	}

	// タスクの登録
	// 現在のタスクの親階層にタスクを生成する。
	template<typename T>
	T* insertTaskFromParent(){
		assert( this->m_parent_task );
		return this->m_parent_task->insertTask<T>();
	}
public :
	/* ***********************************************************************
	 * Static Method
	 * *********************************************************************** */

	// デストラクタ時に呼び出す。
	// 所持する子階層のタスクを全て削除する
	static void FinishTask( GameTask* task ){
		if( !task ){
			return;
		}
		List<Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			GameTask* task = (GameTask*)iter;
			GameTask* next = (GameTask*)iter->next;
			GameTask::DestroyTask( task );
			iter = next;
		}
	}

	// タスクへの更新処理を走らせる
	static void UpdateTask( GameTask* task ){
		if( !task ){
			return;
		}
		task->onUpdate();
		List<Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			GameTask* task = (GameTask*)iter;
			GameTask* next = (GameTask*)task->next;
			UpdateTask( task );
			iter = next;
		}
	}

	// タスクへの描画処理を走らせる
	static void DrawTask( GameTask* task ){
		if( !task ){
			return;
		}
		task->onDraw();
		List<Container>::iterator iter = task->m_child.top();
		while( iter != NULL ){
			GameTask* task = (GameTask*)iter;
			GameTask* next = (GameTask*)task->next;
			DrawTask( task );
			iter = next;
		}
	}

	// タスクの破棄
	// 基本的にこの関数からしかタスクの削除を行ってはいけない
	static void DestroyTask( GameTask* task ){
		assert( task );
		task->onFinish();
		task->m_parent->remove( task );
		delete task;
	}

	// タスクの挿入
	// 指定したリストの子階層にタスクを挿入する。
	template<typename T>
	static T* InsertTask( Sencha::List<Sencha::Container>* tasklist ){
		assert( tasklist );
		T* task = new T();
		task->m_parent = tasklist;
		tasklist->add( task );
		return task;
	}
private :
	/* ************************************************** *
	 * new/delete演算子はアクセスできなくする。
	 * インスタンス生成したい場合はinsertTaskメソッドを使う。
	 * 破棄したい場合はDestroyTaskメソッドを使う。
	 * 別のメモリプールが必要なタスクのみオーバーロードしても良いが、
	 * 必ずprivateにすること
	 * ************************************************** */
	static void* operator new   ( size_t size );
	static void  operator delete( void* p );
};


// グローバルタスク
// 静的なタスクリストを親とした最上位タスク
class GlobalTask : public GameTask {
public :
	virtual void update() override;
	virtual void draw() override;
	virtual void finish() override;
};

// グローバルタスクを生成する。
GlobalTask* CreateGlobalTask();

// 現在のタスク用メモリブロック個数
int GetTaskMemoryCount();

} // namespace Task
} // namespace Sencha