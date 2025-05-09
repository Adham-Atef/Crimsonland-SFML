#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include <fstream>
#include <vector>
#include <string>

const double PI = 3.14159265f;

using namespace sf;
using namespace std;

// Weapon types (IDs)
int levelIDMenu = -1;

Texture gui[7];
Sprite pistol_gui;
Sprite shotgun_gui;
Sprite rifle_gui;
Sprite hp_gui;
Sprite ammo_gui;
Sprite knife_gui;
Sprite hud_gui[3];
Text score;
Text health_percentage;
Text zombies;
Text timeleft;
Text ammo_in_clip;
Text weapon_name;
Clock time_left;
Font font;
int score_ = 0;
bool rush = false;
Music menuMusic;
bool isGameEntered = false;;
struct MenuButton {
	Text label;
	Text phrase;
	FloatRect bounds;
};
bool pauseGame = false;

bool endScene = false;
bool isEndedEndScene = false;


const int MAX_SCORES = 10;
const char* const SCORE_FILE = "highscores.txt";

int scores[MAX_SCORES] = { 0 }; // Array Array Array  ArrayArrayArrayArrayArrayArrayArrayArrayArrayArrayArrayArrayArrayArrayArray
int scoreCount = 0;

void loadScores() {
	ifstream inFile(SCORE_FILE);

	if (!inFile) {
		cout << "No existing high score file. Starting fresh." << endl;
		return;
	}

	scoreCount = 0;
	while (scoreCount < MAX_SCORES && inFile >> scores[scoreCount]) {
		scoreCount++;
	}

	sort(scores, scores + scoreCount, greater<int>());

	inFile.close();
}

void saveScores() {
	sort(scores, scores + scoreCount, greater<int>());

	ofstream outFile(SCORE_FILE, ios::trunc);

	if (!outFile) {
		cerr << "Error saving high scores!" << endl;
		return;
	}

	for (int i = 0; i < scoreCount; i++) {
		outFile << scores[i] << "\n";
	}

	outFile.close();
	cout << "Scores saved successfully (" << scoreCount << " entries)." << endl;
}

void addScoreIfHigh(int newScore) {
	if (scoreCount < MAX_SCORES) {
		scores[scoreCount++] = newScore;
		return;
	}

	if (newScore > scores[MAX_SCORES - 1]) {
		scores[MAX_SCORES - 1] = newScore;
	}
}

void displayScores() {
	sort(scores, scores + scoreCount, greater<int>());

	cout << "\nCurrent High Scores:" << endl;
	if (scoreCount == 0) {
		cout << "No scores recorded yet." << endl;
		return;
	}

	for (int i = 0; i < scoreCount; i++) {
		cout << i + 1 << ". " << scores[i] << endl;
	}
}


struct gameSound {

	SoundBuffer buffer;

	Sound sound;

	bool isplaying = false;



	bool loadFromFile(const string& filename) {

		if (buffer.loadFromFile(filename)) {

			sound.setBuffer(buffer);

			return true;

		}

		cout << "failed to load sound" << '\n';

		return false;

	}





	void play() {

		sound.play();

		isplaying = true;

	}



	void stop() {

		sound.stop();

	}





};
const int num_sound = 14;
gameSound gameSounds[num_sound];



void runEndScene(RenderWindow& window) {
	gameSounds[1].stop();
	gameSounds[5].stop();
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return;

	Music music;
	if (!music.openFromFile("theEndCrimson.Wav")) return;
	music.setLoop(true);
	music.play();

	const int lineCount = 10;
	string fullLines[lineCount] = {
		"You may inherit pride...",
		"pride in the glory of ancestors who shone like stars.",
		"But their greatness is not yours to inherit.",
		"Glory is never passed down; it must be seized.",
		"Fate offers battles not to break you, but to raise you",
		"to grant you a place among the stars, if you dare.",
		"So do not curse your fate.",
		"Embrace the trials it sends.",
		"For only through them can you rise and claim the glory you seek.",
		"........................And it's all your Choice.",
	};

	Text storyText[lineCount];
	for (int i = 0; i < lineCount; i++) {
		storyText[i].setFont(font);
		storyText[i].setCharacterSize(32);
		storyText[i].setPosition(window.mapPixelToCoords({ 120, 200 + i * 80 }));
		storyText[i].setFillColor(Color::Black);
	}


	RectangleShape fade(Vector2f(1920, 1280));
	fade.setFillColor(Color::Black);
	float fadeAlpha = 255;
	float fadeSpeed = 180;

	enum SceneState { FadeIn, Typing, FadeOut, End };
	SceneState state = FadeIn;

	Clock frameClock;
	Clock globalClock;

	int currentLine = 0;
	int currentChar = 0;
	float typingSpeed = 0.03f;
	float lineDelay = 1.5f;
	float lastCharTypedAt = 0.f;
	float lineCompletedAt = 0.f;
	bool lineDone = false;


	while (window.isOpen() && state != End) {
		float dt = frameClock.restart().asSeconds();
		float totalTime = globalClock.getElapsedTime().asSeconds();


		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();

				window.close();
				return;
			}
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				state = FadeOut;
			}
		}


		switch (state) {
		case FadeIn:
			fadeAlpha -= fadeSpeed * dt;
			if (fadeAlpha <= 0) {
				fadeAlpha = 0;
				state = Typing;
			}
			break;

		case Typing:
			if (currentLine < lineCount) {
				if (!lineDone && totalTime - lastCharTypedAt > typingSpeed &&
					currentChar <= fullLines[currentLine].length()) {
					storyText[currentLine].setString(fullLines[currentLine].substr(0, currentChar));
					currentChar++;
					lastCharTypedAt = totalTime;
				}

				if (currentChar > fullLines[currentLine].length() && !lineDone) {
					lineDone = true;
					lineCompletedAt = totalTime;
				}

				if (lineDone && totalTime - lineCompletedAt > lineDelay) {
					currentLine++;
					currentChar = 0;
					lineDone = false;
				}

				if (currentLine >= lineCount) {
					state = FadeOut;
				}
			}
			break;

		case FadeOut:
			fadeAlpha += fadeSpeed * dt;
			if (fadeAlpha >= 255) {
				fadeAlpha = 255;
				state = End;
			}
			break;

		case End:
			isEndedEndScene = true;
			break;
		}


		fade.setFillColor(Color(0, 0, 0, static_cast<Uint8>(fadeAlpha)));

		window.clear();
		window.draw(background);


		for (int i = 0; i < currentLine && i < lineCount; ++i) {
			window.draw(storyText[i]);
		}


		if (state == Typing && currentLine < lineCount) {
			window.draw(storyText[currentLine]);
		}

		window.draw(fade);
		window.display();
	}

	music.stop();

}

struct PauseableTimer {
	Clock clock;
	float totalTime = 0.0f;
	bool paused = false;

	void pause() {
		if (!paused) {
			totalTime += clock.getElapsedTime().asSeconds();
			paused = true;
		}
	}

	void resume() {
		if (paused) {
			clock.restart();
			paused = false;
		}
	}

	float getTime() const {
		if (paused) {
			return totalTime;
		}
		else {
			return totalTime + clock.getElapsedTime().asSeconds();
		}
	}

	void reset() {
		totalTime = 0.0f;
		clock.restart();
		paused = false;
	}
}gameTimer;


void runStoryMode(RenderWindow& window) {
	const int totalLevels = 12;
	const string levelNames[totalLevels] = {
		"Beach", "Desert Road", "City",
		"Safe Zone 1", "Safe Zone 2", "Woods",
		"Safe Zone 3", "Army Camp", "Mission 1",
		"Mission 2", "Mission 3", "Mission 4"
	};
	bool unlocked[totalLevels] = { true, true, true, true, true, true, true, true, true, true, true, true };

	Font font;
	font.loadFromFile("tag.ttf");

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("crimsonMain.png");
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.0f, 2.0f);

	Texture logoTexture;
	logoTexture.loadFromFile("logo.png");
	Sprite logoSprite(logoTexture);
	logoSprite.setPosition(window.mapPixelToCoords({ 1000, -10 }));
	logoSprite.setScale(0.8f, 0.8f);

	//Music menuMusic;
	//menuMusic.openFromFile("MainSound.wav");
	//menuMusic.setLoop(true);
	//menuMusic.play();

	SoundBuffer clickBuffer;
	clickBuffer.loadFromFile("Button.wav");
	Sound clickSound(clickBuffer);

	RectangleShape levelRects[totalLevels];
	Text levelTexts[totalLevels];

	int selectedIndex = -1;
	float startX = 250, startY = 250; // starting position of the first level box    
	float boxW = 150, boxH = 50;      //size of each level box
	float horGap = 30, vertGap = 60;  //horizontal and vertical gaps  

	for (int i = 0; i < totalLevels; ++i) {
		levelRects[i].setSize(Vector2f(boxW, boxH));
		levelRects[i].setFillColor(unlocked[i] ? Color(136, 8, 8, 255) : Color(74, 4, 4, 150));
		int row = i / 3;
		int col = i % 3;
		levelRects[i].setPosition(window.mapPixelToCoords(
			Vector2i(
				static_cast<int>(startX + col * (boxW + horGap)),
				static_cast<int>(startY + row * (boxH + vertGap))
			)
		));

		levelTexts[i].setFont(font);
		levelTexts[i].setString(levelNames[i]);
		levelTexts[i].setCharacterSize(24);
		levelTexts[i].setFillColor(Color::Black);
		FloatRect textRect = levelTexts[i].getLocalBounds();
		levelTexts[i].setOrigin(textRect.width / 2, textRect.height / 2);
		levelTexts[i].setPosition(
			levelRects[i].getPosition().x + boxW / 2,
			levelRects[i].getPosition().y + boxH / 2 - 10
		);

	}

	RectangleShape backBtn(Vector2f(300, 60));
	backBtn.setFillColor(Color(155, 155, 155, 240));
	backBtn.setPosition(window.mapPixelToCoords({ 350, 720 }));

	Text storyText;
	storyText.setFont(font);
	storyText.setString("Story Mode");
	storyText.setCharacterSize(38);
	storyText.setFillColor(Color(160, 155, 155, 240));
	storyText.setPosition(window.mapPixelToCoords({ 150, 100 }));

	Text backText;
	backText.setFont(font);
	backText.setString("Back to Main Menu");
	backText.setCharacterSize(24);
	backText.setFillColor(Color::Black);
	FloatRect bt = backText.getLocalBounds();
	backText.setOrigin(bt.width / 2, bt.height / 2);
	backText.setPosition(window.mapPixelToCoords(
		Vector2i(static_cast<int>(backBtn.getPosition().x + 150),
			static_cast<int>(backBtn.getPosition().y + 20))
	));
	bool onBackButton = false;

	while (window.isOpen()) {
		Event event;
		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
			{
				saveScores();
				window.close();

			}

			if (event.type == Event::MouseMoved) {
				selectedIndex = -1;
				for (int i = 0; i < totalLevels; ++i) {
					if (levelRects[i].getGlobalBounds().contains(mousePos)) {
						selectedIndex = i;
					}
				}
				onBackButton = backBtn.getGlobalBounds().contains(mousePos);
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					return;
				}

				if (event.key.code == Keyboard::Enter) {
					if (selectedIndex >= 0 && unlocked[selectedIndex]) {
						clickSound.play();
						cout << "Starting Level: " << levelNames[selectedIndex] << "\n";
						levelIDMenu = selectedIndex;
						menuMusic.stop();
						return;
					}
					else if (onBackButton) {
						clickSound.play();
						sleep(milliseconds(150));
						return;
					}
				}

				if (event.key.code == Keyboard::Right) selectedIndex = (selectedIndex + 1) % totalLevels;
				if (event.key.code == Keyboard::Left)  selectedIndex = (selectedIndex - 1 + totalLevels) % totalLevels;
				if (event.key.code == Keyboard::Down)  selectedIndex = (selectedIndex + 3) % totalLevels;
				if (event.key.code == Keyboard::Up)    selectedIndex = (selectedIndex - 3 + totalLevels) % totalLevels;
			}

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				if (selectedIndex >= 0 && unlocked[selectedIndex]) {
					clickSound.play();
					cout << "Starting Level: " << levelNames[selectedIndex] << "\n";
					levelIDMenu = selectedIndex;
					menuMusic.stop();

					return;
				}
				else if (onBackButton) {
					clickSound.play();
					return;
				}
			}
		}

		window.clear();
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(storyText);

		for (int i = 0; i < totalLevels; ++i) {
			levelRects[i].setScale(1.f, 1.f);
			if (i == selectedIndex && unlocked[i]) {
				levelRects[i].setScale(1.05f, 1.05f);
			}
			window.draw(levelRects[i]);
			window.draw(levelTexts[i]);
		}

		backBtn.setFillColor(onBackButton ? Color(150, 35, 35, 255) : Color(155, 155, 155, 240));
		window.draw(backBtn);
		window.draw(backText);
		window.display();
	}
}

void runRushMode(RenderWindow& window) {
	Font font;
	if (!font.loadFromFile("tag.ttf")) {
		cout << "Failed to load font for Rush Mode.\n";
	}

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("crimsonMain.png");
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.0f, 2.0f);

	Texture logoTexture;
	logoTexture.loadFromFile("logo.png");
	Sprite logoSprite(logoTexture);
	logoSprite.setPosition(window.mapPixelToCoords({ 1000, -10 }));
	logoSprite.setScale(0.8f, 0.8f);

	//Music menuMusic;
	//menuMusic.openFromFile("MainSound.wav");
	//menuMusic.setLoop(true);
	//menuMusic.play();

	SoundBuffer clickBuffer;
	if (!clickBuffer.loadFromFile("Button.wav")) {
		cout << "Failed to load button click sound.\n";
	}
	Sound clickSound(clickBuffer);

	const string maps[5] = { "Beach", "Desert Road", "Woods", "City", "Army Camp" };
	Text mapText;
	mapText.setFont(font);
	mapText.setCharacterSize(25);
	mapText.setFillColor(Color(74, 8, 8, 240));
	mapText.setString("Choose a map:");
	mapText.setPosition(window.mapPixelToCoords({ 125, 350 }));

	Text rushText;
	rushText.setFont(font);
	rushText.setString("Rush Mode");
	rushText.setCharacterSize(38);
	rushText.setFillColor(Color(160, 155, 155, 240));
	rushText.setPosition(window.mapPixelToCoords({ 150, 100 }));

	Text backToMenuText;
	backToMenuText.setFont(font);
	backToMenuText.setCharacterSize(32);
	backToMenuText.setFillColor(Color(150, 150, 150, 240));
	backToMenuText.setString("Back to Main Menu");
	backToMenuText.setPosition(window.mapPixelToCoords({ 300, 850 }));
	FloatRect backBounds = backToMenuText.getGlobalBounds();

	bool inRushMode = true;
	int selectedMap = 0;

	while (inRushMode && window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();
			}


			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Up) {
					selectedMap = (selectedMap - 1 + 5) % 5;
				}
				else if (event.key.code == Keyboard::Down) {
					selectedMap = (selectedMap + 1) % 5;
				}
				else if (event.key.code == Keyboard::Enter) {
					cout << "Selected map: " << maps[selectedMap] << endl;
					menuMusic.stop();

					clickSound.play();
					inRushMode = false;
				}
				else if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					inRushMode = false;
				}
			}


			if (event.type == Event::MouseMoved) {
				Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
				bool hoveringMap = false;

				for (int i = 0; i < 5; ++i) {
					FloatRect bounds(350, 250 + i * 70, 300, 40);
					if (bounds.contains(mousePos)) {
						selectedMap = i;
						hoveringMap = true;
						break;
					}
				}

				if (backBounds.contains(mousePos)) {
					backToMenuText.setFillColor(Color(150, 35, 35, 255));
				}
				else {
					backToMenuText.setFillColor(Color(150, 150, 150, 240));
				}
			}


			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);

				for (int i = 0; i < 5; ++i) {
					FloatRect bounds(350, 250 + i * 70, 300, 40);
					if (bounds.contains(clickPos)) {
						selectedMap = i;
						cout << "Selected map: " << maps[selectedMap] << endl;
						levelIDMenu = selectedMap + 12;
						menuMusic.stop();

						clickSound.play();
						inRushMode = false;
						break;
					}
				}

				if (backBounds.contains(clickPos)) {
					clickSound.play();
					sleep(milliseconds(150));
					inRushMode = false;
				}
			}
		}

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(mapText);
		window.draw(rushText);

		for (int i = 0; i < 5; ++i) {
			Text mapOption;
			mapOption.setFont(font);
			mapOption.setString(maps[i]);
			mapOption.setCharacterSize(40);
			mapOption.setFillColor(i == selectedMap ? Color(150, 35, 35, 255) : Color(150, 150, 150, 240));
			mapOption.setPosition(window.mapPixelToCoords({ 350, 250 + i * 70 }));
			window.draw(mapOption);
		}

		window.draw(backToMenuText);
		window.display();
	}
}




void runLeaderboard(RenderWindow& window) {

	Font font;
	if (!font.loadFromFile("tag.ttf")) {
		cout << "Failed to load font.\n";
	}

	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("crimsonMain.png")) {
		cout << "Failed to load background.\n";
	}
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.0f, 2.0f);

	Texture logoTexture;
	if (!logoTexture.loadFromFile("logo.png")) {
		cout << "Failed to load logo.\n";

	}
	Sprite logoSprite(logoTexture);
	logoSprite.setPosition(window.mapPixelToCoords({ 1000, -10 }));
	logoSprite.setScale(0.8f, 0.8f);


	SoundBuffer clickBuffer;
	if (!clickBuffer.loadFromFile("Button.wav")) {
		cout << "Failed to load click sound.\n";

	}
	Sound clickSound(clickBuffer);

	Text title;
	title.setFont(font);
	title.setString("Leaderboard");
	title.setCharacterSize(50);
	title.setFillColor(Color(135, 135, 135, 240));
	title.setPosition(window.mapPixelToCoords({ 150, 100 }));

	/*Text placeholder;
	placeholder.setFont(font);
	placeholder.setString("No leaderboard data yet.");
	placeholder.setCharacterSize(30);
	placeholder.setFillColor(Color(180, 180, 180));
	placeholder.setPosition(window.mapPixelToCoords({ 150, 250 }));
	*/

	Text x_[MAX_SCORES];



	for (int i = 0; i < MAX_SCORES - 1; ++i) {  //sort the leaderboard
		for (int j = i + 1; j < MAX_SCORES; ++j) {
			if (scores[j] > scores[i]) {
				int temp = scores[i];
				scores[i] = scores[j];
				scores[j] = temp;
			}
		}
	}

	for (int i = 0; i < MAX_SCORES; i++) { //setup the leaderboard
		x_[i].setFont(font);
		x_[i].setCharacterSize(36);
		x_[i].setPosition(150, 200 + i * 70);
		x_[i].setFillColor(Color(200, 200, 200));
		x_[i].setString(to_string(scores[i]));
	}

	//cout << x[0] << endl;






	Text backButton;
	backButton.setFont(font);
	backButton.setString("Back to Main Menu");
	backButton.setCharacterSize(34);
	backButton.setFillColor(Color(155, 155, 155, 240));
	backButton.setPosition(window.mapPixelToCoords({ 250, 900 }));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
			{
				saveScores();

				window.close();
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				return;

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (backButton.getGlobalBounds().contains(mousePos)) {
					clickSound.play();
					sleep(milliseconds(150));
					return;
				}
			}
		}

		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		if (backButton.getGlobalBounds().contains(mousePos))
			backButton.setFillColor(Color(155, 35, 35, 255));
		else
			backButton.setFillColor(Color(135, 135, 135, 240));

		window.clear();
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		//window.draw(placeholder);
		window.draw(backButton);
		for (int i = 0; i < MAX_SCORES; i++)
		{
			window.draw(x_[i]);
		}
		window.display();
	}
}



void runSettings(RenderWindow& window) {
	Font font;
	if (!font.loadFromFile("tag.ttf")) {
		cout << "Failed to load font.\n";

	}

	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("crimsonMain.png")) {
		cout << "Failed to load background.\n";

	}
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.0f, 2.0f);

	Texture logoTexture;
	if (!logoTexture.loadFromFile("logo.png")) {
		cout << "Failed to load logo.\n";

	}
	Sprite logoSprite(logoTexture);
	logoSprite.setPosition(window.mapPixelToCoords({ 1000, -10 }));
	logoSprite.setScale(0.8f, 0.8f);

	SoundBuffer clickBuffer;
	if (!clickBuffer.loadFromFile("Button.wav")) {
		cout << "Failed to load click sound.\n";

	}
	Sound clickSound(clickBuffer);

	Text title;
	title.setFont(font);
	title.setString("Game Controls");
	title.setCharacterSize(50);
	title.setFillColor(Color(135, 135, 135, 240));
	title.setPosition(window.mapPixelToCoords({ 150, 100 }));

	Text controlsText;
	controlsText.setFont(font);
	controlsText.setCharacterSize(40);
	controlsText.setFillColor(Color(180, 180, 180));
	controlsText.setPosition(250, 300);
	controlsText.setString(
		"W/A/S/D   \t- move\n"
		"Mouse         \t- Aim\n"
		"Left Click  \t- Shoot\n"
		"Q                  \t- Switch Weapons\n"
		"F                   \t- Melee Attack\n"
		"R                   \t- Reload\n"
		"Escape        \t- Back / Pause"
	);

	Text backButton;
	backButton.setFont(font);
	backButton.setString("Back to Main Menu");
	backButton.setCharacterSize(34);
	backButton.setFillColor(Color(155, 155, 155, 240));
	backButton.setPosition(window.mapPixelToCoords({ 250, 900 }));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();

			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				return;

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (backButton.getGlobalBounds().contains(mousePos)) {
					clickSound.play();
					sleep(milliseconds(150));
					return;
				}
			}
		}


		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		if (backButton.getGlobalBounds().contains(mousePos))
			backButton.setFillColor(Color(155, 35, 35, 255));
		else
			backButton.setFillColor(Color(135, 135, 135, 240));

		window.clear();
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		window.draw(controlsText);
		window.draw(backButton);
		window.display();
	}
}


void runCredits(RenderWindow& window) {

	Font font;
	if (!font.loadFromFile("tag.ttf")) {
		cout << "Failed to load font.\n";
	}

	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("crimsonMain.png")) {
		cout << "Failed to load background.\n";
	}
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.0f, 2.0f);

	Texture logoTexture;
	if (!logoTexture.loadFromFile("logo.png")) {
		cout << "Failed to load logo.\n";
	}
	Sprite logoSprite(logoTexture);
	logoSprite.setPosition(1000, -10);
	logoSprite.setScale(0.8f, 0.8f);


	SoundBuffer clickBuffer;
	if (!clickBuffer.loadFromFile("Button.wav")) {
		cout << "Failed to load click sound.\n";
	}
	Sound clickSound(clickBuffer);

	Text title;
	title.setFont(font);
	title.setString("Game Credits");
	title.setCharacterSize(50);
	title.setFillColor(Color(135, 135, 135, 240));
	title.setPosition(150, 100);

	Text creditsText;
	creditsText.setFont(font);
	creditsText.setString("Developed by\n\n   \tAdham Atef\n   \tAli Adel\n   \tHassan Alaa\n   \tHazem Ahmed\n   \tMichael Sameh\n   \tRawan Ahmed\n   \tHossam Ibrahim\n  \n\n Scripted and Directed by\n\n  \tHossam Ibrahim");
	creditsText.setCharacterSize(30);
	creditsText.setFillColor(Color(180, 180, 180));
	creditsText.setPosition(150, 250);

	Text backButton;
	backButton.setFont(font);
	backButton.setString("Back to Main Menu");
	backButton.setCharacterSize(34);
	backButton.setFillColor(Color(155, 155, 155, 240));
	backButton.setPosition(250, 930);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();

				window.close();

			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				return;

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (backButton.getGlobalBounds().contains(mousePos)) {
					clickSound.play();
					sleep(milliseconds(150)); // wait for the sound
					return;
				}
			}
		}


		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		if (backButton.getGlobalBounds().contains(mousePos))
			backButton.setFillColor(Color(155, 35, 35, 255));
		else
			backButton.setFillColor(Color(135, 135, 135, 240));

		window.clear();
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		window.draw(creditsText);
		window.draw(backButton);
		window.display();
	}
}



enum WeaponID {
	KNIFE = 0,
	PISTOL = 1,
	RIFLE = 2,
	BURST_RIFLE = 3,
	SINGLE_RIFLE = 4,
	SHOTGUN = 5,
	PLASMA_PISTOL = 6,
	PLASMA_RIFLE = 7,
	PLASMA_SHOTGUN = 8
};



Texture crosshairTexture;

struct CROSSHAIR {


	Sprite shape;

	CROSSHAIR(Texture& texture, RenderWindow& window) {
		shape.setScale(0.2f, 0.2f);
		shape.setTexture(texture);
		shape.setPosition((float)window.getSize().x / 2.0f, (float)window.getSize().y / 2.0f);
		//shape.setTextureRect(IntRect(0, 0, 313, 206));
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2);
	}

	void mousePosSetter(Vector2f MouseGlobalPos) {
		shape.setPosition(MouseGlobalPos);
	}
};


