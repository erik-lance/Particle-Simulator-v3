#pragma once

struct Position { int x; int y; };
struct Line  { Position start; Position end; };
struct Screen { int width = 1280; int height = 720; };