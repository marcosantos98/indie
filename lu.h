#pragma once

#include <stdio.h>

#include <lauxlib.h>
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
		const char* r = lua_tostring(L, -1);
		lua_pop(L, 1);
		return (char*)r;
	} else {
		printf("Trying to convert to string on a non string value!.\n");
		return 0;
	}
}

static void push_set_field_number(lua_State* L, const char* name, double number) {
	lua_pushnumber(L, number);
	lua_setfield(L, -2, name);
}

static void dumpstack(lua_State *L)
{
	int top = lua_gettop(L);
	for (int i = 1; i <= top; i++)
	{
		printf("%d\t%s\t", i, luaL_typename(L, i));
		switch (lua_type(L, i))
		{
		case LUA_TNUMBER:
			printf("%g\n", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("%s\n", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
			break;
		case LUA_TNIL:
			printf("%s\n", "nil");
			break;
		default:
			printf("%p\n", lua_topointer(L, i));
			break;
		}
	}
}
