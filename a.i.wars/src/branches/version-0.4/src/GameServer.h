#ifndef _GAMESERVER_H_
	#define _GAMESERVER_H_

#include <Fusion.h>

class ClientBot;
class ServerBot;

class GameServer{
private:
	char *m_aidir;	

	virtual void	SetAIDir		(char *ai);
	virtual	void	Update			(void);

	friend	bool	Run(void);
public:
					GameServer		(char *ai);
	virtual			~GameServer		();
	virtual void	Initialise		(void);

	virtual void	InitCD			(void);
	virtual void	ProcessCD		(void);

	virtual bool	RegisterBot		(int key,ClientBot *client);
	
	virtual bool	RotateBody		(int key,int angle);
	virtual bool	RotateLimb		(int key,int angle);

	virtual bool	MoveForward		(int key,unsigned int distance);
	virtual bool	MoveBackward	(int key,unsigned int distance);

	virtual bool	FireOffence		(int key, int power);

	virtual void	Death			(ServerBot *sb);

	virtual char *	GetAIDir		(void);
};

#endif // #ifndef _GAMESERVER_H_

