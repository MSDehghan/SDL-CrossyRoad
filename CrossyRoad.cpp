#include "Genio.h"
#include <cstdlib>
#include <ctime>
#include <deque>

enum TileType { GRASS, WATER, ROAD, RAIL, TREE };
enum ObjectType { PLAYER, CAR, TRAIN, STICK, LAMP, EAGLE, COIN };
enum GameState { START, PLAY, EXITE, PAUSE, GAME_OVER, OUT, CHOOSE_PLAYER };
enum Direction { UP, DOWN, LEFT, RIGHT };

struct Object {
	G_Rect position;
	G_Texture* texture;
	ObjectType type;
	SDL_Point tile;
	bool isMoving = false;
	Direction dir;
	G_Sound* sound = NULL;
	union { //All of Objects Need MoveSpeed Except Train. The Speed of Train Should Be Constant. Instead of Speed , it has a Timer to show when it should come!!!
		int moveSpeed;
		int timer;
	};
};

struct Tile {
	TileType type;
	G_Rect position;
	G_Texture* texture;
};

//For Fonts , Buttons , logo , ... !
struct Element {
	G_Rect position;
	G_Texture* texture;
};

//Constants
const int TILE_LENGTH = 100;
const int playerMoveSpeed = 10;
const int maxTreesInARow = 2; //Choose it carefully! if it be great enough , Player stuck in trees!!!!
const int maxSticksInARow = 5;
const int minSticksInARow = 2;
const int maxCoinsInARow = 2;
const int FPS = 60;
const int delay = 1000 / FPS;
const int cameraBaseSpeed = 1;
int trainMoveSpeed = 18;

//Variables
int startLoop, endLoop;
int cameraSpeed = cameraBaseSpeed;
int gameEvent;
int rows, columns;
int maxScore = 0, score = 0, topScore = 0, coins = 0;
char chars[10] = ""; //Used in updateScore();
bool eagleIntersection = false;

deque<Object> objects;

Tile** map = NULL;
GameState state = START;
G_Rect windowPos;
G_Rect realPlayerClip = { 30,0,40,100 };
Object Player;
Tile grassTile, treeTile, waterTile, roadTile, railTile;
Object Car, Stick, Train, Lamp, Eagle, Coin;
G_Texture* car2Texture;
G_Texture* car3Texture;
G_Texture* redLampTexture;
G_Texture* cowTexture;
G_Texture* sheepTexture;
G_Texture* pigTexture;
Element ScoreText, Logo, TryAgainButton, ClickButton, GameOverText, PauseButton, PlayerChooseButton, PlayerButton, CoinText, TopScoreText;
G_Font* font;
G_Sound* clickSound;

//Function Portotypes
void load();
void initTiles();
void drawTiles();
void drawObjects();
void update();
void checkPlayerStatus();
void adjustCameraSpeed();
void generateTiles(int);
void addObjects(int);
void addCar(int);
void addStick(int);
void addTrain(int);
void addCoins(int);
void deleteObjects();
void draw();
void start();
void choose_player();
void play();
void updateScore();
void pause();
void eagle();
void game_over();
void destroyTiles();
bool clickOnButton(G_Rect*);
void destroy();

int main(int argc, char* args[])
{
	G_InitSDL();
	windowPos = { SDL_WINDOWPOS_UNDEFINED ,SDL_WINDOWPOS_UNDEFINED ,1100,800 };
	G_CreateWindow("CROSSY ROAD For Desktop", windowPos, 255, 255, 255);
	load();
	srand(time(NULL));
	while (state != EXITE)
	{
		startLoop = G_GetTicks();
		gameEvent = G_Event();
		draw();
		if (state != PAUSE) update();
		switch (state)
		{
		case START:
			start();
			break;

		case PLAY:
			play();
			break;

		case OUT:
			eagle();
			break;

		case PAUSE:
			pause();
			break;

		case CHOOSE_PLAYER:
			choose_player();
			break;

		case GAME_OVER:
			game_over();
			break;
		}
		if (gameEvent == G_QUIT) state = EXITE;
		endLoop = G_GetTicks() - startLoop;
		if (endLoop<delay)
		{
			G_Delay(delay - endLoop);
		}
		G_Update();
	}
	destroy();
	return 0;
}

