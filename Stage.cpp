#include "Base.h"
#include "Stage.h"
#include "Math2D.h"
#include "Player.h"
#include "globals.h"
#include <DxLib.h>
#include"Bullet.h"
#include "Input.h"
#include<vector>
#include "Enemy.h"
#include "ExplosionEffect.h"

namespace
{
	const Vector2D START_POS = { WIN_WIDTH / 2,WIN_HEIGHT / 2 };
	const Vector2D START_VEL = { 0.0f,0.0f };
	const Vector2D START_DIR = { 0.0f,-1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	const float PLAYER_COLLISION_RADIUS = 15.0f;

	const unsigned int ENEMY_MAX = 100;
	const unsigned int ENEMY_NUM = 3;

	int gameOveerTimer = 0;
	int enemyTimer = 0;
	//Player* player = nullptr;
	//std::vector<Bullet*>bullets;//弾丸の保管
	//std::vector<Enemy*>enemies;//敵の保管
	//std::vector<ExplosionEffect*>effects;//爆発エフェクトの保管

	std::vector<Base*>objects;//すべてのオブジェクトの保管庫
	//オブジェクトの保管庫にオブジェクトを追加する関数
	void AddObject(Base* obj)
	{
		objects.push_back(obj);
	}
	void UpdateAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Update();
		}
	}
	void DrawAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Draw();
		}
	}
	int stageState = 0;//0:タイトル 1:ゲームプレイ中 2:ゲームオーバー
}

Stage::Stage()
{
}

Stage::~Stage()
{
}


void Stage::Initialize()//initializeは初期状態のこと！
{
	objects.clear();
	stageState = 0;//タイトル画面にする!!!!!!!!
	gameScore_ = 0;

	Player* player = new Player(START_POS, START_VEL, START_COLOR, START_DIR, START_RADIUS, START_OMEGA);
	player->SetCollisionRadius(PLAYER_COLLISION_RADIUS);
	AddObject(player);

	//最初の敵を生成

	for (int i = 0; i < ENEMY_NUM;i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE, 8);
		AddObject(e);
	}
}

void Stage::TitleUpdate()
{
}

//プレイ画面でやりたい内容
void Stage::PlayUpdate()
{
	Player_vs_Enemy();//プレイヤー対敵の当たり判定
	Enemy_vs_Bullet();//敵対玉の当たり判定

	DeleteBullet();//賞味期限切れの弾を消す
	DeleteEnemy();//死んでる敵を消す
	DeleteEffect();//死んでるエフェクトを消す

	UpdateAllObjects();

	enemyTimer++;

	if (enemyTimer > 180)//3秒に1体敵が増えていく感じ？
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE, 8);
		AddObject(e);

		enemyTimer = 0;
	}


	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		ShootBullet();
	}

}

void Stage::GameOverUpdate()
{
}

//タイトル画面の文字とかの描画
void Stage::TitleDraw()
{
	int fsize = GetFontSize();
	SetFontSize(80);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 2 - 176, WIN_HEIGHT / 2 - 84, "ASTEROIDS", GetColor(255, 0, 0));
	DrawString(WIN_WIDTH / 2 - 180, WIN_HEIGHT / 2 - 80, "ASTEROIDS", GetColor(255, 255, 255));
	//DrawString(200,-50, "SPACEキーを押してスタート!!",GetColor(255, 255,255));
	SetFontSize(fsize);
}

void Stage::PlayDraw()
{
	DrawAllObjects();
	int fsize = GetFontSize();
	SetFontSize(fsize * 2);
	DrawFormatString(10, 10, GetColor(255, 255, 255), "SCORE:%020lld", gameScore_);
	SetFontSize(fsize);
}

void Stage::GameOverDraw()
{
	static int gTimer = 0;
	gTimer++;
	static bool colorFlag = false;
	if (gTimer >= 5)
	{
		colorFlag = !colorFlag;
		gTimer = 0;
	}
	unsigned int color = colorFlag ? GetColor(255, 0, 0) : GetColor(255, 255, 255);
	int fsize = GetFontSize();
	SetFontSize(80);
	SetFontThickness(10);
	DrawString(WIN_WIDTH / 2 - 176, WIN_HEIGHT / 2 - 84, "GAME OVER", GetColor(255, 0, 0));
	DrawString(WIN_WIDTH / 2 - 180, WIN_HEIGHT / 2 - 80, "GAME OVER", color);
	SetFontSize(fsize);

}


