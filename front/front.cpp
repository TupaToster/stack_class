#include "front.h"
#include "../lang_tree/tree.h"
#include "../lib/cmd.h"
#include "../lib/text.h"

Tree<Nod> GetG (const char* fileName) {

    Tree<Nod> prog;
    Text input = read_Text (fileName);
    Get_1 (&input.TextString, &prog, prog.getdata ());
}

#define DEFCMD (name, prior, read_func, write_func) void Get_##prior (char** s, Tree<Nod> tree, Nod* iter) read_func

#include "../lang_tree/codegen.h"

#undef DEFCMD