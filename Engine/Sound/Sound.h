#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <array>
#include <string>

#pragma comment(lib,"xaudio2.lib")

namespace IFE
{
	class Sound
	{
	private:
		struct ChunkHeader
		{
			char id[4];
			int32_t size;
		};

		struct RiffHeader
		{
			ChunkHeader chunk;
			char type[4];
		};

		struct FormatChunk
		{
			ChunkHeader chank;
			WAVEFORMATEX fmt;
		};

		struct SoundData
		{
			WAVEFORMATEX wfex;
			float volume = 50;
			IXAudio2SourceVoice* pSourceVoice = nullptr;
			BYTE* pBuffer;
			uint32_t bufferSize;
			std::string name;
			bool free = false;
		};
	private:
		Microsoft::WRL::ComPtr<IXAudio2> xAudio;
		IXAudio2MasteringVoice* masterVoice;
		static const int16_t maxSound = 128;
		std::array<SoundData, maxSound> soundDatas;

	public:
		void Initialize();
		uint16_t LoadWave(std::string);
		void SoundUnLoad(uint16_t soundNum);
		void AllUnLoad();
		void SetVolume(uint16_t soundNum, std::int32_t volume);
		void SoundPlay(uint16_t soundNum, bool roop = true);
		void StopSound(uint16_t soundNum);
		static Sound* Instance();
		static void Finalize();

	private:
		Sound() {};
		Sound(const Sound&) {};
		Sound& operator=(const Sound&) {}
		~Sound() {};
	};
}
