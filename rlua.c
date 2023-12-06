#include "rlua.h"

//TODO: Check types!
Color table_to_color(lua_State* L) {
	lua_getfield(L, -1, "r");
	int r = lua_tonumber(L, -1);
	lua_getfield(L, -2, "g");
	int g = lua_tonumber(L, -1);
	lua_getfield(L, -3, "b");
	int b = lua_tonumber(L, -1);
	lua_getfield(L, -4, "a");
	int a = lua_tonumber(L, -1);
	lua_pop(L, 5); // Remove all pushed color values and the color table
	return (Color) {
		r, g, b, a,
	};
}


int rluaDrawText(lua_State* L) {
	Color color = table_to_color(L);

	int fontSize = try_pop_number(L);
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	const char* text = try_pop_string(L);

	DrawText(text, x, y, fontSize, color);
	return 0;

}

int rluaDrawRectangle(lua_State* L) {
	Color c = table_to_color(L);

	int h = try_pop_number(L);
	int w = try_pop_number(L);
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	DrawRectangle(x, y, w, h, c);

	return 0;
}