void load() {
	grassTile.texture = G_LoadImage("assets/image/grass.png");
	grassTile.position = { 0,0,100,100 };
	grassTile.type = GRASS;

	treeTile.texture = G_LoadImage("assets/image/tree.png");
	treeTile.position = { 0,0,100,100 };
	treeTile.type = TREE;

	waterTile.texture = G_LoadImage("assets/image/sea.png");
	waterTile.position = { 0,0,100,100 };
	waterTile.type = WATER;

	roadTile.texture = G_LoadImage("assets/image/street.png");
	roadTile.position = { 0,0,100,100 };
	roadTile.type = ROAD;

	railTile.texture = G_LoadImage("assets/image/rail.png");
	railTile.position = { 0,0,100,100 };
	railTile.type = RAIL;


	Car.texture = G_LoadImage("assets/image/car.png");
	Car.position = { 0,0,169,100 };
	Car.isMoving = true;
	Car.type = CAR;
	Car.sound = G_LoadSound("assets/sound/car-horn.wav");
	car2Texture = G_LoadImage("assets/image/car2.png");
	car3Texture = G_LoadImage("assets/image/car3.png");

	Stick.texture = G_LoadImage("assets/image/stick1.png");
	Stick.position = { 0,0,100,100 };
	Stick.type = STICK;
	Stick.sound = G_LoadSound("assets/sound/water.wav");

	Train.texture = G_LoadImage("assets/image/train.png");
	Train.position = { 0,0,544,100 };
	Train.type = TRAIN;
	Train.sound = G_LoadSound("assets/sound/train_pass_no_horn.wav");

	Lamp.texture = G_LoadImage("assets/image/green-lamp.png");
	Lamp.position = { 0,0,39,93 };
	Lamp.type = LAMP;
	Lamp.sound = G_LoadSound("assets/sound/train_alarm.wav");
	redLampTexture = G_LoadImage("assets/image/red-lamp.png");

	Eagle.texture = G_LoadImage("assets/image/eagle.png");
	Eagle.position = { 0,0,426,184 };
	Eagle.type = EAGLE;
	Eagle.moveSpeed = 20;
	Eagle.sound = G_LoadSound("assets/sound/eagle_hit.wav");

	Coin.texture = G_LoadImage("assets/image/coin.png");
	Coin.position = { windowPos.w - 100,20,100,100 };
	Coin.dir = LEFT;
	Coin.type = COIN;
	Coin.sound = G_LoadSound("assets/sound/coin_tap.wav");

	initTiles();

	sheepTexture = G_LoadImage("assets/image/player.png");
	cowTexture = G_LoadImage("assets/image/player2.png");
	pigTexture = G_LoadImage("assets/image/player3.png");
	Player.texture = sheepTexture;
	Player.tile = { columns / 2 , rows - 2 };
	Player.position = map[Player.tile.x][Player.tile.y].position;
	Player.type = PLAYER;
	Player.sound = G_LoadSound("assets/sound/car_squish.wav");

	font = G_OpenFont("assets/font/editundo.ttf", 72);
	ScoreText.texture = G_LoadFont(font, "0", 255, 255, 255);
	ScoreText.position = { 10,10,31,42 };

	TopScoreText.texture = G_LoadFont(font, "TOP 0", 255, 255, 255);
	TopScoreText.position = { 10,60,81,52 };

	CoinText.texture = G_LoadFont(font, "0", 253, 230, 75);
	CoinText.position = { windowPos.w - 70 ,8,20,35 };

	GameOverText.texture = G_LoadFont(font, "GAME OVER!", 0, 0, 0);
	GameOverText.position = { (windowPos.w - 345) / 2,(windowPos.h - 43) / 2,345,43 };

	Logo.texture = G_LoadImage("assets/image/logo.png");
	Logo.position = { (windowPos.w - 764) / 2,(windowPos.h - 144) / 3,764,144 };

	ClickButton.texture = G_LoadImage("assets/image/click.png");
	ClickButton.position = { (windowPos.w - 100) / 2,Logo.position.y + 190 ,100,172 };

	TryAgainButton.texture = G_LoadImage("assets/image/again.png");
	TryAgainButton.position = { (windowPos.w - 177) / 2,(windowPos.h / 2) + 150,177,89 };

	PauseButton.texture = G_LoadImage("assets/image/pause.png");
	PauseButton.position = { windowPos.w - 70,70,50,50 };

	PlayerChooseButton.texture = G_LoadImage("assets/image/settings.png");
	PlayerChooseButton.position = { 20,windowPos.h - 120 ,125,100 };

	clickSound = G_LoadSound("assets/sound/click.wav");
}

