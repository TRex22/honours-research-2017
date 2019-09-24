/*Window class*/

class Window
{
private:


public:
	int hand_size = 0;
	int posX = 0;
	int posY = 0;
	int screen_width = 0;
	int screen_height = 0;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font *font = NULL;
	TTF_Font *font_small = NULL;
	TTF_Font *font_large = NULL;

	std::vector<SDL_Rect> rectangles;
	std::vector<SDL_Texture*> textures;
	std::unordered_map<std::string, SDL_Texture*> tex_map;

	// SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_RESIZABLE
	int WindowFlags = (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	Window(GameState gs, std::string window_name, int hand_size, int posX, int posY, int screen_width, int screen_height);
	~Window();

	void Render(GameState gs, int card_width, int card_height);
	void RenderView(Card card, int player_id);
	void render_text(std::string text, TTF_Font *font, int x, int y);
	void render_hand(std::vector<Card> deck, int player_id, int card_width, int card_height, int height);
	void Clear();
};

Window::Window(GameState gs, std::string window_name, int hand_size, int posX, int posY, int screen_width, int screen_height)
{
	this->hand_size = hand_size;
	this->posX = posX;
	this->posY = posY;
	this->screen_width = screen_width;
	this->screen_height = screen_height;

	this->window = SDL_CreateWindow(window_name.c_str(), posX, posY, screen_width, screen_height, WindowFlags);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

	// roboto font
	this->font = TTF_OpenFont("./bin/roboto.ttf", 24);
	if (font == NULL) {
		fprintf(stderr, "error: font not found\n");
		exit(EXIT_FAILURE);
	}
	this->font_small = TTF_OpenFont("./bin/roboto.ttf", 18);
	this->font_large = TTF_OpenFont("./bin/roboto.ttf", 35);

	// Assign textures
	SDL_Texture *bitmapTex = NULL;
	SDL_Surface *bitmapSurface = NULL;
	// Deck 1
	for (unsigned int i = 0; i < gs.northernDeck.size(); i++)
	{
		std::string name = get_hashable_card_name(gs.northernDeck[i], 1);
		// std::cout << name << std::endl;
		bitmapSurface = IMG_Load(gs.northernDeck[i].imageLocation.c_str());
		bitmapTex = SDL_CreateTextureFromSurface(this->renderer, bitmapSurface);

		tex_map[name] = bitmapTex;
		this->textures.push_back(bitmapTex);

		SDL_FreeSurface(bitmapSurface);
	}

	// Deck 2
	for (unsigned int i = 0; i < gs.nilfgaardDeck.size(); i++)
	{
		std::string name = get_hashable_card_name(gs.nilfgaardDeck[i], 2);

		bitmapSurface = IMG_Load(gs.nilfgaardDeck[i].imageLocation.c_str());
		bitmapTex = SDL_CreateTextureFromSurface(this->renderer, bitmapSurface);

		tex_map[name] = bitmapTex;
		this->textures.push_back(bitmapTex);

		SDL_FreeSurface(bitmapSurface);
	}
}

Window::~Window()
{
	rectangles.clear();
	for (unsigned int i = 0; i < this->textures.size(); i++)
	{
		SDL_DestroyTexture(textures[i]);
	}
	textures.clear();

	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void Window::Render(GameState gs, int card_width, int card_height)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(this->renderer);

	int current_height = 40;
	int spacer = 10;
	// print_deck(p1hand, "1");
	render_text("Player 1's Current Hand", this->font_small, (card_width * (hand_size / 2)) + spacer, 10);
	auto p1hand = get_unique_decks(gs.player1Info.getCurrentHand_ExcludingPass());
	// P1
	render_hand(p1hand, 1, card_width, card_height, current_height);

	// siege
	current_height += card_height + spacer;
	render_hand(gs.player1Info.getCurrentBoard()[location(siegeId)], 1, card_width, card_height, current_height);
	render_text("Siege", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	// ranged
	current_height += card_height + spacer;
	render_hand(gs.player1Info.getCurrentBoard()[location(rangedId)], 1, card_width, card_height, current_height);
	render_text("Ranged", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	// close
	current_height += card_height + spacer;
	render_hand(gs.player1Info.getCurrentBoard()[location(closeId)], 1, card_width, card_height, current_height);
	render_text("Close", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	// weather
	auto weather_deck = get_unique_decks(gs.weatherCards);
	current_height += card_height + spacer;
	render_hand(weather_deck, 1, card_width, card_height, current_height);
	render_text("Weather/Special", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	auto p2hand = get_unique_decks(gs.player2Info.getCurrentHand_ExcludingPass());
	// close
	current_height += card_height + spacer;
	render_hand(gs.player2Info.getCurrentBoard()[location(closeId)], 2, card_width, card_height, current_height);
	render_text("Close", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	// ranged
	current_height += card_height + spacer;
	render_hand(gs.player2Info.getCurrentBoard()[location(rangedId)], 2, card_width, card_height, current_height);
	render_text("Ranged", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	// siege
	current_height += card_height + spacer;
	render_hand(gs.player2Info.getCurrentBoard()[location(siegeId)], 2, card_width, card_height, current_height);
	render_text("Siege", this->font_small, (card_width * hand_size) + (spacer*2), current_height);

	//p2
	current_height += card_height + spacer;
	render_hand(p2hand, 2, card_width, card_height, current_height);
	render_text("Player 2's Current Hand", this->font_small, (card_width * hand_size) + (spacer*2) + 135, current_height + spacer + 40);

	// Selection Window
	render_text("Selected Card:", this->font, 900, 15);
	SDL_Rect selectionWindow;
	selectionWindow.w = 430;
	selectionWindow.h = 795;
	selectionWindow.x = 750;
	selectionWindow.y = 50;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(this->renderer, &selectionWindow);

	// Scores
	render_text("Scores", this->font_large, 1200, 50);
	render_text("Player 1:", this->font, 1200, 100);
	std::string apStr = "Attack Points: ";
	apStr.append(std::to_string(gs.player1Info.attackPoints.getCurrentAttackPoints()));
	render_text(apStr, this->font, 1210, 125);
	std::string rwStr = "Round Wins: ";
	rwStr.append(std::to_string(gs.player1Info.roundWins));
	render_text(rwStr, this->font, 1210, 150);

	render_text("Player 2:", this->font, 1200, 200);
	apStr = "Attack Points: ";
	apStr.append(std::to_string(gs.player2Info.attackPoints.getCurrentAttackPoints()));
	render_text(apStr, this->font, 1210, 225);
	rwStr = "Round Wins: ";
	rwStr.append(std::to_string(gs.player2Info.roundWins));
	render_text(rwStr, this->font, 1210, 250);


	std::string rndStr = "Current Player: Player ";
	rndStr.append(std::to_string(gs.currentPlayerId));
	render_text(rndStr, this->font_large, 1200, 400);
	rndStr = "Current Round: ";
	rndStr.append(std::to_string(gs.currentRound));
	render_text(rndStr, this->font_large, 1200, 450);

	if (gs.winPlayerId > -1)
	{
		if (gs.winPlayerId != 0)
		{
			rndStr = "Winner: Player ";
			rndStr.append(std::to_string(gs.winPlayerId));
		}
		else
		{
			rndStr = "It's a draw!";
		}
		render_text(rndStr, this->font_large, 1200, 500);
	}

	SDL_RenderPresent(this->renderer);
}

void Window::RenderView(Card card, int player_id)
{
	if (card.cardName != "Pass Round")
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_Rect cardRect;
		cardRect.w = 410;
		cardRect.h = 775;
		cardRect.x = 760;
		cardRect.y = 60;

		std::string name = get_hashable_card_name(card, player_id);
		auto bitmapTex = tex_map[name];

		SDL_RenderCopy(this->renderer, bitmapTex, NULL, &cardRect);
		SDL_RenderPresent(this->renderer);
	}
	else
	{
		render_text("Pass Round.", this->font_large, 870, 448);
		SDL_RenderPresent(this->renderer);
	}
}

void Window::render_text(std::string text, TTF_Font *font, int x, int y)
{
	SDL_Texture *texture = NULL;

	SDL_Rect rect;
	int text_width;
	int text_height;
	SDL_Surface *surface;
	SDL_Color textColor = {255, 255, 255, 255};
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	this->textures.push_back(texture);

	text_width = surface->w;
	text_height = surface->h;
	SDL_FreeSurface(surface);
	rect.x = x;
	rect.y = y;
	rect.w = text_width;
	rect.h = text_height;
	// SDL_RenderDrawRect(this->renderer, &rect);
	SDL_RenderCopy(this->renderer, texture, NULL, &rect);
}

void Window::render_hand(std::vector<Card> deck, int player_id, int card_width, int card_height, int height)
{
	for (unsigned int i = 0; i < deck.size(); i++)
	{
		std::string name = get_hashable_card_name(deck[i], player_id);
		auto bitmapTex = tex_map[name];

		SDL_Rect cardRect;
		cardRect.w = card_width;
		cardRect.h = card_height;
		cardRect.x = 0;
		cardRect.y = height;

		this->rectangles.push_back(cardRect);
		this->rectangles[i].x = i * card_width;
		this->rectangles[i].y = height;
		SDL_RenderCopy(this->renderer, bitmapTex, NULL, &this->rectangles[i]);
	}
}

void Window::Clear()
{
	rectangles.clear();
	SDL_Rect selectionWindow;
	selectionWindow.w = screen_width;
	selectionWindow.h = screen_height;
	selectionWindow.x = 0;
	selectionWindow.y = 0;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(this->renderer, &selectionWindow);
	SDL_RenderPresent(this->renderer);
}