void Stage::Update()
{
	//	//敵の位置と、当たり判定の半径
	//	//弾の位置
	//	//isAlive_ -> falseにする手段
	//	std::vector<Enemy*>aliveEnemies;
	//	std::vector<Bullet*>aliveBullets;
	//
	//	aliveEnemies.clear();//念のため、毎フレームaliveEnemiesを空にする
	//	aliveBullets.clear();
	//
	//	//for (int i = 0; i < objects.size();i++)
	//	for (auto& obj : objects)
	//	{
	//		if (obj->GetType() ==  OBJ_TYPE::ENEMY)
	//		{
	//			Enemy* e = (Enemy *)obj;
	//			if (e->IsAlive()) {
	//				aliveEnemies.push_back(e);
	//			}
	//		}
	//		else if (obj->GetType() == OBJ_TYPE::BULLET)
	//		{
	//
	//			Bullet* b = (Bullet*)obj;
	//			if (!b->IsDead()) {
	//				aliveBullets.push_back(b);
	//			}
	//		}
	//	}
	//	for (auto& bullet : aliveBullets)
	//	{
	//		for (auto& enemy : aliveEnemies)
	//		{
	//			float dist = Math2D::Length(Math2D::Sub(bullet->GetPos(), enemy->GetPos()));
	//			if (dist < enemy->GetCollisionRadius())
	//			{
	//				//当たった
	//				enemy->Dead();//敵を消す(生存フラグをfalseに)
	//				bullet->Dead();//弾も消す
	//			}
	//		}
	//	}
	//
	//
	//	////////////////
	//	if (enemy->GeSize() == Enemy::Size::LARGE)
	//	{
	//		for (int i = 0;i < 4;i++)
	//		{
	//			Enemy* e = new Enemy(Enemy::Size::MEDIUM, 8);
	//			e->SetPos(enemyPos);
	//			//速さの設定は必要
	//			e->SetVel({ (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) });
	//			enemies.push_back(e);
	//			AddObject(e);
	//		}
	//	}
	//	else if (enemy->GetSize() == Enemy::Size::MEDIUM)
	//	{
	//		for (int i = 0; i < 4;i++)
	//		{
	//			Enemy* e = new Enemy(Enemy::Size::SMALL, 8);
	//			e->SetPos(enemyPos);
	//			//x,yともに-100から100のランダムな速度
	//			e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
	//			enemies.push_back(e);
	//			AddObject(e);
	//		}
	//	}
	//	else
	//	{
	//		//小なら消して、エフェクト生成
	//		ExplosionEffect* effect = new ExplosionEffect(enemy -> GetPos());
	//		effect->SetCharaColor(GetColor(GetRand(256), GetRand(256), GetRand(256)));
	//		AddObject(effect);
	//	}
	//	
	//
	//
	//
	//
	//
	//
	//	//	for (int i = 0;i < enemies.size();i++)
	//	//	{
	//	//		float dist = < Math2D::Length(Math2D::Sub(itr->GetPos(), enemies[i]->GetPos()));
	//	//		if (dist < enemies[i]->GetCollisionRadius())
	//	//		{
	//	//			enemies[i]->Dead();
	//	//
	//	//
	//	//			Vector2D enemyPos = enemies[i]->GetPos();
	//	//			Enemy::Size enemySize = enemies[i]->GetSize();
	//	//			if (enemySize == Enemy::Size::SMALL)
	//	//			{
	//	//				//小は何もしない
	//	//			}
	//	//			else if (enemySize == Enemy::Size::MEDIUM)
	//	//			{
	//	//				for (int i = 0; i < 4;i++)
	//	//				{
	//	//					Enemy* e = new Enemy(Enemy::Size::SMALL, 8);
	//	//					e->SetPos(enemyPos);
	//	//					//x,yともに-100から100のランダムな速度
	//	//                    e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
	//	//					enemies.push_back(e);
	//	//					AddObject(e);
	//	//				}
	//	//			}
	//	//			else if (enemySize == Enemy::Size::LARGE)
	//	//			{
	//	//				for (int i = 0;i < 4;i++)
	//	//				{
	//	//					Enemy* e = new Enemy(Enemy::Size::MEDIUM, 8);
	//	//					e->SetPos(enemyPos);
	//	//					//速さの設定は必要
	//	//					e->SetVel({ (float)(GetRand(200) - 100),(float)(GetRand(200) - 100) });
	//	//					enemies.push_back(e); 
	//	//					AddObject(e);
	//	//
	//	//				}
	//	//
	//	//			}
	//	//
	//	//			itr->Dead();
	//	//		}
	//	//	}
	//	//}
	//
	//
	//	//賞味期限切れの弾を消す
	//	DeleteBullet();
	//	//死んでる弾を消す
	//	
	//	void Stage::Enemy_vs_Bullet()
	//	{
	//		std::vector<Enemy*>aliveEnemies;
	//		std::vector<Bullet*>aliveBullets;
	//	}
	//
	//	void Stage::player_vs_Enemy()
	//	{
	//	}
	//
	//	void Stage::DeleteBullet()
	//	{
	//		//賞味期限切れの弾を消す
	//		//まず、箱の中身を確認して、死んでる弾があったらdeleteする、(箱は残るので、nullptrを入れておく)
	//		for (auto& itr : objects)
	//		{
	//			if (itr->GetType() == OBJ_TYPE::BULLET)
	//			{
	//				//base -> 継承クラスの時は、ちゃんとクラスのポインタに変換してあげないと。継承クラスのメンバ関数は呼び出せない
	//				//継承クラスからbaseクラスの変換は暗黙的に行われる
	//				Bullet* b = (Bullet*)(itr);
	//				if (b->IsDead())
	//				{
	//					delete b;
	//					itr = nullptr;//ポインタをnullptrにしておく
	//				}
	//			}
	//		}
	//		//次に、箱の中身を確認して、nullptrがあったら箱から消す(箱自体を詰める)
	//		for (auto it = objects.begin();it != objects.end();)
	//		{
	//			if (*it == nullptr)
	//			{
	//				it = objects.erase(it);
	//			}
	//			else
	//			{
	//				it++;
	//			}
	//		}
	//	}
	//}

	//タイトル画面のアップデート処理
	//ゲームスタート用のキーが押されたら、stageStateを1にする
	if (stageState == 0)
	{
		if (Input::IsKeyDown(KEY_INPUT_SPACE))
		{
			stageState = 1;
		}
			TitleUpdate();
	}

	//ゲーム画面
	else if (stageState == 1)
	{
		Player* player = nullptr;

		for (auto& obj : objects)
		{
			if (obj->GetType() == OBJ_TYPE::PLAYER)
			{
				player = (Player*)obj;
				break;
			}
		}

		//プレイヤーが死んだらゲームオーバー
		if (player == nullptr || player->IsAlive() == false)
		{
			stageState = 2;
			gameOveerTimer = 0;
		}

		PlayUpdate();
	}

	//ゲームオーバー画面
	else if (stageState == 2)
	{
		gameOveerTimer++;
		if (gameOveerTimer > 180)
		{
			stageState = 0;
		}
		GameOverUpdate();
	}

}