Texture SpriteTextures[4]; // sprite textures :: Index from 0 to n (n = number of the weapons (0 refers to knife , 1 to handgun , 2 to rifle , 3 to shotgun))
Texture ShellTextures[4]; // sprite textures :: Index from 0 to n (n = number of the weapons (0 refers to light shell , 1 to medium shell , 2 to heavy shell , 3 to plasma))
Texture ZombieTextures[3];
Texture woodsBackgroundTexture;
Texture pineTreeTexture;
Texture pineTreesTexture;
Texture backgroundDesertRoad;
Texture safezone2BackgroundTexture;
Sprite airPlane;
Texture mission4BackgroundTexture;
Texture mission4PalmTree;
Texture back_groundT;
Texture trainRailsTexture;
Texture trainTexture;
Texture wellTexture;
Texture treeTexture;
//city
Texture back_ground;
Texture buildingTexture;
Texture cara;
Texture carb;
//city

//army
Texture armycampdeadTexture;
Texture armyBackground;
Texture Tent;
Texture Tank1;
Texture Tank2;

Texture beachdeadtexture;



bool loadTexture(Texture& texture, const string& filePath) {
	if (texture.loadFromFile(filePath)) {
		return true;
	}
	cout << "Failed to load texture: " << filePath << endl;
	return false;
}


void loadingAssets() {


	if (!loadTexture(crosshairTexture, "imgs/crosshair.png")) {
		cout << "Error loading crosshair texture" << endl;
	}

	if (!loadTexture(SpriteTextures[0], "imgs/sprite/sprite_sheet_knife.png")) {
		cout << "Error loading knife texture" << endl;
	}
	if (!loadTexture(SpriteTextures[1], "imgs/sprite/sprite_sheet_handgun.png")) {
		cout << "Error loading handgun texture" << endl;
	}
	if (!loadTexture(SpriteTextures[2], "imgs/sprite/sprite_sheet_rifle.png")) {
		cout << "Error loading rifle texture" << endl;
	}
	if (!loadTexture(SpriteTextures[3], "imgs/sprite/sprite_sheet_shotgun.png")) {
		cout << "Error loading shotgun texture" << endl;
	}

	if (!loadTexture(ZombieTextures[1], "imgs/sprite/zombie3.png")) {
		cout << "Error loading Zombie2 texture" << endl;
	}
	if (!loadTexture(ZombieTextures[0], "imgs/sprite/zombie2.png")) {
		cout << "Error loading Zombie2 texture" << endl;
	}
	if (!loadTexture(ZombieTextures[2], "imgs/sprite/dogsheet.png")) {
		cout << "Error loading Zombie2 texture" << endl;
	}


	if (!loadTexture(ShellTextures[0], "imgs/sprite/lig-shell.png")) {
		cout << "Error loading light shell texture" << endl;
	}
	if (!loadTexture(ShellTextures[1], "imgs/sprite/med-shell.png")) {
		cout << "Error loading medium shell texture" << endl;
	}
	if (!loadTexture(ShellTextures[2], "imgs/sprite/hev-shell.png")) {
		cout << "Error loading heavy shell texture" << endl;
	}
	if (!loadTexture(ShellTextures[3], "imgs/sprite/plasma.png")) {
		cout << "Error loading plasma shell texture" << endl;
	}
	if (!loadTexture(safezone2BackgroundTexture, "imgs/safezone2/AirPort.jpeg")) {
		cout << "Error loading plane2" << endl;
	}

	if (!loadTexture(woodsBackgroundTexture, "imgs/woods_background.jpeg")) {
		cout << "Error loading woods background texture" << endl;

	}

	if (!loadTexture(backgroundDesertRoad, "imgs/desertroad/DesertRoad.png")) {
		cout << "Error loading woods pineTree texture" << endl;
	}
	if (!loadTexture(mission4BackgroundTexture, "imgs/mission4/ac7f31cf-5594-4b77-8457-e41eb96a7153.png")) {
		cout << "Failed to load background!" << endl;
	}
	if (!loadTexture(mission4PalmTree, "imgs/mission4/Palm_tree2_1.png")) {
		cout << "Failed to load background!" << endl;
	}

	//train
	if (!loadTexture(back_groundT, "imgs/train/backgroundT.jpg")) {
		cout << "Error loading back_groundT texture" << endl;
	}
	if (!loadTexture(trainRailsTexture, "imgs/train/train rails.png")) {
		cout << "Error loading trainRails texture" << endl;
	}
	if (!loadTexture(trainTexture, "imgs/train/train.png")) {
		cout << "Error loading train texture" << endl;
	}
	if (!loadTexture(wellTexture, "imgs/train/water.png")) {
		cout << "Error loading well texture" << endl;
	}
	if (!loadTexture(treeTexture, "imgs/train/tree.png")) {
		cout << "Error loading tree texture" << endl;
	}
	//train

//city
	if (!loadTexture(back_ground, "imgs/city/IMG_20250422_172234_439.jpg")) {
		cout << "Error loading back_ground texture" << endl;
	}
	if (!loadTexture(buildingTexture, "imgs/city/CityBuilding2.png")) {
		cout << "Error loading building texture" << endl;
	}
	if (!loadTexture(cara, "imgs/city/car1.png")) {
		cout << "Error loading cara texture" << endl;
	}
	if (!loadTexture(carb, "imgs/city/car2.png")) {
		cout << "Error loading carb texture" << endl;
	}
	//city

	if (!loadTexture(armyBackground, "imgs/army/armygnd.png")) {
		cout << "Error loading carb texture" << endl;
	}
	if (!loadTexture(Tent, "imgs/army/tent.png")) {
		cout << "Error loading carb texture" << endl;
	}
	if (!loadTexture(Tank1, "imgs/army/tank.png")) {
		cout << "Error loading carb texture" << endl;
	}
	if (!loadTexture(Tank2, "imgs/army/tank.png")) {
		cout << "Error loading carb texture" << endl;
	}
	if (!loadTexture(beachdeadtexture, "imgs/beach/beachdead.png")) {
		cout << "Error loading carb texture" << endl;
	}
}









void loadSounds() {

	if (!gameSounds[0].loadFromFile("Sound/foot.wav")) {

		cout << "Error loading foot sound" << endl;

	}



	if (!gameSounds[1].loadFromFile("Sound/ZomibeOnBeach.wav")) {

		cout << "Error loading ZomibeOnBeach sound" << endl;

	}



	if (!gameSounds[2].loadFromFile("Sound/TimeClock.wav")) {

		cout << "Error loading TimeClock" << endl;

	}



	if (!gameSounds[3].loadFromFile("Sound/Fire.wav")) {

		cout << "Error loading Fire" << endl;

	}



	if (!gameSounds[4].loadFromFile("Sound/Shotgun.wav")) {

		cout << "Error loading Shotgun" << endl;

	}



	if (!gameSounds[5].loadFromFile("Sound/StratOfGame.wav")) {

		cout << "Error loading StratOfGame" << endl;

	}



	if (!gameSounds[6].loadFromFile("Sound/Damage.wav")) {

		cout << "Error loading Damage" << endl;

	}



	if (!gameSounds[7].loadFromFile("Sound/ReloadShotgun.wav")) {

		cout << "Error loading ReloadShotgun" << endl;

	}



	if (!gameSounds[8].loadFromFile("Sound/Zombie.wav")) {

		cout << "Error loading Zombie" << endl;

	}



	if (!gameSounds[9].loadFromFile("Sound/Rifle.wav")) {

		cout << "Error loading Rifle" << endl;

	}

	if (!gameSounds[10].loadFromFile("Sound/PlasmaRifle.wav")) {

		cout << "Error loading Rifle" << endl;

	}

	if (!gameSounds[11].loadFromFile("Sound/PlasmaShotgun.wav")) {

		cout << "Error loading Rifle" << endl;

	}

	if (!gameSounds[12].loadFromFile("Sound/AutoRifle.wav")) {

		cout << "Error loading Rifle" << endl;

	}

	if (!gameSounds[13].loadFromFile("Sound/Knife.wav")) {

		cout << "Error loading Rifle" << endl;

	}

}









void foot() {

	gameSounds[0].play();

}

void ZomibeOnBeach() {
	gameSounds[1].sound.setVolume(35);
	gameSounds[1].sound.setLoop(false);

	gameSounds[1].play();
	gameSounds[1].sound.setVolume(35);


}

void TimeClock() {

	gameSounds[2].play();

}

void Fire() {

	gameSounds[3].play();

}

void Shotgun() {

	gameSounds[4].play();

}

void StartOfGame() {

	gameSounds[5].play();
	gameSounds[5].sound.setVolume(30);
}

void Damage() {

	gameSounds[6].play();

}

void ReloadShotgun() {

	gameSounds[7].play();

}

void Zombie() {

	gameSounds[8].play();

}

void RIFLES() {

	gameSounds[9].play();

}

void PlasmaRifle() {

	gameSounds[10].play();

}



void PlasmaShotgun() {

	gameSounds[11].play();

}

void AutoRifle() {

	gameSounds[12].play();

}

void Knife() {

	gameSounds[13].play();

}



void draw_background(RenderWindow& window, Texture& back_ground, int e, int f)
{
	Sprite city;
	city.setTexture(back_ground);
	city.setPosition(e, f);
	window.draw(city);
}

struct Building {
	Sprite spriteB;

	void init(Texture& texture, int x, int y) {
		spriteB.setTexture(texture);
		spriteB.setPosition(x, y);
		spriteB.setScale(0.625f, 0.625f);
	}

	void draw(RenderWindow& window) {
		window.draw(spriteB);
	}
};

const int NUM_BUILDINGS = 6;


struct Map {
	Building buildings[NUM_BUILDINGS];

	void init(Texture& texture) {
		buildings[0].init(texture, -180, 20);
		buildings[1].init(texture, 1750, 20);
		buildings[2].init(texture, 1750, 750);
		buildings[3].init(texture, 780, 20);
		buildings[4].init(texture, -180, 760);
		buildings[5].init(texture, 780, 750);
	}

	void drawBuildings(RenderWindow& window) {
		for (int i = 0; i < NUM_BUILDINGS; i++) {
			buildings[i].draw(window);
		}
	}
};
struct Car {
	Sprite spriteC;

	void init(Texture& texture, int x, int y, float rotation, float z) {
		spriteC.setTexture(texture);
		spriteC.setPosition(x, y);
		spriteC.setScale(z, z);
		spriteC.setRotation(rotation);
	}

	void draw(RenderWindow& window) {
		window.draw(spriteC);
	}
};
const int NUM_CARS = 6;
struct Traffic {
	Car cars[NUM_CARS];

	void init(Texture& cara, Texture& carb) {
		cars[0].init(cara, 450, 200, -45, 0.5f);
		cars[1].init(cara, 900, 300, 45, 0.5f);
		cars[2].init(cara, 200, 900, -45, 0.5f);
		cars[3].init(carb, 1350, 200, 35, 0.8f);
		cars[4].init(carb, 200, 300, 90, 0.8f);
		cars[5].init(carb, 1580, 630, 0, 0.8f);
	}

	void drawCars(RenderWindow& window) {
		for (int i = 0; i < NUM_CARS; i++) {
			cars[i].draw(window);
		}
	}
};


void draw_backgroundT(RenderWindow& window, Texture& back_ground, int e, int f)
{
	Sprite TRAINB;
	TRAINB.setTexture(back_ground);
	TRAINB.setScale(3.25, 3.25);
	TRAINB.setPosition(e, f);
	window.draw(TRAINB);
}
struct Train {
	Sprite trainS;

	void trains(Texture& texture, int x, int y) {
		trainS.setTexture(texture);
		trainS.setPosition(x, y);
		trainS.setOrigin(trainTexture.getSize().x / 2, trainTexture.getSize().y / 2);
	}
	void draw(RenderWindow& window) {
		window.draw(trainS);
	}
};
const int NUM_TRAIN = 3;
struct Trains {
	Train TRAINS[NUM_TRAIN];

	void TRAINSS(Texture& texture) {
		TRAINS[0].trains(texture, 700, 405);
		TRAINS[1].trains(texture, -100, 807);
		TRAINS[2].trains(texture, 1500, 810);
	}

	void drawrails(RenderWindow& window) {
		for (int i = 0; i < NUM_TRAIN; i++) {
			TRAINS[i].draw(window);
		}
	}


};

struct Rail {
	Sprite railS;

	void Rails(Texture& texture, int x, int y) {
		railS.setTexture(texture);
		railS.setPosition(x, y);
	}

	void draw(RenderWindow& window) {
		window.draw(railS);
	}
};
const int NUM_RAILS = 6;
struct TrainRails {
	Rail trainrails[NUM_RAILS];

	void RAILS(Texture& texture) {
		trainrails[0].Rails(texture, 0, 250);
		trainrails[1].Rails(texture, 733, 250);
		trainrails[2].Rails(texture, 1466, 250);
		trainrails[3].Rails(texture, 0, 655);
		trainrails[4].Rails(texture, 733, 655);
		trainrails[5].Rails(texture, 1466, 655);
	}

	void drawtrains(RenderWindow& window) {
		for (int i = 0; i < NUM_RAILS; i++) {
			trainrails[i].draw(window);
		}
	}
};

struct decor {
	Sprite sprite;

	void DECOR(Texture& texture, int x, int y, float m) {
		sprite.setTexture(texture);
		sprite.setPosition(x, y);
		sprite.setScale(m, m);
	}

	void draw(RenderWindow& window) {
		window.draw(sprite);
	}
};

const int NUM_DECOR = 4;
struct decorations {
	decor trees[NUM_DECOR];

	void DECORATIONS(Texture& well, Texture& tree) {
		trees[0].DECOR(well, 300, 50, 1);
		trees[1].DECOR(tree, 480, 100, 2);
		trees[2].DECOR(tree, 500, 150, 2);
		trees[3].DECOR(tree, 250, 125, 2);
	}

	void drawdecorations(RenderWindow& window) {
		for (int i = 0; i < NUM_DECOR; i++) {
			trees[i].draw(window);
		}
	}
};
const int KNIFE_DAMAGE = 35.0f;
struct Weapon {
	WeaponID id;
	string name;
	int clipSize;
	int reloadClipSize;
	int currentClipSize;
	float damage;
	float bulletDelay;
	bool isPlasma;
	int currentSprite;

	void weaponAdd(WeaponID index) {
		id = index;
		isPlasma = false;

		switch (id)
		{
		case KNIFE:
			name = "Knife";
			clipSize = 0;
			damage = KNIFE_DAMAGE;
			bulletDelay = 0.4f;
			currentSprite = 0;
			break;

		case PISTOL:
			name = "Pistol";
			clipSize = 17;
			damage = 50.0f;
			bulletDelay = 0.45f;
			currentSprite = 1;
			break;

		case RIFLE:
			name = "Rifle (Auto)";
			clipSize = 30;
			damage = 75.0f;
			bulletDelay = 0.1f;
			currentSprite = 2;
			break;

		case BURST_RIFLE:
			name = "Rifle (Burst)";
			clipSize = 24;
			damage = 75.0f;
			bulletDelay = 1.0f;
			currentSprite = 2;
			break;

		case SINGLE_RIFLE:
			name = "Rifle (Single)";
			clipSize = 20;
			damage = 100.0f;
			bulletDelay = 0.5f;
			currentSprite = 2;
			break;

		case SHOTGUN:
			name = "Shotgun";
			clipSize = 7;
			damage = 50.0f;
			bulletDelay = 0.65f;
			currentSprite = 3;
			break;

		case PLASMA_PISTOL:
			name = "Plasma Pistol";
			clipSize = 17;
			damage = 100.0f;
			bulletDelay = 0.45f;
			currentSprite = 1;
			isPlasma = true;
			break;

		case PLASMA_RIFLE:
			name = "Plasma Rifle";
			clipSize = 30;
			damage = 100.0f;
			bulletDelay = 0.1f;
			currentSprite = 2;
			isPlasma = true;
			break;

		case PLASMA_SHOTGUN:
			name = "Plasma Shotgun";
			clipSize = 7;
			damage = 50.0f;
			bulletDelay = 0.65f;
			currentSprite = 3;
			isPlasma = true;
			break;

		default:
			name = "Unknown";
			clipSize = 0;
			damage = 0.0f;
			bulletDelay = 1.0f;
			currentSprite = 0;
			break;
		}

		currentClipSize = clipSize;
		reloadClipSize = clipSize * 14;
	}

};



struct Bullet {
	Sprite shape;
	Vector2f velocity;
	float age = 0.0f;
	float lifetime = 2.0f;
	float damage;

	Bullet(float localDamage) {
		damage = localDamage;
	}
};

vector<Bullet> bullets;


Vector2f getDirection(Vector2f from, Vector2f to) {
	Vector2f d = to - from;
	float len = sqrt(d.x * d.x + d.y * d.y);

	return len != 0 ? d / len : Vector2f(0.f, 0.f);
}


Vector2f rotateVector(Vector2f v, float angleDeg) {
	float rad = angleDeg * (PI / 180.f);
	float cosA = cos(rad);
	float sinA = sin(rad);
	return Vector2f(
		v.x * cosA - v.y * sinA,
		v.x * sinA + v.y * cosA
	);
}

Bullet createBullet(Vector2f playerPos, float playerRotationDeg, Weapon currentWeapon, RenderWindow& window) {
	Bullet b(currentWeapon.damage);

	int shellID;
	if (currentWeapon.id == PISTOL || currentWeapon.id == SHOTGUN)
		shellID = 0;
	else if (currentWeapon.id == RIFLE || currentWeapon.id == BURST_RIFLE)
		shellID = 1;
	else if (currentWeapon.id == SINGLE_RIFLE)
		shellID = 2;
	else
		shellID = 3;

	b.shape.setScale(0.5, 0.5);
	b.shape.setTexture(ShellTextures[shellID]);

	b.shape.setOrigin(b.shape.getLocalBounds().width / 2.f, b.shape.getLocalBounds().height / 2.f);

	Vector2f localOffset(20.0f, 18.5f);
	Vector2f rotatedOffset = rotateVector(localOffset, playerRotationDeg);
	Vector2f bulletPos = playerPos + rotatedOffset;
	b.shape.setPosition(bulletPos);

	Vector2f mouseWorld = window.mapPixelToCoords(Mouse::getPosition(window));

	Vector2f dir = getDirection(bulletPos, mouseWorld);

	b.velocity = dir * 800.f;

	float angle = atan2f(dir.y, dir.x) * 180.f / PI;
	b.shape.setRotation(angle + 90);

	b.lifetime = 2.f;

	return b;
}


void updateBullets(float dt) {
	for (int i = 0; i < bullets.size();) {
		bullets[i].shape.move(bullets[i].velocity * dt);
		bullets[i].age += dt;
		if (bullets[i].age >= bullets[i].lifetime)
			bullets.erase(bullets.begin() + i); // remove the bullet from the vector (memory optimization)
		else
			i++;
	}
}


void gui_main() {


	gui[0].loadFromFile("imgs/gui/pistol.png");
	gui[1].loadFromFile("imgs/gui/shotgun.png");
	gui[2].loadFromFile("imgs/gui/rifle.png");
	gui[3].loadFromFile("imgs/gui/hp.png");
	gui[4].loadFromFile("imgs/gui/ammo.png");
	gui[5].loadFromFile("imgs/gui/hud.png");
	gui[6].loadFromFile("imgs/gui/knife.png");

	pistol_gui.setTexture(gui[0]);
	shotgun_gui.setTexture(gui[1]);
	rifle_gui.setTexture(gui[2]);
	hp_gui.setTexture(gui[3]);
	ammo_gui.setTexture(gui[4]);
	knife_gui.setTexture(gui[6]);


	for (int i = 0; i < 3; i++)
	{
		hud_gui[i].setTexture(gui[5]);
	}


	hud_gui[0].setScale(2, 1);
	hud_gui[1].setScale(2, 1);
	hud_gui[2].setScale(4.4, 2.1);



	hp_gui.setScale(2, 2);
	rifle_gui.setScale(2, 2);
	shotgun_gui.setScale(2, 2);
	pistol_gui.setScale(2, 2);
	ammo_gui.setScale(2.2, 2.2);
	knife_gui.setScale(0.3, 0.3);
	knife_gui.setOrigin(knife_gui.getLocalBounds().width / 2, knife_gui.getLocalBounds().height / 2);
	knife_gui.setRotation(25);
	knife_gui.setColor(Color(89, 76, 41)); // Red

	time_left.restart();


	font.loadFromFile("ModernDOS8x14.ttf");
	score.setFont(font);
	zombies.setFont(font);
	health_percentage.setFont(font);
	ammo_in_clip.setFont(font);
	weapon_name.setFont(font);
	timeleft.setFont(font);

	score.setCharacterSize(30);
	health_percentage.setCharacterSize(30);
	zombies.setCharacterSize(17);
	timeleft.setCharacterSize(23);
	weapon_name.setCharacterSize(24);



}


struct PLAYER {

	float  FRAME_WIDTH = 280.0f;
	float  FRAME_HEIGHT = 220.0f;


	float health = 150.0f; // Starting With The full Health (100%)


	CROSSHAIR crosshair;
	Weapon currentWeapon[4];

	Vector2f moveDir;
	bool isMoving;
	float rotation = shape.getRotation();

	int currentWeaponindex = 0;

	float meleeDamage = KNIFE_DAMAGE;
	float lastFireTime = 0;
	float now = 0;

	/*
	   0 : Knife => currentSprite should be 0
	   1 : Pistol (Normal) => currentSprite should be 1
	   2 : Rifle (Automatic) => currentSprite should be 2
	   3 : Rifle (Burst) => currentSprite should be 2
	   4 : Rifle (Single) => currentSprite should be 2
	   5 : Shotgun (Normal) => currentSprite should be 3
	   6 : Pistol (Plasma) => currentSprite should be 1
	   7 : Rifle (Plasma) => currentSprite should be 2
	   8 : Shotgun (Plasma) => currentSprite should be 3
	*/
	//bool isAttackable = false; // This value will be true if the player is in the safe area
	float speed = 5.0f; // speed in one direction
	float diagonalSpeed = abs(speed) / sqrt(2); // speed when moving in diagonal way (cuz we want the magnitude of the vectors = speed to prevent moving faster in diagonal)
	float scaleX; // you should move it on ur own ! , try to make all sprites and their animations with the same scale !
	float scaleY; // you should move it on ur own ! , try to make all sprites and their animations with the same scale !

	Sprite shape; // Our main shape

	Clock fireClock;
	Clock animationClock;
	Clock swtichWeaponClock;

	float animationDelay = 0.025f;
	bool isMeleeAttacked = false;

	int currentState = 1;
	/*
		0 means moving
		1 means idle
		2 means reloading
		3 means meleeattck
	*/

	int movingIndex = 0;
	int idleIndex = 0;

	int reloadIndex = 0;
	int meleeIndex = 0;

	int movingDirection = 1;
	int idleDirection = 1;

	int maxWeapons = 4;
	bool isRushPlayer = false;
	bool isDead = false;
	PLAYER(float x, float y, WeaponID weaponIndex1, WeaponID weaponIndex2, WeaponID weaponIndex3, WeaponID weaponIndex4, int maxWeaponsLocal, RenderWindow& window, float localscaleX = 0.35, float localscaleY = 0.35, bool isRushPlayerLocal = false) : crosshair(crosshairTexture, window) {
		gui_main();
		gameTimer.reset();
		isGameEntered = true;
		maxWeapons = maxWeaponsLocal;
		currentWeapon[0].weaponAdd(weaponIndex1);
		currentWeapon[1].weaponAdd(weaponIndex2);
		currentWeapon[2].weaponAdd(weaponIndex3);
		currentWeapon[3].weaponAdd(weaponIndex4);
		StartOfGame();

		isRushPlayer = isRushPlayerLocal;

		/*

		// first and second for spawning coordinates ,
		third and forth for texture and weapone (Read Up) ,
		fifth and sixth for the scale (it's optional ,u can change it after creating the player (like in changing animations state))

		*/

		scaleX = localscaleX;
		scaleY = localscaleY;
		//shape.
		//shape.setSize(Vector2f(FRAME_WIDTH, FRAME_HEIGHT)); // DON'T CHANGE IT !! , if u want to resize the shape use setScale(); (try to make all sprites with the scale not stretched as u can)
		shape.setScale(Vector2f(scaleX, scaleY));
		shape.setTexture(SpriteTextures[currentWeapon[currentWeaponindex].currentSprite]);
		shape.setPosition(x, y);
		shape.setTextureRect(IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2); // make the origin in the center of the shape
	}

