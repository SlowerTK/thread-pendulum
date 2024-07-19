#include "Application.h"



Application::Window::Window(std::string title, int x, int y, int w, int h) {
	this->w = w;
	this->h = h;
	init(title, x, y, w, h);
}
Application::Window::~Window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}
void Application::Window::init(std::string title, int x, int y, int w, int h) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("SDL_Init Error: %s", SDL_GetError());
		return;
	}
	window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_FOREIGN);
	if (window == nullptr) {
		SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
		SDL_Quit();
		return;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == nullptr) {
		SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
		SDL_Quit();
		return;
	}
	e.type = SDL_FIRSTEVENT;
	if (TTF_Init() != 0) {
		SDL_Log("TTF_Init Error: %s", TTF_GetError());
		SDL_Quit();
		return;
	}
	font = TTF_OpenFont(".\\Softie Cyr.ttf", 32);
	if (font == nullptr) {
		SDL_Log("TTF_OpenFont Error: %s", TTF_GetError());
		SDL_Quit();
		return;
	}
}

int Application::Window::eventChecker() {
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			return aexit;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				if (e.button.x > w / 24 && e.button.x < w / 24 + w / 6 && e.button.y > h / 16 && e.button.y < h / 16 + h / 16) {
					SDL_Log("Pause");
					return pause;
				}
				if (e.button.x > w / 24 && e.button.x < w / 24 + w / 6 && e.button.y > h / 16 * 2 + 10 && e.button.y < h / 16 * 2 + 10 + h / 16) {
					SDL_Log("Restart");
					return restart;
				}
				if (e.button.x > w / 24 && e.button.x < w / 24 + w / 6 && e.button.y > h / 16 * 3 + 20 && e.button.y < h / 16 * 3 + 20 + h / 16) {
					SDL_Log("Info");
					return info;
				}
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				int x = e.button.x;
				int y = e.button.y;

				if (y >= h / 2 - 50 - 15 && y <= h / 2 - 50 + 15) {
					if (x >= 10 && x <= 10 + (w / 4 - 20)) {
						return(SLIDER1_MIN + (x - 10) * (SLIDER1_MAX - SLIDER1_MIN) / (w / 4 - 20));
					}
				}
				if (y >= h / 2 + 50 - 15 && y <= h / 2 + 50 + 15) {
					if (x >= 10 && x <= 10 + (w / 4 - 20)) {
						return(SLIDER1_MIN + (x - 10) * (SLIDER1_MAX - SLIDER1_MIN) / (w / 4 - 20) + 100);
					}
				}
				if (y >= h / 2 + 150 - 15 && y <= h / 2 + 150 + 15) {
					if (x >= 10 && x <= 10 + (w / 4 - 20)) {
						return(SLIDER2_MIN + (x - 10) * (SLIDER2_MAX - SLIDER2_MIN) / (w / 4 - 20) + 201);
					}
				}
				if (y >= h / 2 + 250 - 15 && y <= h / 2 + 250 + 15) {
					if (x >= 10 && x <= 10 + (w / 4 - 20)) {
						return(SLIDER2_MIN + (x - 10) * (SLIDER2_MAX - SLIDER2_MIN) / (w / 4 - 20) + 562);
					}
				}
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			int x, y;
			SDL_GetMouseState(&x, &y);

			if (y >= h / 2 - 50 - 15 && y <= h / 2 - 50 + 15) {
				if (e.wheel.y > 0)
					return incMass1;
				else if (e.wheel.y < 0)
					return decMass1;
			}
			if (y >= h / 2 + 50 - 15 && y <= h / 2 + 50 + 15) {
				if (e.wheel.y > 0)
					return incMass2;
				else if (e.wheel.y < 0)
					return decMass2;
			}
			if (y >= h / 2 + 150 - 15 && y <= h / 2 + 150 + 15) {
				if (e.wheel.y > 0)
					return incAngle1;
				else if (e.wheel.y < 0)
					return decAngle1;
			}
			if (y >= h / 2 + 250 - 15 && y <= h / 2 + 250 + 15) {
				if (e.wheel.y > 0)
					return incAngle2;
				else if (e.wheel.y < 0)
					return decAngle2;
			}
		}
		if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_SPACE) {
				SDL_Log("Pause");
				return pause;
			}
			if (e.key.keysym.sym == SDLK_RIGHT) {
				SDL_Log("Next");
				return next;
			}
		}
	}
	return nothing;
}