void Stage::Release()
{

}

void Stage::Enemy_vs_Bullet()
{
//敵VS弾の当たり判定
//敵の位置と当たり判定の半径
//弾の位置
//isAlive_ -> falseにする手段
	std::vector<Enemy*> aliveEnemies;
	std::vector<Bullet*> aliveBullets;

	aliveEnemies.clear();//念のため、毎フレームaliveEnemiesを空にする
	aliveBullets.clear();//念のため、毎フレームaliveBulletsを空にする

	//for(int i=0; i < objects.size(); i++)
	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			//baseクラスのポインタを敵クラスのポインタに変換してる
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
		else if (obj->GetType() == OBJ_TYPE::BULLET)
		{
			//baseクラスのポインタを弾クラスのポインタに変換してる
			Bullet* b = (Bullet*)obj;
			if (!b->IsDead()) {
				aliveBullets.push_back(b);
			}
		}
	}
	//for(int i=0;i<aliveBullets.size();i++)
	for (auto& bullet : aliveBullets)
	{
		for (auto& enemy : aliveEnemies)
		{
			float dist = Math2D::Length(Math2D::Sub(bullet->GetPos(), enemy->GetPos()));
			if (dist < enemy->GetCollisionRadius())
			{
				//当たった
				enemy->Dead();//敵を消す(生存フラグをfalseに）
				int sc[3] = { 20, 50, 100 };//大中小のスコア
				gameScore_ += sc[enemy->GetSize()];//スコア加算
				//			//TODO:
				//分裂の処理をここでやりたい
				//大か中か小かを判定して
				//大なら中を2~4つ、中なら小を2~4つ、小なら消してエフェクト生成
				if (enemy->GetSize() != Enemy::Size::SMALL)
				{
					int num = GetRand(3) + 2; //2~4のランダムな数
					//大きさによって、分裂数変えると素敵です。
					for (int i = 0; i < num; i++)
					{
						Enemy* e = nullptr;
						if (enemy->GetSize() == Enemy::Size::LARGE)
						{
							//大なら中を2~4つ
							e = new Enemy(Enemy::Size::MEDIUM, 8);
						}
						else
						{
							//中なら小を2~4つ
							e = new Enemy(Enemy::Size::SMALL, 8);
						}
						e->SetPos(enemy->GetPos());
						//速さの設定は必要
						e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
						AddObject(e);
					}
				}
				else
				{
					//小なら消してエフェクト生成
					ExplosionEffect* effect = new ExplosionEffect(enemy->GetPos());
					effect->SetCharaColor(GetColor(GetRand(255), GetRand(255), GetRand(255)));
					AddObject(effect);
				}
				bullet->Dead();//弾も消す
			}
		}
	}
}

