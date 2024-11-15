#pragma once
#include "Scene.h"
#include "json.hpp"

class AniPlayer;
class Block1;
class AniMonster;

using json = nlohmann::json;


class SceneStage1 : public Scene
{
protected:
	AniPlayer* player;
	

	int count = 0;

	std::list<Block1*> block1List;
	ObjectPool<Block1> block1Pool;

	std::list<AniMonster*> monsterList;
	ObjectPool<AniMonster> MonsterPool;
public:
	std::vector < std::vector<int>> stageInfo;


	SceneStage1();
	~SceneStage1() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	void BlockSet();
	void MonsterSet();
};