void Application::Window::Renderer(std::vector<Ball>& b, int pressedButton) const {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	drawLeftMenu(b, pressedButton);
	drawPlatform();
	drawBall(b);
	drawLineFromPlatformToBall(b);
	SDL_RenderPresent(renderer);
}

void Application::Window::drawLeftMenu(std::vector<Ball>& b, int pressedButton) const {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect leftMenu = { 0, 0, w / 4, h };
	SDL_RenderFillRect(renderer, &leftMenu);

	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_Rect stopButton = { w / 24, h / 16, w / 6, h / 16 };
	SDL_RenderFillRect(renderer, &stopButton);
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, "Pause", color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect textRect = { stopButton.x + w / 32, stopButton.y + h / 32 - surface->h / 2, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	SDL_Rect restartButton = { w / 24, h / 16 * 2 + 10, w / 6, h / 16 };
	SDL_RenderFillRect(renderer, &restartButton);
	surface = TTF_RenderText_Solid(font, "Restart", color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	textRect = { restartButton.x + w / 64, restartButton.y + h / 32 - surface->h / 2, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	SDL_Rect infoButton = { w / 24, h / 16 * 3 + 20, w / 6, h / 16 };
	SDL_RenderFillRect(renderer, &infoButton);
	surface = TTF_RenderText_Solid(font, "Info", color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	textRect = { infoButton.x + w / 24, infoButton.y + h / 32 - surface->h / 2, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	static bool isInfo = false;
	if (pressedButton == info) {
		isInfo = !isInfo;
	}
	if (isInfo) {
		drawInfo(b);
	}
	else {
		drawControls(b);
	}
}

void Application::Window::drawInfo(std::vector<Ball>& b) const {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, 0, h / 16 * 5, w / 4 - 1, h / 16 * 5);
	TTF_SetFontSize(font, 14);
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_Color color2 = { 0, 0, 0, 255 };
	int ballCount = 1;
	int i = 0;
	for (auto& ball : b) {
		SDL_Rect ballInfo = { 0, h / 16 * 5 + 1 + i, w / 4, h / 16 };
		SDL_RenderFillRect(renderer, &ballInfo);
		std::string ballInfoText = "Ball " + std::to_string(ballCount);
		SDL_Surface* surface = TTF_RenderText_Solid(font, ballInfoText.c_str(), color2);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_Rect textRect = { ballInfo.x + w / 64, ballInfo.y + h / 32 - surface->h / 2, surface->w, surface->h };
		SDL_RenderCopy(renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		ballInfo = { 0, h / 16 * 5 + h / 16 + 1 + i, w / 4, h / 16 };
		SDL_RenderFillRect(renderer, &ballInfo);
		ballInfoText = "Mass: " + std::to_string(ball.mass);
		surface = TTF_RenderText_Solid(font, ballInfoText.c_str(), color2);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		textRect = { ballInfo.x + w / 24, ballInfo.y + h / 32 - surface->h / 2, surface->w, surface->h };
		SDL_RenderCopy(renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		ballInfo.y += h / 16;
		SDL_RenderFillRect(renderer, &ballInfo);
		double speed = /*RadToDeg*/(ball.omega);
		ballInfoText = "Speed: " + std::to_string(abs(speed));
		surface = TTF_RenderText_Solid(font, ballInfoText.c_str(), color2);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		textRect = { ballInfo.x + w / 24, ballInfo.y + h / 32 - surface->h / 2, surface->w, surface->h };
		SDL_RenderCopy(renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		ballInfo.y += h / 16;
		SDL_RenderFillRect(renderer, &ballInfo);
		double angle = RadToDeg(ball.theta);
		while (angle > 360) { angle -= 360; }
		while (angle < -360) { angle += 360; }
		ballInfoText = "Angle: " + std::to_string(angle);
		surface = TTF_RenderText_Solid(font, ballInfoText.c_str(), color2);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		textRect = { ballInfo.x + w / 24, ballInfo.y + h / 32 - surface->h / 2, surface->w, surface->h };
		SDL_RenderCopy(renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		ballInfo.y += h / 16;
		SDL_RenderFillRect(renderer, &ballInfo);
		double acceleration = ball.alpha * ball.r;
		ballInfoText = "Linear Acc: " + std::to_string(acceleration);
		surface = TTF_RenderText_Solid(font, ballInfoText.c_str(), color2);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		textRect = { ballInfo.x + w / 32, ballInfo.y + h / 32 - surface->h / 2, surface->w, surface->h };
		SDL_RenderCopy(renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		++ballCount;
		i += 5 * h / 16;
	}
	TTF_SetFontSize(font, 32);
}

void Application::Window::drawControls(std::vector<Ball>& b) const {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, 0, h / 16 * 5, w / 4 - 1, h / 16 * 5);
	int i = 0;
	int BallCount = 1;
	for (auto& ball : b) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_Rect track = { 10, (h / 2 - 50) + i, (w / 4 - 20), SLIDER_HEIGHT };
		SDL_RenderFillRect(renderer, &track);

		SDL_Rect handle = { 10 + static_cast<int>(ball.mass * (w / 4 - 20) / (SLIDER1_MAX - SLIDER1_MIN)) - 5, (h / 2 - 50) - 13 + i, 10, 30 };
		SDL_RenderFillRect(renderer, &handle);

		SDL_Color textColor = { 0, 0, 0, 255 };
		std::string text = "Mass " + std::to_string(BallCount) + ": " + std::to_string(static_cast<int>(ball.mass));
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		int textWidth = static_cast<int>(textSurface->w / 1.5);
		int textHeight = static_cast<int>(textSurface->h / 1.5);

		SDL_Rect renderQuad = { 10, h / 3 + i, textWidth, textHeight };
		SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		++BallCount;
		i += 100;
	}
	BallCount = 1;
	for (auto& ball : b) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_Rect track = { 10, (h / 2 - 50) + i, (w / 4 - 20), SLIDER_HEIGHT };
		SDL_RenderFillRect(renderer, &track);

		double angle = RadToDeg(ball.theta);
		while (angle > 360) { angle -= 360; }
		while (angle < 0) { angle += 360; }
		SDL_Rect handle = { 10 + static_cast<int>(angle * (w / 4 - 20) / (SLIDER2_MAX - SLIDER2_MIN)) - 5, (h / 2 - 50) - 13 + i, 10, 30 };
		SDL_RenderFillRect(renderer, &handle);

		SDL_Color textColor = { 0, 0, 0, 255 };
		std::string text = "Angle " + std::to_string(BallCount) + ": " + std::to_string(static_cast<int>(angle));
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		int textWidth = static_cast<int>(textSurface->w / 1.5);
		int textHeight = static_cast<int>(textSurface->h / 1.5);

		SDL_Rect renderQuad = { 10, h / 3 + i, textWidth, textHeight };
		SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
		++BallCount;
		i += 100;
	}
}

void Application::Window::drawPlatform() const {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect platform = { w * 6 / 12, h / 6, w / 4, h / 24 };
	SDL_RenderFillRect(renderer, &platform);
}

void Application::Window::drawBall(std::vector<Ball>& b) const {
	for (auto& ball : b) {
		const int diameter = static_cast<int>(ball.r * 2);
		int x = static_cast<int>(ball.r - 1);
		int y = 0;
		int tx = 1;
		int ty = 1;
		int error = (tx - diameter);

		while (x >= y) {
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x + x), static_cast<int>(ball.y - y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x + x), static_cast<int>(ball.y + y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x - x), static_cast<int>(ball.y - y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x - x), static_cast<int>(ball.y + y));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x + y), static_cast<int>(ball.y - x));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x + y), static_cast<int>(ball.y + x));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x - y), static_cast<int>(ball.y - x));
			SDL_RenderDrawPoint(renderer, static_cast<int>(ball.x - y), static_cast<int>(ball.y + x));

			if (error <= 0) {
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0) {
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
	}
}

