#pragma once
// C++ STL
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <memory>
#include <utility>
#include <cassert>
#include <thread>
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <algorithm>
// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

//libs

#ifdef _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-system.lib")
#endif
//----------------------------------------------------

// windows
#ifndef __linux__
#include <windows.h>
#include <debugapi.h>
#include <winnt.h>
#endif

// engine
#include "engine/chess.h"
#include "engine/Pieces.h"
#include "engine/board.h"

//---------------------------------
struct Piece {
	char team;
	char type;
	sf::Sprite sprite;
};
sf::Texture white[6];
sf::Texture black[6];
sf::Texture houseTextures[8][8];
enum Turn { WHITE, BLACK };
Turn turn = WHITE;
std::shared_ptr<chess::Piece> selPiece = nullptr;
std::thread moveT;
std::vector<sf::Sprite> defaultBoard;

std::pair<char, int> screen2Chess( const sf::Vector2f& screenPos, int sz = 101 ) {
	char file = 'a' + static_cast<int>(screenPos.x / sz);
	int rank = 8 - static_cast<int>(screenPos.y / sz);

	return std::make_pair( file, rank );
}

void switchTurn() {
	turn = (turn == WHITE) ? BLACK : WHITE;
}
//---------------------------------
