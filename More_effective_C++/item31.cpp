
#include <map>
#include <memory>

using namespace std;

//使用虚函数解决二重调度
class SpaceShip;
class SpaceStation;
class Asteroid;

class GameObject
{
public:
    virtual void collide(GameObject& otherObject) = 0;
    virtual void collide(SpaceShip& otherObject) = 0;
    virtual void collide(SpaceStation& otherObject) = 0;
    virtual void collide(Asteroid& otherObject) = 0;
};


class SpaceShip: public GameObject
{
public:
    virtual void collide(GameObject& otherObject) {
        otherObject.collide(*this);
    }
    virtual void collide(SpaceShip& otherObject) {}
    virtual void collide(SpaceStation& otherObject) {}
    virtual void collide(Asteroid& otherObject) {}
}

//模拟虚函数表
class CollisionWithUnknowObject
{
public:
    CollisionWithUnknowObject(GameObject& whatWeHit);
};

class GameObject
{
public:
    virtual void collide(GameObject& otherObject) = 0;
};


class SpaceShip: public GameObject
{
private:
    typedef void (*HitFunctionPtr)(GameObject&);
    typedef map<string, HitFunctionPtr> HitMap;
    static HitMap initializeCollisionMap() {
        HitMap *phm = new HitMap;
        (*phm)["SpaceShip"] = &hitSpaceShip;
        (*phm)["SpaceStation"] = &hitSpaceStation;
        (*phm)["Asteroid"] = &hitAsteroid;
        return phm;
    }
    static HitFunctionPtr  lookup(const GameObject& whatWeHit) {
        static unique_ptr<HitMap> collisionMap(initializeCollisionMap());
        HitMap::iterator mapEntry = collisionMap.find(typeid(whatWeHit).name);
        if (mapEntry == collisionMap.end()) return 0;
        return (*mapEntry).second;
    }
public:
    virtual void collide(GameObject& otherObject) {
        HitFunctionPtr hfp = lookup(otherObject);
        if (hfp) {
            (this->*hfp)(otherObject);
        }
        else {
            throw CollisionWithUnknowObject(otherObject);
        }
    }
    virtual void hitSpaceShip(GameObject&  spaceShip) {
        SpaceShip& otherShip = dynamic<SpaceShip&>(spaceShip);
        ......
    }
    virtual void hitSpaceStation(GameObject&  spaceStation) {
        SpaceStation& otherShip = dynamic<SpaceStation&>(spaceStation);
        ......
    }
    virtual void hitAsteroid(GameObject&  asteroid) {
        Asteroid& otherShip = dynamic<Asteroid&>(asteroid);
        ......
    }
}