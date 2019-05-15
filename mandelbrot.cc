#include <SDL2/SDL.h>
#include <cassert>
#include <algorithm>
#include <complex>

constexpr int height = 600;
constexpr int width  = 1000;

SDL_Window * window     = nullptr;
SDL_Renderer * renderer = nullptr;
SDL_Event event;

bool isrunning = true;

void quit() { isrunning = false; }

double map(int value, int start1, int stop1, int start2, int stop2) {
	if(start1 > stop1) std::swap(start1, stop1);
	if(start2 > stop2) std::swap(start2, stop2);
	return 1.0 * (value - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}

int color(std::complex<double>& c, int& iteration) {
	int i = 0;
	std::complex<double> z(0.0f, 0.0f);
	for(;i < iteration; ++i) {
		z = z * z + c;
		if(std::abs(z) >= 2) break;
	}
	return i;
}

int color(double& cr, double& ci, int& iteration) {
	int i = 0;
	double zr = 0.0f, zi = 0.0f, temp = 0.0f;
	for(;i < iteration; ++i) {
		temp = zr * zr - zi * zi + cr;
		zi = 2 * zr * zi + ci;
		zr = temp;
		if(zr * zr + zi * zi >= 4) break;
	}
	return i;
}

int main(int argc, char ** argv) {

	SDL_Init(SDL_INIT_VIDEO);
	if(SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		assert(window);
		assert(renderer);
		return 1;
	}
	
	SDL_SetWindowTitle(window, "SDL_RenderDrawPoint");

	int r = 0, g = 0, b = 0;
	int iteration = 100, n = 0;

	while(isrunning) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: quit(); break;
				case SDL_KEYDOWN: 
					switch(event.key.keysym.sym) case SDLK_q: quit(); break;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0x0, 0xff, 0x0, 0xff);
		SDL_RenderClear(renderer);

		for(int y = 0; y < height; ++y) {
			for(int x = 0; x < width; ++x) {
				std::complex<double> c(map(x, 0, width,  -2.0f, 1.0f), map(y, 0, height, -1.0f, 1.0f));
				n = color(c, iteration);
				r = g = b = int(n * sinf(n)) % 256;
				SDL_SetRenderDrawColor(renderer, r, g, b, 0xff);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
		
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}