#include "stdafx.h"
#include "SceneStage1.h"
#include "AniPlayer.h"
#include "BackGround1.h"
#include "Block1.h"
#include "rapidcsv.h"
#include "AniMonster.h"
#include "Stage1TileMap.h"
#include "AniMonsterHitEffect.h"
#include "Particle.h"
#include "AniNpc.h"
#include "UiHub.h"
#include "SideGround.h"

SceneStage1::SceneStage1() : Scene(SceneIds::Dev1)
{
}

void SceneStage1::Init()
{
	cameraView.setCenter(50.f, 10.f);
	cameraView.setSize(FRAMEWORK.GetWindowSizeFloat());

	player = AddGo(new AniPlayer("player"));
	npc = AddGo(new AniNpc("npc"));
	AddGo(new BackGround1);
	AddGo(new SideGround);
	tileMap = AddGo(new Stage1TileMap("tilemap"));
	tileMap->SetTile(STAGE1_TABLE->GetTileMapTable(), STAGE1_TABLE->GetTileMapCount());
	monsterHitEffect = AddGo(new AniMonsterHitEffect("monsterHit"));
	uiHub = AddGo(new UiHub("uiHub"));

	Scene::Init();

}

void SceneStage1::Enter()
{
	FONT_MGR.Load("fonts/himalaya.ttf");
	player->SetPosition({ 250.f, -250.f });
	BlockSet();
	Scene::Enter();
	MonsterSet();
	life = maxlife;
	dontMoveTile = tileMap->DontMoveBounds();
}


void SceneStage1::Exit()
{
	for (auto& iter : block1List)
	{
		RemoveGo(iter);
		block1Pool.Return(iter);
	}
	block1List.clear();

	for (auto& iter : monsterList)
	{
		RemoveGo(iter);
		MonsterPool.Return(iter);
	}
	for (auto& iter : particleList)
	{
		RemoveGo(iter);
		particlePool.Return(iter);
	}
	monsterList.clear();
	Scene::Exit();
	FONT_MGR.Unload("fonts/himalaya.ttf");
}

