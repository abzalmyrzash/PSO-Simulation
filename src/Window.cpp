#include "Window.hpp"

Window::Window(const char* p_title, float p_wPer, float p_hPer)
	: window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, SDL_WINDOW_ALLOW_HIGHDPI);
	getResolution(&winW, &winH);
	// std::cout << winW << " " << winH << std::endl;
	winW = round(winW * p_wPer / 100);
	winH = round(winH * p_hPer / 100);
	SDL_SetWindowSize(window, winW, winH);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	mapBox.h = round(winH * 0.8);
	mapBox.w = Map::MAP_WIDTH * mapBox.h / Map::MAP_HEIGHT;
	mapBox.x = round(winH * 0.1);
	mapBox.y = round(winH * 0.1);
	// std::cout << mapBox.w << " " << mapBox.h << std::endl;

	if (window == NULL) {
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}
	SDL_DisplayMode DM;
	SDL_GetDisplayMode(getDisplayIndex(), 0, &DM);
	SDL_SetWindowDisplayMode(window, &DM);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// SDL_RenderSetLogicalSize(renderer, winW, winH);
	SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

	timeLabel = Label(renderer, mapBox.w + mapBox.x + 50, mapBox.y);
	SDL_Color textColor = {0, 0, 0, 255};
	timeLabel.loadFont("res/fonts/roboto/Roboto-Regular.ttf", 20, textColor);
}

SDL_Texture* Window::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;
	return texture;
}

void Window::loadTextures()
{
	robotTex = loadTexture("res/gfx/robot32_simple.png");
	sourceTex = loadTexture("res/gfx/source32.png");
}

SDL_Texture* Window::getSourceTex()
{
	return sourceTex;
}

SDL_Texture* Window::getRobotTex()
{
	return robotTex;
}

int Window::getDisplayIndex()
{
	return SDL_GetWindowDisplayIndex(window);
}

int Window::getRefreshRate()
{
	SDL_DisplayMode mode;
	SDL_GetDisplayMode(getDisplayIndex(), 0, &mode);
	return mode.refresh_rate;
}

void Window::getResolution(int* w, int* h)
{
	SDL_DisplayMode DM;
	SDL_GetDesktopDisplayMode(getDisplayIndex(), &DM);
	*w = DM.w;
	*h = DM.h;
}

void Window::getDPI(float* ddpi, float* hdpi, float* vdpi)
{
	SDL_GetDisplayDPI(getDisplayIndex(), ddpi, hdpi, vdpi);
}

void Window::cleanUp()
{
	SDL_DestroyWindow(window);
}

void Window::clear() {
	SDL_SetRenderDrawColor(renderer, bgr_R, bgr_G, bgr_B, bgr_A);
	SDL_RenderClear(renderer);
}

Vector Window::rescaleLoc(Vector loc){
	Vector loc2 = Vector(0, 0);
	loc2.x = round(loc.x * mapBox.w / Map::MAP_WIDTH) + mapBox.x;
	loc2.y = round((Map::MAP_HEIGHT - loc.y) * mapBox.h / Map::MAP_HEIGHT) + mapBox.y;
	return loc2;
}

void Window::render(Entity* p_entity) {
	SDL_Rect src = p_entity->getCurrentFrame();
	SDL_Texture* tex = p_entity->getTex();
	Vector loc = p_entity->getLoc();
	loc = rescaleLoc(loc);

	SDL_Rect dst;
	dst.w = round(src.w * RESIZE_COEFF);
	dst.h = round(src.h * RESIZE_COEFF);
	dst.x = loc.x - dst.w/2;
	dst.y = loc.y - dst.h/2;

	SDL_RenderCopyEx(renderer, tex, &src, &dst, -p_entity->getAngle() * 180 / M_PI, NULL, SDL_FLIP_NONE);
}

void Window::renderMap(Map* map) {
    // Set render color to blue ( rect will be rendered in this color )
    SDL_SetRenderDrawColor(renderer, map_R, map_G, map_B, map_A);
    SDL_RenderFillRect(renderer, &mapBox);
    SDL_SetRenderDrawColor(renderer, mapOutl_R, mapOutl_G, mapOutl_B, mapOutl_A);
    SDL_RenderDrawRect(renderer, &mapBox);

	for (auto src : map->sources) {
		render(src);
	}
	for (auto rbt : map->robots) {
		render(rbt);
	}
}

void Window::display()
{
	SDL_RenderPresent(renderer);
}

void Window::updateTimeLabel(double seconds, bool precise)
{
	int minutes = seconds/60;
	seconds = fmod(seconds, 60);
	std::string text = utils::format("Time: %02d:%02.f", minutes, floor(seconds));
	if(precise) text = utils::format("Time: %02d:%06.3f", minutes, seconds);
	timeLabel.updateText(text);
}

void Window::renderTimeLabel()
{
	SDL_Texture* tex = timeLabel.getTexture();
	SDL_Rect src = timeLabel.getSrcRect();
	SDL_Rect dst = timeLabel.getDstRect();

	SDL_RenderCopy(renderer, tex, &src, &dst);
}

void Window::renderBestIntensityLoc(Robot *r){
	// draw best local location
	SDL_Color color = {255, 0, 0, 255};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	Vector bestLoc = rescaleLoc(r->bestLoc);
	utils::DrawCircle(renderer, bestLoc.x, bestLoc.y, 3);

	// draw best global location
	color = {0, 255, 0, 255};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	Vector bestGlobalLoc = rescaleLoc(r->bestGlobalLoc);
	utils::DrawCircle(renderer, bestGlobalLoc.x, bestGlobalLoc.y, 7);
}

void Window::renderCommRange(Robot *r){
	SDL_Color color = {0, 0, 255, 255};
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	Vector loc = rescaleLoc(r->loc);
	utils::DrawCircle(renderer, loc.x, loc.y, r->commRange * mapBox.w / Map::MAP_WIDTH);
}