void initTiles()
{
	columns = ceil(windowPos.w / TILE_LENGTH);
	rows = ceil(windowPos.h / TILE_LENGTH) + 1;
	map = new Tile*[columns];
	for (int i = 0; i < columns; i++)
		map[i] = new Tile[rows];

	for (int y = rows - 1; y >= 0; y--)
	{
		if (y >= rows - 2) {
			for (int x = 0; x < columns; x++) {
				map[x][y] = (x == 0 || x == columns - 1) ? treeTile : grassTile;
				map[x][y].position.x = x * 100;
				map[x][y].position.y = (y - 1) * 100;
			}
		}
		else
		{
			generateTiles(y);
			addObjects(y);
		}
	}
}

void drawTiles()
{
	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			G_Draw(map[i][j].texture, &map[i][j].position);
		}
	}
}

void drawObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].dir == LEFT)
			G_Draw(objects[i].texture, &objects[i].position);
		else
			G_DrawEx(objects[i].texture, &objects[i].position, SDL_FLIP_HORIZONTAL);
	}
}

void update()
{
	adjustCameraSpeed();

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
			map[x][y].position.y += cameraSpeed;

	for (int i = 0; i < objects.size(); i++) {
		objects[i].position.y += cameraSpeed;
		if (objects[i].type == CAR) {
			if (objects[i].dir == LEFT && objects[i].position.x < -Car.position.w) {
				objects[i].position.x = windowPos.w;
			}
			else if (objects[i].dir == RIGHT && objects[i].position.x > windowPos.w) {
				objects[i].position.x = -Car.position.w;
			}
		}

		if (objects[i].type == STICK) {
			if (objects[i].dir == LEFT && objects[i].position.x < -Stick.position.w)
				objects[i].position.x = windowPos.w;
			else if (objects[i].dir == RIGHT && objects[i].position.x > windowPos.w)
				objects[i].position.x = -Stick.position.w;
		}

		if (objects[i].type == TRAIN) {
			if (objects[i].isMoving) {
				if (objects[i].dir == LEFT && objects[i].position.x < -Train.position.w) {
					objects[i].position.x = windowPos.w;
					objects[i].isMoving = false;
					objects[i + 1].texture = Lamp.texture; // We Are Sure there is a lamp just after a train!!
					objects[i].timer = ((rand() % 5) + 2) * FPS;
				}
				else if (objects[i].dir == RIGHT && objects[i].position.x > windowPos.w) {
					objects[i].position.x = -Train.position.w;
					objects[i].isMoving = false;
					objects[i + 1].texture = Lamp.texture;
					objects[i].timer = ((rand() % 5) + 2) * FPS;
				}
			}
			else {
				if (objects[i].timer == FPS) {  //1 Second Before it move we should change the lamp!!
					objects[i + 1].texture = redLampTexture;
					G_PlaySound(Lamp.sound, 0);
				}
				if (objects[i].timer == 0) {
					objects[i].isMoving = true;
					G_PlaySound(Train.sound, 0);
				}
				else
					objects[i].timer--;
			}
		}

		if (objects[i].isMoving ) {
			if (objects[i].type == TRAIN)
				objects[i].position.x += (objects[i].dir == LEFT ? -trainMoveSpeed : trainMoveSpeed);
			else
				objects[i].position.x += objects[i].moveSpeed;
		}
	}

	if (map[0][rows - 1].position.y > windowPos.h) {
		for (int y = rows - 1; y > 0; y--)
			for (int x = 0; x < columns; x++)
				map[x][y] = map[x][y - 1];

		generateTiles(0);
		deleteObjects();

		for (int i = 0; i < objects.size(); i++)
			objects[i].tile.y += 1;
		addObjects(0);
		Player.tile.y += 1;
	}
	Player.position.y += cameraSpeed;

	checkPlayerStatus();
}