void SceneStage1::Update(float dt)
{
	Scene::Update(dt);
	
	Direction dir = Direction::NONE;

	if (InputMgr::GetKeyDown(sf::Keyboard::Left))
	{
		player->SetScale({ 1.f,1.f });
		dir = Direction::LEFT;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Right))
	{
		player->SetScale({ -1.f,1.f });
		dir = Direction::RIGHT;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		dir = Direction::UP;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Down))
	{
		dir = Direction::DOWN;
	}
	if (dir == Direction::NONE|| life==0)
	{
		return;
	}
	--life;
	uiHub->SetLife(life);
	if (life == 0)
	{
		player->SetDie();
		return;
	}

	Target first = Target::NONE;
	Target second = Target::NONE;

	sf::Vector2f playerPos = player->GetPosition();
	sf::Vector2f targetpos = player->GetPosition() + directionVector[(int)dir];
	sf::Vector2f target2pos = player->GetPosition() + 2.f * directionVector[(int)dir];
	GameObject* target = nullptr;
	GameObject* target2 = nullptr;

	for (auto monster : monsterList)
	{
		sf::FloatRect monsterRect = monster->GetGlobalBounds();
		if (monsterRect.contains(targetpos))
		{
			first = Target::MONSTER;
			target = monster;
		}
		if (monsterRect.contains(target2pos))
		{
			second = Target::MONSTER;
			target2 = monster;
		}
	}
	for (auto block : block1List)
	{
		sf::FloatRect blockRect = block->GetGlobalBounds();
		if (blockRect.contains(targetpos))
		{
			first = Target::BLOCK;
			target = block;
		}
		if (blockRect.contains(target2pos))
		{
			second = Target::BLOCK;
			target2 = block;
		}
	}
	for (auto& wall : dontMoveTile)
	{
		if (wall.contains(targetpos))
		{
			first = Target::WALL;
		}
		if (wall.contains(target2pos))
		{
			second = Target::WALL;
		}
	}
	switch (first)
	{
	case SceneStage1::Target::NONE:
	{
		switch (dir)
		{
		case SceneStage1::Direction::LEFT:
			player->SetLeftMove();
			break;
		case SceneStage1::Direction::RIGHT:
			player->SetRigthMove();
			break;
		case SceneStage1::Direction::UP:
			player->SetUpMove();
			break;
		case SceneStage1::Direction::DOWN:
			player->SetDownMove();
			break;
		}
	}
	break;
	case SceneStage1::Target::MONSTER:
	{
		AniMonster* monster = dynamic_cast<AniMonster*>(target);
		player->PlayingKick();
		monsterHitEffect->SetPosition(playerPos + directionVector[(int)dir] * 0.5f);
		monsterHitEffect->SetActive(true);
		monsterHitEffect->PlayAnimation("Hit");
		switch (second)
		{
		case SceneStage1::Target::NONE:
			monster->HitingMonster((int)dir);
			break;
		case SceneStage1::Target::WALL:
			ParticleSet(monster->GetPosition());
			ReturnMonster(monster);
			break;
		case SceneStage1::Target::BLOCK:

			ParticleSet(monster->GetPosition());
			ReturnMonster(monster);
			break;
		}
	}
	break;
	case SceneStage1::Target::BLOCK:
	{
		Block1* block = dynamic_cast<Block1*>(target);
		player->PlayingKick();
		monsterHitEffect->SetPosition(playerPos + directionVector[(int)dir] * 0.5f);
		monsterHitEffect->SetActive(true);
		monsterHitEffect->PlayAnimation("Hit");
		if (second == Target::NONE)
		{
			block->HitingBlock1((int)dir);
		}
	}
		break;
	case SceneStage1::Target::WALL:
		return;
		break;
	}
}
void SceneStage1::LateUpdate(float dt)
{
	for (int i = 0; i < 4; ++i)
	{
		contact[i] = false;
		kick[i] = false;
	}
}

void SceneStage1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneStage1::BlockSet()
{
	for (int i = 0; i < 4; ++i)
	{
		Block1* block = block1Pool.Take();
		block1List.push_back(block);

		Block1::Types blockType = (Block1::Types)count;
		block->SetType(blockType);
		count++;

		sf::Vector2f position = sf::Vector2f{ block->GetPosition() };
		block->SetPosition(position);

		AddGo(block);
	}
}

void SceneStage1::ParticleSet(const sf::Vector2f& createPosition)
{
	count = 0;
	for (int i = 0; i < 7; ++i)
	{
		Particle* particle = particlePool.Take();
		particleList.push_back(particle);
		
		Particle::Types particleType = (Particle::Types)count;
		particle->SetType(particleType);
		if (count < 4)
		{
		particle->MonsterDie({ Utils::RandomRange(-400.f, -200.f), 500 });
		}
		if (count > 6)
		{
			particle->MonsterDie({ Utils::RandomRange(200.f, 400.f), 500 });
		}
		else
		{
		particle->MonsterDie({ Utils::RandomRange(-200.f, 200.f), 500 });
		}
		count++;
		AddGo(particle);
		particle->SetPosition(createPosition);
		particle->SetActive(true);
	}
}

void SceneStage1::MonsterSet()
{
	for (int i = 0; i < 3; ++i)
	{
		AniMonster* monster = MonsterPool.Take();
		monsterList.push_back(monster);

		auto& data = MONSTER_TABLE->Get(i);
		monster->SetPosition({ data.positionX ,data.positionY });


		AddGo(monster);
	}
}

void SceneStage1::ReturnMonster(AniMonster* item)
{
	RemoveGo(item);
	MonsterPool.Return(item);
	monsterList.remove(item);
}

void SceneStage1::ReturnParticle(Particle* item)
{
	RemoveGo(item);
	particlePool.Return(item);
	particleList.remove(item);
}