void Stage::Player_vs_Enemy()
{
	std::vector<Enemy*> aliveEnemies;//生きてる敵のリスト
	aliveEnemies.clear();//念のため、毎フレームaliveEnemiesを空にする
	Player* player = nullptr;

	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::PLAYER)
		{
			player = (Player*)obj;
		}
		else if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			//baseクラスのポインタを敵クラスのポインタに変換してる
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
	}

	if (player == nullptr || player->IsAlive() == false)
		return; //プレイヤーがいないか、死んでたらスルー

	for (auto& enemy : aliveEnemies)
	{
		//①敵とプレイヤーの距離を計算
		float dist = Math2D::Length(Math2D::Sub(player->GetPos(), enemy->GetPos()));
		//②敵とプレイヤーの当たり判定の半径を足したものより、
		float collisionDist = player->GetCollisionRadius() + enemy->GetCollisionRadius();
		//距離が近かったら当たったとする
		if (dist < collisionDist)
		{
			//プレイヤーを死なせる
			player->Dead();
			//赤いエフェクトを生成
			ExplosionEffect* effect = new ExplosionEffect(player->GetPos(), 50);
			effect->SetCharaColor(GetColor(255, 0, 0));
			AddObject(effect);

			break;
		}
	}

}

void Stage::Draw()
{
	if (stageState == 0)
	{
		TitleDraw();
	}
	else if (stageState == 1)
	{
		PlayDraw();
	}
	else if (stageState == 2)
	{
		GameOverDraw();
	}
}

void Stage::DeleteBullet()
{
	//賞味期限切れの弾を消す
	//まず、箱の中身を確認して、死んでる弾があったらdeleteする（箱は残るので、nullptrを入れておく）
	//for (int i = 0;i < objects.size();i++)
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::BULLET)
		{
			//base->継承クラスの時は、ちゃんと継承クラスのポインタに変換してあげないと、継承クラスのメンバ関数は呼び出せない
			//継承クラス→baseクラスの変換は暗黙的に行われる
			Bullet* b = (Bullet*)(itr);
			if (b->IsDead())
			{
				delete b;
				itr = nullptr; //ポインタをnullptrにしておく
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す(箱自体を詰める）
	for (auto it = objects.begin(); it != objects.end(); )
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::DeleteEnemy()
{
	//死んでる敵を消す
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::ENEMY)
		{
			Enemy* b = (Enemy*)(itr);
			if (b->IsAlive() == false)
			{
				delete b;
				itr = nullptr; //ポインタをnullptrにしておく
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す(箱自体を詰める）
	for (auto it = objects.begin(); it != objects.end(); )
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::DeleteEffect()
{
	//死んでる敵を消す
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::EFFECT)
		{
			ExplosionEffect* b = (ExplosionEffect*)(itr);
			if (b->IsFinished() == true)
			{
				delete b;
				itr = nullptr; //ポインタをnullptrにしておく
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す(箱自体を詰める）
	for (auto it = objects.begin(); it != objects.end(); )
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::ShootBullet()
{
	Player* player = nullptr;
	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::PLAYER)
		{
			player = (Player*)obj;
			break;
		}
	}
	Vector2D pos = player->GetPos();
	Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
	unsigned int bcol = GetColor(255, 255, 255);
	float r = 2;
	float life = 2.0f;

	Bullet* b = new Bullet(pos, v, bcol, r, life);
	AddObject(b);
}