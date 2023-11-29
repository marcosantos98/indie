#include <stdio.h>
#include <sys/stat.h>

#define ARENA_IMPLEMENTATION
#include <arena.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#define MAP_W 100
#define MAP_H 100

#define SAFE_LUA(L, name) 						\
	do { 										\
		if(luaL_dofile(L, name) != LUA_OK) { 	\
			printf("\033[31mLua error:\033[39m %s\n", lua_tostring(L, -1)); \
			goto end; 						\
		} 									\
	} while(0); 								\

#define PUSH_COLOR(L, color) push_raylib_color(L, color, #color)
#define PUSH_KEY(L, key) push_raylib_key(L, key, #key)

typedef struct {
	int x;
	int y;
} Entity;

typedef struct {
	char id;
} TileInfo;

typedef struct {
	Entity* player;	
	TileInfo* map;
} GameData;

static int* entities = NULL;
static GameData data = {0};
static Arena main_arena = {0};

static void dumpstack (lua_State *L) {
  int top=lua_gettop(L);
  for (int i=1; i <= top; i++) {
    printf("%d\t%s\t", i, luaL_typename(L,i));
    switch (lua_type(L, i)) {
      case LUA_TNUMBER:
        printf("%g\n",lua_tonumber(L,i));
        break;
      case LUA_TSTRING:
        printf("%s\n",lua_tostring(L,i));
        break;
      case LUA_TBOOLEAN:
        printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
        break;
      case LUA_TNIL:
        printf("%s\n", "nil");
        break;
      default:
        printf("%p\n",lua_topointer(L,i));
        break;
    }
  }
}

int try_pop_number(lua_State* L) {
	if(lua_isnumber(L, -1)) {
		int r = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return r;
	} else {
		printf("Trying to convert to number on a non number value!.\n");
		return 0;
	}
}

char* try_pop_string(lua_State* L) {
	if(lua_isstring(L, -1)) {
		char* r = lua_tostring(L, -1);
		lua_pop(L, 1);
		return r;
	} else {
		printf("Trying to convert to string on a non string value!.\n");
		return 0;
	}
}

void push_entity(lua_State* L, Entity* entity, const char* name) {
	lua_newtable(L);
	lua_pushnumber(L, entity->x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, entity->y);
	lua_setfield(L, -2, "y");
	lua_setfield(L, -2, name);	
}

void push_raylib_color(lua_State* L, Color c, const char* name) {
	lua_newtable(L);
	lua_pushnumber(L, c.r);
	lua_setfield(L, -2, "r");
	lua_pushnumber(L, c.g);
	lua_setfield(L, -2, "g");
	lua_pushnumber(L, c.b);
	lua_setfield(L, -2, "b");
	lua_pushnumber(L, c.a);
	lua_setfield(L, -2, "a");
	lua_setglobal(L, name);
}

void push_raylib_key(lua_State* L, int key, const char* name) {
	lua_pushnumber(L, key);
	lua_setglobal(L, name);
}

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

int call_raylib_drawrectangle(lua_State* L) {
	Color c = table_to_color(L);

	int h = try_pop_number(L);
	int w = try_pop_number(L);
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	DrawRectangle(x, y, w, h, c);

	return 0;
}

int call_raylib_iskeydown(lua_State* L) {
	int key = try_pop_number(L);
	lua_pushboolean(L, IsKeyDown(key));
	return 1;
}

int call_raylib_ismousepressed(lua_State* L) {
	int key = try_pop_number(L);
	lua_pushboolean(L, IsMouseButtonPressed(key));
	return 1;
}

int call_raylib_drawtext(lua_State* L) {
	

	Color color = table_to_color(L);

	int fontSize = try_pop_number(L);
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	const char* text = try_pop_string(L);

	DrawText(text, x, y, fontSize, color);
	return 0;
}

int call_raylib_getfps(lua_State* L) {
	lua_pushnumber(L, GetFPS());
	return 1;
}

