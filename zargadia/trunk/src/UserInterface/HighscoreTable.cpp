#include <HighscoreTable.h>

//	Globals
HighscoreTable	*Highscore;

HighscoreTable::HighscoreTable(){}
HighscoreTable::~HighscoreTable()
{
	Write();
}

//==========================
//	HighscoreTable class
//==========================
void HighscoreTable::Initialise(void)
{
	Window *w = TitleUI->GetWindowPtr("Title");

	Textbox *tb;
	tb = (Textbox *)w->AddComponent(new TextBoxSetup(600,160,1,font,"Highscore Table"));
	tb->SetScale(0.5,0.5);
	tb->SetColour(255,0,0,255);
	
	tb = (Textbox *)w->AddComponent(new TextBoxSetup(600,335,1,font,"Press [R] to reset"));
	tb->SetScale(0.5,0.5);
	tb->SetColour(255,0,0,255);

	CapabilityData cap;
	cap.FlashingMilliseconds = 500;
	tb->SetCaps(WNDCOMP_FLASHING,&cap);

	for(int a=0;a<10;a++){
		p[a].tb_name	= (Textbox *)w->AddComponent(new TextBoxSetup(600,180+(a*15),1,font,"name"));
		p[a].tb_name->SetScale(0.5,0.5);
		
		p[a].tb_score	= (Textbox *)w->AddComponent(new TextBoxSetup(700,180+(a*15),1,font,"score"));
		p[a].tb_score->SetScale(0.5,0.5);
	}
}

void HighscoreTable::Read(void)
{
	std::ifstream file("highscores.dat", std::ios::binary);

	char temp[50];

	for(int a=0;a<10;a++){
		file.read((char *)p[a].name,sizeof(char)*3);
		sprintf(temp,"%c%c%c",p[a].name[0],p[a].name[1],p[a].name[2]);
		if(p[a].tb_name!=NULL) p[a].tb_name->UpdateString(temp);

		file.read((char *)&p[a].score,sizeof(int));
		sprintf(temp,"%i",p[a].score);
		if(p[a].tb_score!=NULL) p[a].tb_score->UpdateString(temp);
	}
	file.close();
}

void HighscoreTable::Write(void)
{
	std::ofstream file("highscores.dat",std::ios::binary);
	for(int a=0;a<10;a++){
		file.write((char *)p[a].name,sizeof(char) * 3);
		file.write((char *)&p[a].score,sizeof(int));
	}
	file.close();
}

void HighscoreTable::Default(void)
{
	char *names[10] = {	"CHR", "ADY", "MRK", "STE", "OWN", "RCH", "CAT", "PAU", "ZAR", "ADM" };

	int score = 125000;

	for(int a=0;a<10;a++)
	{
		p[a].name[0]	=	names[a][0];
		p[a].name[1]	=	names[a][1];
		p[a].name[2]	=	names[a][2];
		p[a].score		=	score;
		score			/=	1.8;
	}
}

void HighscoreTable::Update(void)
{
	char temp[50];

	for(int a=0;a<10;a++){
		sprintf(temp,"%c%c%c",p[a].name[0],p[a].name[1],p[a].name[2]);
		if(p[a].tb_name!=NULL) p[a].tb_name->UpdateString(temp);

		sprintf(temp,"%i",p[a].score);
		if(p[a].tb_score!=NULL) p[a].tb_score->UpdateString(temp);
	}
}

void HighscoreTable::Update(char *name, int score)
{
	char buffer[10];

	for(int a=0;a<10;a++)
	{
		if(score > p[a].score)
		{
			for(int b=9;b>a;b--)
			{
				p[b].name[0]	= p[b-1].name[0];
				p[b].name[1]	= p[b-1].name[1];
				p[b].name[2]	= p[b-1].name[2];
				p[b].score		= p[b-1].score;
				
				sprintf(buffer,"%c%c%c",p[b].name[0],p[b].name[1],p[b].name[2]);
				p[b].tb_name->UpdateString(buffer);
				
				sprintf(buffer,"%i",p[b].score);
				p[b].tb_score->UpdateString(buffer);
			}

			p[a].name[0]	= name[0];
			p[a].name[1]	= name[1];
			p[a].name[2]	= name[2];
			p[a].score		= score;

			sprintf(buffer,"%c%c%c",name[0],name[1],name[2]);
			p[a].tb_name->UpdateString(buffer);
			
			sprintf(buffer,"%i",score);
			p[a].tb_score->UpdateString(buffer);
			
			break;
		}
	}
}