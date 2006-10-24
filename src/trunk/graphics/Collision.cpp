#include <graphics/Collision.h>

Collision::Collision()
{
	m_reduce_x	=	0;
	m_reduce_y	=	0;
	m_reduce_z	=	0;

	DisableVectorCorrection();
}

Collision::~Collision()
{
	m_groups.clear();
	m_comparegroups.clear();
	m_collisions.clear();
}

void Collision::AddEntity(unsigned int GroupID, Entity *e)
{
	if(GroupID == m_groups.size()){
		entitylist_t newgroup;
		m_groups.push_back(newgroup);
	}

	if(GroupID < m_groups.size())	m_groups[GroupID].push_back(e);
}

void Collision::AddGroupCollide(unsigned int GroupID, unsigned int *CollideGroupID, int num_groups)
{
	for(int a=0;a<num_groups;a++){
		if((GroupID < m_groups.size()) && (CollideGroupID[a] < m_groups.size())){
			m_comparegroups.push_back(GroupID);
			m_comparegroups.push_back(CollideGroupID[a]);
		}
	}
}

void Collision::AddGroupCollide(unsigned int GroupID, unsigned int CollideGroupID)
{
	if((GroupID < m_groups.size()) && (CollideGroupID < m_groups.size())){
		m_comparegroups.push_back(GroupID);
		m_comparegroups.push_back(CollideGroupID);
	}
}

void Collision::Update(void)
{
	unsigned int	first,second;
	int				g1,g2;
	Entity			*e1,*e2;

	unsigned int a;

	for(a=0;a<m_collisions.size();a++) delete m_collisions[a];
	m_collisions.clear();

	for(a=0;a<m_comparegroups.size();a+=2)
	{
		g1 = m_comparegroups[a+SRC];
		g2 = m_comparegroups[a+DST];

		for(first=0,second=0;first<m_groups[g1].size();first++,second=0)
		{
			//	Get the ptr to the entity itself
			e1 = m_groups[g1][first];

			//	Detect whether the entity is active or not
			if(e1->GetActive() == true){

				CalculateBox(e1,SRC);

				//	Optimisation for when you are colliding a group with itself,
				//	this basically eliminated checking 1 against 2 and then 2 against 1
				//	within the same group, cause that would mean for each loop, you'd be doing 1 unnecessary check
				if(g1==g2) second = first+1;

				for(;second<m_groups[g2].size();second++)
				{
					//	Get the ptr to the entity itself
					e2 = m_groups[g2][second];

					//	Detect whether the entity is active or not
					if(e2->GetActive() == true){
						
						CalculateBox(e2,DST);

						//	Collision detect both bb's against each other
						if(Intersect() == true){
							CollisionPair *pair	=	new CollisionPair;
							pair->GroupID[SRC]	=	g1;
							pair->GroupID[DST]	=	g2;
							pair->entity[SRC]		=	e1;
							pair->entity[DST]		=	e2;
							
							m_collisions.push_back(pair);

							(this->*CorrectVectors)(e1,e2);
						}
					}
				}
			}
		}
	}
}

void Collision::ResizeCollisionArea(int percentx, int percenty,int percentz)
{
	m_reduce_x	=	percentx;
	m_reduce_y	=	percenty;
	m_reduce_z	=	percentz;
}

void Collision::EnableVectorCorrection(void)
{
	CorrectVectors = &Collision::def_VectorCorrection;
}

void Collision::DisableVectorCorrection(void)
{
	CorrectVectors = &Collision::dum_VectorCorrection;
}