	void changingFrames() {
		if (animationClock.getElapsedTime().asSeconds() < animationDelay)
			return;
		animationClock.restart();

		if (currentState == 0) { // Moving
			movingIndex += movingDirection;
			if (movingIndex >= 19) {
				movingIndex = 18;
				movingDirection = -1;
			}
			else if (movingIndex <= 0) {
				movingIndex = 1;
				movingDirection = 1;
			}
			shape.setTextureRect(IntRect(movingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
		}
		else if (currentState == 1) { // Idle
			idleIndex += idleDirection;
			if (idleIndex >= 19) {
				idleIndex = 18;
				idleDirection = -1;
			}
			else if (idleIndex <= 0) {
				idleIndex = 1;
				idleDirection = 1;
			}
			shape.setTextureRect(IntRect(idleIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
		}
		else if (currentState == 2) { // Reloading

			reloadIndex = (reloadIndex + 1) % 20;
			shape.setTextureRect(IntRect(reloadIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
		}

		else if (currentState == 3) {  // Melee Attack

			if (currentWeapon[currentWeaponindex].currentSprite != 0) {
				meleeIndex = (meleeIndex + 1) % 15;
				shape.setTextureRect(IntRect(meleeIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else {
				meleeIndex = (meleeIndex + 1) % 15;
				shape.setTextureRect(IntRect(meleeIndex * FRAME_WIDTH, (currentState - 1) * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}

		}

	}

	bool reloadMoving() {
		if (reloadIndex > 19) {
			return true;
		}
		return false;

	}
	void changeState(int index) {
		currentState = index;

		/*
			0 == moving sprite
			1 == idle (default)
			2 == reloading
			3 == melee attack
		*/

		if (currentState == 3) {
			if (currentWeapon[currentWeaponindex].currentSprite <= 1) {
				shape.setScale(scaleX + 0.05, scaleY + 0.05);
			}
			else {
				shape.setScale(scaleX + 0.15, scaleY + 0.18);
			}
		}
		else {
			shape.setScale(scaleX, scaleY);

		}

		changingFrames();
	}





	void playerMove() {
		if (currentState == 2 || currentState == 3) {

			speed = 2.0f;
			diagonalSpeed = abs(speed) / sqrt(2);
			//return; // Prevent movement if in reload or melee state
		}

		Vector2f moveDir(0.f, 0.f);
		float rotation = shape.getRotation();

		if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::D)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {1, -1};
			rotation = -45;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::A)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {-1, -1};
			rotation = -135;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::D)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {1, 1};
			rotation = 45;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::A)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {-1, 1};
			rotation = 135;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::W)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {0, -1};
			rotation = -90;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {0, 1};
			rotation = 90;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::A)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {-1, 0};
			rotation = 180;
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			moveDir += {1, 0};
			rotation = 0;
			isMoving = true;
		}

		if (moveDir != Vector2f(0.f, 0.f)) {
			isMoving = true;
			if (!currentState == 2 || !currentState == 3) {
				changeState(0);
			}


			float moveSpeed = (moveDir.x != 0 && moveDir.y != 0) ? diagonalSpeed : speed;
			Vector2f newPos = shape.getPosition() + moveDir * moveSpeed;


			shape.setPosition(newPos);
			bool collided = false;




			if (collided) {
				shape.setPosition(shape.getPosition() - moveDir * moveSpeed);
			}
			else {
				shape.setRotation(rotation);
			}
		}
		else {
			if (!currentState == 2 || !currentState == 3) {
				changeState(1);
			}
		}
	}


	void playerMouse(Vector2f MouseGlobalPos) {
		Vector2f aimDirection = MouseGlobalPos - shape.getPosition();
		shape.setRotation(atan2(aimDirection.y, aimDirection.x) * 180 / PI);
	}


	void switchWeapons() {
		if (swtichWeaponClock.getElapsedTime().asSeconds() < 0.2f) return;
		currentWeaponindex++;
		currentWeaponindex = currentWeaponindex % maxWeapons;
		shape.setTexture(SpriteTextures[currentWeapon[currentWeaponindex].currentSprite]);
		swtichWeaponClock.restart().asSeconds();

	}

	void emotesStarter(Event event) {
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
			if (!(currentState == 2 || currentState == 3) && (currentWeapon[currentWeaponindex].currentSprite != 0) && currentWeapon[currentWeaponindex].reloadClipSize != 0) {
				ReloadShotgun();
				changeState(2);
			}
		}
		if ((event.type == Event::KeyPressed && event.key.code == Keyboard::F) || (currentWeapon[currentWeaponindex].id == KNIFE && (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left))) {
			if (!(currentState == 2 || currentState == 3)) {
				changeState(3);
			}
			if (currentWeapon[currentWeaponindex].id == KNIFE) {

				Knife();

			}
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Q) {
			switchWeapons();
		}
	}


	bool isBursting = false;
	int burstShotsFired = 0;
	float burstDelay = 0.1f;
	float nextBurstTime = 0.0f;


	void shootBullets(RenderWindow& window) {
		if (currentState == 2 || currentState == 3) {
			return; // Prevent shooting if in reload or melee state
		}


		now = fireClock.getElapsedTime().asSeconds();



		if (Mouse::isButtonPressed(Mouse::Left) && ((currentWeapon[currentWeaponindex].clipSize == currentWeapon[currentWeaponindex].currentClipSize) || ((now - lastFireTime) >= currentWeapon[currentWeaponindex].bulletDelay))) {

			if (gameTimer.getTime() <= 1.0f) {
				return;
			}

			Vector2f playerPos = shape.getPosition();
			Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
			Vector2f dir = getDirection(playerPos, mousePos);

			if (currentWeapon[currentWeaponindex].currentClipSize > 0) {
				if (currentWeapon[currentWeaponindex].id == SHOTGUN || currentWeapon[currentWeaponindex].id == PLASMA_SHOTGUN) { // Shotgun
					for (int i = -2; i <= 2; ++i) {
						float spread = i * 9.5f;
						Bullet b = createBullet(playerPos, shape.getRotation(), currentWeapon[currentWeaponindex], window);
						b.shape.rotate(spread);
						b.velocity = rotateVector(b.velocity, spread);
						bullets.push_back(b);
					}
					Shotgun();
					currentWeapon[currentWeaponindex].currentClipSize--;

				}
				else if (currentWeapon[currentWeaponindex].id == BURST_RIFLE || (currentWeapon[currentWeaponindex].id == SINGLE_RIFLE)) {


					if (!isBursting && currentWeapon[currentWeaponindex].currentClipSize >= 3) {
						isBursting = true;
						burstShotsFired = 0;
						nextBurstTime = now;
						PlasmaShotgun();


						currentWeapon[currentWeaponindex].currentClipSize -= 3;
					}
				}
				else {
					bullets.push_back(createBullet(playerPos, shape.getRotation(), currentWeapon[currentWeaponindex], window));
					currentWeapon[currentWeaponindex].currentClipSize--;
				}
			}

			if (currentWeapon[currentWeaponindex].currentClipSize) {
				if (currentWeapon[currentWeaponindex].id == RIFLE) {

					AutoRifle();

				}

				if (currentWeapon[currentWeaponindex].id == PISTOL) {

					Fire();

				}



				if (currentWeapon[currentWeaponindex].id == PLASMA_PISTOL) {

					PlasmaShotgun();

				}

				if (currentWeapon[currentWeaponindex].id == PLASMA_RIFLE) {

					PlasmaRifle();

				}
			}



			lastFireTime = now;

		}
	}


	void endState() {
		if (reloadIndex == 19) {
			int neededBullets = currentWeapon[currentWeaponindex].clipSize - currentWeapon[currentWeaponindex].currentClipSize;
			int bulletsToLoad = min(neededBullets, currentWeapon[currentWeaponindex].reloadClipSize);
			currentWeapon[currentWeaponindex].currentClipSize += bulletsToLoad;
			if (!isRushPlayer) {
				currentWeapon[currentWeaponindex].reloadClipSize -= bulletsToLoad;
			}

			changeState(1);
			reloadIndex = 0;
			speed = 5.0f;
			diagonalSpeed = abs(speed) / sqrt(2);
		}
		if (meleeIndex == 14) {
			changeState(1);
			shape.setScale(scaleX, scaleY);
			meleeIndex = 0;
			speed = 5.0f;
			diagonalSpeed = abs(speed) / sqrt(2);
			isMeleeAttacked = false;
		}
	}

	void update(RenderWindow& window) {
		shootBullets(window);
		playerMove();
		changingFrames();
		endState();
		Vector2f MouseGlobalPos = window.mapPixelToCoords(Mouse::getPosition(window));
		crosshair.mousePosSetter(MouseGlobalPos);
		playerMouse(MouseGlobalPos);


		if (isBursting) {
			float now = fireClock.getElapsedTime().asSeconds();

			if (now >= nextBurstTime && burstShotsFired < 3) {
				Bullet b = createBullet(shape.getPosition(), shape.getRotation(), currentWeapon[currentWeaponindex], window);
				bullets.push_back(b);
				burstShotsFired++;
				nextBurstTime = now + burstDelay;
			}
			if (burstShotsFired >= 3) {
				isBursting = false;
			}
		}
	}




};

void gui_game_loop(vector<PLAYER>& playersArr, RenderWindow& window, bool mission_is_on, bool isRush, int mission1_zombies_counter) {
	score.setString("Score: " + to_string(score_));

	if (!playersArr.empty())
	{
		if (isRush && mission_is_on)
		{
			ammo_in_clip.setString(to_string(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].currentClipSize) + " / ∞");

		}
		else {
			ammo_in_clip.setString(to_string(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].currentClipSize) + " / " + to_string(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].reloadClipSize));
		}
	}
	if (!playersArr.empty())
	{
		health_percentage.setString(to_string((int)(playersArr[0].health * 100) / 150) + " %");
	}
	else
	{
		health_percentage.setString(to_string(0) + " %");

	}

	if (isRush && mission_is_on) {
		zombies.setString("Score: " + to_string(mission1_zombies_counter));
	}
	else {
		zombies.setString("Zombies left: " + to_string(mission1_zombies_counter));
	}

	timeleft.setString("Time Left:" + to_string(120 - (int)gameTimer.getTime()));

	if (!playersArr.empty())
	{
		weapon_name.setString(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].name);
	}





	View currentView = window.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();

	// Position the score at the top-left corner of the view
	score.setPosition(viewCenter.x - viewSize.x / 2 + 1725, viewCenter.y - viewSize.y / 2 + 13); //score

	hud_gui[0].setPosition(viewCenter.x - viewSize.x / 2 + 1700, viewCenter.y - viewSize.y / 2 + 10); //score or timer hud

	hud_gui[1].setPosition(viewCenter.x - viewSize.x / 2 + 10, viewCenter.y - viewSize.y / 2 + 10);  //health hud

	hp_gui.setPosition(viewCenter.x - viewSize.x / 2 + 30, viewCenter.y - viewSize.y / 2 + 20);  //health

	health_percentage.setPosition(viewCenter.x - viewSize.x / 2 + 70, viewCenter.y - viewSize.y / 2 + 13);  //health percentage

	zombies.setPosition(viewCenter.x - viewSize.x / 2 + 1708, viewCenter.y - viewSize.y / 2 + 23);  //zombies left

	timeleft.setPosition(viewCenter.x - viewSize.x / 2 + 1705, viewCenter.y - viewSize.y / 2 + 21);  //time left

	hud_gui[2].setPosition(viewCenter.x - viewSize.x / 2 + 800 - 20, viewCenter.y - viewSize.y / 2 + 980 - 985);  //weapons hud

	rifle_gui.setPosition(viewCenter.x - viewSize.x / 2 + 800, viewCenter.y - viewSize.y / 2 + 0);  //Rifle

	shotgun_gui.setPosition(viewCenter.x - viewSize.x / 2 + 800, viewCenter.y - viewSize.y / 2 + 0);  //Shotgun

	pistol_gui.setPosition(viewCenter.x - viewSize.x / 2 + 800, viewCenter.y - viewSize.y / 2 + 0);  //Pistol

	ammo_in_clip.setPosition(viewCenter.x - viewSize.x / 2 + 1000, viewCenter.y - viewSize.y / 2 + 57);  //ammo left

	ammo_gui.setPosition(viewCenter.x - viewSize.x / 2 + 965, viewCenter.y - viewSize.y / 2 + 70);  //ammo 

	knife_gui.setPosition(viewCenter.x - viewSize.x / 2 + 955, viewCenter.y - viewSize.y / 2 + 50);  //knife 

	weapon_name.setPosition(viewCenter.x - viewSize.x / 2 + 960, viewCenter.y - viewSize.y / 2 + 10);  //weapon name 






}
void gui_draw(bool mission_is_on, bool isRush, RenderWindow& window, vector<PLAYER> playersArr) {
	for (int i = 0; i < 3; i++)
	{
		window.draw(hud_gui[i]);
	}
	window.draw(hp_gui);
	window.draw(health_percentage);
	if (rush)
	{

		window.draw(score);
	}
	else if (mission_is_on)
	{
		window.draw(zombies);

	}
	else
	{
		window.draw(timeleft);
	}
	if (!playersArr.empty())
	{
		if (playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 2 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 3 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 4 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 7)  //////RIFLE
		{
			window.draw(rifle_gui);
			window.draw(ammo_in_clip);
			window.draw(ammo_gui);
			window.draw(weapon_name);
		}
		else if (playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 5 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 8)   //////SHOTGUN
		{
			window.draw(shotgun_gui);
			window.draw(ammo_in_clip);
			window.draw(ammo_gui);
			window.draw(weapon_name);
		}
		else if (playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 1 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 6)   ////// Pistol
		{
			window.draw(pistol_gui);
			window.draw(ammo_in_clip);
			window.draw(ammo_gui);
			window.draw(weapon_name);
		}
		else  //////KNIFE
		{
			window.draw(knife_gui);
		}
	}



}



struct ZOMBIE {

	float FRAME_WIDTH = 318.0f;
	float FRAME_HEIGHT = 311.0f;

	float health = 100;
	float maxHealth;
	float speed = 3.0f;
	float diagonalSpeed = abs(speed) / sqrt(2);

	float damage = 20.0f;

	bool isDeadCounter = false;

	Sprite shape;
	Clock animationClock;
	Clock animationIdleClock;
	Clock attackingClock;
	bool isAttacking = false;
	bool hasDealtDamage = false;
	bool isCompletingAttack = false;

	float animationDelay = 0.05f;
	float animationIdleDelay = 0.15f;

	int currentState = 1;
	/*
		0: Moving
		1: Idle
		2: Attacking
		3: Dying
	*/

	int index;

	int movingIndex = 0;
	int idleIndex = 0;
	int attackIndex = 0;
	int dyingIndex = 0;

	int attackIndexMax;
	bool isFar = true;

	int movingDirection = 1;
	int idleDirection = 1;

	bool isDead = false;

	float zombieAttackDistance = 50.0f;
	float playerAvoidDistance = 40.0f;
	float avoidDistance = 65.0f;

	float scaleX;
	float scaleY;

	Clock zombieDeathTimer;
	bool isTimerTriggerd = false;
	bool shoudBeErased = false;
	bool scoreCounted = false;

	ZOMBIE(float x, float y, int localIndex, float localscaleX = 0.35f, float localscaleY = 0.35f) {
		ZomibeOnBeach();
		index = localIndex;

		if (index == 0) {
			FRAME_WIDTH = 256.0f;
			FRAME_HEIGHT = 256.0f;
			scaleX = 0.85f;
			scaleY = 1.15f;

			zombieAttackDistance = 45.0f;
			playerAvoidDistance = 75.0f;
			health = 100.0f;
			speed = 3.0f;
			diagonalSpeed = abs(speed) / sqrt(2);
			damage = 10.0f;
			attackIndexMax = 19;
		}
		else if (index == 1) {
			scaleX = localscaleX;
			scaleY = localscaleY;
			attackIndexMax = 8;
		}
		else if (index == 2) {
			FRAME_WIDTH = 196.0f;
			FRAME_HEIGHT = 95.0f;;
			shape.rotate(90);
			avoidDistance = 120.0f;
			scaleX = 0.5f;
			scaleY = 0.5f;
			attackIndexMax = 4;
			damage = 30;
			health = 250.0f;
			speed = 5.5f;
			diagonalSpeed = abs(speed) / sqrt(2);
		}
		shape.setScale(Vector2f(scaleX, -scaleY));

		shape.setTexture(ZombieTextures[index]);
		shape.setPosition(x, y);

		shape.setTextureRect(IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2);

		maxHealth = health;
	}

