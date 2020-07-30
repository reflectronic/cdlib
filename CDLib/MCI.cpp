#include "pch.h"
#include "MCI.h"

#include <mciapi.h>

bool MCIAudioCDPlayer::IsMetadataSupported()
{
	return false;
}

bool MCIAudioCDPlayer::IsMediaInserted()
{
	MCIERROR mciError;
	TCHAR mciReturnBuffer[512];

	mciError = mciSendString(L"status cdaudio media present", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);

	if (mciError != 0)
	{
		return false;
	}

	return std::wstring(mciReturnBuffer) == L"true";
}

winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCD> MCIAudioCDPlayer::GetInsertedMedia()
{
	auto cds = winrt::single_threaded_vector<winrt::CDLib::IAudioCD>();

	if (IsMediaInserted())
	{
		cds.Append(winrt::make<MCIAudioCD>());
	}

	return cds.GetView();
}

void MCIAudioCDPlayer::Close()
{
}

MCIAudioCD::MCIAudioCD()
{
}

winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> MCIAudioCD::Tracks()
{
	MCIERROR mciError;
	TCHAR mciReturnBuffer[512];

	auto tracks = winrt::single_threaded_vector<winrt::CDLib::IAudioCDTrack>();

	mciError = mciSendString(L"status cdaudio number of tracks", mciReturnBuffer, sizeof(mciReturnBuffer), NULL);

	if (mciError != 0) 
	{
		return tracks.GetView();
	}
	
	auto trackCount = std::stoi(std::wstring(mciReturnBuffer));
	for (int i = 0; i < trackCount; i++)
	{
		TCHAR mciCommandBuffer[512];
		DWORD trackHours, trackMinutes, trackSeconds;

		swprintf_s(mciCommandBuffer, L"status cdaudio length track %d", i + 1);
		mciError = mciSendString(mciCommandBuffer, mciReturnBuffer, sizeof(mciReturnBuffer), NULL);

		if (mciError != 0)
		{
			// Handle error
		}
	}
}

winrt::Windows::Foundation::IReference<wchar_t> MCIAudioCD::DriveLetter()
{
	// TODO: Find out correct way to do this
	return nullptr;
}

winrt::hstring MCIAudioCDTrack::Name()
{
	return winrt::hstring();
}

winrt::Windows::Foundation::TimeSpan MCIAudioCDTrack::Duration()
{
	return winrt::Windows::Foundation::TimeSpan();
}

uint32_t MCIAudioCDTrack::TrackNumber()
{
	return uint32_t();
}

void MCIAudioCDTrack::Play()
{
}

void MCIAudioCDTrack::Pause()
{
}

void MCIAudioCDTrack::Stop()
{
}

MCIAudioCDTrack::MCIAudioCDTrack(uint32_t trackNumber)
{
}
