#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class Graphics;
class Pen;

class PersonBuilder
{
protected:
    Graphics g;
    Pen p;
public:
    PersonBuilder(Graphics g, Pen p)
    {
        this->g = g;
        this->p = p;
    }

    void buildHead() = 0;
    void bulldBody() = 0;
    void buildArmLeft() = 0;
    void buildArmRight() = 0;
    void buildLegLeft() = 0;
    void buildLegRight() = 0;
};

class PersonThinBuilder: PersonBuilder
{
public:
    PersonThinBuilder(Graphics g, Pen p) {}
    void buildHead() override
    {
        g.DrawEllipse(p, 50, 20, 30, 30);
    }

    void  buildBody() override
    {
        g.DrawRectangle(p, 60, 50, 10, 50);
    }

    void buildArmLeft() override
    {
        g.DrawLine(p, 60, 50, 40, 100);
    }

    void buildArmRight() override
    {
        g.DrawLine(p, 70, 50, 90, 100);
    }

    void buildLegLeft() override
    {
        g.DrawLine(p, 60, 100, 45, 150);
    }

    void buildLegRight() override
    {
        g.DrawLine(p, 70, 100, 85, 150);
    }
};

class PersonDirector
{
private:
    PersonBuilder pb;
public:
    PersonDirector(PersonBuilder pb)
    {
        this->pb = pb;
    }

    void createPerson()
    {
        pb.buildHead();
        pb.bulldBody();
        pb.buildArmLeft();
        pb.buildArmRight();
        pb.buildLegLeft();
        pb.buildLegRight();
    }
};

int main()
{
    Pen p = new Pen(Color.Yellow);
    PersonThinBuilder ptb = new PersonThinBuilder(pictureBox1.CreateGraphics(), p);
    PersonDirector pdThin = new PersonDirector(ptb);
    pdThin.createPerson();
    return 0;
}