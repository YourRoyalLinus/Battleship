#pragma once

class GameParams
{
	public:
		enum class State {
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
};