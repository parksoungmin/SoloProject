#pragma once

class SceneStage1;

class AniPlayerDie : public GameObject
{
protected:
	sf::Sprite body;
	Animator animator;
	sf::Vector2f velocity = { 0.f,0.f };

	SceneStage1* stage1;

	std::unordered_map<std::string, AnimationClip*> animationMap;


public:
	AniPlayerDie(const std::string& name = "");
	~AniPlayerDie();

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetGlobalBounds() const override;
	void SetDie();
	void OnDie();
	void SetChange();
	void ChangeScene();
	void SceneSet();
	void Restart();
};
