#include "pch.h"
#define DRAWPOS FALSE

std::pair<std::string, std::string> parseIni() {
	// Boards: Marble1, Marble2, Plastic, Wood, Glass
	// Pieces: Marble, Plastic, Rust, Wood

	// codiguim

	return std::make_pair( "Wood", "Wood" );
}

//-----------------------------------------------------------------------------------------------------------
// setup functions
//-----------------------------------------------------------------------------------------------------------
// carrega as texturas das casas
void loadHouses( const std::string& houseType ) {
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			std::string path = "assets/neo/casas/";
			path += ((row + col) % 2 == 0) ? "b3.png" : "b4.png";

			if (!houseTextures[row][col].loadFromFile( path )) {
				MessageBoxA( 0, "Não foi possível carregar o asset da casa!", 0, 0 );
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------
// gera o tabuleiro
std::vector<sf::Sprite> loadBoard() {
	std::vector<sf::Sprite> boardSprites;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			sf::Sprite houseSprite( houseTextures[row][col] );
			houseSprite.setPosition( col * 101.375f, row * 101.375f );
			boardSprites.push_back( houseSprite );
		}
	}
	return boardSprites;
}
//-----------------------------------------------------------------------------------------------------------
// carrega a textura das peças
std::vector<Piece> loadPieces() {
	std::vector<Piece> pieces;

	white[0].loadFromFile( "assets/neo/white/white_king.png" );
	white[1].loadFromFile( "assets/neo/white/white_queen.png" );
	white[2].loadFromFile( "assets/neo/white/white_bishop.png" );
	white[3].loadFromFile( "assets/neo/white/white_knight.png" );
	white[4].loadFromFile( "assets/neo/white/white_rook.png" );
	white[5].loadFromFile( "assets/neo/white/white_pawn.png" );

	black[0].loadFromFile( "assets/neo/black/black_king.png" );
	black[1].loadFromFile( "assets/neo/black/black_queen.png" );
	black[2].loadFromFile( "assets/neo/black/black_bishop.png" );
	black[3].loadFromFile( "assets/neo/black/black_knight.png" );
	black[4].loadFromFile( "assets/neo/black/black_rook.png" );
	black[5].loadFromFile( "assets/neo/black/black_pawn.png" );

	for (int i = 0; i < 6; ++i) {
		Piece p;
		p.team = 'w';
		switch (i) {
		case 0: p.type = 'K'; break;
		case 1: p.type = 'Q'; break;
		case 2: p.type = 'B'; break;
		case 3: p.type = 'N'; break;
		case 4: p.type = 'R'; break;
		case 5: p.type = 'P'; break;
		}
		p.sprite = sf::Sprite( white[i] );
		pieces.push_back( p );
	}

	for (int i = 0; i < 6; ++i) {
		Piece p;
		p.team = 'b';
		switch (i) {
		case 0: p.type = 'k'; break;
		case 1: p.type = 'q'; break;
		case 2: p.type = 'b'; break;
		case 3: p.type = 'n'; break;
		case 4: p.type = 'r'; break;
		case 5: p.type = 'p'; break;
		}
		p.sprite = sf::Sprite( black[i] );
		pieces.push_back( p );
	}

	return pieces;
}
//-----------------------------------------------------------------------------------------------------------
// AUX FUNCTS
// CONVERTE NOTAÇÕES DE XADREZ , PARA CORDENADAS DA TELA

sf::Vector2f chess2Screen( const std::pair<char, int>& pos, int offX = 0, int offY = 0, int sz = 101 ) {
	int width = 0, height = 0;

	switch (pos.first) {
	case 'a': width = 0; break;
	case 'b': width = sz; break;
	case 'c': width = sz * 2; break;
	case 'd': width = sz * 3; break;
	case 'e': width = sz * 4; break;
	case 'f': width = sz * 5; break;
	case 'g': width = sz * 6; break;
	case 'h': width = sz * 7; break;
	}
	switch (pos.second) {
	case 1: height = 7 * sz; break;
	case 2: height = 6 * sz; break;
	case 3: height = 5 * sz; break;
	case 4: height = 4 * sz; break;
	case 5: height = 3 * sz; break;
	case 6: height = 2 * sz; break;
	case 7: height = 1 * sz; break;
	case 8: height = 0; break;
	default: height = 0; break;
	}

	return sf::Vector2f( width + offX, height + offY );
}

