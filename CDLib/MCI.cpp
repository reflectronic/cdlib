#include "pch.h"
#include "MCI.h"

#include <chrono>

#pragma comment(lib, "Winmm.lib")

#if 0
MCIAudioCDPlayer::MCIAudioCDPlayer()
{
	MCI_OPEN_PARMS openParms = { 0 };
	openParms.lpstrDeviceType = L"cdaudio";

	MCIERROR mciError = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE, (DWORD_PTR)&openParms);
	if (mciError != 0)
	{
		winrt::throw_hresult(HRESULT_FROM_WIN32(mciError));
	}

	deviceHandle = openParms.wDeviceID;
}

bool MCIAudioCDPlayer::IsMetadataSupported()
{
	return false;
}

bool MCIAudioCDPlayer::IsMediaInserted()
{
	MCI_STATUS_PARMS statusParms = { 0 };
	statusParms.dwItem = MCI_STATUS_MEDIA_PRESENT;

	MCIERROR mciError = mciSendCommand(deviceHandle, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&statusParms);
	if (mciError != 0)
	{
		winrt::throw_hresult(HRESULT_FROM_WIN32(mciError));
	}

	return statusParms.dwReturn == 0;
}

winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCD> MCIAudioCDPlayer::GetInsertedMedia()
{
	auto cds = winrt::single_threaded_vector<winrt::CDLib::IAudioCD>();

	if (IsMediaInserted())
	{
		cds.Append(winrt::make<MCIAudioCD>(deviceHandle));
	}

	return cds.GetView();
}

// Maybe we should try setting the project settings to ANSI to see if that fixes this problem...

void MCIAudioCDPlayer::Close()
{
}

MCIAudioCD::MCIAudioCD(MCIDEVICEID deviceHandle)
{
	this->deviceHandle = deviceHandle;
}

winrt::Windows::Foundation::Collections::IVectorView<winrt::CDLib::IAudioCDTrack> MCIAudioCD::Tracks()
{
	auto tracks = winrt::single_threaded_vector<winrt::CDLib::IAudioCDTrack>();

	MCIERROR mciError;
	MCI_STATUS_PARMS statusParms = { 0 };
	statusParms.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
	mciError = mciSendCommand(deviceHandle, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR) &statusParms);

	if (mciError != 0)
	{
		winrt::throw_hresult(HRESULT_FROM_WIN32(mciError));
	}

	if (statusParms.dwReturn == 0)
	{
		return tracks.GetView();
	}

	for (auto i = 0; i < statusParms.dwReturn; i += 1)
	{
		statusParms.dwTrack = (DWORD) i + 1;
		statusParms.dwItem = MCI_STATUS_LENGTH;
		mciError = mciSendCommand(deviceHandle, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD_PTR)&statusParms);

		if (mciError != 0)
		{
			winrt::throw_hresult(HRESULT_FROM_WIN32(mciError));
		}

		DWORD trackLength = (DWORD)statusParms.dwReturn;
		auto duration = std::chrono::minutes(MCI_MSF_MINUTE(trackLength)) + std::chrono::seconds(MCI_MSF_SECOND(trackLength));

		tracks.Append(winrt::make<MCIAudioCDTrack>(std::chrono::duration_cast<winrt::Windows::Foundation::TimeSpan>(duration), i));
	}

	return tracks.GetView();
}

winrt::Windows::Foundation::IReference<char16_t> MCIAudioCD::DriveLetter()
{
	return 'D';
	return nullptr;
}

MCIAudioCDTrack::MCIAudioCDTrack(winrt::Windows::Foundation::TimeSpan& duration, uint32_t trackNumber)
{
	this->duration = duration;
	this->trackNumber = trackNumber;
}

winrt::hstring MCIAudioCDTrack::Name()
{
	return L"Unknown";
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
#endif
