#pragma once
#include "Graphics.h"
#include "SpriteCodex.h"
#include "Vei2.h"
#include "Mouse.h"
class MemeField
{
public:
	MemeField();
	void Draw(Graphics& gfx) const;
	void Update(Mouse& mouse);
	RectI GetRect() const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	void OnRevealClick(const Vei2& screenPos);
	void OnFlagClick(const Vei2& screenPos);
	int CountNeighboursMemes(const Vei2& gridPos);
	void RevealNeighours(const Vei2& gridPos);
	bool Win();
	bool defeat = false;
	bool win = false;
private:
	class Tile
	{
	public:
		int NeighboursMemes = -1;
		void PlaceMeme();
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		bool HasMeme() const;
		void Reveal();
		bool IsRevealed() const;
		void Flag();
		bool IsFlagged() const;
		bool IsHidden() const;
	public: 
		enum class State
		{
			Hidden,
			Revealed,
			Flagged
		};
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};


private:
	static constexpr int width =  Graphics::ScreenWidth / SpriteCodex::tileSize - 2;
	static constexpr int height =  Graphics::ScreenHeight / SpriteCodex::tileSize - 2;
	static constexpr int nTiles = width * height;
	static constexpr int spawnMemeRate = 10; 
	static constexpr int nMemes = nTiles * spawnMemeRate / 100;
	static constexpr int leftField =  (Graphics::ScreenWidth - width * SpriteCodex::tileSize) / 2;
	static constexpr int topField =  (Graphics::ScreenHeight - height * SpriteCodex::tileSize) / 2;
private:
	Tile tiles[nTiles];
	Tile& TileAt(const Vei2 pos);
	const Tile& TileAt(const Vei2 pos)const;
};