//-----------------------------------------------------------------------------------------------------------
// GERA UM VECTOR  COM AS POSIÇÕES NA TELA DE CADA CASA DO TABULEIRO

std::vector<sf::Vector2f> getCasas( int x, int y ) {
	std::vector<sf::Vector2f> coordinates;

	for (int row = 7; row >= 0; --row) {
		for (int col = 0; col < 8; ++col) {
			float x_ = x + col * 101.375f;
			float y_ = y + row * 101.375f;
			coordinates.emplace_back( x_, y_ );
		}
	}

	return coordinates;
}

//-----------------------------------------------------------------------------------------------------------

int main( void ) {
	sf::RenderWindow wnd;
	sf::VideoMode wnd_size( 811, 811 );
	wnd.create( wnd_size, "Chess++" );
	// -------------------------------
	// definicoes
	loadHouses( "Wood" );
	auto boardSprites = loadBoard();
	auto spPieces = loadPieces();
	auto casasCords = getCasas( 0, 0 );
	sf::Font f = sf::Font();
	struct chess::Board board;

	// --------------------------------
	// atribuicoes

	assert( board.init() && "Não consegui iniciar o tabuleiro!" );
	f.loadFromFile( "assets/Arial.ttf" );

	// -------------------------------

	// main loop
	while (wnd.isOpen()) {
		// -----------------------------------------
		// Handle Events
		sf::Event e;
		while (wnd.pollEvent( e )) {
			if (e.type == sf::Event::Closed)
				wnd.close();
		}
		// -----------------------------------------

		wnd.clear();

		// desenha as casas do tabuleiro
		for (auto& houseSprite : boardSprites) {
			wnd.draw( houseSprite );
		}

		// desenha os nomes da casa em suas respectivas cordenadas
		// para fim de testes
		if (DRAWPOS) {
			for (int row = 1; row <= 8; ++row) {
				for (int col = 0; col < 8; ++col) {
					char file = 'a' + col;
					std::pair<char, int> pos = std::make_pair( file, row );
					std::string posName = std::string( 1, file ) + std::to_string( row );
					auto str = sf::Text( posName.c_str(), f, 32 );
					auto screenPos = chess2Screen( pos );
					str.setPosition( screenPos.x, screenPos.y );
					str.setFillColor( sf::Color::Black );
					wnd.draw( str );
				}
			}
		}
		// -----------------------------------------
		// desenha as posicoes iniciais
		for (const auto& piece : board.Board) {
			auto pos = chess2Screen( piece.get()->position );

			sf::Sprite p_sprite;

			switch (piece.get()->name) {
			case 'K':
				p_sprite = sf::Sprite( spPieces[0].sprite );
				break;
			case 'Q':
				p_sprite = sf::Sprite( spPieces[1].sprite );
				break;
			case 'B':
				p_sprite = sf::Sprite( spPieces[2].sprite );
				break;
			case 'N':
				p_sprite = sf::Sprite( spPieces[3].sprite );
				break;
			case 'R':
				p_sprite = sf::Sprite( spPieces[4].sprite );
				break;
			case 'P':
				p_sprite = sf::Sprite( spPieces[5].sprite );
				break;
			case 'k':
				p_sprite = sf::Sprite( spPieces[6].sprite );
				break;
			case 'q':
				p_sprite = sf::Sprite( spPieces[7].sprite );
				break;
			case 'b':
				p_sprite = sf::Sprite( spPieces[8].sprite );
				break;
			case 'n':
				p_sprite = sf::Sprite( spPieces[9].sprite );
				break;
			case 'r':
				p_sprite = sf::Sprite( spPieces[10].sprite );
				break;
			case 'p':
				p_sprite = sf::Sprite( spPieces[11].sprite );
				break;
			default:
				continue;
			}

			p_sprite.setPosition( pos );
			p_sprite.setScale( 101.375f / p_sprite.getGlobalBounds().width, 101.375f / p_sprite.getGlobalBounds().height );
			wnd.draw( p_sprite );
		}

		wnd.display();
	}

	// -------------------------------
	return 0;
}