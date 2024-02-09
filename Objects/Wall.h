#pragma once

struct Line {
	int x1;
	int y1;
	int x2;
	int y2;
};

class Wall {
public:
	Wall(int id);
	Wall();
	Wall(Line line) { m_line = line; }
	~Wall();

	int getID() const { return m_id; }
	Line getLine() const { return m_line; }

	void setID(int id) { m_id = id; }

private:
	int m_id;
	Line m_line;

};