void checkPlayerStatus() {
	if (state != PLAY) return;

	if (Player.position.y + 5 >= windowPos.h) {
		state = OUT;
		Eagle.position.x = Player.position.x - (Eagle.position.w / 2);
		Eagle.position.y = -Eagle.position.h;
		eagleIntersection = false;
		return;
	}

	if (Player.position.x <= -TILE_LENGTH || Player.position.x >= windowPos.w)
		state = GAME_OVER;

	bool onStick = false;
	G_Rect temp = { Player.position.x + realPlayerClip.x + 5 , Player.position.y + realPlayerClip.y + 5 , realPlayerClip.w - 5,realPlayerClip.h - 5 };
	for (int i = 0; i < objects.size(); i++) {
		if (SDL_HasIntersection(&temp, &objects[i].position) == SDL_TRUE) {
			if (objects[i].type == CAR || objects[i].type == TRAIN) {
				state = GAME_OVER;
				G_PlaySound(Player.sound, 0);
			}
			else if (objects[i].type == STICK) {
				onStick = true;
				if (!Player.isMoving) {
					if (objects[i].isMoving) Player.position.x += objects[i].moveSpeed;
					if (Player.position.x >= map[Player.tile.x][0].position.x + TILE_LENGTH) Player.tile.x++;
					else if (Player.position.x <= map[Player.tile.x][0].position.x - TILE_LENGTH) Player.tile.x--;
					if (Player.tile.x >= columns || Player.tile.x < 0) state = GAME_OVER;
				}
			}
			else if (objects[i].type == COIN) {
				coins++;
				G_PlaySound(Coin.sound, 0);
				updateScore();
				objects.erase(objects.begin() + i);
				i--;
				continue;
			}
		}
	}
	if (!Player.isMoving && !onStick && map[Player.tile.x][Player.tile.y].type == WATER)
		state = GAME_OVER;
}

void adjustCameraSpeed()
{
	if (state == OUT) {
		cameraSpeed = -2;
		return;
	}
	if (state != PLAY) {
		cameraSpeed = 0;
		return;
	}
	cameraSpeed = cameraBaseSpeed;
	switch (Player.tile.y) {
	case 4:
		cameraSpeed += 1;
		break;
	case 3:
		cameraSpeed += 3;
		break;
	case 2:
		cameraSpeed += 6;
		break;
	case 1:
		cameraSpeed = playerMoveSpeed;
		break;
	}
}

void generateTiles(int row)
{
	TileType rowType = (TileType)(rand() % 4);
	int rowChance = rand() % 100 + 1;
	int Trees = 0;
	for (int x = 0; x < columns; x++)
	{
		if (rowChance <= 50 /*rowType == GRASS*/) {
			int chance = rand() % 100 + 1;
			if (x == 0 || x == columns - 1)
				map[x][row] = treeTile;
			else {
				if (chance >= 80 && Trees < maxTreesInARow) {
					map[x][row] = treeTile;
					Trees++;
				}
				else {
					map[x][row] = grassTile;
				}
			}
		}
		else if (rowChance <= 70 /*rowType == ROAD*/) {
			map[x][row] = roadTile;
		}
		else if (rowChance <= 75 /*rowType == RAIL*/) {
			map[x][row] = railTile;
		}
		else if (rowChance <= 100 /*rowType == WATER*/) {
			map[x][row] = waterTile;
		}

		map[x][row].position.x = x * 100;
		map[x][row].position.y = map[x][row + 1].position.y - TILE_LENGTH;
	}
}