void Application::Window::drawLineFromPlatformToBall(std::vector<Ball>& b) const {
	for (auto& ball : b) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, ball.px, ball.py, static_cast<int>(ball.x), static_cast<int>(ball.y));
	}
}

void Application::Window::moveBall(std::vector<Ball>& b, double dt, int pressedButton) {
	static double startPoint0 = b[0].theta;
	static double startX0 = b[0].px + b[0].len * sin(startPoint0);
	static double startY0 = b[0].py + b[0].len * cos(startPoint0);
	static double startPoint1 = b[1].theta;
	static double startX1 = b[1].px + b[1].len * sin(startPoint1);
	static double startY1 = b[1].py + b[1].len * cos(startPoint1);
	if (pressedButton == restart) {
		b[0].theta = startPoint0;
		b[0].alpha = 0;
		b[0].omega = 0;
		b[0].x = startX0;
		b[0].y = startY0;
		b[1].theta = startPoint1;
		b[1].alpha = 0;
		b[1].omega = 0;
		b[1].x = startX1;
		b[1].y = startY1;
	}
	static bool moving = true;
	if (pressedButton == pause)
		moving = !moving;
	if (moving || pressedButton == next)
		for (auto& ball : b) {
			ball.alpha = -(g / ball.len) * sin(ball.theta);
			ball.omega += ball.alpha * dt;
			ball.theta += ball.omega * dt;
		}
	for (auto& ball : b) {
		ball.x = ball.px + ball.len * sin(ball.theta);
		ball.y = ball.py + ball.len * cos(ball.theta);
	}
	resolveCollision(b);
	changeRadiusFromMass(b);
}