	void changingFrames() {
		if (animationClock.getElapsedTime().asSeconds() < animationDelay)
			return;
		animationClock.restart();

		if (index == 0) {
			if (currentState == 0) {
				++movingIndex;
				movingIndex = movingIndex % 32;
				shape.setTextureRect(IntRect(movingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 1) {
				++idleIndex;
				idleIndex = idleIndex % 32;
				shape.setTextureRect(IntRect(idleIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 2) {
				++attackIndex;
				attackIndex = (attackIndex) % 20;
				shape.setTextureRect(IntRect(attackIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 3) {
				if (dyingIndex < 16) {
					dyingIndex++;
					shape.setTextureRect(IntRect(dyingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
				}
				else {
					isDead = true;
				}
			}
		}
		else if (index == 1) {
			if (currentState == 0) {
				movingIndex += movingDirection;
				if (movingIndex >= 16) {
					movingIndex = 15;
					movingDirection = -1;
				}
				else if (movingIndex <= 0) {
					movingIndex = 1;
					movingDirection = 1;
				}
				shape.setTextureRect(IntRect(movingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 1) {
				idleIndex += idleDirection;
				if (idleIndex >= 16) {
					idleIndex = 15;
					idleDirection = -1;
				}
				else if (idleIndex <= 0) {
					idleIndex = 1;
					idleDirection = 1;
				}
				shape.setTextureRect(IntRect(idleIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 2) {
				attackIndex++;
				attackIndex = attackIndex % 9;
				shape.setTextureRect(IntRect(attackIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 3) {
				if (dyingIndex < 4) {
					dyingIndex++;
					shape.setTextureRect(IntRect(dyingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
				}
				else {
					isDead = true;
				}
			}
		}
		else if (index == 2) {
			if (currentState == 0) {
				attackIndex = 0;
				++movingIndex;
				movingIndex = movingIndex % 4;
				shape.setTextureRect(IntRect(movingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 1) {
				attackIndex = 0;
				if (animationIdleClock.getElapsedTime().asSeconds() < animationIdleDelay)
					return;
				animationIdleClock.restart();

				idleIndex += idleDirection;
				if (idleIndex >= 4) {
					idleIndex = 3;
					idleDirection = -1;
				}
				else if (idleIndex <= 0) {
					idleIndex = 1;
					idleDirection = 1;
				}
				shape.setTextureRect(IntRect(idleIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 2) {
				attackIndex = (attackIndex + 1) % 5;
				shape.setTextureRect(IntRect(attackIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 3) {
				if (dyingIndex < 3) {
					dyingIndex++;
					shape.setTextureRect(IntRect(dyingIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
				}
				else {
					isDead = true;
				}
			}
		}
	}

	void changeState(int index) {
		currentState = index;
		changingFrames();
	}

	void zombieMoveTowards(Vector2f targetPosition, float distance) {
		if ((distance < zombieAttackDistance) || (currentState == 3)) {
			return;
		}
		else {
			if (attackIndex == attackIndexMax && currentState == 2) {
				isFar = true;
				return;
			}
			changeState(0);
		}
		isAttacking = false;

		Vector2f direction = targetPosition - shape.getPosition();
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);

		if (length != 0) {
			direction /= length;
		}

		shape.move(direction.x * speed, direction.y * speed);
		shape.setRotation(atan2(direction.y, direction.x) * 180 / PI);
		changeState(0);
	}

	void zombieAttack() {
		changeState(2);
	}

	void zombieDie() {
		changeState(3);
	}

	Vector2f findNearestPlayerPosition(const Vector2f& zombiePosition, const vector<PLAYER>& players) {
		if (players.empty()) {
			changeState(1);
			return zombiePosition;
		}
		Vector2f nearestPlayerPos = players[0].shape.getPosition();
		float minDistance = sqrt(pow(nearestPlayerPos.x - zombiePosition.x, 2) + pow(nearestPlayerPos.y - zombiePosition.y, 2));

		for (int i = 1; i < players.size(); ++i) {
			Vector2f playerPos = players[i].shape.getPosition();
			float distance = sqrt(pow(playerPos.x - zombiePosition.x, 2) + pow(playerPos.y - zombiePosition.y, 2));

			if (distance < minDistance) {
				minDistance = distance;
				nearestPlayerPos = playerPos;
			}
		}

		return nearestPlayerPos;
	}

	void avoidOtherZombies(vector<ZOMBIE>& zombies) {
		for (int i = 0; i < zombies.size(); ++i) {
			if (&zombies[i] == this || zombies[i].isDead)
				continue;

			Vector2f otherPos = zombies[i].shape.getPosition();
			float dist = sqrt(pow(shape.getPosition().x - otherPos.x, 2) + pow(shape.getPosition().y - otherPos.y, 2));

			if (dist < avoidDistance && dist > 0.0f) {
				Vector2f away = shape.getPosition() - otherPos;
				float length = sqrt(away.x * away.x + away.y * away.y);
				if (length != 0) {
					away /= length;
					shape.move(away.x * 1.5f, away.y * 1.5f);
				}
			}
		}
	}

	void attackingPlayer(vector<PLAYER>& players) {
		bool isPlayerInRange = false;

		// Check if any player is in range during the damage frame
		if (attackIndex == attackIndexMax) {
			for (auto& player : players) {
				float distance = sqrt(pow(shape.getPosition().x - player.shape.getPosition().x, 2) +
					pow(shape.getPosition().y - player.shape.getPosition().y, 2));
				if (distance < zombieAttackDistance + 20.0f) {
					isPlayerInRange = true;
					if (!hasDealtDamage) {
						player.health -= damage;
						Damage();
						hasDealtDamage = true;
					}
					break;
				}
			}
		}

		if (attackIndex == 0) {
			hasDealtDamage = false;
		}
	}

	void drawHealthBar(RenderWindow& window) {
		float barWidth = 50.0f;
		float barHeight = 6.0f;

		Vector2f zombiePos = shape.getPosition();
		float barX = (zombiePos.x - barWidth / 2) + 10.0f;
		float barY = zombiePos.y - shape.getGlobalBounds().height / 2 - 10.0f;

		RectangleShape background(Vector2f(barWidth, barHeight));
		background.setPosition(barX, barY);
		background.setFillColor(Color(80, 80, 80, 200));

		float healthPercent = max(0.0f, health / maxHealth);

		RectangleShape foreground(Vector2f(barWidth * healthPercent, barHeight));
		foreground.setPosition(barX, barY);
		foreground.setFillColor(Color::Red);

		if (!isDead) {
			window.draw(background);
			window.draw(foreground);
		}
	}

	void update(vector<PLAYER>& players, vector<ZOMBIE>& zombies) {
		if (isDead && !isTimerTriggerd) {
			isTimerTriggerd = true;
			zombieDeathTimer.restart().asSeconds();
		}

		if (isDead) {
			if (isTimerTriggerd && zombieDeathTimer.getElapsedTime().asSeconds() >= 10.0f) {
				shoudBeErased = true;
			}
			return;
		}

		if (health <= 0.0f) {
			if (index != 0)
				shape.setScale(scaleX - 0.10, scaleY - 0.10);
			zombieDie();
		}
		else {
			if (players.empty()) {
				changeState(1);
				changingFrames();
				return;
			}

			Vector2f nearestPlayerPos = findNearestPlayerPosition(shape.getPosition(), players);
			float distance = sqrt(pow(shape.getPosition().x - nearestPlayerPos.x, 2) +
				pow(shape.getPosition().y - nearestPlayerPos.y, 2));

			if (currentState == 2) {
				if (distance >= zombieAttackDistance && attackIndex == 0) {
					isCompletingAttack = false;
					zombieMoveTowards(nearestPlayerPos, distance);
				}
				else {
					attackingPlayer(players);
				}
			}
			else if (distance < zombieAttackDistance) {
				changeState(2);
				isCompletingAttack = true;
				attackingPlayer(players);
			}
			else {
				zombieMoveTowards(nearestPlayerPos, distance);
			}
		}

		avoidOtherZombies(zombies);
		changingFrames();
	}



};


struct planes
{
	Texture tex;
	Sprite spr;
	RectangleShape rect[2];
	RectangleShape square;
};

void playersAvoidOtherZombies(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) {
	for (int i = 0; i < playersArr.size(); ++i) {
		Vector2f playerPos = playersArr[i].shape.getPosition();

		for (int j = 0; j < zombiesArr.size(); ++j) {

			if (zombiesArr[j].isDead) continue;

			Vector2f zombiePos = zombiesArr[j].shape.getPosition();
			float dist = sqrt(pow(playerPos.x - zombiePos.x, 2) + pow(playerPos.y - zombiePos.y, 2));

			if (dist < zombiesArr[j].playerAvoidDistance && dist > 0.0f) {
				Vector2f away = playerPos - zombiePos;

				float length = sqrt(away.x * away.x + away.y * away.y);
				if (length != 0) {
					away /= length;
					playersArr[i].shape.move(away.x * 10.5f, away.y * 10.5f);
				}
			}
		}
	}
}


void bulletIntersection(vector<Bullet>& bullets, vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) { // should also take vector of bulidings 
	for (int i = 0; i < bullets.size(); ++i) {
		Vector2f bulletPos = bullets[i].shape.getPosition();
		bool isBulletGone = false;

		for (int j = 0; j < zombiesArr.size(); ++j) {
			if (zombiesArr[j].isDead) continue;

			Vector2f zombiePos = zombiesArr[j].shape.getPosition();
			float dist = sqrt(pow(bulletPos.x - zombiePos.x, 2) + pow(bulletPos.y - zombiePos.y, 2));

			if (dist < 20.0f && dist > 0.0f) {
				Vector2f away = bulletPos - zombiePos;
				zombiesArr[j].health -= bullets[i].damage;

				bullets.erase(bullets.begin() + i);
				isBulletGone = true;
				break;

			}
		}

		if (isBulletGone) continue;

	}
}

void meleeAttack(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) {
	for (int i = 0; i < playersArr.size(); ++i) {
		Vector2f playerPos = playersArr[i].shape.getPosition();

		for (int j = 0; j < zombiesArr.size(); ++j) {

			if (zombiesArr[j].isDead) continue;

			Vector2f zombiePos = zombiesArr[j].shape.getPosition();
			float dist = sqrt(pow(playerPos.x - zombiePos.x, 2) + pow(playerPos.y - zombiePos.y, 2));

			if (dist < zombiesArr[j].playerAvoidDistance + 25.0f && dist > 0.0f) {

				Vector2f away = playerPos - zombiePos;

				float length = sqrt(away.x * away.x + away.y * away.y);

				if (length != 0) {
					away /= length;
					if (playersArr[i].meleeIndex == 13 && !playersArr[i].isMeleeAttacked) {
						zombiesArr[j].health -= playersArr[i].meleeDamage;
						playersArr[i].isMeleeAttacked = true;
					}
				}
			}
		}
	}


}

void updateEntities(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, vector<Bullet>& bullets, Clock zombieDeathTimer, RenderWindow& window, int mission1_zombies_counter, bool mission_is_on = false, bool isRush = false) {

	for (int i = 0; i < playersArr.size(); i++) playersArr[i].update(window);
	for (int i = 0; i < zombiesArr.size(); i++) zombiesArr[i].update(playersArr, zombiesArr);
	playersAvoidOtherZombies(playersArr, zombiesArr);
	bulletIntersection(bullets, playersArr, zombiesArr);
	meleeAttack(playersArr, zombiesArr);
	gui_game_loop(playersArr, window, mission_is_on, isRush, mission1_zombies_counter);
	//cout << "size is " << zombiesArr.size() << endl;

	for (int i = 0; i < zombiesArr.size(); i++) {
		if (zombiesArr[i].shoudBeErased) {
			zombiesArr.erase(zombiesArr.begin() + i);
			--i;
		}
	}
	for (int i = 0; i < playersArr.size(); i++) {
		if (playersArr[i].health <= 0) {
			playersArr.erase(playersArr.begin() + i);
			--i;
		}
	}


}

void drawEntities(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window, bool mission_is_on = false, bool isRush = false) {


	for (int i = 0; i < zombiesArr.size(); ++i) {
		if (zombiesArr[i].isDead)
			window.draw(zombiesArr[i].shape);
	}

	for (int i = 0; i < bullets.size(); i++) {
		window.draw(bullets[i].shape);
	}

	for (int i = 0; i < zombiesArr.size(); ++i) {
		if (!zombiesArr[i].isDead)
			window.draw(zombiesArr[i].shape);
	}

	for (int i = 0; i < playersArr.size(); i++) {
		window.draw(playersArr[i].shape);

	}
	for (int i = 0; i < zombiesArr.size(); ++i) {
		zombiesArr[i].drawHealthBar(window);
	}
	gui_draw(mission_is_on, isRush, window, playersArr);
	for (int i = 0; i < playersArr.size(); i++) {
		window.draw(playersArr[i].crosshair.shape);
	}
}


const int MAX_LINES = 5;
const int MAX_PARTS = 5;

const float gameTimerConst = 120.0f;


struct Beachlevel {

	bool isStoryOn = true;
	Font font;
	Music music;
	Text skipText;
	Text lines[MAX_LINES];
	RectangleShape fade;
	float fadeSpeed = 200.f;
	float fadeAlpha = 255.f;
	Clock clock;
	float charTimer = 0, holdTimer = 0;
	int currentPart = 0, currentChar = 0;
	bool typing = true, holding = false, fadingOut = false, fadingIn = true, sceneSkipped = false;


	string storyParts[MAX_PARTS][MAX_LINES] = {
	{
		"CrimsonLand,",
		"The Island of Beauty, Prosperity and History.",
		"Throughout the ages, it has faced many Disasters ... many Wars",
		"And History has shown how glorious it was."
	},
	{
		"After a Fierce war with the Colonizers,",
		"CrimsonLand Resistance managed to triumph",
		"and declare CrimsonLand Independence..."
	},
	{
		"But the Colonizers evil did not subside.",
		"They developed an epidemic to spread out among the people of the island.",
		"The Plan seems to succeed,",
		"Zombies are now everywhere in the Island."
	},
	{
		"Would CrimsonLand survive this time and save its History",
		"Or it would be a memory of the Past?!!!"
	},
	{
		"You Are Not Expected To Survive....",
		"But You Can",
		"Glory is not inherited. It is earned .. through fire, fate, and will."
	}
	};

	int linesInPart[MAX_PARTS] = { 4, 3, 4, 2, 3 };
	float displayDurations[MAX_PARTS] = { 3.5f, 3.5f, 4.0f, 3.0f, 4.5f };
	float typingSpeeds[MAX_PARTS] = { 0.05f, 0.05f, 0.045f, 0.045f, 0.06f };


	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	bool missionComplete = false;

	CircleShape brokenumbrella, playercollider;

	Clock zombieSpawn;

	const float zombieSpawnTime = 5.0f, zombierushmodespawn = 3.0f;

	Sprite sand, umbrella[5], lounge[5], sea, beachdead;

	Texture beach, umbrellatexture[5], loungetexture, seatexture, beachdeadtexture;






	RectangleShape shapes[9];

	int frameCounter = 0, frameDelay = 20, beachscore = 0, weapon;

	float zombierespawn = 5.0;

	int mission1_zombies_counter = 0;

	int seaindex = 0;

	Text text;

	float totalTimer;

	Clock zombieSpawnClock;

	bool isGameStarted = false;
	int getRandomOutsideRange_x() {

		int left = rand() % 500 - 500;      // -500 to -1

		int right = rand() % 580 + 1921;    // 1921 to 2500



		// Randomly choose between left and right range

		if (rand() % 2 == 0)

			return left;

		else

			return right;

	}

	int getRandomOutsideRange_y() {

		int left = rand() % 500 - 500;       // -500 to -1

		// Randomly choose one of the two ranges

		return left;

	}


	Beachlevel(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window)

	{

		if (!font.loadFromFile("tag.ttf")) {
			cout << "Failed to load font.\n";
		}




		if (!music.openFromFile("introScene.wav")) {
			cout << "Failed to load music.\n";
		}
		music.setLoop(true);
		music.play();

		skipText.setFont(font);
		skipText.setCharacterSize(24);
		skipText.setFillColor(Color(255, 255, 255, 160));


		for (int i = 0; i < MAX_LINES; ++i) {
			lines[i].setFont(font);
			lines[i].setCharacterSize(32);
			lines[i].setFillColor(Color::White);
			lines[i].setPosition(130, 250 + i * 100);
		}

		fade.setSize(Vector2f(1280, 720));
		fade.setFillColor(Color(0, 0, 0, static_cast<Uint8>(fadeAlpha)));






		seatexture.loadFromFile("imgs/beach/sea.png");

		beach.loadFromFile("imgs/beach/sand.png");

		umbrellatexture[0].loadFromFile("imgs/beach/umbrella1.png");

		umbrellatexture[1].loadFromFile("imgs/beach/umbrella2.png");

		umbrellatexture[2].loadFromFile("imgs/beach/umbrella3.png");

		umbrellatexture[3].loadFromFile("imgs/beach/umbrella4.png");

		umbrellatexture[4].loadFromFile("imgs/beach/umbrella5.png");

		loungetexture.loadFromFile("imgs/beach/lounge.png");

		//font.loadFromFile("imgs/Caliste.ttf");

		sand.setTexture(beach);

		sea.setTexture(seatexture);

		sand.setOrigin(sand.getLocalBounds().width / 2, sand.getLocalBounds().height / 2);

		sand.setScale(1.875, 1.791044776119403);

		brokenumbrella.setRadius(70);

		playercollider.setRadius(40);

		for (int i = 0; i < 5; i++)

		{

			umbrella[i].setTexture(umbrellatexture[i]);

			umbrella[i].setScale(0.5, 0.5);

		}

		for (int i = 0; i < 5; i++)

		{

			lounge[i].setTexture(loungetexture);

			lounge[i].setScale(0.4, 0.4);

			lounge[i].setOrigin(lounge[i].getLocalBounds().width / 2, lounge[i].getLocalBounds().height / 2);

		}

		for (int i = 0; i < 5; i++)

		{

			shapes[i].setSize(Vector2f(55, 120));

			shapes[i].setOrigin(shapes[i].getLocalBounds().width / 2, shapes[i].getLocalBounds().height / 2);

		}

		for (int i = 5; i < 7; i++)

		{

			shapes[i].setSize(Vector2f(1920, 10));

			shapes[i].setOrigin(shapes[i].getLocalBounds().width / 2, shapes[i].getLocalBounds().height / 2);

		}

		for (int i = 7; i < 9; i++)

		{

			shapes[i].setSize(Vector2f(10, 1080));

			shapes[i].setOrigin(shapes[i].getLocalBounds().width / 2, shapes[i].getLocalBounds().height / 2);

		}



		sand.setPosition(960, 540);

		sand.setScale(1.875, 1.791044776119403);

		sea.setPosition(0, 550);

		brokenumbrella.setPosition(490, 570);

		shapes[5].setPosition(960, 1000); //lower border

		shapes[6].setPosition(960, 0); //upper border

		shapes[7].setPosition(1920, 540); //right border

		shapes[8].setPosition(0, 540); //left border



		umbrella[0].setPosition(0, 100);

		umbrella[1].setPosition(400, 550);

		umbrella[2].setPosition(1600, 500);

		umbrella[3].setPosition(1300, 200);

		umbrella[4].setPosition(600, 175);



		lounge[0].setPosition(120, 235);

		lounge[1].setPosition(1811, 650);

		lounge[2].setPosition(1711, 650);

		lounge[3].setPosition(1411, 325);

		lounge[4].setPosition(711, 300);



		shapes[0].setPosition(120, 235);

		shapes[1].setPosition(1811, 650);

		shapes[2].setPosition(1711, 650);

		shapes[3].setPosition(1411, 325);

		shapes[4].setPosition(711, 300);











		for (int i = 0; i < playersArr.size(); i++)

		{

			playersArr[i].shape.setPosition(960, 900);

		}




	}


	void Beachupdate(RenderWindow& window, vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr)

	{


		frameCounter++;

		if (frameCounter >= frameDelay) {

			seaindex = (seaindex + 1) % 4;

			frameCounter = 0;  // Reset counter after updating

		}

		sea.setTextureRect(IntRect(seaindex * 1920, 0, 1920, 750));





		for (int i = 0; i < playersArr.size(); i++) //playercolliderposition

		{

			playercollider.setPosition(playersArr[i].shape.getPosition().x - 40, playersArr[i].shape.getPosition().y - 37);

		}


		if (!playersArr.empty())

		{

			for (int i = 0; i < zombiesArr.size(); i++) ////////////////////////////////loop for the zombies

			{

				for (int j = 0; j < 5; j++) {

					FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //zombie bounding box

					FloatRect intersection; //intersection area

					FloatRect Wall_bound = shapes[j].getGlobalBounds(); //intersected object

					if (zombie_bounds.intersects(Wall_bound))

					{



						zombie_bounds.intersects(Wall_bound, intersection);

						// left/right

						if (intersection.width < intersection.height)

						{

							//right collision

							if (zombie_bounds.left < Wall_bound.left)

							{

								zombiesArr[i].shape.move(-zombiesArr[i].speed, 0);

							}

							//left collision

							else

							{

								zombiesArr[i].shape.move(zombiesArr[i].speed, 0);

							}

						}

						// up/down

						else

						{

							//down collision

							if (zombie_bounds.top < Wall_bound.top)

							{

								zombiesArr[i].shape.move(0, -zombiesArr[i].speed);

							}

							//up collision

							else

							{

								zombiesArr[i].shape.move(0, zombiesArr[i].speed);

							}

						}

					}

				}

				FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //zombie bounding box

				FloatRect intersection; //intersection area

				FloatRect Wall_bound = brokenumbrella.getGlobalBounds(); //intersected object

				if (zombie_bounds.intersects(Wall_bound))

				{



					zombie_bounds.intersects(Wall_bound, intersection);

					// left/right

					if (intersection.width < intersection.height)

					{

						//right collision

						if (zombie_bounds.left < Wall_bound.left)

						{

							zombiesArr[i].shape.move(-zombiesArr[i].speed, 0);

						}

						//left collision

						else

						{

							zombiesArr[i].shape.move(zombiesArr[i].speed, 0);

						}

					}

					// up/down

					else

					{

						//down collision

						if (zombie_bounds.top < Wall_bound.top)

						{

							zombiesArr[i].shape.move(0, -zombiesArr[i].speed);

						}

						//up collision

						else

						{

							zombiesArr[i].shape.move(0, zombiesArr[i].speed);

						}

					}

				}

			}



			for (int i = 0; i < 9; i++) //////////////////////////////////////////////////////////loop for the player

			{

				FloatRect Player_Bounds = playercollider.getGlobalBounds(); //player bounding box

				FloatRect intersection; //intersection area

				FloatRect Wall_bound = shapes[i].getGlobalBounds(); //intersected object

				if (Player_Bounds.intersects(Wall_bound))

				{



					Player_Bounds.intersects(Wall_bound, intersection);

					// left/right

					if (intersection.width < intersection.height)

					{

						//right collision

						if (Player_Bounds.left < Wall_bound.left)

						{

							playersArr[0].shape.move(-playersArr[0].speed, 0);

						}

						//left collision

						else

						{

							playersArr[0].shape.move(playersArr[0].speed, 0);

						}

					}

					// up/down

					else

					{

						//down collision

						if (Player_Bounds.top < Wall_bound.top)

						{

							playersArr[0].shape.move(0, -playersArr[0].speed);

						}

						//up collision

						else

						{

							playersArr[0].shape.move(0, playersArr[0].speed);

						}

					}

				}

			}



			FloatRect Player_Bounds = playercollider.getGlobalBounds(); //player bounding box

			FloatRect intersection; //intersection area

			FloatRect Wall_bound = brokenumbrella.getGlobalBounds(); //intersected object

			if (Player_Bounds.intersects(Wall_bound))

			{



				Player_Bounds.intersects(Wall_bound, intersection);

				// left/right

				if (intersection.width < intersection.height)

				{

					//right collision

					if (Player_Bounds.left < Wall_bound.left)

					{

						playersArr[0].shape.move(-playersArr[0].speed, 0);

					}

					//left collision

					else

					{

						playersArr[0].shape.move(playersArr[0].speed, 0);

					}

				}

				// up/down

				else

				{

					//down collision

					if (Player_Bounds.top < Wall_bound.top)

					{

						playersArr[0].shape.move(0, -playersArr[0].speed);

					}

					//up collision

					else

					{

						playersArr[0].shape.move(0, playersArr[0].speed);

					}

				}

			}

		}




	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window)
	{
		totalTimer = gameTimer.getTime();
		dt = deltaClock.restart().asSeconds();

		if (isStoryOn) {

			if (Keyboard::isKeyPressed(Keyboard::Space) && !sceneSkipped && !pauseGame) {
				sceneSkipped = true;
				fadingOut = true;
				music.stop();
				// Clear all text immediately when skipped
				for (int i = 0; i < MAX_LINES; ++i) {
					lines[i].setString("");
				}
			}


			// Only show skip text if not already skipping
			if (!sceneSkipped && !fadingOut) {
				skipText.setString("Press Space to Skip");
				skipText.setFillColor(Color(255, 255, 255, 160));
				skipText.setPosition(window.getSize().x - 450, window.getSize().y - 50);
			}
			else {
				skipText.setString(""); // Hide skip text when skipping
			}

			// Story progression logic
			if (!sceneSkipped) {
				if (!fadingOut && !fadingIn && typing) {
					charTimer += dt;
					if (charTimer >= typingSpeeds[currentPart]) {
						charTimer = 0;
						bool allComplete = true;
						for (int i = 0; i < linesInPart[currentPart]; ++i) {
							if (lines[i].getString().getSize() < storyParts[currentPart][i].size()) {
								string s = storyParts[currentPart][i].substr(0, lines[i].getString().getSize() + 1);
								lines[i].setString(s);
								allComplete = false;
								break;
							}
						}
						if (allComplete) {
							typing = false;
							holding = true;
							holdTimer = 0;
						}
					}
				}

				if (holding) {
					holdTimer += dt;
					if (holdTimer >= displayDurations[currentPart]) {
						holding = false;
						fadingOut = true;
					}
				}
			}

			// Fade transitions
			if (fadingOut) {
				fadeAlpha += fadeSpeed * dt;
				if (fadeAlpha >= 255.f) {
					fadeAlpha = 255.f;
					if (sceneSkipped || currentPart >= MAX_PARTS - 1) {
						isStoryOn = false;
						music.stop();
						// Reset for potential reuse
						sceneSkipped = false;
						fadingOut = false;
					}
					else {
						currentPart++;
						currentChar = 0;
						typing = true;
						fadingOut = false;
						fadingIn = true;
						for (int i = 0; i < MAX_LINES; ++i)
							lines[i].setString("");
					}
				}
				fade.setFillColor(Color(0, 0, 0, static_cast<Uint8>(fadeAlpha)));
			}

			if (fadingIn && !fadingOut) {
				fadeAlpha -= fadeSpeed * dt;
				if (fadeAlpha <= 0.f) {
					fadeAlpha = 0.f;
					fadingIn = false;
				}
				fade.setFillColor(Color(0, 0, 0, static_cast<Uint8>(fadeAlpha)));
			}

			// Final part styling
			if (currentPart == MAX_PARTS - 1 && !sceneSkipped) {
				if (linesInPart[currentPart] >= 2)
					lines[1].setFillColor(Color(25, 25, 30));
				if (linesInPart[currentPart] >= 3) {
					lines[2].setFillColor(Color::Red);
					lines[2].setPosition(350, 650);
				}
			}
			return;
		}

		if (!isGameStarted) {
			playersArr.push_back(PLAYER(rand() % 1920, rand() % 1080, KNIFE, KNIFE, RIFLE, SHOTGUN, 1, window));
			isGameStarted = true;
		}
		if (!missionComplete) {
			updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
			updateBullets(dt);







			if (gameTimer.getTime() >= gameTimerConst) {
				missionComplete = true;
			}

			if (gameTimer.getTime() <= gameTimerConst && !playersArr.empty()) {
				if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime) {
					zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));
					mission1_zombies_counter++;
					zombieSpawn.restart();
				}
			}
		}

		Beachupdate(window, playersArr, zombiesArr);
	}

	void beachrushmode(RenderWindow& window, vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr)

	{

		if (!playersArr.empty()) //spawn zombie every 3 seconds

		{

			if (zombieSpawn.getElapsedTime().asSeconds() >= zombierushmodespawn) {

				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));

				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));

				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));

				zombieSpawn.restart();

			}

		}

		// Increment score for zombies that are dead and not yet counted

		for (int i = 0; i < zombiesArr.size(); i++) {

			if (zombiesArr[i].isDead && !zombiesArr[i].scoreCounted) {

				beachscore += 5; // Increment score

				zombiesArr[i].scoreCounted = true;// Mark the zombie as processed for scoring

			}

		}

		Beachupdate(window, playersArr, zombiesArr);

		text.setString("Score: " + to_string(beachscore));

		window.draw(text);

	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {

		if (isStoryOn) {
			for (int i = 0; i < linesInPart[currentPart]; ++i) {
				window.draw(lines[i]);
			}
			// Only draw skip text if not fading and not skipped
			if (!fadingOut && !sceneSkipped) {
				window.draw(skipText);
			}
			if (fadeAlpha > 0) {
				window.draw(fade);
			}
			return;
		}

		window.draw(sand);
		window.draw(sea);
		for (int i = 0; i < 5; i++)

		{

			window.draw(lounge[i]); //draw the lounges

		}
		window.draw(umbrella[1]); //draw the brokenumbrella
		drawEntities(playersArr, zombiesArr, window);

		for (int i = 0; i < 5; i++)

		{

			if (i == 1)

			{

				continue;

			}

			window.draw(umbrella[i]);  //draw the umbrellas

		}



		for (int i = 0; i < playersArr.size(); i++) {

			window.draw(playersArr[i].crosshair.shape);//crosshair to be on top

		}
		if (playersArr.empty())

		{

			window.draw(beachdead);

		}

	}
};//beach l
struct Desertroad {
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	const float zombieSpawnTime = 3.0f;
	int intial_num = 10;
	int maxZomies = 100;
	int mapWidth = 2000;
	int mapHeight = 1500;
	int mission1_zombies_counter = 0;
	Clock timer;
	Sprite backgroundDesertRoadSprite;

	Desertroad(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, KNIFE, PISTOL, RIFLE, PLASMA_SHOTGUN, 2, window));
		for (int i = 0; i < intial_num; i++) {
			float x, y;
			int side = rand() % 3;  // 0 -> top , 1 -> right, 2 -> bottom, 3 -> left
			switch (side) {
			case 0: // Top
				x = rand() % mapWidth + 500;
				y = 0;
				break;
			case 1: // Right
				x = mapWidth;
				y = rand() % mapHeight + 250;
				break;
			case 2: // Bottom
				x = rand() % mapWidth + 500;
				y = mapHeight;
				break;
			case 3: // Left
				x = 0;
				y = rand() % mapHeight + 250;
				break;
			}
			zombiesArr.push_back(ZOMBIE(x, y, 1));
		}
		//background

	 // background Sprite
		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background
		backgroundDesertRoadSprite.setScale(
			mapWidth / backgroundDesertRoad.getSize().x,
			mapHeight / backgroundDesertRoad.getSize().y
		);
		// timer and view and point after 2 min
	};
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		View view(window.getDefaultView());

		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}
		//spawn zombie every 3 sec
		else if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime && zombiesArr.size() < maxZomies) {
			float x, y;
			int side = rand() % 4;
			switch (side) {
			case 0: // Top
				x = rand() % (int)2000;
				y = 0;
				break;
			case 1: // Right
				x = mapWidth;
				y = rand() % (int)1500;
				break;
			case 2: // Bottom
				x = rand() % (int)2000;
				y = mapHeight;
				break;
			case 3: // Left
				x = 0;
				y = rand() % (int)1500;
				break;
			}
			zombiesArr.push_back(ZOMBIE(x, y, 1));
			mission1_zombies_counter++;
			zombieSpawn.restart();
		}

		if (!playersArr.empty()) {
			//camera view max view and min view to track player
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;
			float clampedX = max(halfWidth, min(mapWidth - halfWidth, playerPos.x));
			float clampedY = max(halfHeight, min(mapHeight - halfHeight, playerPos.y));
			view.setCenter(clampedX, clampedY);
			window.setView(view);
		}

		for (int i = 0; i < playersArr.size(); i++) {
			Vector2f playerPos = playersArr[i].shape.getPosition();

			const float mapWidth = 2000.0f;
			const float mapHeight = 1500.0f;

			FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
			float halfWidth = playerBounds.width / 2.0f;
			float halfHeight = playerBounds.height / 2.0f;

			if (playerPos.x < halfWidth) playerPos.x = halfWidth;
			if (playerPos.x > mapWidth - halfWidth) playerPos.x = mapWidth - halfWidth;
			if (playerPos.y < halfHeight) playerPos.y = halfHeight;
			if (playerPos.y > mapHeight - halfHeight) playerPos.y = mapHeight - halfHeight;

			playersArr[i].shape.setPosition(playerPos);
		}
	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundDesertRoadSprite);
		drawEntities(playersArr, zombiesArr, window);

	};
};
struct City {
	Map map;
	Traffic traffic;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	bool missionComplete = false;
	Clock Timer;
	float zombie_respawntime = 5.f;
	int zombie_perspawn = 2;
	int totalzombiekilled = 0;
	Clock zombieSpawnClock;
	float zombieSpawnTime = 2.5f;

	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}

	City(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(rand() % 1920, rand() % 1080, PISTOL, KNIFE, SHOTGUN, SHOTGUN, 2, window));

		map.init(buildingTexture);
		traffic.init(cara, carb);
	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}

		if (zombieSpawnClock.getElapsedTime().asSeconds() >= zombie_respawntime &&
			!missionComplete)
		{
			zombieSpawnClock.restart();
			for (int i = 0; i < zombie_perspawn; i++) {

				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));
			}
		}

		for (int i = 0; i < playersArr.size(); i++) {
			for (int j = 0; j < NUM_BUILDINGS; j++) {
				FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
				FloatRect buildBounds = map.buildings[j].spriteB.getGlobalBounds();

				if (playerBounds.intersects(buildBounds)) {
					float overlapX = min(playerBounds.left + playerBounds.width - buildBounds.left,
						buildBounds.left + buildBounds.width - playerBounds.left);
					float overlapY = min(playerBounds.top + playerBounds.height - buildBounds.top,
						buildBounds.top + buildBounds.height - playerBounds.top);

					Vector2f pushDir(0.f, 0.f);
					Vector2f playerPos = playersArr[i].shape.getPosition();

					if (overlapX < overlapY) {
						if (playerPos.x < buildBounds.left + buildBounds.width / 2) {
							pushDir.x = -overlapX;
						}
						else {
							pushDir.x = overlapX;
						}
					}
					else {
						if (playerPos.y < buildBounds.top + buildBounds.height / 2) {
							pushDir.y = -overlapY;
						}
						else {
							pushDir.y = overlapY;
						}
					}

					playersArr[i].shape.setPosition(playerPos + pushDir);
					break;
				}
			}
		}

		for (int i = 0; i < zombiesArr.size(); i++) {
			for (int j = 0; j < NUM_BUILDINGS; j++) {
				FloatRect playerBounds = zombiesArr[i].shape.getGlobalBounds();
				FloatRect buildBounds = map.buildings[j].spriteB.getGlobalBounds();

				if (playerBounds.intersects(buildBounds)) {
					float overlapX = min(playerBounds.left + playerBounds.width - buildBounds.left,
						buildBounds.left + buildBounds.width - playerBounds.left);
					float overlapY = min(playerBounds.top + playerBounds.height - buildBounds.top,
						buildBounds.top + buildBounds.height - playerBounds.top);

					Vector2f pushDir(0.f, 0.f);
					Vector2f playerPos = zombiesArr[i].shape.getPosition();

					if (overlapX < overlapY) {
						if (playerPos.x < buildBounds.left + buildBounds.width / 2) {
							pushDir.x = -overlapX;
						}
						else {
							pushDir.x = overlapX;
						}
					}
					else {
						if (playerPos.y < buildBounds.top + buildBounds.height / 2) {
							pushDir.y = -overlapY;
						}
						else {
							pushDir.y = overlapY;
						}
					}

					zombiesArr[i].shape.setPosition(playerPos + pushDir);
					break;
				}
			}
		}

		for (int i = 0; i < playersArr.size(); i++) {
			for (int j = 0; j < NUM_CARS; j++) {
				FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
				playerBounds.left += playerBounds.width * 0.1f;
				playerBounds.top += playerBounds.height * 0.1f;
				playerBounds.width *= 0.8f;
				playerBounds.height *= 0.8f;

				FloatRect trafficBounds = traffic.cars[j].spriteC.getGlobalBounds();
				trafficBounds.left += trafficBounds.width * 0.2f;
				trafficBounds.top += trafficBounds.height * 0.2f;
				trafficBounds.width *= 0.6f;
				trafficBounds.height *= 0.6f;

				if (playerBounds.intersects(trafficBounds)) {
					float overlapX = min(playerBounds.left + playerBounds.width - trafficBounds.left,
						trafficBounds.left + trafficBounds.width - playerBounds.left);
					float overlapY = min(playerBounds.top + playerBounds.height - trafficBounds.top,
						trafficBounds.top + trafficBounds.height - playerBounds.top);

					Vector2f pushDir(0.f, 0.f);
					Vector2f playerPos = playersArr[i].shape.getPosition();

					if (overlapX < overlapY) {
						pushDir.x = (playerPos.x < trafficBounds.left + trafficBounds.width / 2) ? -overlapX : overlapX;
					}
					else {
						pushDir.y = (playerPos.y < trafficBounds.top + trafficBounds.height / 2) ? -overlapY : overlapY;
					}

					playersArr[i].shape.setPosition(playerPos + pushDir);
					break;
				}
			}
		}

		for (int i = 0; i < zombiesArr.size(); i++) {
			for (int j = 0; j < NUM_CARS; j++) {
				FloatRect playerBounds = zombiesArr[i].shape.getGlobalBounds();
				playerBounds.left += playerBounds.width * 0.1f;
				playerBounds.top += playerBounds.height * 0.1f;
				playerBounds.width *= 0.8f;
				playerBounds.height *= 0.8f;

				FloatRect trafficBounds = traffic.cars[j].spriteC.getGlobalBounds();
				trafficBounds.left += trafficBounds.width * 0.2f;
				trafficBounds.top += trafficBounds.height * 0.2f;
				trafficBounds.width *= 0.6f;
				trafficBounds.height *= 0.6f;

				if (playerBounds.intersects(trafficBounds)) {
					float overlapX = min(playerBounds.left + playerBounds.width - trafficBounds.left,
						trafficBounds.left + trafficBounds.width - playerBounds.left);
					float overlapY = min(playerBounds.top + playerBounds.height - trafficBounds.top,
						trafficBounds.top + trafficBounds.height - playerBounds.top);

					Vector2f pushDir(0.f, 0.f);
					Vector2f playerPos = zombiesArr[i].shape.getPosition();

					if (overlapX < overlapY) {
						pushDir.x = (playerPos.x < trafficBounds.left + trafficBounds.width / 2) ? -overlapX : overlapX;
					}
					else {
						pushDir.y = (playerPos.y < trafficBounds.top + trafficBounds.height / 2) ? -overlapY : overlapY;
					}

					zombiesArr[i].shape.setPosition(playerPos + pushDir);
					break;
				}
			}
		}



		for (int j = 0; j < NUM_CARS; j++) {

			for (int i = 0; i < bullets.size(); i++) {

			}

			for (int i = 0; i < bullets.size(); i++) {
				FloatRect bounds = traffic.cars[j].spriteC.getGlobalBounds();
				bounds.width *= 0.2f;
				bounds.height *= 0.2f;
				bounds.left += traffic.cars[j].spriteC.getGlobalBounds().width * 0.4f;
				bounds.top += traffic.cars[j].spriteC.getGlobalBounds().height * 0.4f;

				if (bullets[i].shape.getGlobalBounds().intersects(bounds)) {
					bullets.erase(bullets.begin() + i);
					i--;
				}
			}
		}

		if (!playersArr.empty())
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, playersArr[0].speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1050) //check for bottom border
			{
				playersArr[0].shape.move(0, -playersArr[0].speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(playersArr[0].speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1920) //check for right border
			{
				playersArr[0].shape.move(-playersArr[0].speed, 0);
			}
		}
	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {

		draw_background(window, back_ground, 0, 0);
		draw_background(window, back_ground, 1000, 0);
		traffic.drawCars(window);
		map.drawBuildings(window);
		drawEntities(playersArr, zombiesArr, window);

	}
};
struct Safezone1 {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	TrainRails TRAINRAILS;
	Trains Trainss;
	decorations decoration;

	bool missionComplete = false;
	Clock Timer;
	float zombie_respawntime = 4.f;
	int zombie_perspawn = 3;
	int totalzombiekilled = 0;
	Clock zombieSpawnClock;

	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}

	Safezone1(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(rand() % 1920, rand() % 1080, RIFLE, PISTOL, KNIFE, SHOTGUN, 3, window));

		TRAINRAILS.RAILS(trainRailsTexture);
		Trainss.TRAINSS(trainTexture);
		decoration.DECORATIONS(wellTexture, treeTexture);
	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}

		if (zombieSpawnClock.getElapsedTime().asSeconds() >= zombie_respawntime && !missionComplete)
		{
			zombieSpawnClock.restart();
			for (int i = 0; i < zombie_perspawn; i++) {

				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));
			}
		}
		for (int j = 0; j < NUM_TRAIN; j++) {
			for (int k = 0; k < bullets.size(); k++) {
				if (bullets[k].shape.getGlobalBounds().intersects(Trainss.TRAINS[j].trainS.getGlobalBounds())) {
					bullets.erase(bullets.begin() + k);
					k--;
				}
			}
		}

		for (int i = 0; i < playersArr.size(); i++) {
			for (int j = 0; j < NUM_TRAIN; j++) {
				FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
				FloatRect trainBounds = Trainss.TRAINS[j].trainS.getGlobalBounds();

				if (playerBounds.intersects(trainBounds)) {
					float overlapX = min(playerBounds.left + playerBounds.width - trainBounds.left,
						trainBounds.left + trainBounds.width - playerBounds.left);
					float overlapY = min(playerBounds.top + playerBounds.height - trainBounds.top,
						trainBounds.top + trainBounds.height - playerBounds.top);

					Vector2f pushDir(0.f, 0.f);
					Vector2f playerPos = playersArr[i].shape.getPosition();

					if (overlapX < overlapY) {
						if (playerPos.x < trainBounds.left + trainBounds.width / 2) {
							pushDir.x = -overlapX;
						}
						else {
							pushDir.x = overlapX;
						}
					}
					else {
						if (playerPos.y < trainBounds.top + trainBounds.height / 2) {
							pushDir.y = -overlapY;
						}
						else {
							pushDir.y = overlapY;
						}
					}

					playersArr[i].shape.setPosition(playerPos + pushDir);
					break;
				}
			}
		}
		for (int i = 0; i < zombiesArr.size(); i++) {
			for (int j = 0; j < NUM_TRAIN; j++) {
				FloatRect playerBounds = zombiesArr[i].shape.getGlobalBounds();
				FloatRect trainBounds = Trainss.TRAINS[j].trainS.getGlobalBounds();

				if (playerBounds.intersects(trainBounds)) {
					float overlapX = min(playerBounds.left + playerBounds.width - trainBounds.left,
						trainBounds.left + trainBounds.width - playerBounds.left);
					float overlapY = min(playerBounds.top + playerBounds.height - trainBounds.top,
						trainBounds.top + trainBounds.height - playerBounds.top);

					Vector2f pushDir(0.f, 0.f);
					Vector2f playerPos = zombiesArr[i].shape.getPosition();

					if (overlapX < overlapY) {
						if (playerPos.x < trainBounds.left + trainBounds.width / 2) {
							pushDir.x = -overlapX;
						}
						else {
							pushDir.x = overlapX;
						}
					}
					else {
						if (playerPos.y < trainBounds.top + trainBounds.height / 2) {
							pushDir.y = -overlapY;
						}
						else {
							pushDir.y = overlapY;
						}
					}

					zombiesArr[i].shape.setPosition(playerPos + pushDir);
					break;
				}
			}
		}
		if (!playersArr.empty())
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, playersArr[0].speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1050) //check for bottom border
			{
				playersArr[0].shape.move(0, -playersArr[0].speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(playersArr[0].speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1920) //check for right border
			{
				playersArr[0].shape.move(-playersArr[0].speed, 0);
			}
		}
	}



	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		draw_backgroundT(window, back_groundT, 0, 0);