void addObjects(int row)
{
	if (map[0][row].type != WATER) addCoins(row);
	switch (map[0][row].type)
	{
	case ROAD:
		addCar(row);
		break;
	case WATER:
		addStick(row);
		break;
	case RAIL:
		addTrain(row);
		break;
	}
}
void addCar(int row) {
	Direction dir = (Direction)((rand() % 2) + 2);
	Car.dir = dir;
	Car.position.y = map[0][row].position.y;
	Car.tile.y = row;
	Car.moveSpeed = (rand() % 5 + 2) * (dir == RIGHT ? 1 : -1);
	int num = (rand() % 3) + 1;
	int space = (windowPos.w - ((num - 1) * Car.position.w)) / num;
	int randPos = rand() % windowPos.w;
	for (int i = 0; i < num; i++)
	{
		objects.push_back(Car);
		if (dir == LEFT)
			objects.back().position.x = windowPos.w + (i*(space + Car.position.w)) - randPos;
		else
			objects.back().position.x = -Car.position.w - (i*(space + Car.position.w)) + randPos;
		short int tex = rand() % 3;
		switch (tex)
		{
		case 1:
			objects.back().texture = car2Texture;
			break;
		case 2:
			objects.back().texture = car3Texture;
			break;
		}
	}
}
void addStick(int row)
{
	Stick.position.y = map[0][row].position.y;
	Stick.tile.y = row;
	if (map[0][row + 1].type == WATER) { // Direction of two rivers should be diffrent.
		for (int i = objects.size() - 1; i >= 0; i--) {
			if (objects[i].tile.y == row + 1 && objects[i].type == STICK) {
				Stick.dir = (objects[i].dir == LEFT ? RIGHT : LEFT);
				Stick.isMoving = (objects[i].isMoving ? rand() % 2 : true); //We Cant Have Two Stopped Rivers
				break;
			}
		}
	}
	else {
		Stick.isMoving = rand() % 2;
		Stick.dir = (Direction)((rand() % 2) + 2);
	}

	if (Stick.isMoving) {
		Stick.moveSpeed = (rand() % 3 + 1) * (Stick.dir == RIGHT ? 1 : -1);
		int num = (rand() % 4) + 3;
		int space = (windowPos.w - ((num - 1) * Stick.position.w)) / num;
		int randPos = rand() % 200;
		for (int i = 0; i < num; i++)
		{
			objects.push_back(Stick);
			if (Stick.dir == LEFT)
				objects.back().position.x = randPos + (i*(space + Stick.position.w));
			else
				objects.back().position.x = windowPos.w - randPos - (i*(space + Stick.position.w));
		}
	}
	else
	{
		int num = 0;
		do {
			for (int i = 0; i < columns; i++)
			{
				if (map[i][row + 1].type != TREE) {
					int chance = rand() % 100 + 1;
					if (chance >= 50 && num < maxSticksInARow) {
						num++;
						Stick.position.x = map[i][row].position.x;
						objects.push_back(Stick);
					}
				}
			}
		} while (num < minSticksInARow);
	}
}

void addTrain(int row) {
	Direction dir = (Direction)((rand() % 2) + 2);
	Train.dir = dir;
	Train.position.y = map[0][row].position.y;
	Train.tile.y = row;
	Train.timer = ((rand() % 5) + 2) * FPS;

	objects.push_back(Train);
	if (dir == LEFT)
		objects.back().position.x = windowPos.w + Train.position.w;
	else
		objects.back().position.x = -Train.position.w;

	Lamp.tile.y = row;
	Lamp.position.y = map[0][row].position.y + TILE_LENGTH - Lamp.position.h;
	Lamp.position.x = map[0][row].position.x + 20;
	objects.push_back(Lamp);
}

void addCoins(int row) {
	int num = 0;
	Coin.position.y = map[0][row].position.y;
	Coin.position.w = Coin.position.h = 100;
	Coin.tile.y = row;
	for (int i = 0; i < columns; i++)
	{
		if (map[i][row].type != TREE) {
			int chance = rand() % 101;
			if (chance >= 90 && num < maxCoinsInARow) {
				num++;
				Coin.position.x = map[i][row].position.x;
				objects.push_back(Coin);
			}
		}
	}
}

void deleteObjects()
{
	while (!objects.empty() && objects.front().tile.y == rows - 1) {
		objects.pop_front();
	}
}

void draw()
{
	drawTiles();
	if (state == GAME_OVER) G_Draw(Player.texture, &Player.position); // If player lose,Draw it Under Objects!!
	drawObjects();
	if (state != GAME_OVER) G_Draw(Player.texture, &Player.position); // But If It is alive , Draw It top of Objects!
	G_Draw(ScoreText.texture, &ScoreText.position);
	Coin.position = { windowPos.w - 50,0,50,50 };
	G_Draw(Coin.texture, &Coin.position);
	G_Draw(CoinText.texture, &CoinText.position);
}

void start() {
	G_Draw(Logo.texture, &Logo.position);
	G_Draw(ClickButton.texture, &ClickButton.position);
	G_Draw(PlayerChooseButton.texture, &PlayerChooseButton.position);
	if (clickOnButton(&PlayerChooseButton.position))
		state = CHOOSE_PLAYER;
	else if (gameEvent == G_MOUSEBUTTONDOWN && G_Mouse == G_BUTTON_LEFT)
		state = PLAY;
}

