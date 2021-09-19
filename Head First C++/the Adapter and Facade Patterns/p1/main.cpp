#include "mallardDuck.h"
#include "turkeyAdapter.h"
#include "wildTurkey.h"

int main() {
    MallardDuck *duck = new MallardDuck();
    WildTurkey *wildTurkey = new WildTurkey();
    Duck *turkeyAdapter = new TurkeyAdapter(wildTurkey);

    turkeyAdapter->quack();
    turkeyAdapter->fly();

    return 0;
}