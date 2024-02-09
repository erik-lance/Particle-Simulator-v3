#pragma once

class Wall {
public:
	Wall(int id);
	Wall();
	~Wall();

	int getID() const { return m_id; }

	void setID(int id) { m_id = id; }

private:
	int m_id;

};

Wall::Wall(int id) {
	m_id = id;
}

Wall::Wall() {
}

Wall::~Wall() {
}