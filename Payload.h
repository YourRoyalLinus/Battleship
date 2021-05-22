#pragma once
#include <cereal/archives/json.hpp>

class Payload {
	public:
		enum class BufferType {
			TURN,
			NEXT_GUESS,
			GUESS_RESULT,
			GAME_OVER
		};
		struct Buffer {
			int xChoordGuess = -1;
			int yChoordGuess = -1;
			bool prevGuessHit = false;
			bool yourTurn = false;
			BufferType bufferType;

			//This method lets cereal know which data members to serialize
			template<class Archive>
			void serialize(Archive& archive) {
				archive(xChoordGuess, yChoordGuess, prevGuessHit, yourTurn, bufferType); //Seralize by passing members to the archive
			}
		};
};