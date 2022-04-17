#include "Audio.h"
#include <fstream>
#include <cassert>
//aaaaaa
#pragma comment(lib,"xaudio2.lib")

bool Audio::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}

	return true;
}

/// <summary>
/// サウンド読み込み
/// </summary>
/// <param name="filename"> ファイル名 </param>
void Audio::SoundLoadWave(const char* filename)
{
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルをバイナリモードで開く
	file.open(baseDirectory + filename, std::ios_base::binary);
	// ファイルオープン失敗を検出する
	assert(file.is_open());

	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}
	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();

	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	// ファイル名で登録
	soundDatas.emplace(filename, soundData);

	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundDatas[filename].wfex);
	assert(SUCCEEDED(result));

	// ファイル名で登録
	pSourceVoices.emplace(filename, pSourceVoice);
}

/// <summary>
/// 
/// </summary>
void Audio::SoundUnload()
{
	// バッファのメモリを解放
	soundDatas.clear();
	pSourceVoices.clear();
}

/// <summary>
/// 再生
/// </summary>
/// <param name="filename">ファイル名</param>
/// <param name="loopFlag">ループフラグ</param>
void Audio::SoundPlayWave(const char* filename, bool loopFlag)
{
	HRESULT result;

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};

	buf.pAudioData = soundDatas[filename].pBuffer;
	buf.AudioBytes = soundDatas[filename].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	//  buf.PlayBegin = 0;
	if (loopFlag) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	result = pSourceVoices[filename]->FlushSourceBuffers(); //最初から再生する
	result = pSourceVoices[filename]->SubmitSourceBuffer(&buf);
	result = pSourceVoices[filename]->Start();
}

/// <summary>
/// 再生を止める
/// </summary>
/// <param name="filename"></param>
void Audio::SoundStop(const char* filename)
{
	pSourceVoices[filename]->Stop();
}