void choose_player() {
	int num = 3;
	PlayerButton.position = { (windowPos.w - (num * 150 - 50)) / 2,(windowPos.h - 100) / 2,100,100 };
	for (int i = 0; i < num; i++) {
		switch (i) {
		case 0: PlayerButton.texture = sheepTexture; break;
		case 1: PlayerButton.texture = cowTexture; break;
		case 2: PlayerButton.texture = pigTexture; break;
		}
		if (clickOnButton(&PlayerButton.position)) {
			Player.texture = PlayerButton.texture;
			state = START;
			break;
		}
		SDL_SetRenderDrawColor(renderer, 0, 196, 255, 255);
		SDL_RenderFillRect(renderer, &PlayerButton.position);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, &PlayerButton.position);
		G_Draw(PlayerButton.texture, &PlayerButton.position);
		PlayerButton.position.x += 150;
	}
}

void play() {
	if (Player.isMoving) {
		switch (Player.dir)
		{
		case UP:
			Player.position.y -= playerMoveSpeed;
			if (Player.position.y <= map[Player.tile.x][Player.tile.y - 1].position.y) {
				Player.isMoving = false;
				score++;
				Player.tile.y--;
				Player.position.y = map[Player.tile.x][Player.tile.y].position.y;
				//Player.position.x = map[Player.tile.x][Player.tile.y].position.x;
				if (map[Player.tile.x][Player.tile.y].type == ROAD) G_PlaySound(Car.sound, 0);
				else if (map[Player.tile.x][Player.tile.y].type == WATER) G_PlaySound(Stick.sound, 0);
			}
			break;
		case RIGHT:
			Player.position.x += playerMoveSpeed;
			if (Player.position.x >= map[Player.tile.x + 1][Player.tile.y].position.x) {
				Player.isMoving = false;
				Player.tile.x++;
				Player.position.x = map[Player.tile.x][Player.tile.y].position.x;
			}
			break;
		case LEFT:
			Player.position.x -= playerMoveSpeed;
			if (Player.position.x <= map[Player.tile.x - 1][Player.tile.y].position.x) {
				Player.isMoving = false;
				Player.tile.x--;
				Player.position.x = map[Player.tile.x][Player.tile.y].position.x;
			}
			break;
		case DOWN:
			Player.position.y += playerMoveSpeed;
			if (Player.position.y >= map[Player.tile.x][Player.tile.y + 1].position.y) {
				Player.isMoving = false;
				score--;
				Player.tile.y++;
				Player.position.y = map[Player.tile.x][Player.tile.y].position.y;
			}
			break;
		}
	}

	if (gameEvent == G_KEYDOWN && !Player.isMoving) {
		switch (G_Keyboard) {
		case GK_UP:
			if (map[Player.tile.x][Player.tile.y - 1].type != TREE) {
				Player.isMoving = true;
				Player.dir = UP;
				//Player.position.y -= playerMoveSpeed;
			}
			break;
		case GK_RIGHT:
			if (Player.tile.x + 1 < columns && map[Player.tile.x + 1][Player.tile.y].type != TREE) {
				Player.isMoving = true;
				Player.dir = RIGHT;
				//Player.position.x += playerMoveSpeed;
			}
			break;
		case GK_LEFT:
			if (Player.tile.x - 1 >= 0 && map[Player.tile.x - 1][Player.tile.y].type != TREE) {
				Player.isMoving = true;
				Player.dir = LEFT;
				//Player.position.x -= playerMoveSpeed;
			}
			break;
		case GK_DOWN:
			if (Player.tile.y + 1 < rows && map[Player.tile.x][Player.tile.y + 1].type != TREE) {
				Player.isMoving = true;
				Player.dir = DOWN;
				//Player.position.y += playerMoveSpeed;
			}
			break;
		}
	}
	G_Draw(PauseButton.texture, &PauseButton.position);
	if (clickOnButton(&PauseButton.position))
		state = PAUSE;
	updateScore();
}
void updateScore()
{
	if (score > maxScore) maxScore = score;
	string all = to_string(maxScore);
	strcpy_s(chars, 10, all.c_str());

	//Destroy Perivious Texture And Make A New
	G_DestroyTexture(ScoreText.texture);
	ScoreText.texture = G_LoadFont(font, chars, 255, 255, 255);

	all = to_string(coins);
	strcpy_s(chars, 10, all.c_str());

	//Destroy Perivious Texture And Make A New
	G_DestroyTexture(CoinText.texture);
	CoinText.texture = G_LoadFont(font, chars, 253, 230, 75);
}
void pause() {
	G_Rect pauseRect = { (windowPos.w - 402) / 2,(windowPos.h - 512) / 2,402,512 };
	G_Draw(PauseButton.texture, &pauseRect);
	if (gameEvent == G_MOUSEBUTTONDOWN && G_Mouse == G_BUTTON_LEFT)
		state = PLAY;
}

