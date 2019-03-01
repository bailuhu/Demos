#include "stdafx.h"
#include "Player.h"


#pragma comment(lib, "libvlccore.lib")
#pragma comment(lib, "libvlc.lib")

Player::Player()
{
	m_pVlcInstance = NULL;
	m_pVlcMedia = NULL;
	m_pVlcPlayer = NULL;
	m_isPause = false;
	m_szPath = new char[MAX_PATH];
}


Player::~Player()
{
	delete [] m_szPath;
}

void Player::set_play_wnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool Player::init()
{
	bool bRet = false;

	do
	{
		if (!m_pVlcInstance)
			m_pVlcInstance = libvlc_new(0, nullptr);

	} while (false);
	return bRet;
}

bool Player::play(char* szFilePath)
{
	bool bRet = false;
	do
	{
		if (strcmp(szFilePath, m_szPath) != 0 ||
			strlen(m_szPath) == 0)
		{//与当前不同视频，重头播放 || 第一次播放视频
			m_pVlcMedia = libvlc_media_new_path(m_pVlcInstance, szFilePath);

			if (m_pVlcPlayer)
				libvlc_media_player_release(m_pVlcPlayer);
			m_pVlcPlayer = libvlc_media_player_new_from_media(m_pVlcMedia);

			if (m_hWnd)
				libvlc_media_player_set_hwnd(m_pVlcPlayer, m_hWnd);

		}//else 同一部片子，继续播放
		

		if (-1 == libvlc_media_player_play(m_pVlcPlayer))
			break;

		memcpy(m_szPath, szFilePath, strlen(szFilePath)+1);
		m_isPause = false;
		bRet = true;

	} while (false);

	return bRet;
}

bool Player::pause()
{
	bool bRet = false;
	if (libvlc_media_player_can_pause(m_pVlcPlayer))
	{
		libvlc_media_player_pause(m_pVlcPlayer);
		m_isPause = true;
		bRet = true;
	}
	
	return bRet;
}

bool Player::stop()
{
	if (m_pVlcPlayer) 
		libvlc_media_player_stop(m_pVlcPlayer);
	return true;
}

libvlc_time_t Player::get_length()
{
	if (m_pVlcPlayer)
		return m_length = libvlc_media_player_get_length(m_pVlcPlayer)/1000;
	else
		return 0;
}

float Player::get_position()
{
	if (m_pVlcPlayer)
		return m_position = libvlc_media_player_get_position(m_pVlcPlayer);
	else
		return 0;
}

bool Player::play_url(char* szFilePath)
{
	bool bRet = false;
	do
	{
		if (strcmp(szFilePath, m_szPath) != 0 ||
			strlen(m_szPath) == 0)
		{//与当前不同视频，重头播放 || 第一次播放视频
			m_pVlcMedia = libvlc_media_new_location(m_pVlcInstance, szFilePath);

			if (m_pVlcPlayer)
				libvlc_media_player_release(m_pVlcPlayer);
			m_pVlcPlayer = libvlc_media_player_new_from_media(m_pVlcMedia);

			if (m_hWnd)
				libvlc_media_player_set_hwnd(m_pVlcPlayer, m_hWnd);

		}//else 同一部片子，继续播放


		if (-1 == libvlc_media_player_play(m_pVlcPlayer))
			break;

		memcpy(m_szPath, szFilePath, strlen(szFilePath) + 1);
		m_isPause = false;
		bRet = true;

	} while (false);

	return bRet;
}
