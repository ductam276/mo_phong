#include <iostream>
#include <raylib.h>
#include <resource_dir.h>	
int main ()
{
	int menu = 1;
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(500, 600, "");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	Font gg = LoadFont("ggsans.ttf");
	ClearBackground(WHITE);
	while (!WindowShouldClose())
	{

		SetWindowTitle("Thuat toan thay the trang");
		if (IsKeyPressed(KEY_BACKSPACE)) {
			menu = 1;
		}
		else if (IsKeyPressed(KEY_ONE) && menu == 1) {
			menu = 2;
		}
		else if (IsKeyPressed(KEY_TWO) && menu == 1) {
			menu = 3;
		}
		else if (IsKeyPressed(KEY_THREE) && menu == 1) {
			menu = 4;
		}
		BeginDrawing();
		if (menu == 1) {
			ClearBackground(WHITE);
			DrawTextEx(gg, "Chon thuat toan mo phong", Vector2{ 0, 0 }, 30, 1, BLACK);
			DrawTextEx(gg, "1. FIFO\n2. LRU\n3. OPT", { 0,30 }, 26, 1, BLACK);
		}
		else if (menu == 2) {
			ClearBackground(WHITE);
			DrawTextEx(gg, "FIFO", { 0, 0 }, 30, 1, BLACK);
			DrawTextEx(gg, "Backspace de quay lai", { 0, 580 }, 20, 1, BLACK);
		}
		else if (menu == 3) {
			ClearBackground(WHITE);
			DrawTextEx(gg, "LRU", { 0, 0 }, 30, 1, BLACK);
			DrawTextEx(gg, "Backspace de quay lai", { 0, 580 }, 20, 1, BLACK);
		}
		else if (menu == 4) {
			ClearBackground(WHITE);
			DrawTextEx(gg, "OPT", { 0, 0 }, 30, 1, BLACK);
			DrawTextEx(gg, "Backspace de quay lai", { 0, 580 }, 20, 1, BLACK);
		}
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);
	UnloadFont(gg);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