int call_gamedata_get_tile_info_at(lua_State* L) {
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	int idx = y * MAP_W + x;

	TileInfo info = data.map[idx];
	lua_newtable(L);
	lua_pushnumber(L, info.id);
	lua_setfield(L, -2, "id");

	return 1;
}

int call_gamedata_set_tile_info_at(lua_State* L) {
	int id = try_pop_number(L);
	int y = try_pop_number(L);
	int x = try_pop_number(L);

	int idx = y * MAP_W + x;
	TileInfo* at = &data.map[idx];
	at->id = id;

	return 0;
}

//TODO: fixme
void gamedata_from_lua(lua_State* L) {
	//lua_getglobal(L, "data");
	//lua_getfield(L, -1, "playerX");
	//data.playerX = try_pop_number(L);
	//lua_getfield(L, -1, "playerY");
	//data.playerY = try_pop_number(L);
	//lua_pop(L, 1);	
}

void setup_lua(lua_State* L) {
	luaL_openlibs(L); //Load std libraries from lua.
	
	// Create Data Table
	lua_newtable(L); // data global index -1

	// data.player
	push_entity(L, data.player, "player");

	// data
	lua_setglobal(L, "data");

	// for future color palette
	PUSH_COLOR(L, RED);
	PUSH_COLOR(L, GREEN);
	PUSH_COLOR(L, BLUE);

	// Global Numbers
	PUSH_KEY(L, KEY_D);
	PUSH_KEY(L, KEY_A);
	PUSH_KEY(L, KEY_W);
	PUSH_KEY(L, KEY_S);
	PUSH_KEY(L, MOUSE_BUTTON_LEFT); 
	PUSH_KEY(L, MAP_W);
	PUSH_KEY(L, MAP_H);

	// Raylib C Funs
	lua_register(L, "DrawRect", call_raylib_drawrectangle);
	lua_register(L, "IsKeyDown", call_raylib_iskeydown);	
	lua_register(L, "IsMouseButtonPressed", call_raylib_ismousepressed);
	lua_register(L, "DrawText", call_raylib_drawtext);
	lua_register(L, "GetFPS", call_raylib_getfps);
	
	//Game C Funs
	lua_register(L, "GetTileAt", call_gamedata_get_tile_info_at);
	lua_register(L, "SetTileAt", call_gamedata_set_tile_info_at);
}

//TODO: Check all scripts
bool check_for_reload() {
	static long long lastModified; // TODO: This needs to be removed after we add more scripts
	struct stat mainLua;
	stat("main.lua", &mainLua);

	if(mainLua.st_mtime > lastModified) {
		lastModified = mainLua.st_mtime;
		return true;
	}

	return false;
}

int main(void) {

	data.map = arena_alloc(&main_arena, sizeof(TileInfo) * (MAP_W * MAP_H));

	int idx = 0 * MAP_W + 0;
	data.map[idx] = (TileInfo){.id = 10};

	data.player = arena_alloc(&main_arena, sizeof(Entity));
	data.player->x = 0;
	data.player->y = 0;

	SetTraceLogLevel(LOG_NONE);
	InitWindow(1280, 720, "untitledgamejam-84 - Mushrooms");
	lua_State* L = luaL_newstate();
	
	setup_lua(L);

	SAFE_LUA(L, "main.lua");

	while(!WindowShouldClose()){
		
		if(check_for_reload()) {
			SAFE_LUA(L, "main.lua");
		} else {
			gamedata_from_lua(L);
		}


		ClearBackground(RAYWHITE);
		BeginDrawing();
		
		lua_getglobal(L, "render");
		if(lua_pcall(L, 0, 0, 0) != LUA_OK) {
			printf("Error during call to render: %s\n", lua_tostring(L, -1));
			exit(1);
		}

		EndDrawing();
	}


end:
	arena_free(&main_arena);
	lua_close(L);
	CloseWindow();
	return 0;
}
