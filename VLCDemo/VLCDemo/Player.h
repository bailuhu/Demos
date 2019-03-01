#pragma once


#include "vlc\vlc.h"

// struct libvlc_instance_t;
// struct libvlc_media_t;
// struct libvlc_media_player_t;


class Player
{
public:
	Player();
	~Player();

	bool init();
	void set_play_wnd(HWND hWnd);
	bool play(char* szFilePath);
	bool pause();
	bool stop();

	bool play_url(char* szFilePath);
	libvlc_time_t get_length();
	float get_position();

private:
	libvlc_instance_t *m_pVlcInstance;
	libvlc_media_t *m_pVlcMedia;
	libvlc_media_player_t *m_pVlcPlayer;
	libvlc_time_t m_length;
	float m_position;
	HWND m_hWnd;
	bool m_isPause;
	char* m_szPath;
};

