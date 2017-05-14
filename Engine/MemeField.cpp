#include "MemeField.h"
#include <random>

void MemeField::Tile::PlaceMeme()
{
	hasMeme = true;
}

void MemeField::Tile::Draw(const Vei2 & screenPos, Graphics & gfx) const
{
	

	switch(state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Revealed:
		SpriteCodex::DrawTile0(screenPos, gfx);
		if(hasMeme)
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	}
}

void MemeField::Draw(Graphics & gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++) {
		for(gridPos.x=0;gridPos.x<width;gridPos.x++){
			TileAt(gridPos).Draw(gridPos*SpriteCodex::tileSize + Vei2(leftField,topField),gfx);
		}
	}
}

void MemeField::Update(Mouse& mouse)
{
	
	while (!mouse.IsEmpty())
	{
		const auto e = mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			Vei2 gridPos = ScreenToGrid(mouse.GetPos());
			Tile& tile = TileAt(gridPos);
			if (tile.IsHidden())
			{
				tile.Reveal();
			}
		}
		else if (e.GetType() == Mouse::Event::Type::RPress)
		{
			Vei2 gridPos = ScreenToGrid(mouse.GetPos());
			Tile& tile = TileAt(gridPos);
			if (!tile.IsRevealed())
			{
				tile.Flag();
			}
		}
	}
	
}

RectI MemeField::GetRect() const
{
	return RectI(leftField,leftField + width*SpriteCodex::tileSize, topField, topField + height*SpriteCodex::tileSize);
}

Vei2 MemeField::ScreenToGrid(const Vei2 & screenPos)
{
	return Vei2 (screenPos - Vei2(leftField, topField))/SpriteCodex::tileSize;
	
}

void MemeField::OnRevealClick(const Vei2 & screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed())
	{
		tile.Reveal();
	}
}

void MemeField::OnFlagClick(const Vei2 & screenPos)
{
	Vei2 gridPos = ScreenToGrid(screenPos);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed())
	{
		tile.Flag();
	}
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Reveal()
{
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

void MemeField::Tile::Flag()
{
	if (IsFlagged()) 
	{
		state = State::Hidden;
	}
	else if (IsHidden())
	{
		state = State::Flagged;
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

bool MemeField::Tile::IsHidden() const
{
	return state == State::Hidden;
}

MemeField::MemeField()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int i = 0; i < nMemes; i++) {
		Vei2 pos;
		do
		{
			pos = { xDist(rng),yDist(rng) };
		} while(TileAt(pos).HasMeme());
		TileAt(pos).PlaceMeme();
	}
}

MemeField::Tile & MemeField::TileAt(const Vei2 pos)
{
	return tiles[pos.x + pos.y*height];
}

const MemeField::Tile & MemeField::TileAt(const Vei2 pos) const
{
	return tiles[pos.x + pos.y*height];
}
