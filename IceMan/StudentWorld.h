#ifndef STUDENTWORLD_H
#define STUDENTWORLD_H

#include "GameWorld.h"
#include <vector>
#include "Actor.h";

class StudentWorld : public GameWorld {
public:
    StudentWorld(std::string assetDir) : GameWorld(assetDir) {};
    virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    void addActor(Actor* a);
    void clearIce(int x, int y);
    bool canActorMoveTo(Actor* a, int x, int y) const;
    int annoyAllNearbyActors(Actor* annoyer, int points, int radius);
    void revealAllNearbyObjects(int x, int y, int radius);
    Actor* findNearbyIceMan(Actor* a, int radius) const;
    Actor* findNearbyPickerUpper(Actor* a, int radius) const;
    void annoyIceMan();
    void giveIceManSonar();
    void giveIceManWater();
    bool facingTowardIceMan(Actor* a) const;
    GraphObject::Direction lineOfSightToIceMan(Actor* a) const;
    bool isNearIceMan(Actor* a, int radius) const;
    GraphObject::Direction determineFirstMoveToExit(int x, int y);
    GraphObject::Direction determineFirstMoveToIceMan(int x, int y);
    void updateDisplayText();

private:
    std::vector<Actor*> m_actors;
    IceMan* m_iceman;
    Ice* m_iceField[64][64];
};

#endif // STUDENTWORLD_H_

//=========================================================================================
//#ifndef STUDENTWORLD_H_
//#define STUDENTWORLD_H_
//
//#include "GameWorld.h"
//#include "GameConstants.h"
//#include "Actor.h"
//#include <string>
//#include <vector>
//using namespace std;
//
//// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//
//class StudentWorld : public GameWorld
//{
//private:
//	Iceman* m_iceman;
//	Ice* m_iceField[64][64];
//	vector<Actor*> m_actors;
//	unsigned int m_barrelsLeft;
//	vector<Protestor*> m_protestors;
//
//public:
//	StudentWorld(std::string assetDir)
//		: GameWorld(assetDir){}
//
//	virtual int init();
//
//	virtual int move();
//
//	virtual void cleanUp()
//	{
//	}
//
//	void clearIce(int x, int y, int dir);
//
//	void updateDisplayText();
//
//	unsigned int getBarrelsLeft();
//
//	int annoyAllNearbyActors(Actor* a, int points, int radius);
//
//	void addActor(Actor* a);
//
//	bool canActorMoveTo(Actor* a, int x, int y) const;
//
//	bool isNearIceman(Actor* a, int radius) const;
//
//	//bool checkRadius(int x, int y, double radius) const;
//
//	bool checkRadius(int x1, int y1, int x2, int y2, double radius) const;
//
//	int intPow(int x) const;
//
//	//bool checkIce(int x, int y, GraphObject::Direction dir);
//
//	bool checkIce(int x, int y);
//
//	bool checkIceBoulder(int x, int y, GraphObject::Direction dir);
//
//	void squirtWater(int x, int y, GraphObject::Direction dir);
//
//	void createBoulders(int numBoulders);
//
//	vector<Protestor*> getProtestors();
//
//	// If the IceMan is within radius of a, return a pointer to the
//// IceMan, otherwise null.
//	//Actor* findNearbyIceMan(Actor* a, int radius) const;
//
//	// If at least one actor that can pick things up is within radius of a,
//	// return a pointer to one of them, otherwise null.
//	//Actor* findNearbyPickerUpper(Actor* a, int radius) const;
//
//	vector<Actor*> getActors();
//	//Iceman getIceman();
//};
//
//#endif // STUDENTWORLD_H_
