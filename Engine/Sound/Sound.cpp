#include "Sound.h"
#include "Debug.h"
#include <cassert>

using namespace IFE;

void IFE::Sound::Initialize()
{
	HRESULT result = XAudio2Create(&xAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result) && "�T�E���h�̏������Ɏ��s���܂���");
	result = xAudio->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result) && "�T�E���h�̏������Ɏ��s���܂���");
}

uint16_t IFE::Sound::LoadWave(std::string filename)
{
	for (uint16_t i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)continue;
		if (soundDatas[i].name == filename)return i;
	}
	std::string name = "Data/Sound/";
	name += filename + ".wav";

	uint16_t num = 0xffff;
	for (uint16_t i = 0; i < Sound::maxSound; i++)
	{
		if (soundDatas[i].free == false)
		{
			num = i;
			break;
		}
	}

	if (num == -1)assert(0 && "SoundData�̋󂫂�����܂���");

	std::ifstream file;

	file.open(name, std::ios_base::binary);
	assert(file.is_open() && "�t�@�C�����J���܂���");

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)assert(0 && "�t�@�C����RIFF�ł͂���܂���");
	if (strncmp(riff.type, "WAVE", 4) != 0)assert(0 && "�t�H�[�}�b�g��WAVE�ł͂���܂���");

	FormatChunk format;

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chank.id, "fmt ", 4) != 0)assert(0 && "�`�����N�̊m�F�Ɏ��s���܂���");
	assert(format.chank.size <= sizeof(format.fmt) && "40byte�͑Ή����Ă܂���");
	file.read((char*)&format.fmt, format.chank.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK ", 4) == 0)
	{
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0)assert(0 && "�f�[�^�̊m�F�Ɏ��s���܂���");

	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	file.close();

	soundDatas[num].wfex = format.fmt;
	soundDatas[num].pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundDatas[num].bufferSize = data.size;
	soundDatas[num].free = true;

	return num;
}

void IFE::Sound::SoundUnLoad(uint16_t soundNum)
{
	if (!soundDatas[soundNum].free)return;
	delete[]soundDatas[soundNum].pBuffer;

	soundDatas[soundNum].pBuffer = 0;
	soundDatas[soundNum].bufferSize = 0;
	soundDatas[soundNum].wfex = {};
	soundDatas[soundNum].free = false;
	soundDatas[soundNum].pSourceVoice = nullptr;
}

void IFE::Sound::AllUnLoad()
{
	for (uint16_t i = 0; i < maxSound; i++)
	{
		SoundUnLoad(i);
	}
}

void IFE::Sound::SoundPlay(uint16_t soundNum, bool roop)
{
	HRESULT result;
	result = xAudio.Get()->CreateSourceVoice(&soundDatas[soundNum].pSourceVoice, &soundDatas[soundNum].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas[soundNum].pBuffer;
	buf.AudioBytes = soundDatas[soundNum].bufferSize;
	if (roop)buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	else buf.LoopCount = 0;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	soundDatas[soundNum].pSourceVoice->SetVolume(soundDatas[soundNum].volume);
	result = soundDatas[soundNum].pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundDatas[soundNum].pSourceVoice->Start();
}

Sound* IFE::Sound::Instance()
{
	static Sound inst;
	return &inst;
}

void IFE::Sound::SetVolume(uint16_t soundNum, int32_t volume)
{
	soundDatas[soundNum].volume = (float)volume / 255.0f;
	if (soundDatas[soundNum].pSourceVoice != nullptr)soundDatas[soundNum].pSourceVoice->SetVolume(soundDatas[soundNum].volume);
}

void IFE::Sound::StopSound(uint16_t soundNum)
{
	if (soundDatas[soundNum].pSourceVoice == nullptr)return;
#ifdef _DEBUG
	HRESULT result = soundDatas[soundNum].pSourceVoice->Stop();
	assert(SUCCEEDED(result));

#else
	soundDatas[soundNum].pSourceVoice->Stop();
#endif
	soundDatas[soundNum].pSourceVoice = nullptr;
}

void IFE::Sound::Finalize()
{
	Sound* inst = Instance();
	inst->xAudio.Reset();
	inst->AllUnLoad();
}
