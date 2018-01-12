//Author: Sivert Andresen Cubedo

#include "ViewPort.hpp"

ViewPort::ViewPort(Map & map, wykobi::point2d<float> & centre, wykobi::vector2d<float> & size) :
	m_map(map),
	m_view({ centre.x, centre.y }, { size.x, size.y }),
	m_type_renderer_vec(map.getNumberOfTypes())
{
}

sf::View & ViewPort::getView() {
	return m_view;
}

wykobi::rectangle<float> ViewPort::getRect() {
	wykobi::point2d<float> p0 = wykobi::make_point(
		m_view.getCenter().x - (m_view.getSize().x / 2.f),
		m_view.getCenter().y - (m_view.getSize().y / 2.f)
	);
	wykobi::point2d<float> p1 = wykobi::make_point(
		p0.x + m_view.getSize().x,
		p0.y + m_view.getSize().y
	);
	return wykobi::make_rectangle(p0, p1);
}

MapGroundTypeRenderer & ViewPort::getMapGroundTypeRenderer(MapGroundType type) {
	return m_type_renderer_vec[type];
}

wykobi::point2d<float> ViewPort::viewToMapPos(wykobi::point2d<float> & pos) {
	sf::Vector2f v = m_view.getInverseTransform().transformPoint({ pos.x, pos.y });
	return wykobi::make_point(v.x, v.y);
}

wykobi::point2d<float> ViewPort::mapToViewPos(wykobi::point2d<float> & pos) {
	sf::Vector2f v = m_view.getTransform().transformPoint({ pos.x, pos.y });
	return wykobi::make_point(v.x, v.y);
}

void ViewPort::render() {
	for (MapGroundTypeRenderer & t : m_type_renderer_vec) {
		t.clear();
	}
	wykobi::rectangle<float> rect = getRect();
	for (MapChunk* chunk : m_map.getChunkInRect(rect)) {
		for (auto it = chunk->getTriangles().begin(); it != chunk->getTriangles().end(); ++it) {
			m_type_renderer_vec[(*it)->getType()].appendTriangle(*(*it));
		}
	}
}

void ViewPort::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	target.setView(m_view);
	for (std::size_t i = 0; i < m_type_renderer_vec.size(); ++i) {
		target.draw(m_type_renderer_vec[i]);
	}
}


//end