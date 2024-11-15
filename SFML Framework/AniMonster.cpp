#include "stdafx.h"
#include "AniMonster.h"
#include "AnimationClip.h"

AniMonster::AniMonster(const std::string& name)
	: GameObject(name)
{
}

AniMonster::~AniMonster()
{
}

void AniMonster::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void AniMonster::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);

}

void AniMonster::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);

}

void AniMonster::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void AniMonster::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);

}

void AniMonster::Init()
{
	sortingLayer = SortingLayers::Foreground;

	animator.SetTarget(&body);

	Idle();

	animator.Play(animationMap["Idle"]);
}

void AniMonster::Release()
{
}

void AniMonster::Reset()
{
	animator.Play(animationMap["Idle"]);
	SetOrigin(Origins::BC);
}

void AniMonster::Update(float dt)
{
	animator.Update(dt);
}

void AniMonster::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void AniMonster::Idle()
{
	AnimationClip* idle = &ResourceMgr<AnimationClip>::Instance().Get("Animations/monster/Monster.csv");

	auto find = animationMap.find("Idle");
	if (find == animationMap.end())
		animationMap.insert({ "Idle", idle });
}