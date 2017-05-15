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
		if(hasMeme)
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		else
		{
			switch (NeighboursMemes)
			{
			case 0:
				SpriteCodex::DrawTile0(screenPos, gfx);
				break;
			case 1:
				SpriteCodex::DrawTile1(screenPos, gfx);
				break;
			case 2:
				SpriteCodex::DrawTile2(screenPos, gfx);
				break;
			case 3:
				SpriteCodex::DrawTile3(screenPos, gfx);
				break;
			case 4:
				SpriteCodex::DrawTile4(screenPos, gfx);
				break;
			case 5:
				SpriteCodex::DrawTile5(screenPos, gfx);
				break;
			case 6:
				SpriteCodex::DrawTile6(screenPos, gfx);
				break;
			case 7:
				SpriteCodex::DrawTile7(screenPos, gfx);
				break;
			case 8:
				SpriteCodex::DrawTile8(screenPos, gfx);
				break;
			}
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
	Color c = Colors::Blue;
	if (win)
	{
		c = Colors::Green;
	}
	else
	{
		if (defeat)
		{
			c = Colors::Red;
		}
	}
	gfx.DrawRect(GetRect().GetExpanded(SpriteCodex::tileSize), c);
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Vei2 gridPos = { x,y };
			TileAt(gridPos).Draw(gridPos*SpriteCodex::tileSize + Vei2(leftField,topField),gfx);
		}
	}

	
	


}

void MemeField::Update(Mouse& mouse)
{
	if (!win && !defeat)
	{
		while (!mouse.IsEmpty())
		{
			const auto e = mouse.Read();
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				OnRevealClick(mouse.GetPos());
			}
			else if (e.GetType() == Mouse::Event::Type::RPress)
			{
				OnFlagClick(mouse.GetPos());
			}
			win = Win(); 
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
	if (GetRect().Contains(screenPos))
	{
		Vei2 gridPos = ScreenToGrid(screenPos);
		Tile& tile = TileAt(gridPos);
		if (tile.IsHidden())
		{
			tile.Reveal();
			if (!tile.HasMeme())
			{
				RevealNeighours(gridPos);
			}
			else
			{
				defeat = true;
			}
		}
	}
}

void MemeField::OnFlagClick(const Vei2 & screenPos)
{
	if (GetRect().Contains(screenPos))
	{
		Vei2 gridPos = ScreenToGrid(screenPos);
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed())
		{
			tile.Flag();
		}
	}
}

int MemeField::CountNeighboursMemes(const Vei2 & gridPos)
{
	const int j1 = std::max(0, gridPos.x - 1);
	const int j2 = std::min(width - 1, gridPos.x + 1);
	const int i1 = std::max(0, gridPos.y - 1);
	const int i2 = std::min(height - 1, gridPos.y + 1);
	int k = 0;

	for (int i = i1; i <= i2; i++)
	{
		for (int j = j1; j <= j2; j++)
		{
			if (TileAt(Vei2(j, i)).HasMeme())
			{
				k++;
			}
		}
	}

	return k;
}

void MemeField::RevealNeighours(const Vei2 & gridPos)
{
	if (TileAt(gridPos).NeighboursMemes == 0)
	{
		const int j1 = std::max(0, gridPos.x - 1);
		const int j2 = std::min(width - 1, gridPos.x + 1);
		const int i1 = std::max(0, gridPos.y - 1);
		const int i2 = std::min(height - 1, gridPos.y + 1);

		for (int i = i1; i <= i2; i++)
		{
			for (int j = j1; j <= j2; j++)
			{
				Tile& tile = TileAt(Vei2(j, i));
				if (tile.IsHidden())
				{
					tile.Reveal();
					RevealNeighours(Vei2(j,i));
				}
			}
		}
	}
}

bool MemeField::Win()
{
	bool test = true;
	for (int i = 0; i < nTiles; i++) {
		if (!tiles[i].HasMeme()) {
			test = test && tiles[i].IsRevealed();
		}
	}
	return test;
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
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Tile& tile = TileAt(Vei2(x,y));
			tile.NeighboursMemes = CountNeighboursMemes(Vei2(x, y));
		}
	}
	for (int i = 0; i < nTiles; i++) {
		//tiles[i].Reveal();
	}
}

MemeField::Tile & MemeField::TileAt(const Vei2 pos)
{
	return tiles[pos.x + pos.y*width];
}

const MemeField::Tile & MemeField::TileAt(const Vei2 pos) const
{
	return tiles[pos.x + pos.y*width];
}
