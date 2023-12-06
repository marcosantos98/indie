#pragma once

#include <lua.h>
#include <raylib.h>

#include "lu.h"

void rluaPushVec2(lua_State*, Vector2);

int rluaDrawText(lua_State*);
int rluaDrawRectangle(lua_State*);
int rluaGetWorldToScreen2D(lua_State*);