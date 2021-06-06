#pragma once

class GameParams
{
	public:
		enum class State {
			MENU,
			SETUP,
			PLAYING,
			OVER
		};
		enum class Turn {
			PLAYER,
			OPPONENT
		};
		enum class Mode {
			PVP,
			SOLO
		};
		enum class Event {
			PLAYER_HIT,
			SHIP_SANK
		};

		static const int SCREEN_WIDTH = 1200;
		static const int SCREEN_HEIGHT = 600;


		static const int SQUARE_PIXEL_SIZE = 75; //Magic numbers WOW!
		static const int SQUARE_PIXEL_HEIGHT = 75;

};
