#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#define DegToRad(x) ((x) * M_PI / 180)
#define RadToDeg(x) ((x) * 180 / M_PI)

constexpr double g = 9.81;
const int SLIDER_HEIGHT = 10;
const int SLIDER1_MIN = 1;
const int SLIDER1_MAX = 100;
const int SLIDER2_MIN = 0;
const int SLIDER2_MAX = 359;

class Application
{
	enum buttons {
		nothing = 922,
		pause,
		next,
		stop,
		restart,
		info,
		incMass1,
		decMass1,
		incMass2,
		decMass2,
		incAngle1,
		decAngle1,
		incAngle2,
		decAngle2,
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
		Ball(double initial_theta, double length, double m, int px, int py)
			: theta(initial_theta), omega(0), alpha(0), len(length), x(0), y(0), r(0), mass(m), px(px), py(py) {}
	};

	class Window {
	public:
		Window(std::string title, int x, int y, int w, int h);
		void init(std::string title, int x, int y, int w, int h);
		~Window();
		int eventChecker();

		void Renderer(std::vector<Ball>& b, int pressedButton) const;
		void drawLeftMenu(std::vector<Ball>& b, int pressedButton) const;
		void drawInfo(std::vector<Ball>& b) const;
		void drawControls(std::vector<Ball>& b) const;
		void drawPlatform() const;
		void drawBall(std::vector<Ball>& b) const;
		void drawLineFromPlatformToBall(std::vector<Ball>& b) const;

		void moveBall(std::vector<Ball>& b, double dt, int pressedButton);
		void resolveCollision(std::vector<Ball>& b);
		void changeRadiusFromMass(std::vector<Ball>& b);

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
		balls[0] = { 0, 234, 50, 2 * w / 3 - 35, h / 6 + h / 24 };
		balls[1] = { M_PI / 2, 234, 50, 2 * w / 3 - 35, h / 6 + h / 24 };
		int pressedButton = nothing;
		double dt = 0.1;
		while ((pressedButton = window.eventChecker()) != aexit) {
			if (pressedButton < nothing)
				if (pressedButton < 101) balls[0].mass = pressedButton;
				else if (pressedButton < 201) balls[1].mass = pressedButton - 100;
				else if (pressedButton < 562) balls[0].theta = DegToRad(pressedButton - 201);
				else balls[1].theta = DegToRad(pressedButton - 562);
			if (pressedButton == incMass1) balls[0].mass = (std::min(static_cast<int>(balls[0].mass) + 1, SLIDER1_MAX));
			else if (pressedButton == decMass1) balls[0].mass = (std::max(static_cast<int>(balls[0].mass) - 1, SLIDER1_MIN));
			else if (pressedButton == incMass2) balls[1].mass = (std::min(static_cast<int>(balls[1].mass) + 1, SLIDER1_MAX));
			else if (pressedButton == decMass2) balls[1].mass = (std::max(static_cast<int>(balls[1].mass) - 1, SLIDER1_MIN));
			else if (pressedButton == incAngle1) balls[0].theta = (DegToRad(RadToDeg(balls[0].theta) + 1));
			else if (pressedButton == decAngle1) balls[0].theta = (DegToRad(RadToDeg(balls[0].theta) - 1));
			else if (pressedButton == incAngle2) balls[1].theta = (DegToRad(RadToDeg(balls[1].theta) + 1));
			else if (pressedButton == decAngle2) balls[1].theta = (DegToRad(RadToDeg(balls[1].theta) - 1));
			SDL_Log("%d", pressedButton);
			window.Renderer(balls, pressedButton);
			window.moveBall(balls, dt, pressedButton);
			SDL_Delay(static_cast<int>(dt));
		}
	};
};