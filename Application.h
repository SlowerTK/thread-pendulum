#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

constexpr double g = 9.81;

class Application
{
	enum buttons {
		nothing,
		pause,
		next,
		stop,
		start,
		aexit
	};

	struct Ball {
		double theta;
		double omega;
		double alpha;
		double len;
		double x;
		double y;
		double r;
		double mass;

		int px;
		int py;
		Ball() = default;
		Ball(double initial_theta, double length, double radius, double m, int px, int py)
			: theta(initial_theta), omega(0), alpha(0), len(length), x(0), y(0), r(radius), mass(m), px(px), py(py) {}
	};

	class Window
	{
	public:
		Window(std::string title, int x, int y, int w, int h);
		~Window();
		SDL_Window* getWindow() const { return this->window; };
		SDL_Renderer* getRenderer() const { return this->renderer; };

		int eventChecker();

		void Renderer(std::vector<Ball>& b, int pressedButton) const;
		void drawLeftMenu(std::vector<Ball>& b) const;
		void drawPlatform() const;
		void drawBall(std::vector<Ball>& b) const;
		void drawLineFromPlatformToBall(std::vector<Ball>& b) const;

		void moveBall(std::vector<Ball>& b, double dt, int pressedButton);
		void resolveCollision(std::vector<Ball>& b);

		void init(std::string title, int x, int y, int w, int h);
		int w, h;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event e;
		TTF_Font* font;
	};
public:
	Application(std::string title, int x, int y, int w, int h) { Run(title, x, y, w, h); };
private:
	void Run(std::string title, int x, int y, int w, int h) {
		Window window(title, x, y, w, h);
		std::vector<Ball> balls(2);
		balls[0] = {0, 234, 20, 1, 2 * w / 3 - 35, h / 4};
		balls[1] = {M_PI/2, 234, 30, 1000, 2 * w / 3 - 35, h / 4};
		int pressedButton = nothing;
		double dt = 0.1;
		while ((pressedButton = window.eventChecker()) != aexit) {
			window.Renderer(balls, pressedButton);
			window.moveBall(balls, dt, pressedButton);
			SDL_Delay(static_cast<int>(dt));
		}
	};
};