#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include <limits>
#include <pthread.h>
#include <errno.h>
#include <algorithm>
#include "SDLauxiliary.h"
#include "TestModel.h"
#include "Intersection.h"
#include "Camera.h"
#include "Lighting.h"
#include "Triangle.h"
#define NTHREADS 8
#define SUPERSAMPLING 8
using namespace std;
using glm::vec3;
using glm::mat3;

/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */
//The screen size mod 8 should be 0
const int SCREEN_WIDTH = 504;
const int SCREEN_HEIGHT = 504;
vec3 buffer[SCREEN_WIDTH][SCREEN_HEIGHT];
vec3 progressEst[SCREEN_WIDTH][SCREEN_HEIGHT];
//the flag array for all wokers
int drawFlag[NTHREADS];
SDL_Surface* screen;
int t;
vector<Triangle> triangles;
//initialise camera object with giving position and focalLength
Camera camera(0.0, 0.0, -3.0, SCREEN_HEIGHT);
//initialise light object with giving position and light intensity
Lighting lighting(vec3( 0, -0.5, -0.7 ),14.f * vec3( 1, 1, 1 ));

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void Update();
void Draw(int workerID);
void drawBuffer();
/* this is the worker thread, it draw part of the scene based on its workerID into buffer */
void* worker(void* p) {
	int workerID = *(int *)p;
	Draw(workerID);
	drawFlag[workerID] = 1;//change the flag to 1 when finish drawing
	return NULL;
}
int main( int argc, char* argv[] ){
	for(int i = 0; i < NTHREADS; i++){
		drawFlag[i] = 0;//initialise all flag to 0
	}
	/*initialise buffer and timeEst array to 0*/
	for(int y = 0; y < SCREEN_HEIGHT; ++y){
		for(int x = 0; x < SCREEN_WIDTH; ++x){
			buffer[x][y] = vec3(0,0,0);
			progressEst[x][y] = vec3(0,0,0);
		}
	}
	screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT );
	t = SDL_GetTicks();	// Set start value for timer.
	LoadTestModel( triangles );
	pthread_t workerT[NTHREADS];
	int workerID[NTHREADS];//store the worker's ID
	/*create worker threads*/
	for (int i = 0; i<NTHREADS; i++) {
		workerID[i] = i;
		if(pthread_create(&workerT[i], NULL, worker, &workerID[i])){
			fprintf(stderr, "error creating thread.\n");
			exit(EXIT_FAILURE);
		}
	}
	//the following variables are used to calcualte the progress of rendering process
	int countPixel = 0;
	float pencentage = 0.f;
	float temp = 0.f;
	while( NoQuitMessageSDL() )
	{	//if the pixel at the same position has different value in these two buffer, it means the pixel has been rendered
		for(int y = 0; y < SCREEN_HEIGHT; ++y){
			for(int x = 0; x < SCREEN_WIDTH; ++x){
				if(buffer[x][y]!=progressEst[x][y]){
					countPixel+=1;
				}
			}
		}
		pencentage = (float)countPixel*100.f/(float)(SCREEN_WIDTH*SCREEN_HEIGHT);
		if(pencentage>temp){//only print the progress if there are progress
			cout<<"current progress:"<<pencentage<<"%"<<endl;
		}
		temp = pencentage;
		countPixel = 0;
		Update();
		drawBuffer();

	}

	SDL_SaveBMP( screen, "screenshot.bmp" );
	return 0;
}
/*since we are not able to call PutPixelSDL in worker threads,
we create this function to put the pixels from buffer to screen*/
void drawBuffer(){
	if(*min_element(drawFlag,drawFlag+NTHREADS)==1){//This function will execute only of all the workers finish their jobs
		if( SDL_MUSTLOCK(screen) )//lock before draw
		SDL_LockSurface(screen);
		for(int y = 0; y < SCREEN_HEIGHT; ++y){
			for(int x = 0; x < SCREEN_WIDTH; ++x){
				PutPixelSDL( screen, x, y, buffer[x][y]);
			}
		}
		if( SDL_MUSTLOCK(screen) )
		SDL_UnlockSurface(screen);//unlock after draw

		SDL_UpdateRect( screen, 0, 0, 0, 0 );

		int t2 = SDL_GetTicks();
		float dt = float(t2-t);
		t = t2;
		cout << "Render time: " << dt << " ms." << endl;

		drawFlag[0] = 0;
	}
}
void Update()
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	// t = t2;
	// cout << "Render time: " << dt << " ms." << endl;
	Uint8* keystate = SDL_GetKeyState( 0 );
	//camera transformation and light position transformation
	//since our program only render the scene 1 time, the following commands are disabled
	if( keystate[SDLK_UP] )
	{
		camera.translate(vec3(0,0,camera.moveSpeed*dt/1000.f));
	}
	if( keystate[SDLK_DOWN] )
	{
		camera.translate(vec3(0,0,-camera.moveSpeed*dt/1000.f));
	}
	if( keystate[SDLK_LEFT] )
	{
		camera.rotate(-camera.rotateSpeed*dt/1000.f);
	}
	if( keystate[SDLK_RIGHT] )
	{
		camera.rotate(camera.rotateSpeed*dt/1000.f);
	}
	if( keystate[SDLK_SPACE] )
	{
		camera.position = vec3(0,0,-3);//go back to original position
		camera.rotation = mat3(1);
		camera.yaw = 0;
		lighting.lightPos = vec3(0, -0.5, -0.7);
	}
	if( keystate[SDLK_w] ){
		lighting.lightPos += vec3(0,0,camera.moveSpeed*dt/1000.f)*camera.rotation;
	}
	if( keystate[SDLK_s] ){
		lighting.lightPos += vec3(0,0,-camera.moveSpeed*dt/1000.f)*camera.rotation;
	}
	if( keystate[SDLK_a] ){
		lighting.lightPos += vec3(-camera.moveSpeed*dt/1000.f,0,0)*camera.rotation;
	}
	if( keystate[SDLK_d] ){
		lighting.lightPos += vec3(camera.moveSpeed*dt/1000.f,0,0)*camera.rotation;
	}
	if( keystate[SDLK_q] ){
		lighting.lightPos += vec3(0,-camera.moveSpeed*dt/1000.f,0)*camera.rotation;
	}
	if( keystate[SDLK_e] ){
		lighting.lightPos += vec3(0,camera.moveSpeed*dt/1000.f,0)*camera.rotation;
	}
}
void Draw(int workerID)
{
	int height = SCREEN_HEIGHT/NTHREADS;


	Intersection intersect;
	/*super sampling*/
	int patchSize = SUPERSAMPLING;
	for( int y=workerID*height; y<(workerID+1)*height; ++y )//divided the screen into worker th parts
	{
		for( int x=0; x<SCREEN_WIDTH; ++x )
		{
			vec3 allColor(0,0,0);
			for(int i = 0; i < patchSize; i++){
				for(int j = 0; j < patchSize; j++){
					vec3 dir= camera.rotation * vec3(x-SCREEN_WIDTH/2.0+(float)i/patchSize, y-SCREEN_HEIGHT/2.0+(float)j/patchSize, camera.focalLength);
					if(Intersection::ClosestIntersection(camera.position,dir,triangles,intersect, -1)){//if find the Closest Intersection
						allColor += lighting.CombineColor(intersect,triangles, 10, 1);//get the colour of [i][j] part from the selected pixel
					}
				}
			}
			//pxiel colour is the average of the colour of the different parts from the pixel
			buffer[x][y] = allColor/(float)(patchSize*patchSize);

		}
	}
}
