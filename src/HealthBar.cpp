#include "HealthBar.h"
#include "Game.h"

HealthBar::HealthBar(GameObject& object, int& current, int& total, float barscale, SDL_Colour fillcolour, SDL_Colour framecolour)
	: m_WatchObject(object), m_WatchCurrent(current), m_WatchTotal(total), m_cFrameColour(framecolour), m_cFillColour(fillcolour),
	m_rFrame({ 0, 0, static_cast<int>(100.0f * barscale), static_cast<int>(20.0f * barscale) }),
	m_rFill({ 0, 0, static_cast<int>(100.0f * barscale), static_cast<int>(20.0f * barscale) }) {}

void HealthBar::update()
{
	m_rFrame.x = static_cast<int>(m_WatchObject.getPosition().x - (m_rFrame.w * 0.5));
	m_rFrame.y = static_cast<int>(m_WatchObject.getPosition().y - m_WatchObject.getHeight() - (m_rFrame.h * 0.5));
	m_rFill = { m_rFrame.x, m_rFrame.y,
		static_cast<int>(static_cast<float>(m_rFrame.w) * (static_cast<float>(m_WatchCurrent) / static_cast<float>(m_WatchTotal))),
		m_rFrame.h };
}

void HealthBar::draw()
{
	SDL_SetRenderDrawBlendMode(TheGame::Instance()->getRenderer(), SDL_BLENDMODE_BLEND);
	
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), m_cFrameColour.r, m_cFrameColour.g, m_cFrameColour.b, m_cFrameColour.a);
	SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &m_rFrame);
	
	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), m_cFillColour.r, m_cFillColour.g, m_cFillColour.b, m_cFillColour.a);
	SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &m_rFill);

	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), m_cFrameColour.r, m_cFrameColour.g, m_cFrameColour.b, (m_cFrameColour.a * 2 >= 255 ? 255 : m_cFrameColour.a * 2));
	SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &m_rFrame);
}
