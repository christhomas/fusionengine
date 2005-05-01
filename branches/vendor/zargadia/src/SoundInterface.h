#ifndef _SOUNDINTERFACE_H_
	#define _SOUNDINTERFACE_H_

#include <Zargadia.h>

class SoundConfig{
public:

	std::string TitleMusic;
	std::string GameMusic;
	std::string CreditsMusic;

	std::string TitleSFX[2];
	std::string GameSFX[10];

			SoundConfig		();
			~SoundConfig	();
	void	Read			(void);
};

class SoundInterface{
public:
	SoundConfig cfg;

	ISoundBuffer	*TitleMusic;
	ISoundBuffer	*GameMusic;
	ISoundBuffer	*CreditsMusic;

	ISoundBuffer	*TitleSFX[2];
	ISoundBuffer	*GameSFX[10];

//============================================
//	Titlescreen Sound/Music array identifiers
//============================================
	enum{
		SOUND_ZARGADIA = 0,
		SOUND_WHOOSH,
	};

//=====================================
//	Game Sound/Music array identifiers
//=====================================
	enum{
		SOUND_ALIEN_EXPLODE = 0,
		SOUND_ALIEN_FIRE,
		SOUND_MOTHERSHIP,
		SOUND_MOTHERSHIP_FIRE,
		SOUND_MOTHERSHIP_EXPLODE,
		SOUND_DEFENCE_DAMAGE,
		SOUND_DEFENCE_EXPLODE,
		SOUND_DEFENCE_HIT,
		SOUND_PLAYER_EXPLODE,
		SOUND_PLAYER_FIRE,
	};

			SoundInterface		();
			~SoundInterface		();
	void	LoadTitleMusic		(void);
	void	LoadCreditsMusic	(void);
	void	LoadGameMusic		(void);

	void	LoadTitleSFX		(void);
	void	LoadGameSFX			(void);
};

extern	SoundInterface	*Sound;

#endif // #ifndef _SOUNDINTERFACE_H_