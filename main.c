#include "raylib.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include<stdlib.h>
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 500
#define SCALE_FACTOR 100.0
Color *colorMap = NULL;
uint8_t *heightMap = NULL;
typedef struct{
	float x;
	float y;
	float height;
	float angle;
	float horizon;
	float zfar;
} camera_t;
camera_t camera = {
	.x=512.0,
	.y=512.0,
	.height = 150.0,
	.horizon=100.0,
	.angle = 1.5*3.141592,
	.zfar=600.0
};
void processinput() {
	bool isMovingBackward = IsKeyDown(KEY_DOWN);

	// Handle rotation with reversed controls when moving backward
	if (IsKeyDown(KEY_LEFT)) {
		camera.angle -=   (isMovingBackward ? -0.05 : 0.05);
	}
	if (IsKeyDown(KEY_RIGHT)) {
		camera.angle +=   (isMovingBackward ? -0.05 : 0.05);
	}
	if (IsKeyDown(KEY_UP)) {
		camera.x+=2*cosf(camera.angle);
		camera.y-=2*sinf(camera.angle);
	}
	if (IsKeyDown(KEY_DOWN)) {
		camera.x-=2*cosf(camera.angle);
		camera.y+=2*sinf(camera.angle);
	}
	if (IsKeyDown(KEY_E)) {
		camera.height+=5;
	}
	if (IsKeyDown(KEY_D)) {
		camera.height-=5;
	}
	if (IsKeyDown(KEY_Q)) {
		camera.horizon+=10.5;
	}
	if (IsKeyDown(KEY_W)) {
		camera.horizon-=10.5;
	}
}
int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GIF Palette Example");
	SetTargetFPS(60);
	Image colormapImage = LoadImage("map0.color.gif");
	if (!colormapImage.data) {
		printf("Failed to load colormap GIF.\n");
		CloseWindow();
		return -1;
	}
	Image heightmapImage = LoadImage("map0.height.gif");
	if (!heightmapImage.data) {
		printf("Failed to load heightmap GIF.\n");
		UnloadImage(colormapImage);
		CloseWindow();
		return -1;
	}
	colorMap = (Color *)malloc(1024 * 1024 * sizeof(Color));
	heightMap = (uint8_t *)malloc(1024 * 1024 * sizeof(uint8_t));
	Color *colorPixels = (Color *)colormapImage.data;
	Color *heightPixels = (Color* )heightmapImage.data;
	for (int i = 0; i < 1024 * 1024; i++) {
		colorMap[i] = colorPixels[i];
		heightMap[i] = heightPixels[i].r;  // Use red channel for height
	}
	UnloadImage(colormapImage);
	UnloadImage(heightmapImage);
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		processinput();
		float sinangle=sinf(camera.angle);
		float cosangle=cosf(camera.angle);
		float plx=  cosangle*camera.zfar + sinangle*camera.zfar;
		float ply=  sinangle*camera.zfar - cosangle*camera.zfar;
		float prx=  cosangle*camera.zfar - sinangle*camera.zfar;
		float pry=  sinangle*camera.zfar + cosangle*camera.zfar;
		for (int i =0; i< SCREEN_WIDTH;i++) {
			float delta_x= (plx+(prx-plx) /SCREEN_WIDTH*i)/camera.zfar;
			float delta_y= (ply+(pry-ply) /SCREEN_WIDTH*i)/camera.zfar;
			float rx=camera.x;
			float ry=camera.y;
			float max_height = SCREEN_HEIGHT;
			for (int z=1;z<camera.zfar;z++){
				rx+=delta_x;
				ry-=delta_y;
				int mapoffset = ((1024* ((int)(ry)&1023)) + ((int)(rx) & 1023)); 
				int proj_height=(int)((camera.height-heightMap[mapoffset])/z*SCALE_FACTOR+camera.horizon);
				if (proj_height < 0) {
					proj_height=0;
				}
				if (proj_height>SCREEN_HEIGHT) {
					proj_height=SCREEN_HEIGHT-1;
				}
				if (proj_height < max_height) {
					for (int y = proj_height; y< max_height;y++) {
						Color pixel = colorMap[mapoffset];
						DrawPixel(i, y, colorMap[mapoffset]);
					}
					max_height=proj_height;
				}
				// DrawPixel(rx,ry,RED);
			}
		}
		EndDrawing();
	}
	free(colorMap);    // Add cleanup for allocated memory
	free(heightMap);
	CloseWindow();
	return 0;
}