		TRAINRAILS.drawtrains(window);
		Trainss.drawrails(window);
		//decoration.drawdecorations(window);
		drawEntities(playersArr, zombiesArr, window);


	}
};
struct Safezone2 {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	RectangleShape treeColliders[15];
	Sprite background;
	Sprite tree[15];

	Clock Timer;
	bool missionComplete = false;

	Clock zombieSpawnClock;
	float zombieSpawnTime = 1.5f;
	float timer = 120.0f;


	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		// Randomly choose between left and right range
		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		// Randomly choose one of the two ranges
		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}

	Safezone2(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, PLASMA_RIFLE, PLASMA_RIFLE, PLASMA_RIFLE, 4, window));

		background.setTexture(mission4BackgroundTexture);
		background.setScale(
			(float)window.getSize().x / mission4BackgroundTexture.getSize().x,
			(float)window.getSize().y / mission4BackgroundTexture.getSize().y
		);
		tree[0].setPosition(150.f, 200.f);
		tree[1].setPosition(200.f, 220.f);
		tree[2].setPosition(750.f, 220.f);
		tree[3].setPosition(1050.f, 190.f);

		tree[5].setPosition(250.f, 500.f);
		tree[6].setPosition(550.f, 520.f);

		tree[8].setPosition(1150.f, 510.f);
		tree[9].setPosition(1450.f, 490.f);
		tree[10].setPosition(350.f, 800.f);

		tree[12].setPosition(950.f, 780.f);
		tree[13].setPosition(1000.f, 830.f);
		tree[14].setPosition(1550.f, 790.f);

		for (int i = 0; i < 15; ++i) {
			tree[i].setTexture(mission4PalmTree);
			tree[i].setOrigin(tree[i].getLocalBounds().width / 2, tree[i].getLocalBounds().height / 2);
			treeColliders[i].setOrigin(treeColliders[i].getLocalBounds().width / 2, treeColliders[i].getLocalBounds().height / 2);
			treeColliders[i].setPosition(tree[i].getPosition());
			treeColliders[i].setSize(Vector2f(5, 5));

		}

	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		if (zombieSpawnClock.getElapsedTime().asSeconds() >= zombieSpawnTime) {

			zombieSpawnClock.restart();



			zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));
		}
		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}

		for (int i = 0; i < playersArr.size(); i++) {
			for (int j = 0; j < 15; j++) {


				Vector2f otherPos = treeColliders[j].getPosition();
				float dist = sqrt(pow(playersArr[i].shape.getPosition().x - otherPos.x, 2) + pow(playersArr[i].shape.getPosition().y - otherPos.y, 2));

				if (dist < 50.0f && dist > 0.0f) {
					Vector2f away = playersArr[i].shape.getPosition() - otherPos;

					float length = sqrt(away.x * away.x + away.y * away.y);
					if (length != 0) {
						away /= length;
						playersArr[i].shape.move(away.x * 10.5f, away.y * 10.5f);
					}
				}



				for (int k = 0; k < bullets.size(); k++)
				{
					if (bullets[k].shape.getGlobalBounds().intersects(treeColliders[j].getGlobalBounds()))
					{
						bullets.erase(bullets.begin() + k);
						k--;
					}
				}

			}
		}
		for (int i = 0; i < zombiesArr.size(); i++) {

			for (int j = 0; j < 15; j++) {
				Vector2f otherPos = treeColliders[j].getPosition();
				float dist = sqrt(pow(zombiesArr[i].shape.getPosition().x - otherPos.x, 2) +
					pow(zombiesArr[i].shape.getPosition().y - otherPos.y, 2));

				if (dist < 50.0f && dist > 0.0f) {
					Vector2f away = zombiesArr[i].shape.getPosition() - otherPos;
					float length = sqrt(away.x * away.x + away.y * away.y);
					if (length != 0) {
						away /= length;
						zombiesArr[i].shape.move(away.x * 10.5f, away.y * 10.5f);
					}
				}
			}
		}
		if (!playersArr.empty())
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, playersArr[0].speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1050) //check for bottom border
			{
				playersArr[0].shape.move(0, -playersArr[0].speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(playersArr[0].speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1920) //check for right border
			{
				playersArr[0].shape.move(-playersArr[0].speed, 0);
			}
		}
	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(background);

		for (int i = 0; i < 15; i++) {
			window.draw(tree[i]);

		}
		drawEntities(playersArr, zombiesArr, window);

	}
};
struct Woods {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	bool missionComplete = false;
	Clock Timer;
	float zombie_respawntime = 4.5f;
	int zombie_perspawn = 2;
	int dog_perspawn = 1;
	int totalzombiekilled = 0;
	Clock zombieSpawnClock;
	float zombieSpawnTime = 2.5f;
	Sprite backgroundWoods;
	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}

	Woods(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(rand() % 1920, rand() % 1080, RIFLE, PISTOL, KNIFE, SHOTGUN, 3, window));
		backgroundWoods.setTexture(woodsBackgroundTexture);

	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}


		if (zombieSpawnClock.getElapsedTime().asSeconds() >= zombie_respawntime &&
			!missionComplete)
		{
			zombieSpawnClock.restart();
			for (int i = 0; i < zombie_perspawn; i++) {
				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));
			}
			for (int i = 0; i < dog_perspawn; i++) {
				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 2));
			}
		}

		if (!playersArr.empty())
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, playersArr[0].speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1050) //check for bottom border
			{
				playersArr[0].shape.move(0, -playersArr[0].speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(playersArr[0].speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1920) //check for right border
			{
				playersArr[0].shape.move(-playersArr[0].speed, 0);
			}
		}
	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundWoods);
		drawEntities(playersArr, zombiesArr, window);

	};
};
struct Safezone3 {
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	Clock zombieSpawn;
	const float zombieSpawnTime = 3.0f;
	int intial_num = 10;
	int maxZomies = 100;
	Clock timer;


	Texture airport_background_tex;
	Sprite airport_background;



	const int number_of_planes = 5;
	planes plane[5];

	void background(RenderWindow& window) {




		for (int i = 0; i < number_of_planes; i++) //loop to set the color of the first plane 4 rectangles and size and origin
		{
			for (int j = 0; j < number_of_planes - 3; j++)
			{
				plane[i].rect[j].setFillColor(Color::Transparent);
				plane[i].rect[j].setSize(Vector2f(130, 5));
				plane[i].rect[j].setOrigin(130 / 2, 1 / 2);


			}

		}

		plane[0].rect[0].setPosition(1025, 130); //body rectangle
		plane[0].rect[0].setRotation(90);
		plane[0].rect[1].setPosition(990, 160);  //wings rectangle

		plane[1].rect[0].setPosition(200, 530); //body rectangle
		plane[1].rect[1].setRotation(90); //wings rectangle
		plane[1].rect[1].setPosition(180, 530);

		plane[2].rect[0].setPosition(880, 850); //body rectangle
		plane[2].rect[0].setRotation(315);
		plane[2].rect[1].setRotation(405); //wings rectangle
		plane[2].rect[1].setPosition(902, 835);

		plane[3].rect[0].setSize(Vector2f(120, 1));
		plane[3].rect[1].setSize(Vector2f(120, 1));
		plane[3].rect[0].setPosition(1710, 780); //body rectangle
		plane[3].rect[0].setRotation(300);
		plane[3].rect[1].setPosition(1680, 820); //wings rectangle
		plane[3].rect[1].setRotation(390);

		plane[4].rect[0].setPosition(1700, 150); //body rectangle
		plane[4].rect[0].setRotation(300);
		plane[4].rect[1].setPosition(1700, 150); //wings rectangle 
		plane[4].rect[1].setRotation(390);


		for (int i = 0; i < number_of_planes; i++)
		{
			plane[i].square.setSize(Vector2f(150, 150));
			plane[i].square.setFillColor(Color::Transparent);

		}

		plane[0].square.setPosition(1015, 60);

		plane[1].square.setPosition(120, 550);

		plane[2].square.setOrigin(150 / 2, 150 / 2);
		plane[2].square.rotate(45);
		plane[2].square.setPosition(830, 770);

		plane[3].square.setOrigin(150 / 2, 150 / 2);
		plane[3].square.setPosition(1770, 855);
		plane[3].square.rotate(32);

		plane[4].square.setOrigin(150 / 2, 150 / 2);
		plane[4].square.setPosition(1770, 210);
		plane[4].square.rotate(32);


		airport_background_tex.loadFromFile("imgs/safezone2/AirPort.jpeg");

		Vector2u windowsize = window.getSize();

		float scaleX = 1;
		float scaleY = 1;
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background

		airport_background.setTexture(airport_background_tex);

		airport_background.setScale(
			mapWidth / airport_background_tex.getSize().x,
			mapHeight / airport_background_tex.getSize().y
		);
		for (int i = 0; i < number_of_planes; i++)
		{
			plane[i].tex.loadFromFile("imgs/safezone2/plane2.png");
			plane[i].spr.setTexture(plane[i].tex);
			plane[i].spr.setScale(0.8, 0.8);
			plane[i].spr.setOrigin(plane[i].spr.getGlobalBounds().width, plane[i].spr.getGlobalBounds().height);
		}

		plane[0].spr.setPosition(915, 70);
		plane[0].spr.setRotation(180);

		plane[1].spr.setPosition(270, 450);
		plane[1].spr.setRotation(270);

		plane[2].spr.setPosition(900, 950);
		plane[2].spr.setRotation(45);

		plane[3].spr.setPosition(1662, 700);
		plane[3].spr.setRotation(210);

		plane[4].spr.setPosition(1650, 60);
		plane[4].spr.setRotation(210);
	}



	void safeZone3_draw_entities(RenderWindow& window, planes(&plane)[5]) {
		window.draw(airport_background);
		for (int i = 0; i < number_of_planes; i++)
		{
			window.draw(plane[i].square);
			window.draw(plane[i].spr);
			for (int j = 0; j < number_of_planes - 3; j++)
			{
				window.draw(plane[i].rect[j]);
				window.draw(plane[i].rect[j]);
			}

		}
	}
	const int speed = 5;
	const int zombie_speed = 4;
	bool bullet_touched_a_plane = false;

	void colliders(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) {
		if (!playersArr.empty())
		{
			for (int i = 0; i < zombiesArr.size(); i++) ////////////////////////////////loop for the zombies
			{
				for (int j = 0; j < number_of_planes; j++) {
					FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //player bounding box
					FloatRect intersection; //intersection area
					FloatRect Wall_bound = plane[j].square.getGlobalBounds(); //intersected object
					if (zombie_bounds.intersects(Wall_bound))
					{

						zombie_bounds.intersects(Wall_bound, intersection);
						// left/right
						if (intersection.width < intersection.height)
						{
							//right collision
							if (zombie_bounds.left < Wall_bound.left)
							{
								zombiesArr[i].shape.move(-zombie_speed, 0);
							}
							//left collision
							else
							{
								zombiesArr[i].shape.move(zombie_speed, 0);
							}
						}
						// up/down
						else
						{
							//down collision
							if (zombie_bounds.top < Wall_bound.top)
							{
								zombiesArr[i].shape.move(0, -zombie_speed);
							}
							//up collision
							else
							{
								zombiesArr[i].shape.move(0, zombie_speed);
							}
						}
					}
				}
			}







			for (int i = 0; i < number_of_planes; i++) //////////////////////////////////////////////////////////loop for the player
			{
				FloatRect Player_Bounds = playersArr[0].shape.getGlobalBounds(); //player bounding box
				FloatRect intersection; //intersection area
				FloatRect Wall_bound = plane[i].square.getGlobalBounds(); //intersected object
				if (Player_Bounds.intersects(Wall_bound))
				{

					Player_Bounds.intersects(Wall_bound, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}
			}

		}
		if (!bullets.empty())
		{


			for (int i = 0; i < bullets.size();) {
				for (int j = 0; j < number_of_planes; j++)
				{
					for (int k = 0; k < number_of_planes - 3; k++)
					{
						if (bullets[i].shape.getGlobalBounds().intersects(plane[j].rect[k].getGlobalBounds()))
						{
							bullet_touched_a_plane = true;
						}

					}
				}
				if (bullets[i].age >= bullets[i].lifetime || bullet_touched_a_plane) {
					bullets.erase(bullets.begin() + i);
					bullet_touched_a_plane = false;
				}

				else

					i++;
			}
		}
	}

	Safezone3(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		background(window);
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, RIFLE, PISTOL, KNIFE, SHOTGUN, 3, window));
		for (int i = 0; i < intial_num; i++) {
			float x, y;
			int side = rand() % 3;  // 0 -> top , 1 -> right, 2 -> bottom, 3 -> left

			int mapWidth = 2000;
			int mapHeight = 1500;
			switch (side) {
			case 0: // Top
				x = rand() % mapWidth;
				y = 0;
				break;
			case 1: // Right
				x = mapWidth;
				y = rand() % mapHeight;
				break;
			case 2: // Bottom
				x = rand() % mapWidth;
				y = mapHeight;
				break;
			case 3: // Left
				x = 0;
				y = rand() % mapHeight;
				break;
			}

			zombiesArr.push_back(ZOMBIE(x, y, 1));
		}



