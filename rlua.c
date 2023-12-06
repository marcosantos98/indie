#include "rlua.h"

#include <stdlib.h>

double get_number_field(lua_State* L, int idx, const char* name) {
	lua_getfield(L, idx, name);
	double number = lua_tonumber(L, -1);
	// lua_pop(L, 1);
	return number;
}

//TODO: Check types!
Color table_to_color(lua_State* L) {
	lua_getfield(L, -1, "r");
	int r = lua_tonumber(L, -1);
	printf("R: %d\n", r);
	lua_pop(L, 1);
	exit(1);
	Color c = (Color) {
		.r = get_number_field(L, -1, "r"),
		.g = get_number_field(L, -2, "g"),
		.b = get_number_field(L, -3, "b"),
		.a = get_number_field(L, -4, "a"),
	};
	printf("%f %f %f %f\n", c.r, c.g, c.b, c.a);
	return c;
}

void rluaPushVec2(lua_State* L, Vector2 vec) {
	lua_createtable(L, 0, 2);
	push_set_field_number(L, "x", vec.x);	
	push_set_field_number(L, "y", vec.y);	
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

int rluaGetWorldToScreen2D(lua_State*) {

	return 0;
}