void Application::Window::resolveCollision(std::vector<Ball>& b) {
	Ball ball1 = b[0];
	Ball ball2 = b[1];
	double dist = std::sqrt((ball1.x - ball2.x) * (ball1.x - ball2.x) + (ball1.y - ball2.y) * (ball1.y - ball2.y));
	double overlap = ball1.r + ball2.r - dist;
	if (overlap > 0) {
		double angle = std::atan2(ball2.y - ball1.y, ball2.x - ball1.x);
		double sine = std::sin(angle);
		double cosine = std::cos(angle);

		double vx1 = ball1.omega * ball1.len * cosine;
		double vy1 = ball1.omega * ball1.len * sine;
		double vx2 = ball2.omega * ball2.len * cosine;
		double vy2 = ball2.omega * ball2.len * sine;

		double vx1Final = (vx1 * (ball1.mass - ball2.mass) + 2 * ball2.mass * vx2) / (ball1.mass + ball2.mass);
		double vx2Final = (vx2 * (ball2.mass - ball1.mass) + 2 * ball1.mass * vx1) / (ball1.mass + ball2.mass);

		ball1.omega = (vx1Final * cosine - vy1 * sine) / ball1.len;
		ball2.omega = (vx2Final * cosine - vy2 * sine) / ball2.len;


		double separation = 0.5 * (overlap + 0.01);
		ball1.x -= separation * cosine;
		ball1.y -= separation * sine;
		ball2.x += separation * cosine;
		ball2.y += separation * sine;

		b[0] = ball1;
		b[1] = ball2;
	}
}



void Application::Window::changeRadiusFromMass(std::vector<Ball>& b) {
	for (auto& ball : b)
		if (ball.mass <= 15) ball.r = 15;
		else ball.r = 15 + (ball.mass - 15) * (65 - 15) / (100 - 15);
}