		// timer and view and point after 2 min
	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		View view(window.getDefaultView());

		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}

		//spawn zombie every 3 sec

		if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime && zombiesArr.size() < maxZomies) {
			float x, y;
			int side = rand() % 4;


			switch (side) {
			case 0: // Top
				x = rand() % (int)2000;
				y = 0;
				break;
			case 1: // Right
				x = 2000;
				y = rand() % (int)1500;
				break;
			case 2: // Bottom
				x = rand() % (int)2000;
				y = 1500;
				break;
			case 3: // Left
				x = 0;
				y = rand() % (int)1500;
				break;
			}
			zombiesArr.push_back(ZOMBIE(x, y, 1));
			zombieSpawn.restart();


		}
		if (!playersArr.empty()) {
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;

			float clampedX = max(halfWidth, min(2000 - halfWidth, playerPos.x));
			float clampedY = max(halfHeight, min(1500 - halfHeight, playerPos.y));

			view.setCenter(clampedX, clampedY);
			window.setView(view);
		}
		for (int i = 0; i < playersArr.size(); i++) {
			Vector2f playerPos = playersArr[i].shape.getPosition();

			const float mapWidth = 2000.0f;
			const float mapHeight = 1500.0f;

			FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
			float halfWidth = playerBounds.width / 2.0f;
			float halfHeight = playerBounds.height / 2.0f;

			if (playerPos.x < halfWidth) playerPos.x = halfWidth;
			if (playerPos.x > mapWidth - halfWidth) playerPos.x = mapWidth - halfWidth;
			if (playerPos.y < halfHeight) playerPos.y = halfHeight;
			if (playerPos.y > mapHeight - halfHeight) playerPos.y = mapHeight - halfHeight;

			playersArr[i].shape.setPosition(playerPos);
		}
	}

	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		colliders(playersArr, zombiesArr);
		safeZone3_draw_entities(window, plane);
		drawEntities(playersArr, zombiesArr, window);

	}
};
struct Army {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	bool missionComplete = false;
	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		// Randomly choose between left and right range
		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		// Randomly choose one of the two ranges
		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}
	bool rush = false; // rushMode flag 
	bool bullet_touched = false;
	Clock zombieSpawn;
	const float zombieSpawnTime = 5.0f;
	const int intial_num = 4;

	int armycampscore = 0;//score
	Sprite armycampdead;
	Font font;
	Text score;
	Sprite tentSprite;
	Sprite tankSprite1;
	Sprite tankSprite2;
	RectangleShape tankShape;
	RectangleShape tankShape1;
	RectangleShape tentshape;
	Sprite sprite; // background Sprite
	float mapWidth = 2000; // width of background
	float mapHeight = 1500; // height of background
	Clock timer;
	Army(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(

			PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, RIFLE, PISTOL, KNIFE, 4, window)
		);

		armycampdead.setTexture(armycampdeadTexture);
		armycampdead.setOrigin(armycampdead.getLocalBounds().width / 2, armycampdead.getLocalBounds().height / 2);
		armycampdead.setPosition(960, 540);
		armycampdead.setScale(2, 2);
		font.loadFromFile("img/Caliste.ttf");
		score.setFont(font);
		score.setCharacterSize(50);

		for (int i = 0; i < intial_num; i++) {
			float x, y;
			int side = rand() % 3;  // 0 -> top , 1 -> right, 2 -> bottom, 3 -> left

			int mapWidth = 2000;
			int mapHeight = 1500;
			switch (side) {
			case 0: // Top
				x = rand() % mapWidth;
				y = 0;
				break;
			case 1: // Right
				x = mapWidth;
				y = rand() % mapHeight;
				break;
			case 2: // Bottom
				x = rand() % mapWidth;
				y = mapHeight;
				break;
			case 3: // Left
				x = 0;
				y = rand() % mapHeight;
				break;
			}

			zombiesArr.push_back(ZOMBIE(x, y, 1));
		}
		tentSprite.setTexture(Tent);
		tentSprite.setPosition(400, 400);
		tentSprite.setScale(0.2f, 0.2f);



		tankSprite1.setTexture(Tank1);
		tankSprite1.setPosition(800, 600);



		tankSprite2.setTexture(Tank2);
		tankSprite2.setPosition(100, 100);

		tankShape.setSize(Vector2f(85, 150));
		tankShape.setPosition(890, 655);

		tankShape1.setSize(Vector2f(85, 150));
		tankShape1.setPosition(190, 155);

		tentshape.setSize(Vector2f(160, 160));
		tentshape.setPosition(422, 420);

		sprite.setTexture(armyBackground);
		sprite.setScale(
			mapWidth / armyBackground.getSize().x,
			mapHeight / armyBackground.getSize().y
		);
	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, mission1_zombies_counter);
		updateBullets(dt);
		if (gameTimer.getTime() >= gameTimerConst) {
			missionComplete = true;
		}
		View view(window.getDefaultView());

		//spawn zombie every 5 sec
		if (!rush) {
			if (!playersArr.empty()) {
				if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime) {
					for (int i = 0; i < 8; i++)
					{
						int x = getRandomOutsideRange_x();
						int y = getRandomOutsideRange_y();
						zombiesArr.push_back(ZOMBIE(x, y, 1));
						zombieSpawn.restart();
					}
				}
			}
		}
		else if (rush) {
			if (!playersArr.empty()) {
				if (zombieSpawn.getElapsedTime().asSeconds() >= 3.0f) {
					for (int i = 0; i < 3; i++)
					{
						int x = getRandomOutsideRange_x();
						int y = getRandomOutsideRange_y();
						zombiesArr.push_back(ZOMBIE(x, y, 1));
						zombieSpawn.restart();
					}
				}
				// Increment score for zombies that are dead and not yet counted
				for (int i = 0; i < zombiesArr.size(); i++) {
					if (zombiesArr[i].isDead && !zombiesArr[i].scoreCounted) {
						armycampscore += 5; // Increment score
						zombiesArr[i].scoreCounted = true;// Mark the zombie as processed for scoring
					}
				}
			}
		}


		int zombie_speed = 4;
		int speed = 5;

		if (!playersArr.empty()) {

			for (int i = 0; i < zombiesArr.size(); i++) //loop for the zombies
			{
				FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //player bounding box
				FloatRect intersection; //intersection area
				FloatRect Wall_bound = tentshape.getGlobalBounds(); //intersected object(tent)
				FloatRect Wall_bound1 = tankShape.getGlobalBounds(); //intersected object(tank1)
				FloatRect Wall_bound2 = tankShape1.getGlobalBounds(); //intersected object(tank2)
				if (zombie_bounds.intersects(Wall_bound))
				{

					zombie_bounds.intersects(Wall_bound, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (zombie_bounds.left < Wall_bound.left)
						{
							zombiesArr[i].shape.move(-zombie_speed, 0);
						}
						//left collision
						else
						{
							zombiesArr[i].shape.move(zombie_speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (zombie_bounds.top < Wall_bound.top)
						{
							zombiesArr[i].shape.move(0, -zombie_speed);
						}
						//up collision
						else
						{
							zombiesArr[i].shape.move(0, zombie_speed);
						}
					}
				}
				if (zombie_bounds.intersects(Wall_bound1))
				{

					zombie_bounds.intersects(Wall_bound1, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (zombie_bounds.left < Wall_bound1.left)
						{
							zombiesArr[i].shape.move(-zombie_speed, 0);
						}
						//left collision
						else
						{
							zombiesArr[i].shape.move(zombie_speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (zombie_bounds.top < Wall_bound1.top)
						{
							zombiesArr[i].shape.move(0, -zombie_speed);
						}
						//up collision
						else
						{
							zombiesArr[i].shape.move(0, zombie_speed);
						}
					}
				}
				if (zombie_bounds.intersects(Wall_bound2))
				{

					zombie_bounds.intersects(Wall_bound2, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (zombie_bounds.left < Wall_bound2.left)
						{
							zombiesArr[i].shape.move(-zombie_speed, 0);
						}
						//left collision
						else
						{
							zombiesArr[i].shape.move(zombie_speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (zombie_bounds.top < Wall_bound2.top)
						{
							zombiesArr[i].shape.move(0, -zombie_speed);
						}
						//up collision
						else
						{
							zombiesArr[i].shape.move(0, zombie_speed);
						}
					}
				}
			}


			if (!bullets.empty())
			{


				for (int i = 0; i < bullets.size();) {

					if (bullets[i].shape.getGlobalBounds().intersects(tankShape.getGlobalBounds()) || bullets[i].shape.getGlobalBounds().intersects(tankShape1.getGlobalBounds()))
					{
						bullet_touched = true;
					}



					if (bullets[i].age >= bullets[i].lifetime || bullet_touched) {
						bullets.erase(bullets.begin() + i);
						bullet_touched = false;
					}

					else

						i++;

				}
			}

			if (!playersArr.empty()) {
				FloatRect Player_Bounds = playersArr[0].shape.getGlobalBounds(); //player bounding box
				FloatRect intersection; //intersection area
				FloatRect Wall_bound = tentshape.getGlobalBounds(); //intersected object
				FloatRect Wall_bound1 = tankShape.getGlobalBounds(); //intersected object
				FloatRect Wall_bound2 = tankShape1.getGlobalBounds(); //intersected object
				if (Player_Bounds.intersects(Wall_bound))
				{

					Player_Bounds.intersects(Wall_bound, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}

				if (Player_Bounds.intersects(Wall_bound1))
				{

					Player_Bounds.intersects(Wall_bound1, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound1.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound1.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}

				if (Player_Bounds.intersects(Wall_bound2))
				{

					Player_Bounds.intersects(Wall_bound2, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound2.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound2.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}
			}

			//border collision
			int speed = 5;
			int zombie_speed = 4;
			if (!playersArr.empty())
			{
				if (playersArr[0].shape.getPosition().y <= 30) // check for top border
				{
					playersArr[0].shape.move(0, speed);
				}
				if (playersArr[0].shape.getPosition().y >= 1450) //check for bottom border
				{
					playersArr[0].shape.move(0, -speed);
				}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
				{
					playersArr[0].shape.move(speed, 0);
				}
				if (playersArr[0].shape.getPosition().x >= 1930) //check for right border
				{
					playersArr[0].shape.move(-speed, 0);
				}
			}


			//camera view --- player_Tracking
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;

			float vX = max(halfWidth, min(mapWidth - halfWidth, playerPos.x));
			float vY = max(halfHeight, min(mapHeight - halfHeight, playerPos.y));

			view.setCenter(vX, vY);
			window.setView(view);
		}
	}


	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(sprite);
		window.draw(tentSprite);
		window.draw(tankSprite1);
		window.draw(tankSprite2);
		drawEntities(playersArr, zombiesArr, window);

	}
};
struct Mission1 {
	bool missionComplete = false;

	Texture airport_background_tex;
	Sprite airport_background;
	const int number_of_planes = 5;
	planes plane[5];
	bool bullet_touched_a_plane = false;

	Clock zombie_spawn_timer;
	float zombies_number_per_round = 1;

	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int maxZombies = 80;
	int totalZombieKilled = maxZombies;
	int mission1_zombies_counter = 0;



	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		// Randomly choose between left and right range
		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		// Randomly choose one of the two ranges
		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}

	Mission1(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, RIFLE, PISTOL, KNIFE, 4, window));

		for (int i = 0; i < number_of_planes; i++) //loop to set the color of the first plane 4 rectangles and size and origin
		{
			for (int j = 0; j < number_of_planes - 3; j++)
			{
				plane[i].rect[j].setFillColor(Color::Transparent);
				plane[i].rect[j].setSize(Vector2f(130, 5));
				plane[i].rect[j].setOrigin(130 / 2, 1 / 2);


			}

		}

		plane[0].rect[0].setPosition(1000, 130); //body rectangle
		plane[0].rect[0].setRotation(90);
		plane[0].rect[1].setPosition(990, 160);  //wings rectangle

		plane[1].rect[0].setPosition(200, 530); //body rectangle
		plane[1].rect[1].setRotation(90); //wings rectangle
		plane[1].rect[1].setPosition(180, 530);

		plane[2].rect[0].setPosition(880, 850); //body rectangle
		plane[2].rect[0].setRotation(315);
		plane[2].rect[1].setRotation(405); //wings rectangle
		plane[2].rect[1].setPosition(902, 835);

		plane[3].rect[0].setSize(Vector2f(120, 1));
		plane[3].rect[1].setSize(Vector2f(120, 1));
		plane[3].rect[0].setPosition(1710, 780); //body rectangle
		plane[3].rect[0].setRotation(300);
		plane[3].rect[1].setPosition(1680, 820); //wings rectangle
		plane[3].rect[1].setRotation(390);

		plane[4].rect[0].setPosition(1700, 150); //body rectangle
		plane[4].rect[0].setRotation(300);
		plane[4].rect[1].setPosition(1700, 150); //wings rectangle 
		plane[4].rect[1].setRotation(390);


		for (int i = 0; i < number_of_planes; i++)
		{
			plane[i].square.setSize(Vector2f(150, 150));
			plane[i].square.setFillColor(Color::Transparent);
		}

		plane[0].square.setPosition(1015, 60);

		plane[1].square.setPosition(120, 550);

		plane[2].square.setOrigin(150 / 2, 150 / 2);
		plane[2].square.rotate(45);
		plane[2].square.setPosition(830, 770);

		plane[3].square.setOrigin(150 / 2, 150 / 2);
		plane[3].square.setPosition(1770, 855);
		plane[3].square.rotate(32);

		plane[4].square.setOrigin(150 / 2, 150 / 2);
		plane[4].square.setPosition(1770, 210);
		plane[4].square.rotate(32);


		airport_background_tex.loadFromFile("imgs/safezone2/AirPort.jpeg");

		Vector2u windowsize = window.getSize();
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background

		airport_background.setScale(
			mapWidth / airport_background_tex.getSize().x,
			mapHeight / airport_background_tex.getSize().y
		);

		airport_background.setTexture(airport_background_tex);
		//airport_background.setScale(scaleX, scaleY);

		for (int i = 0; i < number_of_planes; i++)
		{
			plane[i].tex.loadFromFile("imgs/safezone2/plane2.png");
			plane[i].spr.setTexture(plane[i].tex);
			plane[i].spr.setScale(0.8, 0.8);
			plane[i].spr.setOrigin(plane[i].spr.getGlobalBounds().width, plane[i].spr.getGlobalBounds().height);
		}

		plane[0].spr.setPosition(915, 70);
		plane[0].spr.setRotation(180);

		plane[1].spr.setPosition(270, 450);
		plane[1].spr.setRotation(270);

		plane[2].spr.setPosition(900, 950);
		plane[2].spr.setRotation(45);

		plane[3].spr.setPosition(1662, 700);
		plane[3].spr.setRotation(210);

		plane[4].spr.setPosition(1650, 60);
		plane[4].spr.setRotation(210);
	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		if (totalZombieKilled <= 0) {
			missionComplete = true;
		}



		if (zombie_spawn_timer.getElapsedTime().asSeconds() >= 2.5f && mission1_zombies_counter < maxZombies && !playersArr.empty()) //Random Spawn The Zombies
		{
			float number_x = getRandomOutsideRange_x();
			float number_y = getRandomOutsideRange_y();
			zombiesArr.push_back(ZOMBIE(number_x, number_y, 1));
			mission1_zombies_counter++;

			zombie_spawn_timer.restart();
		}

		for (int i = 0; i < zombiesArr.size(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				totalZombieKilled--;
				zombiesArr[i].isDeadCounter = true;;
			}
		}

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, totalZombieKilled, true);
		updateBullets(dt);
		View view(window.getDefaultView());
		if (!playersArr.empty())
		{
			//camera view max view and min view to track player
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;

			float clampedX = max(halfWidth, min(2000 - halfWidth, playerPos.x));
			float clampedY = max(halfHeight, min(1500 - halfHeight, playerPos.y));

			view.setCenter(clampedX, clampedY);
			window.setView(view);
		}
		if (!bullets.empty())
		{


			for (int i = 0; i < bullets.size();) {
				for (int j = 0; j < number_of_planes; j++)
				{
					for (int k = 0; k < number_of_planes - 3; k++)
					{
						if (bullets[i].shape.getGlobalBounds().intersects(plane[j].rect[k].getGlobalBounds()))
						{
							bullet_touched_a_plane = true;
						}

					}
				}
				if (bullets[i].age >= bullets[i].lifetime || bullet_touched_a_plane) {
					bullets.erase(bullets.begin() + i);
					bullet_touched_a_plane = false;
				}

				else

					i++;
			}
		}



		//border collision
		int speed = 5;
		int zombie_speed = 4;
		if (!playersArr.empty())
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1450) //check for bottom border
			{
				playersArr[0].shape.move(0, -speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1930) //check for right border
			{
				playersArr[0].shape.move(-speed, 0);
			}
		}

		// plane , zombies , player collider
		if (!playersArr.empty())
		{
			for (int i = 0; i < zombiesArr.size(); i++) ////////////////////////////////loop for the zombies
			{
				for (int j = 0; j < number_of_planes; j++) {
					FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //player bounding box
					FloatRect intersection; //intersection area
					FloatRect Wall_bound = plane[j].square.getGlobalBounds(); //intersected object
					if (zombie_bounds.intersects(Wall_bound))
					{

						zombie_bounds.intersects(Wall_bound, intersection);
						// left/right
						if (intersection.width < intersection.height)
						{
							//right collision
							if (zombie_bounds.left < Wall_bound.left)
							{
								zombiesArr[i].shape.move(-zombie_speed, 0);
							}
							//left collision
							else
							{
								zombiesArr[i].shape.move(zombie_speed, 0);
							}
						}
						// up/down
						else
						{
							//down collision
							if (zombie_bounds.top < Wall_bound.top)
							{
								zombiesArr[i].shape.move(0, -zombie_speed);
							}
							//up collision
							else
							{
								zombiesArr[i].shape.move(0, zombie_speed);
							}
						}
					}
				}
			}


			for (int i = 0; i < number_of_planes; i++) //////////////////////////////////////////////////////////loop for the player
			{
				FloatRect Player_Bounds = playersArr[0].shape.getGlobalBounds(); //player bounding box
				FloatRect intersection; //intersection area
				FloatRect Wall_bound = plane[i].square.getGlobalBounds(); //intersected object
				if (Player_Bounds.intersects(Wall_bound))
				{

					Player_Bounds.intersects(Wall_bound, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}
			}

		}



	}

	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(airport_background);


		for (int i = 0; i < number_of_planes; i++)
		{
			window.draw(plane[i].square);
			window.draw(plane[i].spr);
			for (int j = 0; j < number_of_planes - 3; j++)
			{
				window.draw(plane[i].rect[j]);
				window.draw(plane[i].rect[j]);
			}

		}
		drawEntities(playersArr, zombiesArr, window, true);

	}


};
struct Mission2
{
	float zombies_number_per_round = 1;
	int number_of_planes = 5;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	Clock zombie_spawn_timer;

	Texture rails_tex;
	Sprite rails[4];

	Texture train_tex;
	Sprite train[4];

	Texture tree_tex;
	Sprite tree[2];

	Texture water_tex;
	Sprite water;

	Texture trainstation_background_tex;
	Sprite trainstation_background;

	RectangleShape rectangle_collider[5];
	int maxZombies = 130;
	int totalZombieKilled = maxZombies;
	bool missionComplete = false;


	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		// Randomly choose between left and right range
		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		// Randomly choose one of the two ranges
		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}
	Mission2(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, BURST_RIFLE, PISTOL, KNIFE, 4, window));


		trainstation_background_tex.loadFromFile("imgs/mission2/backgroundT.jpg");
		rails_tex.loadFromFile("imgs/mission2/train rails.png");
		train_tex.loadFromFile("imgs/mission2/train.png");
		tree_tex.loadFromFile("imgs/mission2/tree.png");
		water_tex.loadFromFile("imgs/mission2/water.png");

		for (int i = 0; i < 5; i++) //rectangle collider setup
		{
			rectangle_collider[i].setSize(Vector2f(750, 40));
			rectangle_collider[i].setFillColor(Color::Transparent);
			rectangle_collider[i].setOrigin(rectangle_collider[i].getLocalBounds().width / 2, rectangle_collider[i].getLocalBounds().height / 2);
			rectangle_collider[i].setRotation(90);
		}



		for (int i = 0; i < 4; i++) { //rails setup
			rails[i].setTexture(rails_tex);
			rails[i].setOrigin(rails[i].getLocalBounds().width / 2, rails[i].getLocalBounds().height / 2);
			rails[i].setScale(2, 2);
			rails[i].setRotation(90);
		}


		for (int i = 0; i < 4; i++) { //train setup
			train[i].setTexture(train_tex);
			train[i].setOrigin(train[i].getLocalBounds().width / 2, train[i].getLocalBounds().height / 2);
			train[i].setScale(1.5, 1.5);
			train[i].setRotation(90);

		}


		for (int i = 0; i < 2; i++) { //tree setup
			tree[i].setTexture(tree_tex);
			tree[i].setOrigin(tree[i].getLocalBounds().width / 2, tree[i].getLocalBounds().height / 2);
			tree[i].setScale(4, 4);
		}

		water.setTexture(water_tex); //water setup
		water.setOrigin(water.getLocalBounds().width / 2, water.getLocalBounds().height / 2);


		trainstation_background.setTexture(trainstation_background_tex); //background



		for (int i = 0; i < 4;) // set the positions of the rails
		{
			for (int j = 1; j < 5;)
			{
				rails[i].setPosition(j * 380, 750);
				i++;
				j++;
			}
			break;
		}
		train[0].setPosition(380, 0);  //first train position
		train[1].setPosition(2 * 380, 1400); //second train position
		train[2].setPosition(3 * 380, 700); //third train position
		train[3].setPosition(4 * 380, 1500); //forth train position


		for (int i = 0; i < 4; i++) //set the first 4 rectangle colliders (trains rectangle collider)
		{
			rectangle_collider[i].setPosition(train[i].getPosition().x, train[i].getPosition().y - 20);
		}

		rectangle_collider[4].setSize(Vector2f(110, 110)); //setsize for the water collider
		rectangle_collider[4].setPosition(1815, 1170); //setposition for the water rectangle collider

		tree[0].setPosition(980, 700); //first tree position
		tree[1].setPosition(580, 180); //second tree position

		water.setPosition(1780, 850);  //water position


		Vector2u windowsize = window.getSize();
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background

		trainstation_background.setScale(
			mapWidth / trainstation_background_tex.getSize().x,
			mapHeight / trainstation_background_tex.getSize().y
		);
	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {

		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, totalZombieKilled, true);
		updateBullets(dt);

		if (totalZombieKilled <= 0) {
			missionComplete = true;
		}

		if (zombie_spawn_timer.getElapsedTime().asSeconds() >= 2.5f && mission1_zombies_counter < maxZombies && !playersArr.empty()) //Random Spawn The Zombies
		{


			float number_x = getRandomOutsideRange_x();
			float number_y = getRandomOutsideRange_y();
			zombiesArr.push_back(ZOMBIE(number_x, number_y, 1));
			mission1_zombies_counter++;

			zombie_spawn_timer.restart();

		}
		for (int i = 0; i < zombiesArr.size(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				totalZombieKilled--;
				zombiesArr[i].isDeadCounter = true;;
			}
		}


		View view(window.getDefaultView());
		if (!playersArr.empty())
		{
			//camera view max view and min view to track player
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;

			float clampedX = max(halfWidth, min(2000 - halfWidth, playerPos.x));
			float clampedY = max(halfHeight, min(1500 - halfHeight, playerPos.y));

			view.setCenter(clampedX, clampedY);
			window.setView(view);
		}

		if (!bullets.empty()) //bullets intersect
		{


			for (int i = 0; i < bullets.size();) {
				bool bullet_intersected = false;
				for (int j = 0; j < 5; j++)
				{


					if (bullets[i].shape.getGlobalBounds().intersects(rectangle_collider[j].getGlobalBounds()))
					{
						bullet_intersected = true;
					}


				}
				if (bullets[i].age >= bullets[i].lifetime || bullet_intersected) {
					bullets.erase(bullets.begin() + i);
					bullet_intersected = false;
				}

				else

					i++;
			}
		}
		int speed = 5;
		if (!playersArr.empty())  //border collider
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1450) //check for bottom border
			{
				playersArr[0].shape.move(0, -speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1930) //check for right border
			{
				playersArr[0].shape.move(-speed, 0);
			}
		}


		if (!playersArr.empty())
		{
			int zombie_speed = 6;
			for (int i = 0; i < zombiesArr.size(); i++) ////////////////////////////////loop for the zombies
			{
				for (int j = 0; j < 5; j++) {
					FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //zombie bounding box
					FloatRect intersection; //intersection area
					FloatRect Wall_bound = rectangle_collider[j].getGlobalBounds(); //intersected object
					if (zombie_bounds.intersects(Wall_bound))
					{

						zombie_bounds.intersects(Wall_bound, intersection);
						// left/right
						if (intersection.width < intersection.height)
						{
							//right collision
							if (zombie_bounds.left < Wall_bound.left)
							{
								zombiesArr[i].shape.move(-zombie_speed, 0);
							}
							//left collision
							else
							{
								zombiesArr[i].shape.move(zombie_speed, 0);
							}
						}
						// up/down
						else
						{
							//down collision
							if (zombie_bounds.top < Wall_bound.top)
							{
								zombiesArr[i].shape.move(0, -zombie_speed);
							}
							//up collision
							else
							{
								zombiesArr[i].shape.move(0, zombie_speed);
							}
						}
					}
				}
			}








			for (int j = 0; j < 5; j++)
			{
				FloatRect Player_Bounds = playersArr[0].shape.getGlobalBounds(); //player bounding box
				FloatRect intersection; //intersection area
				FloatRect Wall_bound = rectangle_collider[j].getGlobalBounds(); //intersected object
				if (Player_Bounds.intersects(Wall_bound))
				{

					Player_Bounds.intersects(Wall_bound, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}
			}



		}


	}

	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(trainstation_background);

		for (int i = 0; i < 4; i++) {
			window.draw(rails[i]);
			window.draw(train[i]);
		}
		window.draw(water);
		for (int i = 0; i < 5; i++)
		{
			window.draw(rectangle_collider[i]);
		}
		drawEntities(playersArr, zombiesArr, window, true);

	}
};
struct Mission3
{
	bool missionComplete = false;
	Texture city_background_tex;
	Sprite city_background;