void eagle()
{
	Eagle.position.y += Eagle.moveSpeed;
	G_Draw(Eagle.texture, &Eagle.position);
	if (!eagleIntersection && SDL_HasIntersection(&Player.position, &Eagle.position)) {
		Player.position.x = windowPos.w; //Move it to a hidden Area
		G_PlaySound(Eagle.sound, 0);
		eagleIntersection = true;
	}

	if (Eagle.position.y > windowPos.h)
		state = GAME_OVER;
}

void game_over()
{
	G_Rect gameOverRect = { 0,(windowPos.h - 100) / 2,windowPos.w,100 };
	SDL_SetRenderDrawColor(renderer, 255, 120, 0, 255);
	SDL_RenderFillRect(renderer, &gameOverRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	G_Draw(GameOverText.texture, &GameOverText.position);
	G_Draw(TryAgainButton.texture, &TryAgainButton.position);

	if (maxScore > topScore) topScore = maxScore;
	string all = "TOP " + to_string(topScore);
	strcpy_s(chars, 10, all.c_str());
	G_DestroyTexture(TopScoreText.texture);
	TopScoreText.texture = G_LoadFont(font, chars, 255, 255, 255);
	G_Draw(TopScoreText.texture, &TopScoreText.position);

	if (clickOnButton(&TryAgainButton.position)) {
		destroyTiles();
		objects.clear(); //Destroy Objects!
		maxScore = score = coins = 0;
		updateScore();
		initTiles(); //ReGenerate The Envirement!
		Player.tile = { columns / 2 , rows - 2 };
		Player.position = map[Player.tile.x][Player.tile.y].position;
		state = START;
	}
}

void destroyTiles() {
	for (int i = 0; i < rows; i++)
		delete[] map[i];

	delete[] map;
}
bool clickOnButton(G_Rect* r) {
	SDL_Point mouse_point = { G_motion.x , G_motion.y };
	if (gameEvent == G_MOUSEBUTTONDOWN && G_Mouse == G_BUTTON_LEFT && SDL_PointInRect(&mouse_point, r)) {
		G_PlaySound(clickSound, 0);
		return true;
	}
	return false;
}
void destroy() {
	TTF_CloseFont(font); // Close Font

	destroyTiles();
	G_DestroyTexture(grassTile.texture);
	G_DestroyTexture(treeTile.texture);
	G_DestroyTexture(waterTile.texture);
	G_DestroyTexture(roadTile.texture);
	G_DestroyTexture(railTile.texture);
	G_DestroyTexture(Car.texture);
	G_FreeSound(Car.sound);
	G_DestroyTexture(car2Texture);
	G_DestroyTexture(car3Texture);
	G_DestroyTexture(Stick.texture);
	G_FreeSound(Stick.sound);
	G_DestroyTexture(Train.texture);
	G_FreeSound(Train.sound);
	G_DestroyTexture(Lamp.texture);
	G_FreeSound(Lamp.sound);
	G_DestroyTexture(redLampTexture);
	G_DestroyTexture(Eagle.texture);
	G_FreeSound(Eagle.sound);
	G_DestroyTexture(Coin.texture);
	G_FreeSound(Coin.sound);
	G_DestroyTexture(sheepTexture);
	G_DestroyTexture(cowTexture);
	G_DestroyTexture(pigTexture);
	G_FreeSound(Player.sound);
	G_DestroyTexture(ScoreText.texture);
	G_DestroyTexture(TopScoreText.texture);
	G_DestroyTexture(CoinText.texture);
	G_DestroyTexture(GameOverText.texture);
	G_DestroyTexture(Logo.texture);
	G_DestroyTexture(ClickButton.texture);
	G_DestroyTexture(TryAgainButton.texture);
	G_DestroyTexture(PauseButton.texture);
	G_DestroyTexture(PlayerChooseButton.texture);
	G_FreeSound(clickSound);
	G_QuitSDL();
}