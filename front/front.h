#pragma once
#include "../lib/stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define DEFCMD (name, prior, read_func, write_func) void Get_##prior (char** s, Tree<Nod> tree, Nod* iter);

#include "../lang_tree/codegen.h"

#undef DEFCMD