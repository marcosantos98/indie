#pragma once

#include <lua.h>

static int try_pop_number(lua_State* L) {
	if(lua_isnumber(L, -1)) {
		int r = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return r;
	} else {
		printf("Trying to convert to number on a non number value!.\n");
		return 0;
	}
}

static char* try_pop_string(lua_State* L) {
	if(lua_isstring(L, -1)) {
		char* r = lua_tostring(L, -1);
		lua_pop(L, 1);
		return r;
	} else {
		printf("Trying to convert to string on a non string value!.\n");
		return 0;
	}
}