

//避免了混合类型赋值
class AbstractAnimal {
protected:
    AbstractAnimal& operator=(const AbstractAnimal& rhs);
public:
    virtual ~AbstractAnimal() = 0;
};

class Animal: public AbstractAnimal
{
public:
    Animal& operator=(const Animal& rhs);

};

class Lizard: public AbstractAnimal
{
public:
    Lizard& operator=(const Lizard& rhs);
    ...
};

class Chicken public AbstractAnimal
{
public:
    Chicken& operator=(const Chicken& rhs);
    ...
};