	Texture building1_tex;
	Sprite building1;

	Texture building2_tex;
	Sprite building2[4];

	Texture building3_tex;
	Sprite building3[4];

	Texture car1_tex;
	Sprite car1[4];

	Texture car2_tex;
	Sprite car2[2];

	RectangleShape rect_collider[15];
	float zombies_number_per_round = 1;

	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	Clock zombie_spawn_timer;
	int maxZombies = 150;
	int totalZombieKilled = maxZombies;

	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		// Randomly choose between left and right range
		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		// Randomly choose one of the two ranges
		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}
	Mission3(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, PLASMA_SHOTGUN, PLASMA_PISTOL, KNIFE, 4, window));




		city_background_tex.loadFromFile("imgs/mission3/city_background.png"); //LOAD FILES
		building1_tex.loadFromFile("imgs/mission3/CityBuilding2.png");
		building2_tex.loadFromFile("imgs/mission3/CityBuilding1.png");
		building3_tex.loadFromFile("imgs/mission3/CityBuilding3.png");
		car1_tex.loadFromFile("imgs/mission3/car1.png");
		car2_tex.loadFromFile("imgs/mission3/car2.png");



		for (int i = 0; i < 4; i++) //Set textures and origin
		{
			city_background.setTexture(city_background_tex);
			building1.setTexture(building1_tex);
			building2[i].setTexture(building2_tex);
			building3[i].setTexture(building3_tex);
			car1[i].setTexture(car1_tex);


			building1.setOrigin(building1.getLocalBounds().width / 2, building1.getLocalBounds().height / 2);
			building2[i].setOrigin(building2[i].getLocalBounds().width / 2, building2[i].getLocalBounds().height / 2);
			building3[i].setOrigin(building3[i].getLocalBounds().width / 2, building3[i].getLocalBounds().height / 2);
			car1[i].setOrigin(car1[i].getLocalBounds().width / 2, car1[i].getLocalBounds().height / 2);

		}
		car2[0].setTexture(car2_tex);
		car2[1].setTexture(car2_tex);
		car2[0].setOrigin(car2[0].getLocalBounds().width / 2, car2[0].getLocalBounds().height / 2);
		car2[1].setOrigin(car2[1].getLocalBounds().width / 2, car2[1].getLocalBounds().height / 2);

		for (int i = 0; i < 4; i++)
		{
			building2[i].setScale(0.8, 0.8);
			car1[i].setScale(0.4, 0.4);
		}

		building1.setPosition(1007, 740);

		building2[0].setPosition(1000, 20);

		building2[1].setRotation(180);
		building2[1].setPosition(1000, 1420);

		building2[2].setScale(0.5, 1.3);
		building2[2].setRotation(90);
		building2[2].setPosition(70, 745);

		building2[3].setScale(0.5, 1.3);
		building2[3].setRotation(270);
		building2[3].setPosition(1930, 745);

		building3[0].setPosition(90, 20);
		building3[1].setPosition(1920, 20);
		building3[2].setPosition(90, 1450);
		building3[3].setPosition(1920, 1450);


		car1[0].setPosition(588, 290);
		car1[1].setRotation(90);
		car1[1].setPosition(1569, 200);
		car1[2].setRotation(180);
		car1[2].setPosition(1707, 1257);
		car1[2].setRotation(270);
		car1[3].setPosition(150, 1100);

		car2[0].setPosition(992, 960);
		car2[1].setPosition(1185, 205);

		for (int i = 0; i < 15; i++)
		{
			rect_collider[i].setOrigin(rect_collider[i].getLocalBounds().width / 2, rect_collider[i].getLocalBounds().height / 2);
			rect_collider[i].setFillColor(Color::Transparent);
		}
		for (int i = 0; i < 4; i++)
		{
			rect_collider[i].setSize(Vector2f(180, 150));
		}

		rect_collider[0].setPosition(5, -30);

		rect_collider[1].setPosition(1820, -30);

		rect_collider[2].setPosition(10, 1340);

		rect_collider[3].setPosition(1820, 1340);



		for (int i = 4; i < 6; i++)
		{
			rect_collider[i].setSize(Vector2f(500, 140));
		}
		rect_collider[4].setPosition(750, -40);
		rect_collider[5].setPosition(750, 1350);



		for (int i = 6; i < 8; i++)
		{
			rect_collider[i].setSize(Vector2f(310, 200));
			rect_collider[i].setRotation(90);
		}
		rect_collider[6].setPosition(200, 590);
		rect_collider[7].setPosition(2000, 590);


		rect_collider[8].setSize(Vector2f(500, 250));
		rect_collider[8].setPosition(750, 615);

		for (int i = 9; i < 11; i++)
		{
			rect_collider[i].setSize(Vector2f(200, 50));
			rect_collider[i].setRotation(90);
		}

		rect_collider[9].setPosition(1210, 20);
		rect_collider[10].setPosition(1018, 790);

		for (int i = 11; i < 15; i++)
		{
			rect_collider[i].setSize(Vector2f(100, 1));
			rect_collider[i].setRotation(315);
		}

		rect_collider[11].setPosition(550, 310);

		rect_collider[12].setRotation(225);
		rect_collider[12].setPosition(1605, 245);

		rect_collider[13].setRotation(225);
		rect_collider[13].setPosition(1740, 1300);

		rect_collider[14].setRotation(315);
		rect_collider[14].setPosition(105, 1130);



		Vector2u windowsize = window.getSize();
		float mapWidth = 2000; // width of backgrounddswaw
		float mapHeight = 1500; // height of background

		city_background.setScale(
			mapWidth / city_background_tex.getSize().x,
			mapHeight / city_background_tex.getSize().y
		);
	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, totalZombieKilled, true);
		updateBullets(dt);

		if (totalZombieKilled <= 0) {
			missionComplete = true;
		}

		if (zombie_spawn_timer.getElapsedTime().asSeconds() >= 2.5f && mission1_zombies_counter < maxZombies && !playersArr.empty()) //Random Spawn The Zombies
		{


			float number_x = getRandomOutsideRange_x();
			float number_y = getRandomOutsideRange_y();
			zombiesArr.push_back(ZOMBIE(number_x, number_y, 1));
			mission1_zombies_counter++;

			zombie_spawn_timer.restart();

		}
		for (int i = 0; i < zombiesArr.size(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				totalZombieKilled--;
				zombiesArr[i].isDeadCounter = true;;
			}
		}
		View view(window.getDefaultView());
		if (!playersArr.empty())
		{
			//camera view max view and min view to track player
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;

			float clampedX = max(halfWidth, min(2000 - halfWidth, playerPos.x));
			float clampedY = max(halfHeight, min(1500 - halfHeight, playerPos.y));

			view.setCenter(clampedX, clampedY);
			window.setView(view);
		}

		if (!bullets.empty()) //bullets intersect
		{


			for (int i = 0; i < bullets.size();) {
				bool bullet_intersected = false;
				for (int j = 0; j < 15; j++)
				{


					if (bullets[i].shape.getGlobalBounds().intersects(rect_collider[j].getGlobalBounds()))
					{
						bullet_intersected = true;
					}


				}
				if (bullets[i].age >= bullets[i].lifetime || bullet_intersected) {
					bullets.erase(bullets.begin() + i);
					bullet_intersected = false;
				}

				else

					i++;
			}
		}

		int speed = 5;
		if (!playersArr.empty())  //border collider
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1450) //check for bottom border
			{
				playersArr[0].shape.move(0, -speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1930) //check for right border
			{
				playersArr[0].shape.move(-speed, 0);
			}
		}

		if (!playersArr.empty())
		{
			int zombie_speed = 6;
			for (int i = 0; i < zombiesArr.size(); i++) ////////////////////////////////loop for the zombies
			{
				for (int j = 0; j < 15; j++) {
					FloatRect zombie_bounds = zombiesArr[i].shape.getGlobalBounds(); //zombie bounding box
					FloatRect intersection; //intersection area
					FloatRect Wall_bound = rect_collider[j].getGlobalBounds(); //intersected object
					if (zombie_bounds.intersects(Wall_bound))
					{

						zombie_bounds.intersects(Wall_bound, intersection);
						// left/right
						if (intersection.width < intersection.height)
						{
							//right collision
							if (zombie_bounds.left < Wall_bound.left)
							{
								zombiesArr[i].shape.move(-zombie_speed, 0);
							}
							//left collision
							else
							{
								zombiesArr[i].shape.move(zombie_speed, 0);
							}
						}
						// up/down
						else
						{
							//down collision
							if (zombie_bounds.top < Wall_bound.top)
							{
								zombiesArr[i].shape.move(0, -zombie_speed);
							}
							//up collision
							else
							{
								zombiesArr[i].shape.move(0, zombie_speed);
							}
						}
					}
				}
			}








			for (int j = 0; j < 15; j++)
			{
				FloatRect Player_Bounds = playersArr[0].shape.getGlobalBounds(); //player bounding box
				FloatRect intersection; //intersection area
				FloatRect Wall_bound = rect_collider[j].getGlobalBounds(); //intersected object
				if (Player_Bounds.intersects(Wall_bound))
				{

					Player_Bounds.intersects(Wall_bound, intersection);
					// left/right
					if (intersection.width < intersection.height)
					{
						//right collision
						if (Player_Bounds.left < Wall_bound.left)
						{
							playersArr[0].shape.move(-speed, 0);
						}
						//left collision
						else
						{
							playersArr[0].shape.move(speed, 0);
						}
					}
					// up/down
					else
					{
						//down collision
						if (Player_Bounds.top < Wall_bound.top)
						{
							playersArr[0].shape.move(0, -speed);
						}
						//up collision
						else
						{
							playersArr[0].shape.move(0, speed);
						}
					}
				}
			}



		}


	}

	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(city_background);
		window.draw(car2[0]);
		window.draw(car2[1]);
		for (int i = 0; i < 4; i++)
		{

			window.draw(building1);
			window.draw(car1[i]);


		}


		for (int i = 0; i < 4; i++)
		{
			window.draw(building2[i]);
			window.draw(building3[i]);
		}

		for (int i = 0; i < 15; i++)
		{
			window.draw(rect_collider[i]);
		}
		drawEntities(playersArr, zombiesArr, window, true);

	}

};
struct Mission4 {

	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	int max_zombies = 100;
	int totalZombieKilled = max_zombies;
	float zombie_respawntime = 3.f;
	int zombie_perspawn = 3;
	int totalzombiekilled = max_zombies;
	RectangleShape treeColliders[15];
	Sprite tree[15];
	Sprite background;

	bool missionComplete = false;
	Clock Timer;

	Clock zombieSpawnClock;
	float zombieSpawnTime = 2.5f;

	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		// Randomly choose between left and right range
		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		// Randomly choose one of the two ranges
		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}



	Mission4(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {

		playersArr.push_back(PLAYER(rand() % 1920, rand() % 1080, PLASMA_SHOTGUN, SHOTGUN, PISTOL, KNIFE, 3, window));

		background.setTexture(mission4BackgroundTexture);

		background.setScale(
			(float)window.getSize().x / mission4BackgroundTexture.getSize().x,
			(float)window.getSize().y / mission4BackgroundTexture.getSize().y
		);
		tree[0].setPosition(150.f, 200.f);
		tree[1].setPosition(200.f, 220.f);
		tree[2].setPosition(750.f, 220.f);
		tree[3].setPosition(1050.f, 190.f);

		tree[5].setPosition(250.f, 500.f);
		tree[6].setPosition(550.f, 520.f);

		tree[8].setPosition(1150.f, 510.f);
		tree[9].setPosition(1450.f, 490.f);
		tree[10].setPosition(350.f, 800.f);

		tree[12].setPosition(950.f, 780.f);
		tree[13].setPosition(1000.f, 830.f);
		tree[14].setPosition(1550.f, 790.f);

		for (int i = 0; i < 15; ++i) {
			tree[i].setTexture(mission4PalmTree);
			tree[i].setOrigin(tree[i].getLocalBounds().width / 2, tree[i].getLocalBounds().height / 2);
			treeColliders[i].setOrigin(treeColliders[i].getLocalBounds().width / 2, treeColliders[i].getLocalBounds().height / 2);
			treeColliders[i].setPosition(tree[i].getPosition());
			treeColliders[i].setSize(Vector2f(5, 5));

		}






	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		dt = deltaClock.restart().asSeconds();

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, totalZombieKilled, true);
		updateBullets(dt);

		if (totalZombieKilled <= 98) {
			missionComplete = true;
		}

		if (zombieSpawnClock.getElapsedTime().asSeconds() >= 1.5f && mission1_zombies_counter < max_zombies && !playersArr.empty()) //Random Spawn The Zombies
		{


			float number_x = getRandomOutsideRange_x();
			float number_y = getRandomOutsideRange_y();
			zombiesArr.push_back(ZOMBIE(number_x, number_y, 1));
			mission1_zombies_counter++;

			zombieSpawnClock.restart();

		}
		for (int i = 0; i < zombiesArr.size(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				totalZombieKilled--;
				zombiesArr[i].isDeadCounter = true;;
			}
		}

		for (int i = 0; i < playersArr.size(); i++) {
			for (int j = 0; j < 15; j++) {


				Vector2f otherPos = treeColliders[j].getPosition();
				float dist = sqrt(pow(playersArr[i].shape.getPosition().x - otherPos.x, 2) + pow(playersArr[i].shape.getPosition().y - otherPos.y, 2));

				if (dist < 50.0f && dist > 0.0f) {
					Vector2f away = playersArr[i].shape.getPosition() - otherPos;

					float length = sqrt(away.x * away.x + away.y * away.y);
					if (length != 0) {
						away /= length;
						playersArr[i].shape.move(away.x * 10.5f, away.y * 10.5f);
					}
				}



				for (int k = 0; k < bullets.size(); k++)
				{
					if (bullets[k].shape.getGlobalBounds().intersects(treeColliders[j].getGlobalBounds()))
					{
						bullets.erase(bullets.begin() + k);
						k--;
					}
				}

			}
		}


		for (int j = 0; j < 15; j++) {
			for (int z = 0; z < zombiesArr.size(); z++) {
				Vector2f otherPos = treeColliders[j].getPosition();
				float dist = sqrt(pow(zombiesArr[z].shape.getPosition().x - otherPos.x, 2) +
					pow(zombiesArr[z].shape.getPosition().y - otherPos.y, 2));

				if (dist < 50.0f && dist > 0.0f) {
					Vector2f away = zombiesArr[z].shape.getPosition() - otherPos;
					float length = sqrt(away.x * away.x + away.y * away.y);
					if (length != 0) {
						away /= length;
						zombiesArr[z].shape.move(away.x * 10.5f, away.y * 10.5f);
					}
				}
			}

		}

		for (int i = 0; i < zombiesArr.size(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				totalzombiekilled--;
				zombiesArr[i].isDeadCounter = true;;
			}
		}

		//border collision
		int speed = 5;
		int zombie_speed = 4;
		if (!playersArr.empty())
		{
			if (playersArr[0].shape.getPosition().y <= 30) // check for top border
			{
				playersArr[0].shape.move(0, speed);
			}
			if (playersArr[0].shape.getPosition().y >= 1450) //check for bottom border
			{
				playersArr[0].shape.move(0, -speed);
			}if (playersArr[0].shape.getPosition().x <= 30) //check for left border
			{
				playersArr[0].shape.move(speed, 0);
			}
			if (playersArr[0].shape.getPosition().x >= 1930) //check for right border
			{
				playersArr[0].shape.move(-speed, 0);
			}
		}
	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(background);

		for (int i = 0; i < 15; i++) {
			window.draw(tree[i]);

		}
		drawEntities(playersArr, zombiesArr, window, true);

	}
};


int globalScore = 0;

struct BeachRush {
	int frameCounter = 0, frameDelay = 20, seaindex = 0;
	int score = 0;
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float minSpawnTime = 0.5f;
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate

	bool isSaved = false;
	int initialNumZombies = 10;
	int maxZombies = 100;
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	int mission1_zombies_counter = 0;
	Texture beach, seatexture;
	Sprite sand, sea;

	//Clock gameTimer;

	BeachRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, PISTOL, RIFLE, PLASMA_SHOTGUN, 1, window, 0.35, 0.35, true));

		// Spawn initial zombies randomly on the edges


		// Set background scaling
		seatexture.loadFromFile("imgs/beach/sea.png");

		beach.loadFromFile("imgs/beach/sand.png");

		sand.setTexture(beach);

		sea.setTexture(seatexture);

		sand.setOrigin(0, 0);
		sea.setOrigin(0, 0);

		// Set position to top-left of map
		sand.setPosition(0, 0);
		//sea.setPosition(0, 0);
		sea.setPosition(0, 970);

		// Scale to match map dimensions
		sand.setScale(
			(float)mapWidth / beach.getSize().x,
			(float)mapHeight / beach.getSize().y
		);

		/*sea.setScale(
			(float)mapWidth / seatexture.getSize().x,
			1
		);*/
		sea.setScale(2, 1);

		zombieSpawn.restart();

		//background

	 // background Sprite

		// timer and view and point after 2 min
	};
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score);
			cout << score << endl;
			isSaved = true;
		}
		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);
		frameCounter++;

		if (frameCounter >= frameDelay) {

			seaindex = (seaindex + 1) % 7;

			frameCounter = 0;  // Reset counter after updating

		}

		sea.setTextureRect(IntRect(seaindex * 1920, 0, 1920, 750));
		// Dynamically adjust zombie spawn time
		if (!playersArr.empty()) {

			float elapsed = gameTimer.getTime();
			float t = min(elapsed / spawnRampDuration, 1.0f);
			zombieSpawnTime = 3.0f - t * (3.0f - minSpawnTime);

			if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime) {
				float x, y;
				int side = rand() % 4;
				switch (side) {
				case 0: x = rand() % mapWidth; y = 0; break;
				case 1: x = mapWidth; y = rand() % mapHeight; break;
				case 2: x = rand() % mapWidth; y = mapHeight; break;
				case 3: x = 0; y = rand() % mapHeight; break;
				}

				zombiesArr.push_back(ZOMBIE(x, y, 1));

				if (t >= 0.5f) {
					zombiesArr.push_back(ZOMBIE(x, y, 2));
				}

				zombieSpawn.restart();
			}
			if (!playersArr.empty()) {
				//camera view max view and min view to track player
				Vector2f playerPos = playersArr[0].shape.getPosition();
				float halfWidth = window.getSize().x / 2.0f;
				float halfHeight = window.getSize().y / 2.0f;
				float clampedX = max(halfWidth, min(mapWidth - halfWidth, playerPos.x));
				float clampedY = max(halfHeight, min(mapHeight - halfHeight, playerPos.y));
				view.setCenter(clampedX, clampedY);
				window.setView(view);
			}

			for (int i = 0; i < playersArr.size(); i++) {
				Vector2f playerPos = playersArr[i].shape.getPosition();

				const float mapWidth = 2000.0f;
				const float mapHeight = 1500.0f;

				FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
				float halfWidth = playerBounds.width / 2.0f;
				float halfHeight = playerBounds.height / 2.0f;

				if (playerPos.x < halfWidth) playerPos.x = halfWidth;
				if (playerPos.x > mapWidth - halfWidth) playerPos.x = mapWidth - halfWidth;
				if (playerPos.y < halfHeight) playerPos.y = halfHeight;
				if (playerPos.y > mapHeight - halfHeight) playerPos.y = mapHeight - halfHeight;

				playersArr[i].shape.setPosition(playerPos);
			}
			if (!playersArr.empty())
			{
				// Get player position
				Vector2f pos = playersArr[0].shape.getPosition();

				// Prevent moving out of top border
				if (pos.y <= 0)
					playersArr[0].shape.move(0, playersArr[0].speed);

				// Prevent moving out of bottom border
				if (pos.y + playersArr[0].shape.getGlobalBounds().height >= mapHeight - 250)
					playersArr[0].shape.move(0, -playersArr[0].speed);

				// Prevent moving out of left border
				if (pos.x <= 0)
					playersArr[0].shape.move(playersArr[0].speed, 0);

				// Prevent moving out of right border
				if (pos.x + playersArr[0].shape.getGlobalBounds().width >= mapWidth + 100)
					playersArr[0].shape.move(-playersArr[0].speed, 0);

				for (int i = 0; i < zombiesArr.size(); i++) {
					if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
						score += 100;
						zombiesArr[i].isDeadCounter = true;
					}
				}
			}

		}
	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(sand);
		window.draw(sea);
		drawEntities(playersArr, zombiesArr, window, true, true);

	};
};
struct DesertroadRush {
	int score = 0;
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float minSpawnTime = 0.5f;
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate

	int initialNumZombies = 10;
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	int mission1_zombies_counter = 0;
	bool isSaved = false;
	Sprite backgroundDesertRoadSprite;

	DesertroadRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, PISTOL, RIFLE, PLASMA_SHOTGUN, 1, window, 0.35, 0.35, true));

		// Spawn initial zombies randomly on the edges


		// Set background scaling
		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		backgroundDesertRoadSprite.setScale(
			(float)mapWidth / backgroundDesertRoad.getSize().x,
			(float)mapHeight / backgroundDesertRoad.getSize().y
		);


		zombieSpawn.restart();

		//background

	 // background Sprite
		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background
		backgroundDesertRoadSprite.setScale(
			mapWidth / backgroundDesertRoad.getSize().x,
			mapHeight / backgroundDesertRoad.getSize().y
		);
		// timer and view and point after 2 min
	};
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score);
			cout << score << endl;
			isSaved = true;
		}
		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);

		// Dynamically adjust zombie spawn time
		if (!playersArr.empty()) {

			float elapsed = gameTimer.getTime();
			float t = min(elapsed / spawnRampDuration, 1.0f);
			zombieSpawnTime = 3.0f - t * (3.0f - minSpawnTime);

			if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime) {
				float x, y;
				int side = rand() % 4;
				switch (side) {
				case 0: x = rand() % mapWidth; y = 0; break;
				case 1: x = mapWidth; y = rand() % mapHeight; break;
				case 2: x = rand() % mapWidth; y = mapHeight; break;
				case 3: x = 0; y = rand() % mapHeight; break;
				}

				zombiesArr.push_back(ZOMBIE(x, y, 1));

				if (t >= 0.5f) {
					zombiesArr.push_back(ZOMBIE(x, y, 2));
				}

				zombieSpawn.restart();
			}

		}
		if (!playersArr.empty()) {
			//camera view max view and min view to track player
			Vector2f playerPos = playersArr[0].shape.getPosition();
			float halfWidth = window.getSize().x / 2.0f;
			float halfHeight = window.getSize().y / 2.0f;
			float clampedX = max(halfWidth, min(mapWidth - halfWidth, playerPos.x));
			float clampedY = max(halfHeight, min(mapHeight - halfHeight, playerPos.y));
			view.setCenter(clampedX, clampedY);
			window.setView(view);
		}

		for (int i = 0; i < playersArr.size(); i++) {
			Vector2f playerPos = playersArr[i].shape.getPosition();

			const float mapWidth = 2000.0f;
			const float mapHeight = 1500.0f;

			FloatRect playerBounds = playersArr[i].shape.getGlobalBounds();
			float halfWidth = playerBounds.width / 2.0f;
			float halfHeight = playerBounds.height / 2.0f;

			if (playerPos.x < halfWidth) playerPos.x = halfWidth;
			if (playerPos.x > mapWidth - halfWidth) playerPos.x = mapWidth - halfWidth;
			if (playerPos.y < halfHeight) playerPos.y = halfHeight;
			if (playerPos.y > mapHeight - halfHeight) playerPos.y = mapHeight - halfHeight;

			playersArr[i].shape.setPosition(playerPos);
		}
		if (!playersArr.empty())
		{
			// Get player position
			Vector2f pos = playersArr[0].shape.getPosition();

			// Prevent moving out of top border
			if (pos.y <= 0)
				playersArr[0].shape.move(0, playersArr[0].speed);

			// Prevent moving out of bottom border
			if (pos.y + playersArr[0].shape.getGlobalBounds().height >= mapHeight + 100)
				playersArr[0].shape.move(0, -playersArr[0].speed);

			// Prevent moving out of left border
			if (pos.x <= 0)
				playersArr[0].shape.move(playersArr[0].speed, 0);

			// Prevent moving out of right border
			if (pos.x + playersArr[0].shape.getGlobalBounds().width >= mapWidth + 100)
				playersArr[0].shape.move(-playersArr[0].speed, 0);
			for (int i = 0; i < zombiesArr.size(); i++) {
				if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
					score += 100;
					zombiesArr[i].isDeadCounter = true;;
				}
			}
		}

	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundDesertRoadSprite);
		drawEntities(playersArr, zombiesArr, window, true, true);

	};
};
struct WoodsRush {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int score = 0;
	int mission1_zombies_counter = 0;
	bool missionComplete = false;
	Clock Timer;
	float zombie_respawntime = 4.5f;
	int zombie_perspawn = 2;
	int dog_perspawn = 1;
	int totalzombiekilled = 0;
	Clock zombieSpawnClock;
	Sprite backgroundWoods;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float minSpawnTime = 0.5f;
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	bool isSaved = false;
	int getRandomOutsideRange_x() {
		int left = rand() % 500 - 500;      // -500 to -1
		int right = rand() % 580 + 1921;    // 1921 to 2500

		if (rand() % 2 == 0)
			return left;
		else
			return right;
	}

