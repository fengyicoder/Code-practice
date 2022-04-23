
#include <map>
#include <memory>

using namespace std;

//使用非成员的碰撞处理函数
#include "SpaceShip.h";
#include "SpaceStation.h";
#include "Asteroid.h";


namespace {
    void shipAsteroid(GameObject& spaceShip, GameObject& asteroid) 
    void shipStation(GameObject& spaceShip, GameObject& spaceStation);
    void asteroidStation(GameObject& asteroid, GameObject& spaceStation);
    void asteroidShip(GameObject& asteroid, GameObject& spaceShip) {
        shipAsteroid(spaceShip, asteroid);
    }
    void stationShip(GameObject& spaceStation, GameObject& spaceShip) {
        shipStation(spaceShip, spaceStation);
    }
    void stationAsteroid(GameObject& spaceStation, GameObject& asteroid) {
        asteroidStation(spaceStation, asteroid);
    }

    typedef void (*HitFunctionPtr)(GameObject&, GameObject&);
    typedef map<pair<string, string>, HitFunctionPtr> HitMap;
    pair<string, string> makeStringPair(const char *s1, const char *s2) {
        return pair<string, string>(s1, s2);
    }
    HitMap* initializerCollisionMap() {
        HitMap *phm = new HitMap;
        (*phm)[makeStringPair("SpaceShip", "Asteroid")] = &shipAsteroid;
        (*phm)[makeStringPair("SpaceShip", "SpaceStation")] = &shipStation;
        ......
        return phm;
    }
    HitFunctionPtr lookup(const string& class1, const string& class2) {
        static unique_ptr<HitMap> collisionMap(initializerCollisionMap());
        HitMap::iterator mapEntry = collisionMap->find(make_pair(class1, class2));
        if (mapEntry == collisionMap->end()) return 0;
        return (*mapEntry).second;
    }
}

void processCollision(GameObject& object1, GameObject& object2) {
    HitFunctionPtr phf = lookup(typeid(object1).name(), typeid(object2).name());
    if (phf) phf(object1, object2);
    else throw UnknowCollision(object1, object2);
}

//也可将映射表放入一个类，并由它提供动态修改映射关系的成员函数
namespace {
    class CollisionMap {
    public:
        typedef void (*HitFunctionPtr)(GameObject&, GameObject&);
        void addEntry(const string& type1, const string& type2, 
                    HitFunctionPtr collisionFunction, bool symmetric=true);
        void removeEntry(const string& type1, const string& type2);
        HitFunctionPtr lookup(const string& type1, const string& type2);
        static CollisionMap& theCollisionMap();
    private:
        CollisionMap();
        CollisionMap(const CollisionMap&);
    };
    class RegisterCollisionFunction {
    public:
        RegisterCollisionFunction(const string& type1, const string& type2, CollisionMap::HitFunctionPtr collisionFunction, bool symmetric=true)
        {
            CollisionMap::theCollisionMap().addEntry(type1, type2, collisionFunction, symmetric);
        }
    };
    void shipAsteroid(GameObject& spaceShip, GameObject& asteroid);
    void shipStation(GameObject& spaceShip, GameObject& spaceStation);
    void asteroidStation(GameObject& asteroid, GameObject& spaceStation);
    void asteroidShip(GameObject& asteroid, GameObject& spaceShip) {
        shipAsteroid(spaceShip, asteroid);
    }
    void stationShip(GameObject& spaceStation, GameObject& spaceShip) {
        shipStation(spaceShip, spaceStation);
    }
    void stationAsteroid(GameObject& spaceStation, GameObject& asteroid) {
        asteroidStation(spaceStation, asteroid);
    }
}

CollisionMap::theCollisionMap().addEntry("SpaceShip", "Asteroid", &shipAsteroid);
RegisterCollisionFunction cf1("SpaceShip", "Asteroid", &shipAsteroid);