#include "protos.h"
#include "stack.h"

int main (int argc, char* argv[]) {

    Stack<int> lol;

    lol.push (19);
    lol.push (19);
    lol.push (19);
    lol.push (19);
    lol.push (19);
    lol.push (19);
    lol.push (19);
    lol.push (19);
    lol.push (19);
    dump (lol);
    lol.pop ();
    lol.pop ();
    lol.pop ();
    lol.pop ();
    dump (lol);
}