	int getRandomOutsideRange_y() {
		int up = rand() % 500 - 500;       // -500 to -1
		int down = rand() % 500 + 1081;     // 1081 to 1580

		if (rand() % 2 == 0)
			return up;
		else
			return down;
	}

	WoodsRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PLASMA_RIFLE, PISTOL, RIFLE, PLASMA_SHOTGUN, 1, window, 0.35, 0.35, true));
		backgroundWoods.setTexture(woodsBackgroundTexture);
		backgroundWoods.setScale(
			(float)mapWidth / woodsBackgroundTexture.getSize().x,
			(float)mapHeight / woodsBackgroundTexture.getSize().y
		);

	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score);
			cout << score << endl;
			isSaved = true;
		}

		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			float elapsed = gameTimer.getTime();
			float t = min(elapsed / spawnRampDuration, 1.0f);
			zombieSpawnTime = 3.0f - t * (3.0f - minSpawnTime);

			if (zombieSpawn.getElapsedTime().asSeconds() >= zombieSpawnTime) {
				float x, y;
				int side = rand() % 4;
				switch (side) {
				case 0: x = rand() % mapWidth; y = 0; break;
				case 1: x = mapWidth; y = rand() % mapHeight; break;
				case 2: x = rand() % mapWidth; y = mapHeight; break;
				case 3: x = 0; y = rand() % mapHeight; break;
				}

				zombiesArr.push_back(ZOMBIE(x, y, 1));

				if (t >= 0.5f) {
					zombiesArr.push_back(ZOMBIE(x, y, 2));
				}

				zombieSpawn.restart();
			}

			if (!playersArr.empty()) {
				//camera view max view and min view to track player
				Vector2f playerPos = playersArr[0].shape.getPosition();
				float halfWidth = window.getSize().x / 2.0f;
				float halfHeight = window.getSize().y / 2.0f;
				float clampedX = max(halfWidth, min(mapWidth - halfWidth, playerPos.x));
				float clampedY = max(halfHeight, min(mapHeight - halfHeight, playerPos.y));
				view.setCenter(clampedX, clampedY);
				window.setView(view);
			}

			if (!playersArr.empty())
			{
				// Get player position
				Vector2f pos = playersArr[0].shape.getPosition();

				// Prevent moving out of top border
				if (pos.y <= 0)
					playersArr[0].shape.move(0, playersArr[0].speed);

				// Prevent moving out of bottom border
				if (pos.y + playersArr[0].shape.getGlobalBounds().height >= mapHeight + 100)
					playersArr[0].shape.move(0, -playersArr[0].speed);

				// Prevent moving out of left border
				if (pos.x <= 0)
					playersArr[0].shape.move(playersArr[0].speed, 0);

				// Prevent moving out of right border
				if (pos.x + playersArr[0].shape.getGlobalBounds().width >= mapWidth + 100)
					playersArr[0].shape.move(-playersArr[0].speed, 0);

			}
		}
		for (int i = 0; i < zombiesArr.size(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				score += 100;
				zombiesArr[i].isDeadCounter = true;
			}
		}

	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundWoods);
		drawEntities(playersArr, zombiesArr, window, true, true);

	};
};




struct levelHandler {
	int id = -1;

	void* currentLevel = nullptr;

	void init(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {

		gameTimer.reset();
		switch (id) {
		case 0:
			currentLevel = new Beachlevel(playersArr, zombiesArr, window);
			break;
		case 1:
			currentLevel = new Desertroad(playersArr, zombiesArr, window);
			break;
		case 2:
			currentLevel = new City(playersArr, zombiesArr, window);
			break;
		case 3:
			currentLevel = new Safezone1(playersArr, zombiesArr, window);
			break;
		case 4:
			currentLevel = new Safezone2(playersArr, zombiesArr, window);
			break;
		case 5:
			currentLevel = new Woods(playersArr, zombiesArr, window);
			break;
		case 6:
			currentLevel = new Safezone3(playersArr, zombiesArr, window);
			break;
		case 7:
			currentLevel = new Army(playersArr, zombiesArr, window);
			break;
		case 8:
			currentLevel = new Mission1(playersArr, zombiesArr, window);
			break;
		case 9:
			currentLevel = new Mission2(playersArr, zombiesArr, window);
			break;
		case 10:
			currentLevel = new Mission3(playersArr, zombiesArr, window);
			break;
		case 11:
			currentLevel = new Mission4(playersArr, zombiesArr, window);
			break;
		case 12:
			currentLevel = new BeachRush(playersArr, zombiesArr, window);
			break;
		case 13:
			currentLevel = new DesertroadRush(playersArr, zombiesArr, window);
			break;
		case 14:
			currentLevel = new WoodsRush(playersArr, zombiesArr, window);
			break;
		}

	}

	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		switch (id) {
		case 0:
			((Beachlevel*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 1:
			((Desertroad*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 2:
			((City*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 3:
			((Safezone1*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 4:
			((Safezone2*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 5:
			((Woods*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 6:
			((Safezone3*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 7:
			((Army*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 8:
			((Mission1*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 9:
			((Mission2*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 10:
			((Mission3*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 11:
			((Mission4*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 12:
			((BeachRush*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 13:
			((DesertroadRush*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 14:
			((WoodsRush*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		}

	}

	bool getMissionComplete() {
		switch (id) {
		case 0: return ((Beachlevel*)currentLevel)->missionComplete;
		case 1: return ((Desertroad*)currentLevel)->missionComplete;
		case 2: return ((City*)currentLevel)->missionComplete;
		case 3: return ((Safezone1*)currentLevel)->missionComplete;
		case 4: return ((Safezone2*)currentLevel)->missionComplete;
		case 5: return ((Woods*)currentLevel)->missionComplete;
		case 6: return ((Safezone3*)currentLevel)->missionComplete;
		case 7: return ((Army*)currentLevel)->missionComplete;
		case 8: return ((Mission1*)currentLevel)->missionComplete;
		case 9: return ((Mission2*)currentLevel)->missionComplete;
		case 10: return ((Mission3*)currentLevel)->missionComplete;
		case 11: return ((Mission4*)currentLevel)->missionComplete;
		case 12: return ((BeachRush*)currentLevel)->missionComplete;
		case 13: return ((DesertroadRush*)currentLevel)->missionComplete;
		case 14: return ((WoodsRush*)currentLevel)->missionComplete;
		default: return false;
		}
	}

	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		switch (id) {
		case 0:
			((Beachlevel*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 1:
			((Desertroad*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 2:
			((City*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 3:
			((Safezone1*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 4:
			((Safezone2*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 5:
			((Woods*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 6:
			((Safezone3*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 7:
			((Army*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 8:
			((Mission1*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 9:
			((Mission2*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 10:
			((Mission3*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 11:
			((Mission4*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 12:
			((BeachRush*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 13:
			((DesertroadRush*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 14:
			((WoodsRush*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		}
	}

	void deleteCurrentLevel() {
		switch (id) {
		case 0: delete (Beachlevel*)currentLevel; break;
		case 1: delete (Desertroad*)currentLevel; break;
		case 2: delete (City*)currentLevel; break;
		case 3: delete (Safezone1*)currentLevel; break;
		case 4: delete (Safezone2*)currentLevel; break;
		case 5: delete (Woods*)currentLevel; break;
		case 6: delete (Safezone3*)currentLevel; break;
		case 7: delete (Army*)currentLevel; break;
		case 8: delete (Mission1*)currentLevel; break;
		case 9: delete (Mission2*)currentLevel; break;
		case 10: delete (Mission3*)currentLevel; break;
		case 11: delete (Mission4*)currentLevel; break;
		case 12: delete (BeachRush*)currentLevel; break;
		case 13: delete (DesertroadRush*)currentLevel; break;
		case 14: delete (WoodsRush*)currentLevel; break;
		}
		//currentLevel = nullptr;
	}
	~levelHandler() {
		switch (id) {
		case 0: delete (Beachlevel*)currentLevel; break;
		case 1: delete (Desertroad*)currentLevel; break;
		case 2: delete (City*)currentLevel; break;
		case 3: delete (Safezone1*)currentLevel; break;
		case 4: delete (Safezone2*)currentLevel; break;
		case 5: delete (Woods*)currentLevel; break;
		case 6: delete (Safezone3*)currentLevel; break;
		case 7: delete (Army*)currentLevel; break;
		case 8: delete (Mission1*)currentLevel; break;
		case 9: delete (Mission2*)currentLevel; break;
		case 10: delete (Mission3*)currentLevel; break;
		case 12: delete (BeachRush*)currentLevel; break;
		case 13: delete (DesertroadRush*)currentLevel; break;
		case 14: delete (WoodsRush*)currentLevel; break;
		}
	}
};






void showDeath(vector<PLAYER>& playersArr, RenderWindow& window) {

	Sprite beachdead;
	beachdead.setTexture(beachdeadtexture);
	beachdead.setOrigin(beachdead.getLocalBounds().width / 2, beachdead.getLocalBounds().height / 2);
	gameTimer.pause();
	beachdead.setPosition(window.mapPixelToCoords({ 1920 / 2,1080 / 2 }));

	beachdead.setScale(2, 2);
	if (playersArr.empty())

	{

		window.draw(beachdead);

	}
}

bool showInstructionsBool = false;
bool isMainmenuTriggerdByPause = false;
void showInstructions(RenderWindow& window, Event& event, Font& font, levelHandler& currentLevel, vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) {

	Texture photoTexture;
	Texture instructionsTexture;
	if (!instructionsTexture.loadFromFile("imgs/pause/backgroundI.jpg")) return;
	photoTexture.loadFromFile("imgs/pause/backgroundC.png");
	//SoundBuffer backBuffer;
	//backBuffer.loadFromFile("imgs/pause/Button.wav");
	//Sound backSound;
	//backSound.setBuffer(backBuffer);
	//sprite.setTexture(texture);
	Sprite photoSprite;
	Sprite instructionsSprite;
	instructionsSprite.setTexture(instructionsTexture);
	instructionsSprite.setScale(1.5, 1.5);
	photoSprite.setTexture(photoTexture);
	photoSprite.setPosition(window.mapPixelToCoords({ 1700, 25 }));
	photoSprite.setScale(1.75, 1.75);

	Text insText;
	insText.setFont(font);
	insText.setString(" W --->> move up \n\n S --->> move down\n\n D --->> move right\n\n A --->> move left\n\n F --->> melee attack\n\n R --->> reload\n\n Q --->> switch weapons\n\n Aim using Mouse and Click to shoot\n");
	insText.setCharacterSize(50);
	insText.setFillColor(Color::White);
	insText.setPosition(window.mapPixelToCoords({ 50, 50 }));

	Text backButton;
	backButton.setFont(font);
	backButton.setString("Back");
	backButton.setCharacterSize(50);
	backButton.setFillColor(Color::Red);
	backButton.setPosition(window.mapPixelToCoords({ 1700, 950 }));

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				return;

			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

			if (backButton.getGlobalBounds().contains(mouseWorldPos))
				backButton.setFillColor(Color::White);
			else
				backButton.setFillColor(Color::Red);

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				if (backButton.getGlobalBounds().contains(mouseWorldPos))
				{
					/*backSound.play();*/
					pauseGame = false;
					return;
				}

			}
		}
		window.clear();

		window.draw(instructionsSprite);
		window.draw(photoSprite);
		window.draw(insText);
		window.draw(backButton);
		window.display();


	}

}

bool running = true;
bool levelStarted = false;



void pause_menu(RenderWindow& window, Event& event, Font& font, levelHandler& currentLevel, vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) {
	window.setMouseCursorVisible(true);

	Texture pauseBackgroundTexture;
	pauseBackgroundTexture.loadFromFile("imgs/pause/main3.jpg");
	Sprite pauseBackgroundSprite;
	pauseBackgroundSprite.setTexture(pauseBackgroundTexture);
	pauseBackgroundSprite.setScale(4.25, 4.25);

	//Font fonts;
	//fonts.loadFromFile("imgs/pause/tag.ttf");

	Text resume, back, instructions, exit;

	resume.setFont(font);
	resume.setString("Continue");
	resume.setCharacterSize(40);
	resume.setFillColor(Color::White);
	resume.setOrigin(resume.getLocalBounds().width / 2, resume.getLocalBounds().height / 2);
	resume.setPosition(window.mapPixelToCoords({ 1920 / 2, 1080 / 2 - 100 }));

	back.setFont(font);
	back.setString("Back to main menu");
	back.setCharacterSize(40);
	back.setFillColor(Color::White);
	back.setOrigin(back.getLocalBounds().width / 2, back.getLocalBounds().height / 2);
	back.setPosition(window.mapPixelToCoords({ 1920 / 2, 1080 / 2 - 20 }));

	//instructions.setFont(font);
	//instructions.setString("Instructions");
	//instructions.setCharacterSize(40);
	//instructions.setFillColor(Color::White);
	//instructions.setOrigin(instructions.getLocalBounds().width / 2, instructions.getLocalBounds().height / 2);
	//instructions.setPosition(1920 / 2, 1080 / 2 + 60);

	exit.setFont(font);
	exit.setString("Quit");
	exit.setCharacterSize(40);
	exit.setFillColor(Color::White);
	exit.setOrigin(exit.getLocalBounds().width / 2, exit.getLocalBounds().height / 2);
	exit.setPosition(window.mapPixelToCoords({ 1920 / 2, 1080 / 2 + 60 }));

	int PmenuSelection = 0;
	bool usingMouse = false;  // Variable to track if mouse is used

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			// Detect if mouse or keyboard is used
			if (event.type == Event::MouseMoved) {
				usingMouse = true;  // Mouse is being used
			}
			else if (event.type == Event::KeyPressed) {
				usingMouse = false; // Keyboard is being used
				// To move in choices
				if (event.key.code == Keyboard::Up)
					PmenuSelection = (PmenuSelection - 1 + 3) % 3;
				else if (event.key.code == Keyboard::Down)
					PmenuSelection = (PmenuSelection + 1) % 3;
				else if (event.key.code == Keyboard::Enter) {
					if (PmenuSelection == 0)
						return;
					else if (PmenuSelection == 1) {
						isMainmenuTriggerdByPause = true;
						return;  // add function menu instead of return
					}
					/*	else if (PmenuSelection == 2 && showInstructionsBool) {
							return;
						}*/

					else if (PmenuSelection == 2) {
						saveScores();

						window.close();
					}
				}
			}

			// Mouse button pressed logic
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				if (resume.getGlobalBounds().contains(mouseWorldPos))
					return;
				if (back.getGlobalBounds().contains(mouseWorldPos))
					isMainmenuTriggerdByPause = true;


				if (exit.getGlobalBounds().contains(mouseWorldPos))

				{

					saveScores();
					window.close();
				}
				return;
			}
		}

		Vector2i mousePos = Mouse::getPosition(window);
		Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

		// Hover effect logic based on input method
		if (usingMouse) {
			// Mouse hover effect
			if (resume.getGlobalBounds().contains(mouseWorldPos))
				resume.setFillColor(Color::Red);
			else
				resume.setFillColor(Color::White);

			if (back.getGlobalBounds().contains(mouseWorldPos))
				back.setFillColor(Color::Red);
			else
				back.setFillColor(Color::White);

			if (instructions.getGlobalBounds().contains(mouseWorldPos))
				instructions.setFillColor(Color::Red);
			else
				instructions.setFillColor(Color::White);

			if (exit.getGlobalBounds().contains(mouseWorldPos))
				exit.setFillColor(Color::Red);
			else
				exit.setFillColor(Color::White);
		}
		else {
			// Keyboard hover effect
			if (PmenuSelection == 0)
				resume.setFillColor(Color::Red);
			else
				resume.setFillColor(Color::White);

			if (PmenuSelection == 1)
				back.setFillColor(Color::Red);
			else
				back.setFillColor(Color::White);

			if (PmenuSelection == 2)
				exit.setFillColor(Color::Red);
			else
				exit.setFillColor(Color::White);
		}

		window.draw(resume);
		window.draw(back);
		window.draw(instructions);
		window.draw(exit);
		window.display();
	}

	window.setMouseCursorVisible(false);
}


int main() {


	loadScores();


	////////////////////////////////////////////////////////////////////////////EndScene Boolean


	RenderWindow window(VideoMode(1920, 1080), "CrimsonLand", Style::Fullscreen);

	int levelID;

	Font font;
	font.loadFromFile("tag.ttf");

	Texture backgroundTexture;
	backgroundTexture.loadFromFile("crimsonMain.png");
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(2.0f, 2.0f);

	Texture logoTexture;
	logoTexture.loadFromFile("logo.png");
	Sprite logoSprite(logoTexture);
	logoSprite.setPosition(window.mapPixelToCoords({ 1000, -10 }));
	logoSprite.setScale(0.8f, 0.8f);

	SoundBuffer clickBuffer;
	clickBuffer.loadFromFile("Button.wav");
	Sound clickSound(clickBuffer);

	menuMusic.openFromFile("MainSound.wav");
	menuMusic.setLoop(true);
	menuMusic.play();

	const int buttonCount = 6;
	MenuButton buttons[buttonCount];

	const string buttonLabels[buttonCount] = {
		"Story Mode", "Rush Mode", "Leaderboard",
		"Game Controls", "Game Credits", "Quit"
	};

	const string buttonPhrases[buttonCount] = {
		"Live the story of CrimsonLand",
		"Challenge and Get Scores",
		"Top Ten High Scores",
		"Customize your experience",
		"Meet CrimsonLand Heroes",
		"See You Later"
	};

	float yStart = 250;
	for (int i = 0; i < buttonCount; ++i) {
		buttons[i].label.setFont(font);
		buttons[i].label.setString(buttonLabels[i]);
		buttons[i].label.setCharacterSize(44);
		buttons[i].label.setPosition(window.mapPixelToCoords(
			Vector2i(200, yStart + i * 100)
		));		buttons[i].label.setFillColor(Color(155, 155, 155, 240));
		buttons[i].bounds = buttons[i].label.getGlobalBounds();

		buttons[i].phrase.setFont(font);
		buttons[i].phrase.setString(buttonPhrases[i]);
		buttons[i].phrase.setCharacterSize(24);
		buttons[i].phrase.setFillColor(Color(135, 135, 140));
		buttons[i].phrase.setPosition(window.mapPixelToCoords(
			Vector2i(550, yStart + i * 100 + 10)
		));
	}

	int selectedIndex = -1;
	string nextScreen = "";

	bool levelStarted = false;

	loadingAssets(); // insure we have loaded all textures correctly
	loadSounds();



	vector<PLAYER> playersArr;
	vector<ZOMBIE> zombiesArr;
	window.setFramerateLimit(60);


	displayScores();

	levelHandler currentLevel;

	bool kk = false;
	while (window.isOpen()) {
		if (levelIDMenu != -1 && !pauseGame) running = false;
		if (running) {

			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) {
					saveScores();

					window.close();
				}


				window.setMouseCursorVisible(true);

				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::Up) {
						selectedIndex = (selectedIndex - 1 + buttonCount) % buttonCount;
					}
					else if (event.key.code == Keyboard::Down) {
						selectedIndex = (selectedIndex + 1) % buttonCount;
					}
					else if (event.key.code == Keyboard::Enter && selectedIndex != -1) {
						clickSound.play();
						if (selectedIndex == 0) {
							//menuMusic.stop();
							nextScreen = "story";
							running = false;
						}
						else if (selectedIndex == 1) {
							//menuMusic.stop();
							nextScreen = "rush";
							running = false;
						}
						else if (selectedIndex == 2)
							nextScreen = "leaderboard";
						else if (selectedIndex == 3)
							nextScreen = "settings";
						else if (selectedIndex == 4)
							nextScreen = "credits";
						else if (selectedIndex == 5) {
							saveScores();

							window.close();
						}
					}
				}

				if (event.type == Event::MouseMoved) {
					Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
					selectedIndex = -1;
					for (int i = 0; i < buttonCount; ++i) {
						if (buttons[i].bounds.contains(mousePos)) {
							selectedIndex = i;
							break;
						}
					}
				}

				if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
					if (selectedIndex != -1) {
						clickSound.play();
						if (selectedIndex == 0) {
							//menuMusic.stop();
							nextScreen = "story";
							running = false;
						}
						else if (selectedIndex == 1) {
							//menuMusic.stop();
							nextScreen = "rush";
							running = false;
						}
						else if (selectedIndex == 2)
							nextScreen = "leaderboard";
						else if (selectedIndex == 3)
							nextScreen = "settings";
						else if (selectedIndex == 4)
							nextScreen = "credits";
						else if (selectedIndex == 5) {
							saveScores();

							window.close();
						}
					}
				}
			}

			window.clear();
			window.draw(backgroundSprite);
			window.draw(logoSprite);

			for (int i = 0; i < buttonCount; ++i) {
				if (i == selectedIndex) {
					buttons[i].label.setFillColor(Color(150, 35, 35, 255));
					window.draw(buttons[i].phrase);
				}
				else {
					buttons[i].label.setFillColor(Color(155, 155, 155, 240));
				}
				window.draw(buttons[i].label);
			}

			window.display();

			if (!nextScreen.empty()) {
				cout << "Switch to screen: " << nextScreen << endl;

				if (nextScreen == "story") {
					runStoryMode(window);
				}
				else if (nextScreen == "rush") {
					runRushMode(window);
				}
				else if (nextScreen == "leaderboard") {
					runLeaderboard(window);
				}
				else if (nextScreen == "settings") {
					runSettings(window);
				}
				else if (nextScreen == "credits") {
					runCredits(window);
				}

				nextScreen = "";
				running = true;
				//    menuMusic.play(); // resume music 
			}
		}
		else {
			if (!levelStarted) {
				currentLevel.id = levelIDMenu;
				currentLevel.init(playersArr, zombiesArr, window);
				endScene = false;
				isEndedEndScene = false;
				levelStarted = true;
			}

			window.setMouseCursorVisible(false);

			if (currentLevel.getMissionComplete()) {
				if (currentLevel.id == 11 && !endScene && !isEndedEndScene) {

					runEndScene(window);
					isEndedEndScene = true;
					///////////////////////////////////////////////////////////////////////////////Add returnToMainMenu after the scene ends

				}
			}

			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) {
					saveScores();
					window.close();
				}
				for (int i = 0; i < playersArr.size(); i++) {
					playersArr[i].emotesStarter(event);
				}
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::T) {
					kk = true;
				}
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				{
					pauseGame = !pauseGame;
				}
				if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
					gameSounds[12].stop();
					gameSounds[10].stop();
				}
			}

			if (!pauseGame) {
				if (levelStarted) {
					gameTimer.resume();
					currentLevel.update(playersArr, zombiesArr, window);
				}
			}
			else {

				gameTimer.pause();
				pause_menu(window, event, font, currentLevel, playersArr, zombiesArr);
				pauseGame = false;
			}

			window.clear();



			if (levelStarted) {
				currentLevel.draw(playersArr, zombiesArr, window);
			}
			if (isGameEntered && !currentLevel.getMissionComplete())
				if (playersArr.empty())showDeath(playersArr, window);


			if ((currentLevel.getMissionComplete() && ((currentLevel.id != 11) || ((currentLevel.id == 11) && !endScene && isEndedEndScene))) || kk || isMainmenuTriggerdByPause) {

				window.setView(window.getDefaultView());

				running = true;
				menuMusic.play();

				currentLevel.deleteCurrentLevel();
				levelStarted = false;
				levelIDMenu = -1;
				playersArr.clear();
				zombiesArr.clear();
				bullets.clear();
				kk = false;
				isMainmenuTriggerdByPause = false;
				isGameEntered = false;
				gameSounds[1].stop();
				gameSounds[5].stop();

			}


			window.display();


		}

	}

	saveScores();

	return 0;
}

