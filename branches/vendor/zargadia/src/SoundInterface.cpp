#include <SoundInterface.h>

//	Globals
SoundInterface *Sound;

extern std::string fusioncfg;

//=============================
//	SoundConfig class
//=============================
SoundConfig::SoundConfig()
{
	Read();
}

SoundConfig::~SoundConfig(){}

void SoundConfig::Read(void)
{
	if(fusion->xmlIsOpen() == true){
		void *root = fusion->xmlGetRootNode();
		void *snode = fusion->xmlFindNode(root,"sound");
		
		if(snode != NULL){
			unsigned int numNodes = fusion->xmlCountNodes(snode);
			
			for(unsigned int a=0;a<numNodes;a++){
				void *cnode = fusion->xmlGetChild(snode,a);
				std::string name,file;
				
				if(fusion->xmlGetNodeName(cnode) == "track"){
					name = fusion->xmlGetNodeProperty(cnode,"name");
					file = fusion->xmlGetNodeProperty(cnode,"file");
					
					if(name.empty() == false && file.empty() == false){
						if(name == "titlemusic")			TitleMusic = file;
						if(name == "gamemusic")				GameMusic = file;
						if(name == "credits")				CreditsMusic = file;
						
						if(name == "title zargadia")		TitleSFX[SoundInterface::SOUND_ZARGADIA] = file;
						if(name == "title whoosh")			TitleSFX[SoundInterface::SOUND_WHOOSH] = file;
						
						if(name == "alien fire")			GameSFX[SoundInterface::SOUND_ALIEN_FIRE] = file;
						if(name == "alien explode")			GameSFX[SoundInterface::SOUND_ALIEN_EXPLODE] = file;
						if(name == "mothership")			GameSFX[SoundInterface::SOUND_MOTHERSHIP] = file;
						if(name == "mothership fire")		GameSFX[SoundInterface::SOUND_MOTHERSHIP_FIRE] = file;
						if(name == "mothership explode")	GameSFX[SoundInterface::SOUND_MOTHERSHIP_EXPLODE] = file;
						if(name == "defence hit")			GameSFX[SoundInterface::SOUND_DEFENCE_HIT] = file;
						if(name == "defence damage")		GameSFX[SoundInterface::SOUND_DEFENCE_DAMAGE] = file;
						if(name == "defence explode")		GameSFX[SoundInterface::SOUND_DEFENCE_EXPLODE] = file;
						if(name == "player fire")			GameSFX[SoundInterface::SOUND_PLAYER_FIRE] = file;
						if(name == "player explode")		GameSFX[SoundInterface::SOUND_PLAYER_EXPLODE] = file;
					}
				}
			}
		}
	}
}

//=============================
//	Sound Class
//=============================

SoundInterface::SoundInterface(){}

SoundInterface::~SoundInterface()
{
	TitleMusic->Stop();
	CreditsMusic->Stop();
	GameMusic->Stop();
}

void SoundInterface::LoadTitleSFX(void)
{
	//TitleSFX[SOUND_ZARGADIA]	=	fusion->Sound->LoadSample(cfg.TitleSFX[SOUND_ZARGADIA]);
	//TitleSFX[SOUND_WHOOSH]		=	fusion->Sound->LoadSample(cfg.TitleSFX[SOUND_WHOOSH]);
}

void SoundInterface::LoadGameSFX(void)
{
	GameSFX[SOUND_ALIEN_EXPLODE]			=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_ALIEN_EXPLODE]);
	GameSFX[SOUND_ALIEN_FIRE]					=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_ALIEN_FIRE]);
	//GameSFX[SOUND_MOTHERSHIP]					=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_MOTHERSHIP]);
	//GameSFX[SOUND_MOTHERSHIP_FIRE]		=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_MOTHERSHIP_FIRE]);
	//GameSFX[SOUND_MOTHERSHIP_EXPLODE]	=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_MOTHERSHIP_EXPLODE]);
	GameSFX[SOUND_DEFENCE_DAMAGE]			=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_DEFENCE_DAMAGE]);
	GameSFX[SOUND_DEFENCE_EXPLODE]		=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_DEFENCE_EXPLODE]);
	GameSFX[SOUND_DEFENCE_HIT]				=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_DEFENCE_HIT]);
	GameSFX[SOUND_PLAYER_EXPLODE]			=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_PLAYER_EXPLODE]);
	GameSFX[SOUND_PLAYER_FIRE]				=	fusion->Sound->LoadSample(cfg.GameSFX[SOUND_PLAYER_FIRE]);
}

void SoundInterface::LoadTitleMusic(void)
{
	TitleMusic	=	fusion->Sound->LoadModule(cfg.TitleMusic);
}

void SoundInterface::LoadCreditsMusic(void)
{
	CreditsMusic = fusion->Sound->LoadModule(cfg.CreditsMusic);
}

void SoundInterface::LoadGameMusic(void)
{
	GameMusic = fusion->Sound->LoadModule(cfg.GameMusic);
}
