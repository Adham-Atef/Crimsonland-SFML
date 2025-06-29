#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <random>


const double PI = 3.14159265f;

using namespace sf;
using namespace std;

// Weapon types (IDs)
int levelIDMenu = -1;

bool tryAgain = false;



Texture gui[8];

Sprite pistol_gui;


Sprite shotgun_gui;

Sprite rifle_gui;

Sprite hp_gui;

Sprite ammo_gui;

Sprite knife_gui;

Sprite hud_gui[3];

Sprite hp_gui_p2;

Sprite hud_gui_p2[3];



Texture gui_multi[8];

Sprite pistol_gui_multi[2];

Sprite dead_gui_multi[2];


Sprite shotgun_gui_multi[2];

Sprite rifle_gui_multi[2];

Sprite hp_gui_multi;

Sprite ammo_gui_multi[2];

Sprite knife_gui_multi[2];

Sprite hud_gui_multi[3];





Text ammo_in_clip_multi[2];

Text weapon_name_multi[2];



Text score;

Text colon_text;

Text score_p2;

Text health_percentage;

Text health_percentage_p2;

Text zombies;

Text timeleft;

Text ammo_in_clip;

Text weapon_name;

Clock time_left;

Font font;

int score_ = 0;

int score_2 = 0;

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

string playerName = "Player7"; // Change it With GUI !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string player1 = "Player 1";
string player2 = "Player 2";
string storyPlayer = "Player";


float musicVolume = 50.f;
float sfxVolume = 50.f;



struct Scores {

	int scoreCount = 0;

	int scores[MAX_SCORES] = { 0 };

	string playerName[MAX_SCORES] = { "" };

} mainScores;
int scoreCount = 0;

void loadScores() {
	ifstream inFile(SCORE_FILE);

	if (!inFile) {
		cout << "No existing high score file. Starting fresh." << endl;
		return;
	}

	mainScores.scoreCount = 0;

	int score;

	string name;

	while (mainScores.scoreCount < MAX_SCORES && inFile >> score) {

		inFile.ignore(1);

		getline(inFile, name);

		mainScores.scores[mainScores.scoreCount] = score;

		mainScores.playerName[mainScores.scoreCount] = name;

		mainScores.scoreCount++;

	}



	vector<pair<int, string>> scorePairs(mainScores.scoreCount);

	for (int i = 0; i < mainScores.scoreCount; i++) {

		scorePairs[i] = { mainScores.scores[i], mainScores.playerName[i] };

	}

	sort(scorePairs.begin(), scorePairs.end(), greater<pair<int, string>>());

	for (int i = 0; i < mainScores.scoreCount; i++) {

		mainScores.scores[i] = scorePairs[i].first;

		mainScores.playerName[i] = scorePairs[i].second;

	}
	inFile.close();
}

void saveScores() {
	vector<pair<int, string>> scorePairs(mainScores.scoreCount);

	for (int i = 0; i < mainScores.scoreCount; i++) {

		scorePairs[i] = { mainScores.scores[i], mainScores.playerName[i] };

	}

	sort(scorePairs.begin(), scorePairs.end(), greater<pair<int, string>>());

	for (int i = 0; i < mainScores.scoreCount; i++) {

		mainScores.scores[i] = scorePairs[i].first;

		mainScores.playerName[i] = scorePairs[i].second;

	}

	ofstream outFile(SCORE_FILE, ios::trunc);

	if (!outFile) {

		cerr << "Error saving high scores!" << endl;

		return;

	}



	for (int i = 0; i < mainScores.scoreCount; i++) {

		outFile << mainScores.scores[i] << "\n" << mainScores.playerName[i] << "\n";

	}



	outFile.close();

	cout << "Scores saved successfully (" << mainScores.scoreCount << " entries)." << endl;

}



void addScoreIfHigh(int newScore, const string& newName) {

	for (int i = 0; i < mainScores.scoreCount; i++) {

		if (mainScores.playerName[i] == newName) {

			if (newScore > mainScores.scores[i]) {

				mainScores.scores[i] = newScore;

			}

			return;

		}

	}



	if (mainScores.scoreCount < MAX_SCORES) {

		mainScores.scores[mainScores.scoreCount] = newScore;

		mainScores.playerName[mainScores.scoreCount] = newName;

		mainScores.scoreCount++;

		return;

	}



	int minIndex = 0;

	for (int i = 1; i < mainScores.scoreCount; i++) {

		if (mainScores.scores[i] < mainScores.scores[minIndex]) {

			minIndex = i;

		}

	}



	if (newScore > mainScores.scores[minIndex]) {

		mainScores.scores[minIndex] = newScore;

		mainScores.playerName[minIndex] = newName;

	}

}



void displayScores() {

	vector<pair<int, string>> scorePairs(mainScores.scoreCount);

	for (int i = 0; i < mainScores.scoreCount; i++) {

		scorePairs[i] = { mainScores.scores[i], mainScores.playerName[i] };

	}

	sort(scorePairs.begin(), scorePairs.end(), greater<pair<int, string>>());



	cout << "\nCurrent High Scores:" << endl;

	if (mainScores.scoreCount == 0) {

		cout << "No scores recorded yet." << endl;

		return;

	}



	for (int i = 0; i < mainScores.scoreCount; i++) {

		cout << i + 1 << ". " << scorePairs[i].second << ": " << scorePairs[i].first << endl;

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


bool runDesertScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return false;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Music music;
	if (!music.openFromFile("MainSound.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	Texture talking;
	if (!talking.loadFromFile("imgs/speechbox.png")) return false;
	Sprite talk(talking);

	Texture desertScene;
	if (!desertScene.loadFromFile("imgs/desertScene.png")) return false;
	Sprite scene(desertScene);
	scene.setScale(6.8f, 3.5f);
	scene.setPosition(-1000, -30);

	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);

	Text framesText[5];
	for (int i = 0; i < 5; i++) {
		framesText[i].setFont(font);
		framesText[i].setFillColor((i % 2 == 0 && i != 0) ? Color::Black : Color::Black);

		switch (i) {
		case 0:
			framesText[i].setCharacterSize(26);
			framesText[i].setPosition(450, 300);
			framesText[i].setString("You are now so close to your home.");
			break;
		case 1:
			framesText[i].setCharacterSize(20);
			framesText[i].setPosition(450, 300);
			framesText[i].setString("I must go where I told you. My family is waiting for me.");
			break;
		case 2:
			framesText[i].setCharacterSize(18);
			framesText[i].setPosition(620, 570);
			framesText[i].setString("ohhh! I hope you find them doing well. Thank you a lot for saving my life");
			break;
		case 3:
			framesText[i].setCharacterSize(20);
			framesText[i].setPosition(450, 300);
			framesText[i].setString("You are welcome. Take this pistol and take care of yourself.");
			break;
		case 4:
			framesText[i].setCharacterSize(26);
			framesText[i].setPosition(650, 590);
			framesText[i].setString("Thank You. Goodbye!");
			break;
		}
	}

	RectangleShape fade(Vector2f(1920, 1080));
	fade.setFillColor(Color(0, 0, 0, 255));

	Clock sceneClock;
	int frame = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
				frame = 7;
		}

		float time = sceneClock.getElapsedTime().asSeconds();
		if (frame < 7) {
			if (time >= 2 && time < 4) frame = 1;
			if (time >= 4 && time < 6) frame = 2;
			if (time >= 6 && time < 8) frame = 3;
			if (time >= 8 && time < 10) frame = 4;
			if (time >= 10 && time < 12) frame = 5;
			if (time >= 12 && time < 14) frame = 6;
			if (time >= 14) frame = 7;
		}

		window.clear();
		window.draw(background);
		window.draw(scene);
		window.draw(pressSpace);

		switch (frame) {
		case 0: {
			int alpha = static_cast<int>(255 - (time / 2.0f) * 255);
			alpha = std::clamp(alpha, 0, 255);
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 1: {
			talk.setPosition(330, 170);
			window.draw(talk);
			window.draw(framesText[0]);
			break;
		}
		case 2: {
			talk.setPosition(330, 170);
			window.draw(talk);
			window.draw(framesText[1]);
			break;
		}
		case 3: {
			talk.setPosition(540, 440);
			window.draw(talk);
			window.draw(framesText[2]);
			break;
		}
		case 4: {
			talk.setPosition(330, 170);
			window.draw(talk);
			window.draw(framesText[3]);
			break;
		}
		case 5: {
			talk.setPosition(540, 440);
			window.draw(talk);
			window.draw(framesText[4]);
			break;
		}
		case 6: {
			int alpha = static_cast<int>(((time - 12.0f) / 2.0f) * 255);
			alpha = std::clamp(alpha, 0, 255);
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 7: {
			music.stop();
			return true;
		}
		}

		window.display();
	}

	return false;
}


bool runCityScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return false;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Music music;
	if (!music.openFromFile("MainSound.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	Texture talking;
	if (!talking.loadFromFile("imgs/speechbox.png")) return false;
	Sprite talk(talking);
	talk.setPosition(-90, -40);
	talk.setScale(1.79f, 0.8f);


	Texture desertScene;
	if (!desertScene.loadFromFile("imgs/city_scene.png")) return false;
	Sprite scene(desertScene);
	scene.setScale(6.8f, 3.6f);
	scene.setPosition(-700, -30);


	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);


	Text framesText[2];
	for (int i = 0; i < 2; i++) {
		framesText[i].setFont(font);
		framesText[i].setFillColor(Color::Black);

		if (i == 0) {
			framesText[i].setCharacterSize(22);
			framesText[i].setPosition(35, 50);
			framesText[i].setString("To all who managed to survive, anyone who's still alive around!! Head to the Train Staion. it's a SAFE ZONE.");
		}
		if (i == 1) {
			framesText[i].setCharacterSize(22);
			framesText[i].setPosition(7, 50);
			framesText[i].setString("Again... To all who managed to survive, anyone who's still alive around!! Head to the Train Staion. it's a SAFE ZONE.");
		}
	}
	RectangleShape fade(Vector2f(1920, 1080));
	fade.setFillColor(Color(0, 0, 0, 255));

	Clock sceneClock;
	int frame = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				frame = 4;
			}
		}


		float time = sceneClock.getElapsedTime().asSeconds();
		if (frame < 4) {
			if (time >= 3.5 && time < 7) frame = 1;
			if (time >= 7 && time < 10.5) frame = 2;
			if (time >= 10.5 && time < 14) frame = 3;
			if (time >= 14 && time < 17.5) frame = 4;
		}


		window.clear();
		window.draw(background);
		window.draw(scene);
		if (frame >= 1 && frame < 3) window.draw(talk);
		window.draw(pressSpace);

		switch (frame) {
		case 0: {
			int alpha = static_cast<int>(255 - (time / 3.5f) * 255);
			if (alpha < 0) alpha = 0;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 1: { window.draw(framesText[0]);  break; }
		case 2: { window.draw(framesText[1]);  break; }
		case 3: {
			int alpha = static_cast<int>(((time - 10.5f) / 3.5f) * 255);
			if (alpha > 255) alpha = 255;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 4: {
			music.stop();
			return true;
		}
		}

		window.display();

	}
	return false;
}



bool runTrainScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return false;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Music music;
	if (!music.openFromFile("MainSound.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	Texture talking1;
	if (!talking1.loadFromFile("imgs/talk1.png")) return false;
	Sprite talk1(talking1);
	talk1.setPosition(850, 190);
	talk1.setScale(0.8f, 0.4f);


	Texture talking2;
	if (!talking2.loadFromFile("imgs/talk2.png")) return false;
	Sprite talk2(talking2);
	talk2.setPosition(580, 250);
	talk2.setScale(0.5f, 0.37f);


	Texture desertScene;
	if (!desertScene.loadFromFile("imgs/trainScene.png")) return false;
	Sprite scene(desertScene);
	scene.setScale(4.15f, 2.6f);
	scene.setPosition(0, 0);


	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);


	Text framesText[2];
	for (int i = 0; i < 2; i++) {
		framesText[i].setFont(font);
		framesText[i].setFillColor(Color::Black);

		if (i == 0) {
			framesText[i].setCharacterSize(26);
			framesText[i].setPosition(670, 500);
			framesText[i].setString("Where would we go then?!!");
		}
		if (i == 1) {
			framesText[i].setCharacterSize(24);
			framesText[i].setPosition(990, 460);
			framesText[i].setString("The Farm is a safe zone. Perhaps,it's still safe.\nIt will be our destination...");
		}
	}
	RectangleShape fade(Vector2f(1920, 1080));
	fade.setFillColor(Color(0, 0, 0, 255));

	Clock sceneClock;
	int frame = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				frame = 4;
			}
		}


		float time = sceneClock.getElapsedTime().asSeconds();
		if (frame < 4) {
			if (time >= 3.5 && time < 7) frame = 1;
			if (time >= 7 && time < 10.5) frame = 2;
			if (time >= 10.5 && time < 14) frame = 3;
			if (time >= 14 && time < 17.5) frame = 4;
		}


		window.clear();
		window.draw(background);
		window.draw(scene);
		window.draw(pressSpace);

		switch (frame) {
		case 0: {
			int alpha = static_cast<int>(255 - (time / 3.5f) * 255);
			if (alpha < 0) alpha = 0;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 1: { window.draw(talk2); window.draw(framesText[0]); break; }
		case 2: { window.draw(talk1); window.draw(framesText[1]); break; }
		case 3: {
			int alpha = static_cast<int>(((time - 10.5f) / 3.5f) * 255);
			if (alpha > 255) alpha = 255;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 4: {
			music.stop();
			return true;
		}
		}

		window.display();

	}
	return false;
}




bool runWoodsScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture smsTex, sceneTex;
	if (!smsTex.loadFromFile("imgs/sms.png") || !sceneTex.loadFromFile("imgs/WoodsScene.png")) return false;
	Sprite sms(smsTex); sms.setPosition(200.f, 200.f);
	Sprite scene(sceneTex); scene.setScale(4.25f, 2.75f); scene.setPosition(-30, 0);

	RectangleShape fade(Vector2f(1920, 1080));
	float fadeAlpha = 255.f, fadeSpeed = 200.f;
	fade.setFillColor(Color(0, 0, 0, (Uint8)fadeAlpha));

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Text pressSpace("Press Space to skip...", font, 26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(200, 900);

	string lines[] = {
		"Alert!",
		"The Airport is Under Attack!! The Airport is NOT safe anymore.",
		"Rescue Teams can not reach them right now.",
		"So, We ask everyone who can help to provide help."
	};

	Text alerts[4];
	for (int i = 0; i < 4; ++i) {
		alerts[i].setFont(font);
		alerts[i].setString(lines[i]);
		alerts[i].setCharacterSize(36);
		alerts[i].setFillColor(Color::White);
		alerts[i].setPosition(700.f, 300.f + i * 100.f);
	}

	Music music;
	if (!music.openFromFile("MainSound.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	Clock clock;
	enum SceneState { FadeInScene, ShowScene, FadeOutScene, FadeInSMS, ShowSMS, FadeOutAll };
	SceneState state = FadeInScene;
	Clock stateClock;

	while (window.isOpen()) {
		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed)
				window.close();
			if (ev.type == Event::KeyPressed && ev.key.code == Keyboard::Space && state == ShowSMS)
				state = FadeOutAll;
		}

		float dt = clock.restart().asSeconds();


		switch (state) {
		case FadeInScene:
			fadeAlpha -= fadeSpeed * dt;
			if (fadeAlpha <= 0.f) {
				fadeAlpha = 0.f;
				state = ShowScene;
				stateClock.restart();
			}
			break;

		case ShowScene:
			if (stateClock.getElapsedTime().asSeconds() >= 2.5f)
				state = FadeOutScene;
			break;

		case FadeOutScene:
			fadeAlpha += fadeSpeed * dt;
			if (fadeAlpha >= 255.f) {
				fadeAlpha = 255.f;
				state = FadeInSMS;
				stateClock.restart();
			}
			break;

		case FadeInSMS:
			fadeAlpha -= fadeSpeed * dt;
			if (fadeAlpha <= 0.f) {
				fadeAlpha = 0.f;
				state = ShowSMS;
				stateClock.restart();
			}
			break;

		case ShowSMS:
			if (stateClock.getElapsedTime().asSeconds() >= 10.f)
				state = FadeOutAll;
			break;

		case FadeOutAll:
			fadeAlpha += fadeSpeed * dt;
			if (fadeAlpha >= 255.f) {
				fadeAlpha = 255.f;
				return true;
			}
			break;
		}

		fade.setFillColor(Color(0, 0, 0, (Uint8)fadeAlpha));

		window.clear();
		if (state == FadeInScene || state == ShowScene || state == FadeOutScene)
			window.draw(scene);

		else {
			window.draw(sms);
			window.draw(pressSpace);
			for (auto& txt : alerts) window.draw(txt);
		}
		window.draw(fade);
		window.display();
	}
	return false;
}



bool runCampScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return false;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Music music;
	if (!music.openFromFile("MainSound.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	Texture talking1;
	if (!talking1.loadFromFile("imgs/talk1.png")) return false;
	Sprite talk1(talking1);
	talk1.setPosition(20, -150);
	talk1.setScale(0.88f, 0.5f);

	Texture desertScene;
	if (!desertScene.loadFromFile("imgs/armyScene.png")) return false;
	Sprite scene1(desertScene);
	scene1.setScale(4.2f, 2.79f);
	scene1.setPosition(0, 0);

	Texture airScene;
	if (!airScene.loadFromFile("imgs/Army_Scene.png")) return false;
	Sprite scene2(airScene);
	scene2.setScale(4.2f, 2.6f);
	scene2.setPosition(0, 0);

	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);

	Text framesText[3];
	for (int i = 0; i < 3; i++) {
		framesText[i].setFont(font);
		framesText[i].setFillColor(Color::Black);
		framesText[i].setCharacterSize(20);

		if (i == 0)
			framesText[i].setPosition(170, 210),
			framesText[i].setString("We won't escape anymore.. We must fight and save our Island\n save our People...save our History\n We won't just our History, we'll enrich it.");
		else if (i == 1)
			framesText[i].setPosition(120, 210),
			framesText[i].setString("There are weapons and armor in the Army Camp but zombies have seized it\n I'll go ther and you'll go with me.\n We all will fight for CrimsonLand...");
		else if (i == 2)
			framesText[i].setPosition(250, 370),
			framesText[i].setString("I'm gonna free the Camp and then let you in\n Don't get close till I tell you to come!");
	}

	RectangleShape fade(Vector2f(1920, 1080));
	fade.setFillColor(Color(0, 0, 0, 255));

	Clock sceneClock;
	int frame = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
				frame = 7;
		}

		float time = sceneClock.getElapsedTime().asSeconds();
		if (frame < 7) {
			if (time >= 3.5 && time < 7) frame = 1;
			if (time >= 7 && time < 10.5) frame = 2;
			if (time >= 10.5 && time < 14) frame = 3;
			if (time >= 14 && time < 17.5) frame = 4;
			if (time >= 17.5 && time < 21) frame = 5;
			if (time >= 21 && time < 24.5) frame = 6;
			if (time >= 24.5) frame = 7;
		}

		window.clear();
		window.draw(background);
		if (frame >= 0 && frame <= 3)
			window.draw(scene1);
		if (frame >= 4)
			window.draw(scene2);

		if (frame >= 1 && frame < 3)
			talk1.setPosition(20, -150), window.draw(talk1);
		if (frame == 5)
			talk1.setScale(0.57f, 0.33f), talk1.setPosition(170, 150), window.draw(talk1);

		window.draw(pressSpace);

		switch (frame) {
		case 0: {
			int alpha = static_cast<int>(255 - (time / 3.5f) * 255);
			if (alpha < 0) alpha = 0;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 1: window.draw(framesText[0]); break;
		case 2: window.draw(framesText[1]); break;
		case 3: {
			int alpha = static_cast<int>(((time - 10.5f) / 3.5f) * 255);
			if (alpha > 255) alpha = 255;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 4: {
			int alpha = static_cast<int>(255 - ((time - 14.0f) / 3.5f) * 255);
			if (alpha < 0) alpha = 0;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 5: window.draw(framesText[2]); break;
		case 6: {
			int alpha = static_cast<int>(((time - 21.0f) / 3.5f) * 255);
			if (alpha > 255) alpha = 255;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 7:
			music.stop();
			return true;
		}

		window.display();
	}
	return false;
}





bool runstartMissionScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return false;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Music music;
	if (!music.openFromFile("MainSound.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);


	Texture talking1;
	if (!talking1.loadFromFile("imgs/talk1.png")) return false;
	Sprite talk1(talking1);
	talk1.setPosition(200, -100);
	talk1.setScale(0.9f, 0.4f);


	Texture desertScene;
	if (!desertScene.loadFromFile("imgs/startMission.png")) return false;
	Sprite scene(desertScene);
	scene.setScale(4.1f, 2.6f);
	scene.setPosition(0, 0);


	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);


	Text framesText[2];
	for (int i = 0; i < 2; i++) {
		framesText[i].setFont(font);
		framesText[i].setFillColor(Color::Black);

		if (i == 0) {
			framesText[i].setCharacterSize(23);
			framesText[i].setPosition(300, 200);
			framesText[i].setString("Now, everyone takes enough weapons and armor. There's a plenty.");
		}
		if (i == 1) {
			framesText[i].setCharacterSize(24);
			framesText[i].setPosition(310, 200);
			framesText[i].setString("Go and fight in groups. And I'm gonna fight alone. I can do that.");
		}
	}
	RectangleShape fade(Vector2f(1920, 1080));
	fade.setFillColor(Color(0, 0, 0, 255));

	Clock sceneClock;
	int frame = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				frame = 4;
			}
		}


		float time = sceneClock.getElapsedTime().asSeconds();
		if (frame < 4) {
			if (time >= 3.5 && time < 7) frame = 1;
			if (time >= 7 && time < 10.5) frame = 2;
			if (time >= 10.5 && time < 14) frame = 3;
			if (time >= 14 && time < 17.5) frame = 4;
		}


		window.clear();
		window.draw(background);
		window.draw(scene);
		if (frame >= 1 && frame < 3) window.draw(talk1);
		window.draw(pressSpace);

		switch (frame) {
		case 0: {
			int alpha = static_cast<int>(255 - (time / 3.5f) * 255);
			if (alpha < 0) alpha = 0;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 1: { window.draw(framesText[0]);  break; }
		case 2: { window.draw(framesText[1]);  break; }
		case 3: {
			int alpha = static_cast<int>(((time - 10.5f) / 3.5f) * 255);
			if (alpha > 255) alpha = 255;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 4: {
			music.stop();
			return true;
		}
		}

		window.display();

	}
	return false;
}



void runMissionScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Font font;
	if (!font.loadFromFile("tag.ttf")) {
		cout << "Failed to load font\n";
		return;
	}

	Music music;
	if (!music.openFromFile("MainSound.wav")) return;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	const int frames = 5;
	const float fadeDuration = 2.0f;
	float textDurations[frames] = { 6.2f, 5.1f, 5.7f, 4.5f, 9.6f };
	const float letterDelay = 0.06f;

	Texture frameTextures[frames];
	Sprite frameSprites[frames];

	for (int i = 0; i < frames; ++i) {
		string path = "imgs/frame" + to_string(i + 1) + ".png";
		if (!frameTextures[i].loadFromFile(path)) {
			cout << "Failed to load " << path << endl;
			return;
		}
		frameSprites[i].setTexture(frameTextures[i]);
		frameSprites[i].setScale(4.15f, 2.65f);
	}

	string narration[frames] = {
		"All CrimsonLand People are fighting side by side and Zombies menance is about to end.",
		"Some CrimsonLand cities came back to life.",
		"The Colonizers eventually failed and the epidemic became out of control.",
		"It has spread out in the whole world.",
		"And now, the Colonizers are facing their fate....\nAnd CrimsonLand is the inspiration to the whole world just as it has been throughout ages."
	};

	RectangleShape textBox(Vector2f(1050, 90));
	textBox.setFillColor(Color(0, 0, 0, 180));
	textBox.setPosition(70, 50);

	Text storyText("", font, 24);
	storyText.setFillColor(Color::White);
	storyText.setPosition(90, 70);

	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);

	RectangleShape fadeRect(Vector2f(1920, 1080));
	fadeRect.setFillColor(Color::Black);

	bool skipAll = false;

	for (int currentFrame = 0; currentFrame < frames && !skipAll; ++currentFrame) {


		Clock fadeClock;
		while (window.isOpen() && !skipAll && fadeClock.getElapsedTime().asSeconds() < fadeDuration) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					return;
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
					skipAll = true;
			}

			float alpha = 255 - (fadeClock.getElapsedTime().asSeconds() / fadeDuration) * 255;
			if (alpha < 0) alpha = 0;
			fadeRect.setFillColor(Color(0, 0, 0, static_cast<Uint8>(alpha)));

			window.clear();
			window.draw(frameSprites[currentFrame]);
			window.draw(textBox);
			window.draw(fadeRect);
			window.draw(pressSpace);
			window.display();
		}

		if (skipAll) break;

		string fullText = narration[currentFrame], displayedText = "";
		Clock textClock;
		int charIndex = 0;
		float lastLetterTime = 0;

		while (window.isOpen() && !skipAll && textClock.getElapsedTime().asSeconds() < textDurations[currentFrame]) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					return;
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
					skipAll = true;
			}

			float elapsed = textClock.getElapsedTime().asSeconds();
			if (charIndex < fullText.size() && elapsed - lastLetterTime > letterDelay) {
				displayedText += fullText[charIndex++];
				storyText.setString(displayedText);
				lastLetterTime = elapsed;
			}

			window.clear();
			window.draw(frameSprites[currentFrame]);
			window.draw(textBox);
			window.draw(storyText);
			window.draw(pressSpace);
			window.display();
		}

		if (skipAll) break;


		fadeClock.restart();
		while (window.isOpen() && !skipAll && fadeClock.getElapsedTime().asSeconds() < fadeDuration) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed)
					return;
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
					skipAll = true;
			}

			float alpha = (fadeClock.getElapsedTime().asSeconds() / fadeDuration) * 255;
			if (alpha > 255) alpha = 255;
			fadeRect.setFillColor(Color(0, 0, 0, static_cast<Uint8>(alpha)));

			window.clear();
			window.draw(frameSprites[currentFrame]);
			window.draw(textBox);
			window.draw(storyText);
			window.draw(fadeRect);
			window.draw(pressSpace);
			window.display();
		}
	}
}


bool runBeachScene(RenderWindow& window) {
	window.setMouseCursorVisible(false);
	window.setView(window.getDefaultView());
	Texture bgTexture;
	if (!bgTexture.loadFromFile("crimsongndd.png")) return false;
	Sprite background(bgTexture);

	Font font;
	if (!font.loadFromFile("tag.ttf")) return false;

	Music music;
	if (!music.openFromFile("Sound/beachScene.wav")) return false;
	music.setLoop(true);
	music.play();
	music.setVolume(musicVolume);

	Texture talking1;
	if (!talking1.loadFromFile("imgs/talk2.png")) return false;
	Sprite talk1(talking1);
	talk1.setPosition(1100, 270);
	talk1.setScale(0.8f, 0.4f);


	Texture talking2;
	if (!talking2.loadFromFile("imgs/talk1.png")) return false;
	Sprite talk2(talking2);
	talk2.setPosition(280, 400);
	talk2.setScale(0.8f, 0.4f);


	Texture desertScene;
	if (!desertScene.loadFromFile("imgs/beachScene.png")) return false;
	Sprite scene(desertScene);
	scene.setScale(4.15f, 2.6f);
	scene.setPosition(0, 0);


	Text pressSpace;
	pressSpace.setFont(font);
	pressSpace.setString("Press Space to skip...");
	pressSpace.setCharacterSize(26);
	pressSpace.setFillColor(Color::White);
	pressSpace.setPosition(100, 900);


	Text framesText[6];
	for (int i = 0; i < 6; i++) {
		framesText[i].setFont(font);
		framesText[i].setFillColor(Color::Black);

		if (i == 0) {
			framesText[i].setCharacterSize(26);
			framesText[i].setPosition(450, 690);
			framesText[i].setString("Hey man, happy Victory Day.");
		}
		if (i == 1) {
			framesText[i].setCharacterSize(24);
			framesText[i].setPosition(1260, 520);
			framesText[i].setString("Thanks, my friend.\n Happy Victory Day to you too.");
		}
		if (i == 2) {
			framesText[i].setCharacterSize(24);
			framesText[i].setPosition(420, 670);
			framesText[i].setString("So.. You're still heading to the beach today?\n Skipping lunch with us, huh?");
		}
		if (i == 3) {
			framesText[i].setCharacterSize(23);
			framesText[i].setPosition(1220, 500);
			framesText[i].setString(" Yeah, I'm sorry.\nYou know that place holds a special meaning for me.\n I just can't let this day go by without being there.");
		}
		if (i == 4) {
			framesText[i].setCharacterSize(24);
			framesText[i].setPosition(420, 670);
			framesText[i].setString("I get it.\n You were a great soldier..You deserve that moment.\n Alright, we'll catch up later.");
		}
		if (i == 5) {
			framesText[i].setCharacterSize(26);
			framesText[i].setPosition(1260, 535);
			framesText[i].setString("Thanks for understanding. See you soon.");
		}
	}
	RectangleShape fade(Vector2f(1920, 1080));
	fade.setFillColor(Color(0, 0, 0, 255));

	Clock sceneClock;
	int frame = 0;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				frame = 8;
			}
		}


		float time = sceneClock.getElapsedTime().asSeconds();
		if (frame < 8) {
			if (time >= 3.5 && time < 7) frame = 1;
			if (time >= 7 && time < 10.5) frame = 2;
			if (time >= 10.5 && time < 14) frame = 3;
			if (time >= 14 && time < 17.5) frame = 4;
			if (time >= 17.5 && time < 21) frame = 5;
			if (time >= 21 && time < 24.5) frame = 6;
			if (time >= 24.5 && time < 28) frame = 7;
			if (time > 28) frame = 8;
		}


		window.clear();
		window.draw(background);
		window.draw(scene);
		window.draw(pressSpace);

		switch (frame) {
		case 0: {
			int alpha = static_cast<int>(255 - (time / 3.5f) * 255);
			if (alpha < 0) alpha = 0;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 1: { window.draw(talk2); window.draw(framesText[0]); break; }
		case 2: { window.draw(talk1); window.draw(framesText[1]); break; }
		case 3: { window.draw(talk2); window.draw(framesText[2]); break; }
		case 4: { window.draw(talk1); window.draw(framesText[3]); break; }
		case 5: { window.draw(talk2); window.draw(framesText[4]); break; }
		case 6: { window.draw(talk1); window.draw(framesText[5]); break; }
		case 7: {
			int alpha = static_cast<int>(((time - 24.5f) / 3.5f) * 255);
			if (alpha > 255) alpha = 255;
			fade.setFillColor(Color(0, 0, 0, alpha));
			window.draw(fade);
			break;
		}
		case 8: {
			music.stop();
			return true;
		}
		}

		window.display();

	}
	return false;
}



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
	music.setVolume(musicVolume);

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



const int totalLevels = 12;
bool unlocked[totalLevels];



void saveStoryProgress(bool unlocked[], int totalLevels) {
	ofstream outFile("progress.dat", ios::binary);
	if (outFile)
		outFile.write(reinterpret_cast<char*>(unlocked), sizeof(bool) * totalLevels);
}


void loadStoryProgress(bool unlocked[], int totalLevels) {
	ifstream inFile("progress.dat", ios::binary);
	if (inFile)
		inFile.read(reinterpret_cast<char*>(unlocked), sizeof(bool) * totalLevels);
	else {
		unlocked[0] = true;
		for (int i = 1; i < totalLevels; ++i)
			unlocked[i] = false;
	}
}




void runStoryMode(RenderWindow& window) {
	const int totalLevels = 12;
	const string levelNames[totalLevels] = {
		"Beach", "Desert Road", "City",
		"Safe Zone 1", "Safe Zone 2", "Woods",
		"Safe Zone 3", "Army Camp", "Mission 1",
		"Mission 2", "Mission 3", "Mission 4"
	};

	loadStoryProgress(unlocked, totalLevels);

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
	clickSound.setVolume(sfxVolume);

	RectangleShape levelRects[totalLevels];
	Text levelTexts[totalLevels];

	int selectedIndex = -1;
	float startX = 250, startY = 250;
	float boxW = 150, boxH = 50;
	float horGap = 30, vertGap = 60;

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
			if (event.type == Event::Closed) {
				saveScores();
				saveStoryProgress(unlocked, totalLevels);
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
					saveStoryProgress(unlocked, totalLevels);
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
						saveStoryProgress(unlocked, totalLevels);
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
					saveStoryProgress(unlocked, totalLevels);
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
				levelRects[i].setScale(1.12f, 1.12f);
			}
			window.draw(levelRects[i]);
			window.draw(levelTexts[i]);
		}

		backBtn.setFillColor(onBackButton ? Color(125, 15, 15, 255) : Color(155, 155, 155, 240));
		window.draw(backBtn);
		window.draw(backText);
		window.display();
	}
}






bool getPlayerName(RenderWindow& window, Font& font, Sprite& backgroundSprite, Sprite& logoSprite, Sound& clickSound) {
	Text ask("Enter Your Name:", font, 35);
	ask.setFillColor(Color(160, 160, 160));
	ask.setPosition(200, 300);

	Text nameText("", font, 40);
	nameText.setFillColor(Color(200, 50, 50));
	nameText.setPosition(200, 400);

	Text instructions("\t\t\t\t\t\t\t\t\t\t\tPress\n\n Enter to confirm\t  Backspace to delete\t  Esc to cancel.", font, 25);
	instructions.setFillColor(Color(150, 150, 150, 240));
	instructions.setPosition(100, 750);

	string input = "";
	bool enteringName = true;

	while (enteringName && window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();
			}
			else if (event.type == Event::TextEntered) {
				if (event.text.unicode < 128 && isprint(event.text.unicode)) {
					if (input.length() < 20) input += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 8) {
					if (!input.empty()) input.pop_back();
				}
			}
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Enter && !input.empty()) {
					clickSound.play();
					playerName = input;
					return true;
				}
				else if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					return false;
				}
			}
		}

		nameText.setString(input);

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(ask);
		window.draw(nameText);
		window.draw(instructions);
		window.display();
	}

	return false;
}

bool getMultiPlayerName(RenderWindow& window, Font& font, Sprite& backgroundSprite, Sprite& logoSprite, Sound& clickSound) {
	Text ask1("Enter Player 1 Name:", font, 35);
	ask1.setFillColor(Color(160, 160, 160));
	ask1.setPosition(200, 300);

	Text ask2("Enter Player 2 Name:", font, 35);
	ask2.setFillColor(Color(160, 160, 160));
	ask2.setPosition(200, 550);

	Text name1Text("", font, 40);
	name1Text.setFillColor(Color(200, 50, 50));
	name1Text.setPosition(200, 400);

	Text name2Text("", font, 40);
	name2Text.setFillColor(Color(200, 50, 50));
	name2Text.setPosition(200, 650);

	Text instructions("\t\t\t\t\t\t\t\t\t\t\tPress\n\n Enter to confirm\t  Backspace to delete\t  Esc to cancel.", font, 25);
	instructions.setFillColor(Color(150, 150, 150, 240));
	instructions.setPosition(100, 800);

	string input1 = "";
	string input2 = "";
	bool firstNameEntered = false;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();
			}
			else if (event.type == Event::TextEntered) {
				if (event.text.unicode < 128 && isprint(event.text.unicode)) {
					if (!firstNameEntered && input1.length() < 20)
						input1 += static_cast<char>(event.text.unicode);
					else if (firstNameEntered && input2.length() < 20)
						input2 += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 8) {
					if (!firstNameEntered && !input1.empty())
						input1.pop_back();
					else if (firstNameEntered && !input2.empty())
						input2.pop_back();
				}
			}
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Enter) {
					if (!firstNameEntered && !input1.empty()) {
						clickSound.play();
						player1 = input1;
						firstNameEntered = true;
					}
					else if (firstNameEntered && !input2.empty()) {
						clickSound.play();
						player2 = input2;
						return true;
					}
				}
				else if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					return false;
				}
			}
		}

		name1Text.setString(input1);
		name2Text.setString(input2);

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(ask1);
		window.draw(ask2);
		window.draw(name1Text);
		window.draw(name2Text);
		window.draw(instructions);
		window.display();
	}

	return false;
}


void rushSingleMaps(RenderWindow& window, Font& font, Sprite& backgroundSprite, Sprite& logoSprite,
	Sound& clickSound, int& selectedMap, bool& backToRushMain) {
	const int MAP_COUNT = 5;
	const string mapNames[MAP_COUNT] = { "Beach", "Desert Road", "Woods", "City", "Army Camp" };

	Text title("Rush Mode", font, 38);
	title.setFillColor(Color(160, 155, 155, 240));
	title.setPosition(150, 100);

	Text choose("Choose a map:", font, 25);
	choose.setFillColor(Color(74, 8, 8, 240));
	choose.setPosition(125, 350);

	Text mapOptions[MAP_COUNT];
	for (int i = 0; i < MAP_COUNT; ++i) {
		mapOptions[i].setFont(font);
		mapOptions[i].setString(mapNames[i]);
		mapOptions[i].setCharacterSize(40);
		mapOptions[i].setPosition(350, 250 + i * 70);
		mapOptions[i].setFillColor(Color(150, 150, 150, 240));
	}

	Text back;
	back.setFont(font);
	back.setString("Back");
	back.setCharacterSize(38);
	back.setPosition(375, 850);
	back.setFillColor(Color(150, 150, 150, 240));

	int selected = 0;
	bool inMapMenu = true;

	while (inMapMenu && window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Up)
					selected = (selected - 1 + MAP_COUNT + 1) % (MAP_COUNT + 1);
				else if (event.key.code == Keyboard::Down)
					selected = (selected + 1) % (MAP_COUNT + 1);
				else if (event.key.code == Keyboard::Enter) {
					clickSound.play();
					if (selected == MAP_COUNT) {
						backToRushMain = true;
						inMapMenu = false;
					}
					else {
						selectedMap = selected;
						levelIDMenu = selected + 12;
						inMapMenu = false;
						backToRushMain = false;
					}
				}
				else if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					backToRushMain = true;
					inMapMenu = false;
				}
			}

			if (event.type == Event::MouseMoved) {
				Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
				for (int i = 0; i < MAP_COUNT; ++i) {
					if (mapOptions[i].getGlobalBounds().contains(mousePos)) {
						selected = i;
					}
				}
				if (back.getGlobalBounds().contains(mousePos)) {
					selected = MAP_COUNT;
				}
			}
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
				for (int i = 0; i < MAP_COUNT; ++i) {
					if (mapOptions[i].getGlobalBounds().contains(clickPos)) {
						selectedMap = i;
						levelIDMenu = selectedMap + 12;
						clickSound.play();
						menuMusic.stop();
						backToRushMain = false;
						inMapMenu = false;
						break;
					}
				}
				if (back.getGlobalBounds().contains(clickPos)) {
					clickSound.play();
					backToRushMain = true;
					inMapMenu = false;
				}
			}
		}

		for (int i = 0; i < MAP_COUNT; ++i)
			mapOptions[i].setFillColor(i == selected ? Color(150, 35, 35, 255) : Color(150, 150, 150, 240));
		back.setFillColor(selected == MAP_COUNT ? Color(150, 35, 35, 255) : Color(150, 150, 150, 240));

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		window.draw(choose);
		for (int i = 0; i < MAP_COUNT; ++i)
			window.draw(mapOptions[i]);
		window.draw(back);
		window.display();
	}
}


void rushMultiMaps(RenderWindow& window, Font& font, Sprite& backgroundSprite, Sprite& logoSprite,
	Sound& clickSound, int& selectedMap, bool& backToRushMain) {
	const int MAP_COUNT = 5;
	const string mapNames[MAP_COUNT] = { "Beach", "Desert Road", "Woods", "City", "Army Camp" };

	Text title("Rush Mode", font, 38);
	title.setFillColor(Color(160, 155, 155, 240));
	title.setPosition(150, 100);

	Text choose("Choose a map:", font, 25);
	choose.setFillColor(Color(74, 8, 8, 240));
	choose.setPosition(125, 350);

	Text mapOptions[MAP_COUNT];
	for (int i = 0; i < MAP_COUNT; ++i) {
		mapOptions[i].setFont(font);
		mapOptions[i].setString(mapNames[i]);
		mapOptions[i].setCharacterSize(40);
		mapOptions[i].setPosition(350, 250 + i * 70);
		mapOptions[i].setFillColor(Color(150, 150, 150, 240));
	}

	Text back;
	back.setFont(font);
	back.setString("Back");
	back.setCharacterSize(38);
	back.setPosition(375, 850);
	back.setFillColor(Color(150, 150, 150, 240));

	int selected = 0;
	bool inMapMenu = true;

	while (inMapMenu && window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Up)
					selected = (selected - 1 + MAP_COUNT + 1) % (MAP_COUNT + 1);
				else if (event.key.code == Keyboard::Down)
					selected = (selected + 1) % (MAP_COUNT + 1);
				else if (event.key.code == Keyboard::Enter) {
					clickSound.play();
					if (selected == MAP_COUNT) {
						backToRushMain = true;
						inMapMenu = false;
					}
					else {
						selectedMap = selected;
						levelIDMenu = selected + 17;
						inMapMenu = false;
						backToRushMain = false;
					}
				}
				else if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					backToRushMain = true;
					inMapMenu = false;
				}
			}

			if (event.type == Event::MouseMoved) {
				Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
				for (int i = 0; i < MAP_COUNT; ++i) {
					if (mapOptions[i].getGlobalBounds().contains(mousePos)) {
						selected = i;
					}
				}
				if (back.getGlobalBounds().contains(mousePos)) {
					selected = MAP_COUNT;
				}
			}
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f clickPos(event.mouseButton.x, event.mouseButton.y);
				for (int i = 0; i < MAP_COUNT; ++i) {
					if (mapOptions[i].getGlobalBounds().contains(clickPos)) {
						selectedMap = i;
						levelIDMenu = selectedMap + 17;
						clickSound.play();
						menuMusic.stop();
						backToRushMain = false;
						inMapMenu = false;
						break;
					}
				}
				if (back.getGlobalBounds().contains(clickPos)) {
					clickSound.play();
					backToRushMain = true;
					inMapMenu = false;
				}
			}
		}

		for (int i = 0; i < MAP_COUNT; ++i)
			mapOptions[i].setFillColor(i == selected ? Color(150, 35, 35, 255) : Color(150, 150, 150, 240));
		back.setFillColor(selected == MAP_COUNT ? Color(150, 35, 35, 255) : Color(150, 150, 150, 240));

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		window.draw(choose);
		for (int i = 0; i < MAP_COUNT; ++i)
			window.draw(mapOptions[i]);
		window.draw(back);
		window.display();
	}
}


bool rushMainMenu(RenderWindow& window, Font& font, Sprite& backgroundSprite, Sprite& logoSprite,
	Sound& clickSound, int& selectedMap) {

	const int MENU_OPTION_COUNT = 3;
	string options[MENU_OPTION_COUNT] = { "Single Player", "Multiplayer", "Back to Main Menu" };

	Text title("Rush Mode", font, 38);
	title.setFillColor(Color(160, 155, 155, 240));
	title.setPosition(150, 100);

	Text optionText[MENU_OPTION_COUNT];
	for (int i = 0; i < MENU_OPTION_COUNT; ++i) {
		optionText[i].setFont(font);
		optionText[i].setString(options[i]);
		optionText[i].setCharacterSize(40);
		optionText[i].setFillColor(Color(150, 150, 150, 240));
		optionText[i].setPosition(350, 350 + i * 125);
	}
	optionText[2].setPosition(350, 850);
	optionText[2].setCharacterSize(35);

	int selected = 0;
	bool stayInMenu = true;

	while (stayInMenu && window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				saveScores();
				window.close();
			}
			else if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Up)
					selected = (selected - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT;
				else if (event.key.code == Keyboard::Down)
					selected = (selected + 1) % MENU_OPTION_COUNT;
				else if (event.key.code == Keyboard::Enter) {
					clickSound.play();
					sleep(milliseconds(150));

					if (selected == 0) {
						bool gotName = getPlayerName(window, font, backgroundSprite, logoSprite, clickSound);
						if (gotName) {
							bool goBack = false;
							rushSingleMaps(window, font, backgroundSprite, logoSprite, clickSound, selectedMap, goBack);
							if (!goBack)
								return false;
						}
					}
					else if (selected == 1) {
						bool gotName = getMultiPlayerName(window, font, backgroundSprite, logoSprite, clickSound);
						if (gotName) {
							bool goBack = false;
							rushMultiMaps(window, font, backgroundSprite, logoSprite, clickSound, selectedMap, goBack);
							if (!goBack)
								return false;
						}
					}
					else {
						return true;
					}
				}
				else if (event.key.code == Keyboard::Escape) {
					clickSound.play();
					sleep(milliseconds(150));
					return true;
				}
			}
			else if (event.type == Event::MouseMoved) {
				Vector2f mousePos = window.mapPixelToCoords(Vector2i(event.mouseMove.x, event.mouseMove.y));
				for (int i = 0; i < MENU_OPTION_COUNT; ++i) {
					if (optionText[i].getGlobalBounds().contains(mousePos)) {
						selected = i;
						break;
					}
				}
			}
			else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f clickPos = window.mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));
				for (int i = 0; i < MENU_OPTION_COUNT; ++i) {
					if (optionText[i].getGlobalBounds().contains(clickPos)) {
						clickSound.play();
						sleep(milliseconds(150));

						if (i == 0) {
							bool gotName = getPlayerName(window, font, backgroundSprite, logoSprite, clickSound);
							if (gotName) {
								bool goBack = false;
								rushSingleMaps(window, font, backgroundSprite, logoSprite, clickSound, selectedMap, goBack);
								if (!goBack)
									return false;
							}
						}
						else if (i == 1) {
							bool gotName = getMultiPlayerName(window, font, backgroundSprite, logoSprite, clickSound);
							if (gotName) {
								bool goBack = false;
								rushMultiMaps(window, font, backgroundSprite, logoSprite, clickSound, selectedMap, goBack);
								if (!goBack)
									return false;
							}
						}
						else {
							return true;
						}
					}
				}
			}
		}

		for (int i = 0; i < MENU_OPTION_COUNT; ++i) {
			optionText[i].setFillColor(i == selected ? Color(150, 35, 35, 255) : Color(150, 150, 150, 240));
		}

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		for (int i = 0; i < MENU_OPTION_COUNT; ++i)
			window.draw(optionText[i]);
		window.display();
	}

	return true;
}



void runRushMode(RenderWindow& window) {
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
	clickSound.setVolume(sfxVolume);

	int selectedMap = 0;
	bool returnToMainMenu = rushMainMenu(window, font, backgroundSprite, logoSprite, clickSound, selectedMap);

	if (!returnToMainMenu) {

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
	clickSound.setVolume(sfxVolume);

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



	vector<pair<int, string>> scorePairs(mainScores.scoreCount);

	for (int i = 0; i < mainScores.scoreCount; i++) {

		scorePairs[i] = { mainScores.scores[i], mainScores.playerName[i] };

	}

	sort(scorePairs.begin(), scorePairs.end(), greater<pair<int, string>>());



	for (int i = 0; i < mainScores.scoreCount; i++) { //setup the leaderboard

		x_[i].setFont(font);

		x_[i].setCharacterSize(36);

		x_[i].setPosition(150, 200 + i * 70);

		x_[i].setFillColor(Color(200, 200, 200));

		x_[i].setString(scorePairs[i].second + " : " + to_string(scorePairs[i].first));

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
		for (int i = 0; i < mainScores.scoreCount; i++)

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
	clickSound.setVolume(sfxVolume);

	menuMusic.setVolume(musicVolume);

	Text title;
	title.setFont(font);
	title.setString("Game Controls");
	title.setCharacterSize(50);
	title.setFillColor(Color(135, 135, 135, 240));
	title.setPosition(window.mapPixelToCoords({ 150, 100 }));

	Text controlsText;
	controlsText.setFont(font);
	controlsText.setCharacterSize(26);
	controlsText.setFillColor(Color(180, 180, 180));
	controlsText.setPosition(250, 200);
	controlsText.setString(
		"W/A/S/D   \t- move\n"
		"Mouse         \t- Aim\n"
		"Left Click  \t- Shoot\n"
		"Q                  \t- Switch Weapons\n"
		"F                   \t- Melee Attack\n"
		"R                   \t- Reload\n"
		"Escape        \t- Back / Pause\n\n"
		"In Multiplayer: Using Controller\n"
		"Left Analog   \t- move\n"
		"Right Analog\t- Aim\n"
		"R1                     \t- Shoot\n"
		"R2                    \t- Reload\n"
		"L1                     \t- Melee Attack\n"
		"L2                    \t- Switch Weapons\n"
	);

	Text backButton;
	backButton.setFont(font);
	backButton.setString("Back to Main Menu");
	backButton.setCharacterSize(34);
	backButton.setFillColor(Color(155, 155, 155, 240));
	backButton.setPosition(window.mapPixelToCoords({ 250, 900 }));


	RectangleShape musicBarBack(Vector2f(300, 10));
	musicBarBack.setFillColor(Color(50, 50, 50));
	musicBarBack.setPosition(450, 760);

	RectangleShape musicBarFill(Vector2f((musicVolume / 100.f) * 300.f, 10));
	musicBarFill.setFillColor(Color(200, 100, 100));
	musicBarFill.setPosition(450, 760);

	Text musicLabel("Music Volume", font, 26);
	musicLabel.setFillColor(Color(180, 180, 180));
	musicLabel.setPosition(230, 745);


	RectangleShape sfxBarBack(Vector2f(300, 10));
	sfxBarBack.setFillColor(Color(50, 50, 50));
	sfxBarBack.setPosition(450, 820);

	RectangleShape sfxBarFill(Vector2f((sfxVolume / 100.f) * 300.f, 10));
	sfxBarFill.setFillColor(Color(250, 150, 100));
	sfxBarFill.setPosition(450, 820);

	Text sfxLabel("SFX Volume", font, 26);
	sfxLabel.setFillColor(Color(180, 180, 180));
	sfxLabel.setPosition(230, 805);

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


				if (musicBarBack.getGlobalBounds().contains(mousePos)) {
					float relX = mousePos.x - musicBarBack.getPosition().x;
					relX = clamp(relX, 0.f, 300.f);
					musicBarFill.setSize(Vector2f(relX, 10));
					musicVolume = (relX / 300.f) * 100.f;
					menuMusic.setVolume(musicVolume);
				}


				if (sfxBarBack.getGlobalBounds().contains(mousePos)) {
					float relX = mousePos.x - sfxBarBack.getPosition().x;
					relX = clamp(relX, 0.f, 300.f);
					sfxBarFill.setSize(Vector2f(relX, 10));
					sfxVolume = (relX / 300.f) * 100.f;
					clickSound.setVolume(sfxVolume);
					clickSound.play();
				}
			}
		}

		Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		backButton.setFillColor(
			backButton.getGlobalBounds().contains(mousePos) ?
			Color(155, 35, 35, 255) : Color(135, 135, 135, 240)
		);

		window.clear();
		window.draw(backgroundSprite);
		window.draw(logoSprite);
		window.draw(title);
		window.draw(controlsText);


		window.draw(musicLabel);
		window.draw(musicBarBack);
		window.draw(musicBarFill);

		window.draw(sfxLabel);
		window.draw(sfxBarBack);
		window.draw(sfxBarFill);

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
	clickSound.setVolume(sfxVolume);

	Text title;
	title.setFont(font);
	title.setString("Game Credits");
	title.setCharacterSize(50);
	title.setFillColor(Color(135, 135, 135, 240));
	title.setPosition(150, 100);

	Text creditsText;
	creditsText.setFont(font);
	creditsText.setString("Developed by\n\n   \tAli Adel\n   \tAdham Atef\n   \tHossam Ibrahim\n   \tRawan Ahmed\n   \tHazem Ahmed\n   \tMichael Sameh\n   \tHassan Alaa\n  \n\n\tDirected by\n\n  \tHossam Ibrahim");
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
		window.draw(creditsText);
		window.draw(backButton);
		window.display();
	}
}





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

Texture perksTexture[12];



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

	//perks
	if (!loadTexture(perksTexture[0], "imgs/perks/pistol.jpg")) {
		cout << "Error loading knife texture" << endl;
	}
	if (!loadTexture(perksTexture[1], "imgs/perks/rifle.jpg")) {
		cout << "Error loading handgun texture" << endl;
	}
	if (!loadTexture(perksTexture[2], "imgs/perks/burst_rifle.jpg")) {
		cout << "Error loading rifle texture" << endl;
	}
	if (!loadTexture(perksTexture[3], "imgs/perks/single_rifle.jpg")) {
		cout << "Error loading shotgun texture" << endl;
	}
	if (!loadTexture(perksTexture[4], "imgs/perks/shotgun.jpg")) {
		cout << "Error loading knife texture" << endl;
	}
	if (!loadTexture(perksTexture[5], "imgs/perks/plasma_pistol.jpg")) {
		cout << "Error loading handgun texture" << endl;
	}
	if (!loadTexture(perksTexture[6], "imgs/perks/plasma_rifle.jpg")) {
		cout << "Error loading rifle texture" << endl;
	}
	if (!loadTexture(perksTexture[7], "imgs/perks/plasma_shotgun.jpg")) {
		cout << "Error loading shotgun texture" << endl;
	}
	if (!loadTexture(perksTexture[8], "imgs/perks/health.jpg")) {
		cout << "Error loading knife texture" << endl;
	}
	if (!loadTexture(perksTexture[9], "imgs/perks/shield.jpg")) {
		cout << "Error loading handgun texture" << endl;
	}
	if (!loadTexture(perksTexture[10], "imgs/perks/double.jpg")) {
		cout << "Error loading rifle texture" << endl;
	}
	if (!loadTexture(perksTexture[11], "imgs/perks/slow.jpg")) {
		cout << "Error loading shotgun texture" << endl;
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
	gameSounds[0].sound.setVolume(sfxVolume);

}

void ZomibeOnBeach() {
	gameSounds[1].sound.setVolume(35);
	gameSounds[1].sound.setLoop(false);

	gameSounds[1].play();
	if (sfxVolume >= 95)gameSounds[1].sound.setVolume(35);
	else gameSounds[1].sound.setVolume(sfxVolume / 3);

}

void TimeClock() {

	gameSounds[2].sound.setVolume(sfxVolume);
	gameSounds[2].play();

}

void Fire() {

	gameSounds[3].sound.setVolume(sfxVolume);
	gameSounds[3].play();

}

void Shotgun() {

	gameSounds[4].sound.setVolume(sfxVolume);
	gameSounds[4].play();

}

void StartOfGame() {

	gameSounds[5].play();
	if (sfxVolume >= 90)gameSounds[5].sound.setVolume(30);
	else gameSounds[5].sound.setVolume(sfxVolume / 3);
}

void Damage() {

	gameSounds[6].play();
	gameSounds[6].sound.setVolume(sfxVolume);
}

void ReloadShotgun() {

	gameSounds[7].play();
	gameSounds[7].sound.setVolume(sfxVolume);
}

void Zombie() {

	gameSounds[8].play();
	gameSounds[8].sound.setVolume(sfxVolume);
}

void RIFLES() {

	gameSounds[9].play();
	gameSounds[9].sound.setVolume(sfxVolume);
}

void PlasmaRifle() {

	gameSounds[10].play();
	gameSounds[10].sound.setVolume(sfxVolume);
}



void PlasmaShotgun() {

	gameSounds[11].play();
	gameSounds[11].sound.setVolume(sfxVolume);
}

void AutoRifle() {

	gameSounds[12].play();
	gameSounds[12].sound.setVolume(sfxVolume);
}

void Knife() {

	gameSounds[13].play();
	gameSounds[13].sound.setVolume(sfxVolume);
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
		reloadClipSize = clipSize * 7;
	}

};



struct Bullet {
	int playerID = 0;
	Sprite shape;
	Vector2f velocity;
	float age = 0.0f;
	float lifetime = 2.0f;
	float damage;

	Bullet(int id, float localDamage) {
		playerID = id;
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

Bullet createBullet(int playerID, Vector2f playerPos, float playerRotationDeg, Weapon currentWeapon, RenderWindow& window, Vector2f mouseWorld) {
	Bullet b(playerID, currentWeapon.damage);

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
void gui_main_multiplayer() {
	// Your existing texture loading code...
	gui_multi[0].loadFromFile("imgs/gui/pistol.png");
	gui_multi[1].loadFromFile("imgs/gui/shotgun.png");
	gui_multi[2].loadFromFile("imgs/gui/rifle.png");
	gui_multi[3].loadFromFile("imgs/gui/hp.png");
	gui_multi[4].loadFromFile("imgs/gui/ammo.png");
	gui_multi[5].loadFromFile("imgs/gui/hud.png");
	gui_multi[6].loadFromFile("imgs/gui/knife.png");
	gui_multi[7].loadFromFile("imgs/gui/dead.png");

	// Original GUI elements initialization
	for (int i = 0; i < 2; i++) {
		pistol_gui_multi[i].setTexture(gui_multi[0]);
		shotgun_gui_multi[i].setTexture(gui_multi[1]);
		rifle_gui_multi[i].setTexture(gui_multi[2]);
		ammo_gui_multi[i].setTexture(gui_multi[4]);
		knife_gui_multi[i].setTexture(gui_multi[6]);
		dead_gui_multi[i].setTexture(gui_multi[7]);

	}

	// Initialize second player's GUI elements
	hp_gui_p2.setTexture(gui_multi[3]);
	hp_gui_multi.setTexture(gui_multi[3]);


	hp_gui_p2.setScale(2, 2);
	hud_gui_p2[0].setScale(2, 1);
	hud_gui_p2[1].setScale(4.4, 2.1);

	// Original HUD initialization
	for (int i = 0; i < 3; i++) {
		hud_gui_multi[i].setTexture(gui_multi[5]);

	}
	for (int i = 0; i < 3; i++) {
		hud_gui_p2[i].setTexture(gui_multi[5]);
	}
	// Set scales (match player 1's scale)
	hud_gui_multi[0].setScale(2, 1);
	hud_gui_multi[1].setScale(4.4, 2.1);
	hud_gui_multi[2].setScale(4.4, 2.1);
	hp_gui_multi.setScale(2, 2);
	hp_gui_p2.setScale(2, 2);
	hud_gui_p2[2].setScale(2, 1);


	for (int i = 0; i < 2; i++) {
		rifle_gui_multi[i].setScale(2, 2);
		shotgun_gui_multi[i].setScale(2, 2);
		pistol_gui_multi[i].setScale(2, 2);
		dead_gui_multi[i].setScale(2, 2);
		ammo_gui_multi[i].setScale(2.2, 2.2);
		knife_gui_multi[i].setScale(0.3, 0.3);
		knife_gui_multi[i].setOrigin(knife_gui_multi[i].getLocalBounds().width / 2, knife_gui_multi[i].getLocalBounds().height / 2);
		knife_gui_multi[i].setRotation(25);
		knife_gui_multi[i].setColor(Color(89, 76, 41)); // Red
	}

	time_left.restart();

	// Font initialization
	font.loadFromFile("ModernDOS8x14.ttf");
	score.setFont(font);
	score_p2.setFont(font);
	colon_text.setFont(font);
	zombies.setFont(font);
	health_percentage.setFont(font);
	health_percentage_p2.setFont(font);
	for (int i = 0; i < 2; i++) {
		ammo_in_clip_multi[i].setFont(font);
		weapon_name_multi[i].setFont(font);
	}
	timeleft.setFont(font);

	// Character sizes
	score.setCharacterSize(30);
	score_p2.setCharacterSize(30);
	colon_text.setCharacterSize(30);
	colon_text.setFillColor(Color::White);
	health_percentage.setCharacterSize(30);
	health_percentage_p2.setCharacterSize(30);
	zombies.setCharacterSize(17);
	timeleft.setCharacterSize(23);
	colon_text.setString(":");
	for (int i = 0; i < 2; i++) {
		weapon_name_multi[i].setCharacterSize(24);

	}
}


int playerIndex = 0;

struct PLAYER {

	int id;

	bool secondplayer = false;
	Vector2f lastAimDir = Vector2f(1.f, 0.f);
	Vector2f crosshairPos;


	float  FRAME_WIDTH = 280.0f;
	float  FRAME_HEIGHT = 220.0f;


	float health = 150.0f; // Starting With The full Health (100%)
	CircleShape shieldVisual;


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

	bool hasShield = false;

	bool slowSpeed = false;

	int score = 0;

	Clock shieldClock;
	Clock speedClock;
	bool doubleScore = false;
	Clock doubleScoreClock;

	PLAYER(float x, float y, WeaponID weaponIndex1, WeaponID weaponIndex2, WeaponID weaponIndex3, WeaponID weaponIndex4, int maxWeaponsLocal, RenderWindow& window, float localscaleX = 0.35, float localscaleY = 0.35, bool isRushPlayerLocal = false) : crosshair(crosshairTexture, window) {
		id = playerIndex++;
		gui_main();
		gui_main_multiplayer();
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
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2);
		shieldVisual.setRadius(std::max(FRAME_WIDTH, FRAME_HEIGHT) / 2);
		shieldVisual.setOrigin(shieldVisual.getRadius(), shieldVisual.getRadius());
		shieldVisual.setFillColor(Color(0, 0, 255, 65));
		shieldVisual.setOutlineThickness(2);
		shieldVisual.setOutlineColor(Color(0, 100, 255, 180));
		shieldVisual.setScale(scaleX / 1.25, scaleY / 1.25);
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


	void toggleShield() {
		hasShield = true;
		shieldClock.restart();
	}




	void playerMove() {
		if (currentState == 2 || currentState == 3) {

			speed = 2.0f;
			diagonalSpeed = abs(speed) / sqrt(2);
			//return; // Prevent movement if in reload or melee state
		}

		Vector2f moveDir(0.f, 0.f);
		float rotation = shape.getRotation();
		if (secondplayer)
		{
			float x = Joystick::getAxisPosition(0, Joystick::X);
			float y = Joystick::getAxisPosition(0, Joystick::Y);
			const float deadzone = 15.0f;
			if (abs(x) < deadzone) x = 0;
			if (abs(y) < deadzone) y = 0;

			if (x != 0 || y != 0) {
				// Use joystick
				moveDir = Vector2f(x, y);
				float length = sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
				moveDir /= length;
				rotation = atan2(moveDir.y, moveDir.x) * 180 / PI;
				isMoving = true;
			}
		}
		else
		{
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
		if (isDead) return;

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
		if (isDead) return;

		if (secondplayer)
		{
			float z = Joystick::getAxisPosition(0, Joystick::Z);
			if (z < -20) {
				if (!(currentState == 2 || currentState == 3) && (currentWeapon[currentWeaponindex].currentSprite != 0) && currentWeapon[currentWeaponindex].reloadClipSize != 0) {
					ReloadShotgun();
					changeState(2);
				}
			}
			if (Joystick::isButtonPressed(0, 4)) {
				if (!(currentState == 2 || currentState == 3)) {
					changeState(3);
				}
				if (currentWeapon[currentWeaponindex].id == KNIFE) {

					Knife();

				}
			}

			if (z > 20) {
				switchWeapons();
			}
		}
		else {
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
	}


	bool isBursting = false;
	int burstShotsFired = 0;
	float burstDelay = 0.1f;
	float nextBurstTime = 0.0f;


	void shootBullets(RenderWindow& window) {
		if (currentState == 2 || currentState == 3) {
			return;
		}


		now = fireClock.getElapsedTime().asSeconds();


		if (secondplayer) {
			if ((Joystick::isButtonPressed(0, 5) || Joystick::isButtonPressed(0, 1)) && ((currentWeapon[currentWeaponindex].clipSize == currentWeapon[currentWeaponindex].currentClipSize) || ((now - lastFireTime) >= currentWeapon[currentWeaponindex].bulletDelay))) {

				if (gameTimer.getTime() <= 1.0f) {
					return;
				}

				Vector2f playerPos = shape.getPosition();
				float angle = atan2f(lastAimDir.y, lastAimDir.x) * 180.f / PI;

				if (currentWeapon[currentWeaponindex].currentClipSize > 0) {
					if (currentWeapon[currentWeaponindex].id == SHOTGUN || currentWeapon[currentWeaponindex].id == PLASMA_SHOTGUN) { // Shotgun
						for (int i = -2; i <= 2; ++i) {
							float spread = i * 9.5f;
							Bullet b = createBullet(id, playerPos, angle, currentWeapon[currentWeaponindex], window, crosshairPos);
							b.shape.rotate(spread);
							b.velocity = rotateVector(b.velocity, spread);
							bullets.push_back(b);
						}
						Shotgun();
						currentWeapon[currentWeaponindex].currentClipSize--;

					}
					else if (currentWeapon[currentWeaponindex].id == BURST_RIFLE) {

						if (!isBursting && currentWeapon[currentWeaponindex].currentClipSize >= 3) {
							isBursting = true;
							burstShotsFired = 0;
							nextBurstTime = now;
							PlasmaShotgun();

							currentWeapon[currentWeaponindex].currentClipSize -= 3;
						}

					}
					else if (currentWeapon[currentWeaponindex].id == SINGLE_RIFLE) {

						if (currentWeapon[currentWeaponindex].currentClipSize > 0) {
							PlasmaShotgun(); // or SingleShot(); if you have a separate function
							bullets.push_back(createBullet(id, playerPos, angle, currentWeapon[currentWeaponindex], window, crosshairPos));
							currentWeapon[currentWeaponindex].currentClipSize--;

						}

					}

					else {
						bullets.push_back(createBullet(id, playerPos, angle, currentWeapon[currentWeaponindex], window, crosshairPos));
						currentWeapon[currentWeaponindex].currentClipSize--;
					}
				}

				if (currentWeapon[currentWeaponindex].id == RIFLE) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						AutoRifle();

				}

				if (currentWeapon[currentWeaponindex].id == PISTOL) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						Fire();

				}



				if (currentWeapon[currentWeaponindex].id == PLASMA_PISTOL) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						PlasmaShotgun();

				}

				if (currentWeapon[currentWeaponindex].id == PLASMA_RIFLE) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						PlasmaRifle();

				}



				lastFireTime = now;

			}
		}
		else
		{
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
							Bullet b = createBullet(id, playerPos, shape.getRotation(), currentWeapon[currentWeaponindex], window, mousePos);
							b.shape.rotate(spread);
							b.velocity = rotateVector(b.velocity, spread);
							bullets.push_back(b);
						}
						Shotgun();
						currentWeapon[currentWeaponindex].currentClipSize--;

					}
					else if (currentWeapon[currentWeaponindex].id == BURST_RIFLE) {

						if (!isBursting && currentWeapon[currentWeaponindex].currentClipSize >= 3) {
							isBursting = true;
							burstShotsFired = 0;
							nextBurstTime = now;
							PlasmaShotgun();

							currentWeapon[currentWeaponindex].currentClipSize -= 3;
						}

					}
					else if (currentWeapon[currentWeaponindex].id == SINGLE_RIFLE) {

						if (currentWeapon[currentWeaponindex].currentClipSize > 0) {
							PlasmaShotgun(); // or SingleShot(); if you have a separate function
							bullets.push_back(createBullet(id, playerPos, shape.getRotation(), currentWeapon[currentWeaponindex], window, mousePos));
							currentWeapon[currentWeaponindex].currentClipSize--;

						}

					}

					else {
						bullets.push_back(createBullet(id, playerPos, shape.getRotation(), currentWeapon[currentWeaponindex], window, mousePos));
						currentWeapon[currentWeaponindex].currentClipSize--;
					}
				}

				if (currentWeapon[currentWeaponindex].id == RIFLE) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						AutoRifle();

				}

				if (currentWeapon[currentWeaponindex].id == PISTOL) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						Fire();

				}



				if (currentWeapon[currentWeaponindex].id == PLASMA_PISTOL) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						PlasmaShotgun();

				}

				if (currentWeapon[currentWeaponindex].id == PLASMA_RIFLE) {
					if (currentWeapon[currentWeaponindex].currentClipSize > 0)
						PlasmaRifle();

				}



				lastFireTime = now;

			}
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
		if (isDead) return;

		if (secondplayer) {
			crosshair.shape.setColor(Color(0, 0, 255, 255));
		}
		shootBullets(window);
		playerMove();
		changingFrames();
		if (hasShield && shieldClock.getElapsedTime().asSeconds() >= 10.f) {
			hasShield = false;
		}


		if (doubleScore && doubleScoreClock.getElapsedTime().asSeconds() >= 10.f) {
			doubleScore = false;
		}

		endState();

		if (secondplayer)
		{

			float stickX = Joystick::getAxisPosition(0, Joystick::U);
			float stickY = Joystick::getAxisPosition(0, Joystick::V);
			const float deadzone = 15.0f;

			if (abs(stickX) > deadzone || abs(stickY) > deadzone) {
				float maxDistance = 250.0f;
				float normX = stickX / 100.0f;
				float normY = stickY / 100.0f;
				float length = sqrt(normX * normX + normY * normY);
				if (length > 1.0f) {
					normX /= length;
					normY /= length;
				}
				lastAimDir = Vector2f(normX, normY);
				Vector2f offset(lastAimDir.x * maxDistance, lastAimDir.y * maxDistance);
				crosshairPos = shape.getPosition() + offset;
			}
			crosshair.mousePosSetter(crosshairPos);
			playerMouse(crosshairPos);

			if (isBursting) {
				float now = fireClock.getElapsedTime().asSeconds();

				if (now >= nextBurstTime && burstShotsFired < 3) {
					Bullet b = createBullet(id, shape.getPosition(), shape.getRotation(), currentWeapon[currentWeaponindex], window, crosshairPos);
					bullets.push_back(b);
					burstShotsFired++;
					nextBurstTime = now + burstDelay;
				}
				if (burstShotsFired >= 3) {
					isBursting = false;
				}
			}
		}
		else
		{
			Vector2f MouseGlobalPos = window.mapPixelToCoords(Mouse::getPosition(window));
			crosshair.mousePosSetter(MouseGlobalPos);
			playerMouse(MouseGlobalPos);


			if (isBursting) {
				float now = fireClock.getElapsedTime().asSeconds();

				if (now >= nextBurstTime && burstShotsFired < 3) {
					Bullet b = createBullet(id, shape.getPosition(), shape.getRotation(), currentWeapon[currentWeaponindex], window, MouseGlobalPos);
					bullets.push_back(b);
					burstShotsFired++;
					nextBurstTime = now + burstDelay;
				}
				if (burstShotsFired >= 3) {
					isBursting = false;
				}
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

	timeleft.setString("Time Left:" + to_string(120 - (int)time_left.getElapsedTime().asSeconds()));

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


void gui_game_loop_multiplayer(vector<PLAYER>& playersArr, RenderWindow& window, bool mission_is_on, bool isRush, int mission1_zombies_counter) {
	score.setString("  Score  ");

	score_p2.setString(to_string(score_) + " : " + to_string(score_2));
	if (!playersArr.empty())
	{
		if (isRush && mission_is_on)
		{
			ammo_in_clip_multi[0].setString(to_string(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].currentClipSize) + " / ∞");
			if (playersArr.size() > 1) {
				ammo_in_clip_multi[1].setString(to_string(playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].currentClipSize) + " / ∞");
			}
		}
		else {
			ammo_in_clip_multi[0].setString(to_string(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].currentClipSize) + " / " + to_string(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].reloadClipSize));
			if (playersArr.size() > 1) {
				ammo_in_clip_multi[1].setString(to_string(playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].currentClipSize) + " / " + to_string(playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].reloadClipSize));
			}
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

	timeleft.setString("Time Left:" + to_string(120 - (int)time_left.getElapsedTime().asSeconds()));

	if (!playersArr.empty())
	{
		weapon_name_multi[0].setString(playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].name);
		if (playersArr.size() > 1) {
			weapon_name_multi[1].setString(playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].name);
		}
	}
	View currentView = window.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();
	hud_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 800 - 20, viewCenter.y - viewSize.y / 2 + 980 - 985); //score for player 2

	// Player 1 GUI (left side)
	hud_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 10, viewCenter.y - viewSize.y / 2 + 10);
	hp_gui_multi.setPosition(viewCenter.x - viewSize.x / 2 + 30, viewCenter.y - viewSize.y / 2 + 20);

	health_percentage.setString(playersArr[0].isDead ? "0 %" :
		to_string((int)(playersArr[0].health * 100) / 150) + " %");
	health_percentage.setPosition(viewCenter.x - viewSize.x / 2 + 70, viewCenter.y - viewSize.y / 2 + 13);

	// Player 2 GUI (right side)
	if (playersArr.size() > 1) {
		hud_gui_p2[0].setPosition(viewCenter.x + viewSize.x / 2 - 180, viewCenter.y - viewSize.y / 2 + 10);
		hp_gui_p2.setPosition(viewCenter.x + viewSize.x / 2 - 155, viewCenter.y - viewSize.y / 2 + 20);
		health_percentage_p2.setString((playersArr[1].isDead ? "0 %" :
			to_string((int)(playersArr[1].health * 100) / 150) + " %"));
		health_percentage_p2.setPosition(viewCenter.x + viewSize.x / 2 - 115, viewCenter.y - viewSize.y / 2 + 13);
	}
	else {
		health_percentage_p2.setString(""); // Clear if no player 2
	}

	// Original score positioning
	score.setPosition(viewCenter.x - viewSize.x / 2 + 895, viewCenter.y - viewSize.y / 2 + 5); //score for player 1
	score_p2.setPosition(viewCenter.x - viewSize.x / 2 + 925, viewCenter.y - viewSize.y / 2 + 50);//score for player 2
	colon_text.setPosition(viewCenter.x - viewSize.x / 2 + 900, viewCenter.y - viewSize.y / 2 + 27);
	hud_gui_multi[2].setPosition(viewCenter.x - viewSize.x / 2 + 200, viewCenter.y - viewSize.y / 2 + 980 - 980);  //weapons hud
	hud_gui_p2[1].setPosition(viewCenter.x - viewSize.x / 2 + 1350, viewCenter.y - viewSize.y / 2 + 980 - 980);

	rifle_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 220, viewCenter.y - viewSize.y / 2 + 0);  //Rifle for player 1
	rifle_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1540, viewCenter.y - viewSize.y / 2 + 0);  //Rifle player 2

	shotgun_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 220, viewCenter.y - viewSize.y / 2 + 0);  //Shotgun
	shotgun_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1550, viewCenter.y - viewSize.y / 2 + 0);  //Shotgun player 2

	pistol_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 220, viewCenter.y - viewSize.y / 2 + 0);  //Pistol
	pistol_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1550, viewCenter.y - viewSize.y / 2 + 0);  //Pistol player 2

	dead_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 220, viewCenter.y - viewSize.y / 2 + 0);  //Pistol
	dead_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1550, viewCenter.y - viewSize.y / 2 + 0);  //Pistol player 2

	ammo_in_clip_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 420, viewCenter.y - viewSize.y / 2 + 57);  //ammo left
	ammo_in_clip_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1420, viewCenter.y - viewSize.y / 2 + 57);  //ammo left player 2

	ammo_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 380, viewCenter.y - viewSize.y / 2 + 70);  //ammo 
	ammo_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1380, viewCenter.y - viewSize.y / 2 + 70);  //ammo player2

	knife_gui_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 335, viewCenter.y - viewSize.y / 2 + 50);  //knife 
	knife_gui_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 335, viewCenter.y - viewSize.y / 2 + 50);  //knife player 2

	weapon_name_multi[0].setPosition(viewCenter.x - viewSize.x / 2 + 390, viewCenter.y - viewSize.y / 2 + 10);  //weapon name
	weapon_name_multi[1].setPosition(viewCenter.x - viewSize.x / 2 + 1380, viewCenter.y - viewSize.y / 2 + 10);  //weapon name player 2
	// Zombies and time displays
	zombies.setString("Zombies left: " + to_string(mission1_zombies_counter));
	zombies.setPosition(viewCenter.x - viewSize.x / 2 + 1708, viewCenter.y - viewSize.y / 2 + 23);

	timeleft.setString("Time Left:" + to_string(120 - (int)time_left.getElapsedTime().asSeconds()));
	timeleft.setPosition(viewCenter.x - viewSize.x / 2 + 1705, viewCenter.y - viewSize.y / 2 + 21);

	// Update rush mode flag
	rush = isRush;  // Make sure you have this global variable declared
}
void gui_draw_multiplayer(bool mission_is_on, RenderWindow& window, const vector<PLAYER>& playersArr) {


	// Draw player 1 GUI

	window.draw(hud_gui_multi[1]);
	if (!playersArr.empty()) {
		if (!playersArr[0].isDead) {

			window.draw(hud_gui_multi[0]);
			window.draw(hud_gui_multi[2]);
			window.draw(hp_gui_multi);
			window.draw(health_percentage);
		}
	}

	if (isGameEntered && playersArr.size() > 1) {
		if (!playersArr[1].isDead) {
			for (int i = 0; i < 2; i++)
			{
				window.draw(hud_gui_p2[i]);
				window.draw(hp_gui_p2);
				window.draw(health_percentage_p2);

			}
		}
	}


	// Draw mission-specific elements
	if (rush) {
		window.draw(score);
		//	window.draw(colon_text);
		window.draw(score_p2);
	}
	else if (mission_is_on) {
		window.draw(zombies);
	}
	else {
		window.draw(timeleft);
	}
	if (!playersArr.empty())
	{
		if (playersArr[0].isDead) {

		}

		else if ((playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 2 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 3 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 4 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 7))   //////RIFLE
		{

			window.draw(rifle_gui_multi[0]);
			window.draw(ammo_in_clip_multi[0]);
			window.draw(ammo_gui_multi[0]);
			window.draw(weapon_name_multi[0]);

		}
		else if ((playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 5 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 8))
			//////SHOTGUN
		{

			window.draw(shotgun_gui_multi[0]);
			window.draw(ammo_in_clip_multi[0]);
			window.draw(ammo_gui_multi[0]);
			window.draw(weapon_name_multi[0]);

		}

		else if ((playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 1 ||
			playersArr[0].currentWeapon[playersArr[0].currentWeaponindex].id == 6))
			////// Pistol
		{

			window.draw(pistol_gui_multi[0]);
			window.draw(ammo_in_clip_multi[0]);
			window.draw(ammo_gui_multi[0]);
			window.draw(weapon_name_multi[0]);

		}
		else  //////KNIFE
		{

			window.draw(knife_gui_multi[0]);

		}
		if (playersArr.size() > 1) {
			if (playersArr[1].isDead) {
			}
			else if (playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 2 ||
				playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 3 ||
				playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 4 ||
				playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 7)  //////RIFLE
			{

				window.draw(rifle_gui_multi[1]);
				window.draw(ammo_in_clip_multi[1]);
				window.draw(ammo_gui_multi[1]);
				window.draw(weapon_name_multi[1]);

			}

			else if (playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 5 ||
				playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 8)  //////SHOTGUN
			{

				window.draw(shotgun_gui_multi[1]);
				window.draw(ammo_in_clip_multi[1]);
				window.draw(ammo_gui_multi[1]);
				window.draw(weapon_name_multi[1]);

			}


			else if (playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 1 ||
				playersArr[1].currentWeapon[playersArr[1].currentWeaponindex].id == 6)   ////// Pistol
			{

				window.draw(pistol_gui_multi[1]);
				window.draw(ammo_in_clip_multi[1]);
				window.draw(ammo_gui_multi[1]);
				window.draw(weapon_name_multi[1]);

			}
			else  //////KNIFE
			{

				window.draw(knife_gui_multi[1]);

			}
		}
	}

}



bool slowEffectActive = false;
Clock slowEffectClock;


struct ZOMBIE {

	float FRAME_WIDTH = 318.0f;
	float FRAME_HEIGHT = 311.0f;

	float health = 100;
	float maxHealth;
	float speed = 3.0f;
	float diagonalSpeed = abs(speed) / sqrt(2);
	Vector2f position;
	float normalSpeed = 100.f;
	float damage = 20.0f;
	int ScoreShouldBe;
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
	int lastBulletID = -1;
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
			ScoreShouldBe = 10;

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
			ScoreShouldBe = 20;

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




	void zombieAttack() {
		changeState(2);
	}

	void zombieDie() {
		changeState(3);
	}

	void zombieMoveTowards(Vector2f targetPosition, float distance, const vector<PLAYER>& players) {
		if ((distance < zombieAttackDistance) || (currentState == 3)) {
			// If no valid target, go idle
			bool allDead = true;
			for (const auto& p : players)
				if (!p.isDead) allDead = false;

			if (allDead) {
				changeState(1); // idle
				isAttacking = false;
			}
			return;
		}

		if (attackIndex == attackIndexMax && currentState == 2) {
			isFar = true;
			return;
		}

		changeState(0); // walking
		isAttacking = false;

		Vector2f direction = targetPosition - shape.getPosition();
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);

		if (length != 0) {
			direction /= length;
		}

		float moveSpeed = slowEffectActive ? speed / 2.0f : speed;
		shape.move(direction.x * moveSpeed, direction.y * moveSpeed);
		shape.setRotation(atan2(direction.y, direction.x) * 180 / PI);
	}


	Vector2f findNearestPlayerPosition(const Vector2f& zombiePosition, const vector<PLAYER>& players) {
		if (players.empty()) {
			changeState(1); // idle
			return zombiePosition;
		}

		bool allDead = true;
		for (const auto& p : players)
			if (!p.isDead) allDead = false;

		if (allDead) {
			changeState(1); // idle
			isAttacking = false;
			return zombiePosition;
		}

		Vector2f nearestPlayerPos = zombiePosition;
		float minDistance = FLT_MAX;

		for (const auto& player : players) {
			if (player.isDead) continue;

			Vector2f playerPos = player.shape.getPosition();
			float distance = sqrt(pow(playerPos.x - zombiePosition.x, 2) + pow(playerPos.y - zombiePosition.y, 2));

			if (distance < minDistance) {
				minDistance = distance;
				nearestPlayerPos = playerPos;
			}
		}

		return nearestPlayerPos;
	}

	void attackingPlayer(vector<PLAYER>& players) {
		if (attackIndex == attackIndexMax) {
			bool playerHit = false;

			for (auto& player : players) {
				if (player.isDead) continue;

				float distance = sqrt(
					pow(shape.getPosition().x - player.shape.getPosition().x, 2) +
					pow(shape.getPosition().y - player.shape.getPosition().y, 2));

				if (distance < zombieAttackDistance + 20.0f) {
					playerHit = true;

					if (!hasDealtDamage) {
						if (!player.hasShield) {
							player.health -= damage;
							Damage();
						}
						hasDealtDamage = true;
					}
					break;
				}
			}

			if (!playerHit) {
				// Nobody nearby to hit → go idle
				changeState(1);
				isAttacking = false;
			}
		}

		if (attackIndex == 0) {
			hasDealtDamage = false;
		}
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
					zombieMoveTowards(nearestPlayerPos, distance, players);
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
				zombieMoveTowards(nearestPlayerPos, distance, players);
			}
		}


		avoidOtherZombies(zombies);
		changingFrames();
	}



};



struct DeathCircle {
	CircleShape circle;
	int perkID;
	Clock timePerk;
	DeathCircle(float x, float y, int id) {
		perkID = id;
		circle.setTexture(&perksTexture[id]);
		circle.setRadius(25.0f);
		circle.setPosition(x, y - 30.0f);
		//circle.setFillColor(sf::Color(0,0,0));
	}
};

vector<DeathCircle> deathArr;


void deathCircleEnter(vector<PLAYER>& playersArr) {
	for (int i = 0; i < playersArr.size(); i++) {
		for (int j = 0; j < deathArr.size(); j++) {
			if (playersArr[i].shape.getGlobalBounds().intersects(deathArr[j].circle.getGlobalBounds())) {
				if (deathArr[j].perkID < 8) {
					Weapon newWeapon;
					newWeapon.weaponAdd(static_cast<WeaponID>(deathArr[j].perkID + PISTOL));
					playersArr[i].currentWeapon[0] = newWeapon;
					playersArr[i].shape.setTexture(SpriteTextures[newWeapon.currentSprite]);

				}
				else if (deathArr[j].perkID == 8) {
					playersArr[i].health += 50.0f;
					if (playersArr[i].health > 150.0f) {
						playersArr[i].health = 150.0f;
					}
				}
				else if (deathArr[j].perkID == 9) {
					playersArr[i].toggleShield();
				}
				else if (deathArr[j].perkID == 10) {
					playersArr[i].doubleScore = true;
					playersArr[i].doubleScoreClock.restart();
				}
				else if (deathArr[j].perkID == 11) {
					slowEffectActive = true;
					slowEffectClock.restart();
				}
				deathArr.erase(deathArr.begin() + j);
				--j;
			}
		}
	}
}

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
				zombiesArr[j].lastBulletID = bullets[i].playerID;
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

void updateEntities(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, vector<Bullet>& bullets, Clock zombieDeathTimer, RenderWindow& window, int mission1_zombies_counter, bool mission_is_on = false, bool isRush = false, bool isMulti = false) {

	if (slowEffectActive && slowEffectClock.getElapsedTime().asSeconds() >= 10.f) {
		slowEffectActive = false;
	}


	for (int i = 0; i < playersArr.size(); i++) playersArr[i].update(window);
	for (int i = 0; i < zombiesArr.size(); i++) zombiesArr[i].update(playersArr, zombiesArr);
	playersAvoidOtherZombies(playersArr, zombiesArr);
	bulletIntersection(bullets, playersArr, zombiesArr);
	meleeAttack(playersArr, zombiesArr);
	if (!isMulti) {
		gui_game_loop(playersArr, window, mission_is_on, isRush, mission1_zombies_counter);
	}
	else {
		gui_game_loop_multiplayer(playersArr, window, mission_is_on, isRush, mission1_zombies_counter);
	}
	//cout << "size is " << zombiesArr.size() << endl;

	for (int i = 0; i < zombiesArr.size(); i++) {
		if (zombiesArr[i].shoudBeErased) {
			zombiesArr.erase(zombiesArr.begin() + i);
			--i;
		}
	}
	for (int i = 0; i < playersArr.size(); i++) {
		if (playersArr[i].health <= 0) {
			if (!isMulti) {
				playersArr.erase(playersArr.begin() + i);
				--i;
			}
			else {
				playersArr[i].crosshair.shape.setScale(0, 0);
				playersArr[i].shape.setScale(0, 0);
				playersArr[i].isDead = true;


			}
		}
	}

	for (int i = 0; i < deathArr.size(); i++) {
		if (deathArr[i].timePerk.getElapsedTime().asSeconds() >= 10.25f) {
			deathArr.erase(deathArr.begin() + i);
			--i;
		}
	}
	deathCircleEnter(playersArr);
}

void drawEntities(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window, bool mission_is_on = false, bool isRush = false, bool isMulti = false) {


	for (int i = 0; i < zombiesArr.size(); ++i) {
		if (zombiesArr[i].isDead)
			window.draw(zombiesArr[i].shape);
	}
	for (int i = 0; i < deathArr.size(); i++) {
		window.draw(deathArr[i].circle);
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
		if (playersArr[i].hasShield) {
			playersArr[i].shieldVisual.setPosition(playersArr[i].shape.getPosition());
			window.draw(playersArr[i].shieldVisual);
		}

	}
	for (int i = 0; i < zombiesArr.size(); ++i) {
		zombiesArr[i].drawHealthBar(window);
	}
	if (!isMulti) {
		gui_draw(mission_is_on, isRush, window, playersArr);

	}
	else {
		gui_draw_multiplayer(mission_is_on, window, playersArr);
	}
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
	bool isTryingAgain = false;

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

		isTryingAgain = tryAgain;
		if (!sceneSkipped) runBeachScene(window);


		if (!font.loadFromFile("tag.ttf")) {
			cout << "Failed to load font.\n";
		}




		if (!music.openFromFile("introScene.wav")) {
			cout << "Failed to load music.\n";
		}
		music.setLoop(true);
		music.play();
		music.setVolume(musicVolume);

		skipText.setFont(font);
		skipText.setCharacterSize(24);
		skipText.setFillColor(Color(255, 255, 255, 160));


		for (int i = 0; i < MAX_LINES; ++i) {
			lines[i].setFont(font);
			lines[i].setCharacterSize(32);
			lines[i].setFillColor(Color::White);
			lines[i].setPosition(130, 250 + i * 100);
		}

		fade.setSize(Vector2f(1920, 1080));
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

		if (isTryingAgain) {

			sceneSkipped = true;

			fadingOut = true;

			music.stop();

			for (int i = 0; i < MAX_LINES; ++i) {

				lines[i].setString("");

			}

			isStoryOn = false;

		}

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
		//font.loadFromFile("img/Caliste.ttf");
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

		if (totalZombieKilled <= 0) {
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
			for (int i = 0; i < playersArr.size(); i++) {

				if (playersArr[0].shape.getPosition().y <= 30) // check for top border

				{

					playersArr[0].shape.move(0, speed);

				}

				if (playersArr[0].shape.getPosition().y >= 1050) //check for bottom border

				{

					playersArr[0].shape.move(0, -speed);

				}if (playersArr[0].shape.getPosition().x <= 30) //check for left border

				{

					playersArr[0].shape.move(speed, 0);

				}

				if (playersArr[0].shape.getPosition().x >= 1900) //check for right border

				{

					playersArr[0].shape.move(-speed, 0);

				}
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
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate

	bool isSaved = false;
	int initialNumZombies = 10;
	int maxZombies = 100;
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	int mission1_zombies_counter = 0;
	Texture beach, seatexture;
	Sprite sand, sea;
	int score = 0;

	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;


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

	BeachRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));

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
			addScoreIfHigh(score, playerName);
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
		if (!playersArr.empty()) {
			float elapsed = gameTimer.getTime();

			float zombie1SpawnTime;
			if (elapsed <= phase1Duration) {
				float t = elapsed / phase1Duration;
				zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
			}
			else {
				zombie1SpawnTime = minSpawnTime;
			}

			if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
				zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 1));
				zombie1Spawn.reset();
			}

			float zombie2SpawnTime;
			if (elapsed > phase1Duration) {
				float t = (elapsed - phase1Duration) / phase2Duration;
				zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

				if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
					zombiesArr.push_back(ZOMBIE(getRandomOutsideRange_x(), getRandomOutsideRange_y(), 2));
					zombie2Spawn.reset();
				}
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
			if (pos.y + playersArr[0].shape.getGlobalBounds().height >= mapHeight - 250)
				playersArr[0].shape.move(0, -playersArr[0].speed);

			// Prevent moving out of left border
			if (pos.x <= 0)
				playersArr[0].shape.move(playersArr[0].speed, 0);

			// Prevent moving out of right border
			if (pos.x + playersArr[0].shape.getGlobalBounds().width >= mapWidth + 100)
				playersArr[0].shape.move(-playersArr[0].speed, 0);

			for (int i = 0; i < zombiesArr.size() && !playersArr.empty(); i++) {
				if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
					if (playersArr[0].doubleScore) {
						score += (2 * zombiesArr[i].ScoreShouldBe);
					}
					else {
						score += zombiesArr[i].ScoreShouldBe;
					}
					zombiesArr[i].isDeadCounter = true;
					if ((rand() % 100) + 1 <= 20) {
						deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
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
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f;
	const float spawnRampDuration = 120.0f;
	//int initialNumZombies = 10;
	const float mapWidth = 2000;
	const float mapHeight = 1500;
	int mission1_zombies_counter = 0;
	bool isSaved = false;
	Sprite backgroundDesertRoadSprite;
	int score = 0;
	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;



	DesertroadRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));



		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		backgroundDesertRoadSprite.setScale(
			(float)mapWidth / backgroundDesertRoad.getSize().x,
			(float)mapHeight / backgroundDesertRoad.getSize().y
		);


		zombieSpawn.restart();


		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background
		backgroundDesertRoadSprite.setScale(
			mapWidth / backgroundDesertRoad.getSize().x,
			mapHeight / backgroundDesertRoad.getSize().y
		);
	};
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {


		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score, playerName);
			cout << score << endl;
			isSaved = true;
		}
		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);


		if (!playersArr.empty()) {
			float elapsed = gameTimer.getTime();

			float zombie1SpawnTime;
			if (elapsed <= phase1Duration) {
				float t = elapsed / phase1Duration;
				zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
			}
			else {
				zombie1SpawnTime = minSpawnTime;
			}

			if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
				float x, y;
				int side = rand() % 4;
				switch (side) {
				case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
				case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
				case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
				case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
				}
				zombiesArr.push_back(ZOMBIE(x, y, 1));
				zombie1Spawn.reset();
			}

			float zombie2SpawnTime;
			if (elapsed > phase1Duration) {
				float t = (elapsed - phase1Duration) / phase2Duration;
				zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

				if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 2));
					zombie2Spawn.reset();
				}
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
			for (int i = 0; i < zombiesArr.size() && !playersArr.empty(); i++) {
				if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
					if (playersArr[0].doubleScore) {
						score += (2 * zombiesArr[i].ScoreShouldBe);
					}
					else {
						score += zombiesArr[i].ScoreShouldBe;
					}
					zombiesArr[i].isDeadCounter = true;
					if ((rand() % 100) + 1 <= 20) {
						deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
					}

				}
			}
		}

	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundDesertRoadSprite);
		drawEntities(playersArr, zombiesArr, window, true, true);

	};
};
struct CityRush {
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
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate
	int score = 0;
	bool isSaved = false;
	int mapWidth = 1920; // width of background
	int mapHeight = 1080; // height of background

	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;

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

	CityRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));

		map.init(buildingTexture);
		traffic.init(cara, carb);
	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score, playerName);
			cout << score << endl;
			isSaved = true;
		}

		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			if (!playersArr.empty()) {
				float elapsed = gameTimer.getTime();

				float zombie1SpawnTime;
				if (elapsed <= phase1Duration) {
					float t = elapsed / phase1Duration;
					zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
				}
				else {
					zombie1SpawnTime = minSpawnTime;
				}

				if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 1));
					zombie1Spawn.reset();
				}

				float zombie2SpawnTime;
				if (elapsed > phase1Duration) {
					float t = (elapsed - phase1Duration) / phase2Duration;
					zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

					if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
						float x, y;
						int side = rand() % 4;
						switch (side) {
						case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
						case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
						case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
						case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
						}
						zombiesArr.push_back(ZOMBIE(x, y, 2));
						zombie2Spawn.reset();
					}
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
		for (int i = 0; i < zombiesArr.size() && !playersArr.empty(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				if (playersArr[0].doubleScore) {
					score += (2 * zombiesArr[i].ScoreShouldBe);
				}
				else {
					score += zombiesArr[i].ScoreShouldBe;
				}
				zombiesArr[i].isDeadCounter = true;
				if ((rand() % 100) + 1 <= 20) {
					deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
				}

			}
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
		drawEntities(playersArr, zombiesArr, window, true, true);

	}
};
struct WoodsRush {
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
	Sprite backgroundWoods;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	bool isSaved = false;
	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;
	int score = 0;
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
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		backgroundWoods.setTexture(woodsBackgroundTexture);
		backgroundWoods.setScale(
			(float)mapWidth / woodsBackgroundTexture.getSize().x,
			(float)mapHeight / woodsBackgroundTexture.getSize().y
		);

	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score, playerName);
			cout << score << endl;
			isSaved = true;
		}

		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			if (!playersArr.empty()) {
				float elapsed = gameTimer.getTime();

				float zombie1SpawnTime;
				if (elapsed <= phase1Duration) {
					float t = elapsed / phase1Duration;
					zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
				}
				else {
					zombie1SpawnTime = minSpawnTime;
				}

				if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 1));
					zombie1Spawn.reset();
				}

				float zombie2SpawnTime;
				if (elapsed > phase1Duration) {
					float t = (elapsed - phase1Duration) / phase2Duration;
					zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

					if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
						float x, y;
						int side = rand() % 4;
						switch (side) {
						case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
						case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
						case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
						case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
						}
						zombiesArr.push_back(ZOMBIE(x, y, 2));
						zombie2Spawn.reset();
					}
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
		for (int i = 0; i < zombiesArr.size() && !playersArr.empty(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				if (playersArr[0].doubleScore) {
					score += (2 * zombiesArr[i].ScoreShouldBe);
				}
				else {
					score += zombiesArr[i].ScoreShouldBe;
				}
				zombiesArr[i].isDeadCounter = true;
				if ((rand() % 100) + 1 <= 20) {
					deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
				}

			}
		}

	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundWoods);
		drawEntities(playersArr, zombiesArr, window, true, true);

	};
};
struct ArmyRush {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	bool missionComplete = false;
	int score = 0;
	bool isSaved = false;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate
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
	const int intial_num = 4;

	int armycampscore = 0;//score
	Sprite armycampdead;
	Font font;
	Text scoreText;
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

	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;
	ArmyRush(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 2, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));


		armycampdead.setTexture(armycampdeadTexture);
		armycampdead.setOrigin(armycampdead.getLocalBounds().width / 2, armycampdead.getLocalBounds().height / 2);
		armycampdead.setPosition(960, 540);
		armycampdead.setScale(2, 2);
		font.loadFromFile("img/Caliste.ttf");
		scoreText.setFont(font);
		scoreText.setCharacterSize(50);

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
		if (playersArr.empty() && !isSaved) {
			addScoreIfHigh(score, playerName);
			cout << score << endl;
			isSaved = true;
		}

		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, score, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			if (!playersArr.empty()) {
				float elapsed = gameTimer.getTime();

				float zombie1SpawnTime;
				if (elapsed <= phase1Duration) {
					float t = elapsed / phase1Duration;
					zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
				}
				else {
					zombie1SpawnTime = minSpawnTime;
				}

				if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 1));
					zombie1Spawn.reset();
				}

				float zombie2SpawnTime;
				if (elapsed > phase1Duration) {
					float t = (elapsed - phase1Duration) / phase2Duration;
					zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

					if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
						float x, y;
						int side = rand() % 4;
						switch (side) {
						case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
						case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
						case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
						case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
						}
						zombiesArr.push_back(ZOMBIE(x, y, 2));
						zombie2Spawn.reset();
					}
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
		for (int i = 0; i < zombiesArr.size() && !playersArr.empty(); i++) {
			if (zombiesArr[i].health <= 0 && !zombiesArr[i].isDeadCounter) {
				if (playersArr[0].doubleScore) {
					score += (2 * zombiesArr[i].ScoreShouldBe);
				}
				else {
					score += zombiesArr[i].ScoreShouldBe;
				}
				zombiesArr[i].isDeadCounter = true;
				if ((rand() % 100) + 1 <= 20) {
					deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
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
		drawEntities(playersArr, zombiesArr, window, true, true);

	}
};

struct BeachRushMulti {
	int frameCounter = 0, frameDelay = 20, seaindex = 0;
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate

	bool isSaved = false;
	int initialNumZombies = 10;
	int maxZombies = 100;
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	int mission1_zombies_counter = 0;
	Texture beach, seatexture;
	Sprite sand, sea;


	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;

	BeachRushMulti(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 4, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr.push_back(PLAYER(1920 / 6, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr[1].secondplayer = true;

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

		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, -1, true, true, true);
		updateBullets(dt);
		frameCounter++;

		if (frameCounter >= frameDelay) {

			seaindex = (seaindex + 1) % 7;

			frameCounter = 0;  // Reset counter after updating

		}

		sea.setTextureRect(IntRect(seaindex * 1920, 0, 1920, 750));
		if (!playersArr.empty()) {
			float elapsed = gameTimer.getTime();

			float zombie1SpawnTime;
			if (elapsed <= phase1Duration) {
				float t = elapsed / phase1Duration;
				zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
			}
			else {
				zombie1SpawnTime = minSpawnTime;
			}

			if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
				float x, y;
				int side = rand() % 4;
				switch (side) {
				case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
				case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
				case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
				case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
				}
				zombiesArr.push_back(ZOMBIE(x, y, 1));
				zombie1Spawn.reset();
			}

			float zombie2SpawnTime;
			if (elapsed > phase1Duration) {
				float t = (elapsed - phase1Duration) / phase2Duration;
				zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

				if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 2));
					zombie2Spawn.reset();
				}
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
					if (playersArr[zombiesArr[i].lastBulletID].doubleScore) {
						playersArr[zombiesArr[i].lastBulletID].score += (2 * zombiesArr[i].ScoreShouldBe);
					}
					else {
						playersArr[zombiesArr[i].lastBulletID].score += zombiesArr[i].ScoreShouldBe;
					}
					zombiesArr[i].isDeadCounter = true;
					if ((rand() % 100) + 1 <= 20) {
						deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
					}

				}
				score_ = playersArr[0].score;
				score_2 = playersArr[1].score;
			}

		}


	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(sand);
		window.draw(sea);
		drawEntities(playersArr, zombiesArr, window, true, true, true);

	};
};
struct DesertroadRushMulti {
	bool missionComplete = false;
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f;
	const float spawnRampDuration = 120.0f;
	//int initialNumZombies = 10;
	const float mapWidth = 2000;
	const float mapHeight = 1500;
	int mission1_zombies_counter = 0;
	bool isSaved = false;
	Sprite backgroundDesertRoadSprite;

	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;



	DesertroadRushMulti(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 4, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr.push_back(PLAYER(1920 / 6, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr[1].secondplayer = true;


		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		backgroundDesertRoadSprite.setScale(
			(float)mapWidth / backgroundDesertRoad.getSize().x,
			(float)mapHeight / backgroundDesertRoad.getSize().y
		);


		zombieSpawn.restart();


		backgroundDesertRoadSprite.setTexture(backgroundDesertRoad);
		float mapWidth = 2000; // width of background
		float mapHeight = 1500; // height of background
		backgroundDesertRoadSprite.setScale(
			mapWidth / backgroundDesertRoad.getSize().x,
			mapHeight / backgroundDesertRoad.getSize().y
		);
	};
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {



		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, -1, true, true, true);
		updateBullets(dt);


		if (!playersArr.empty()) {
			float elapsed = gameTimer.getTime();

			float zombie1SpawnTime;
			if (elapsed <= phase1Duration) {
				float t = elapsed / phase1Duration;
				zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
			}
			else {
				zombie1SpawnTime = minSpawnTime;
			}

			if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
				float x, y;
				int side = rand() % 4;
				switch (side) {
				case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
				case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
				case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
				case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
				}
				zombiesArr.push_back(ZOMBIE(x, y, 1));
				zombie1Spawn.reset();
			}

			float zombie2SpawnTime;
			if (elapsed > phase1Duration) {
				float t = (elapsed - phase1Duration) / phase2Duration;
				zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

				if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 2));
					zombie2Spawn.reset();
				}
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
					if (playersArr[zombiesArr[i].lastBulletID].doubleScore) {
						playersArr[zombiesArr[i].lastBulletID].score += (2 * zombiesArr[i].ScoreShouldBe);
					}
					else {
						playersArr[zombiesArr[i].lastBulletID].score += zombiesArr[i].ScoreShouldBe;
					}
					zombiesArr[i].isDeadCounter = true;
					if ((rand() % 100) + 1 <= 20) {
						deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
					}

				}
				score_ = playersArr[0].score;
				score_2 = playersArr[1].score;
			}
		}

	};
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundDesertRoadSprite);
		drawEntities(playersArr, zombiesArr, window, true, true, true);

	};
};
struct CityRushMulti {
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
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate

	bool isSaved = false;
	int mapWidth = 1920; // width of background
	int mapHeight = 1080; // height of background

	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;

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

	CityRushMulti(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 4, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr.push_back(PLAYER(1920 / 6, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr[1].secondplayer = true;
		map.init(buildingTexture);
		traffic.init(cara, carb);
	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {


		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, -1, true, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			if (!playersArr.empty()) {
				float elapsed = gameTimer.getTime();

				float zombie1SpawnTime;
				if (elapsed <= phase1Duration) {
					float t = elapsed / phase1Duration;
					zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
				}
				else {
					zombie1SpawnTime = minSpawnTime;
				}

				if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 1));
					zombie1Spawn.reset();
				}

				float zombie2SpawnTime;
				if (elapsed > phase1Duration) {
					float t = (elapsed - phase1Duration) / phase2Duration;
					zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

					if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
						float x, y;
						int side = rand() % 4;
						switch (side) {
						case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
						case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
						case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
						case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
						}
						zombiesArr.push_back(ZOMBIE(x, y, 2));
						zombie2Spawn.reset();
					}
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
				if (playersArr[zombiesArr[i].lastBulletID].doubleScore) {
					playersArr[zombiesArr[i].lastBulletID].score += (2 * zombiesArr[i].ScoreShouldBe);
				}
				else {
					playersArr[zombiesArr[i].lastBulletID].score += zombiesArr[i].ScoreShouldBe;
				}
				zombiesArr[i].isDeadCounter = true;
				if ((rand() % 100) + 1 <= 20) {
					deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
				}

			}
			score_ = playersArr[0].score;
			score_2 = playersArr[1].score;
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
		drawEntities(playersArr, zombiesArr, window, true, true, true);

	}
};
struct WoodsRushMulti {
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
	Sprite backgroundWoods;
	Clock zombieSpawn;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate
	const int mapWidth = 2000;
	const int mapHeight = 1500;
	bool isSaved = false;
	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;

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

	WoodsRushMulti(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 4, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr.push_back(PLAYER(1920 / 6, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr[1].secondplayer = true;		backgroundWoods.setTexture(woodsBackgroundTexture);
		backgroundWoods.setScale(
			(float)mapWidth / woodsBackgroundTexture.getSize().x,
			(float)mapHeight / woodsBackgroundTexture.getSize().y
		);

	}
	void update(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {


		dt = deltaClock.restart().asSeconds();
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, -1, true, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			if (!playersArr.empty()) {
				float elapsed = gameTimer.getTime();

				float zombie1SpawnTime;
				if (elapsed <= phase1Duration) {
					float t = elapsed / phase1Duration;
					zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
				}
				else {
					zombie1SpawnTime = minSpawnTime;
				}

				if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 1));
					zombie1Spawn.reset();
				}

				float zombie2SpawnTime;
				if (elapsed > phase1Duration) {
					float t = (elapsed - phase1Duration) / phase2Duration;
					zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

					if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
						float x, y;
						int side = rand() % 4;
						switch (side) {
						case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
						case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
						case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
						case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
						}
						zombiesArr.push_back(ZOMBIE(x, y, 2));
						zombie2Spawn.reset();
					}
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
				if (playersArr[zombiesArr[i].lastBulletID].doubleScore) {
					playersArr[zombiesArr[i].lastBulletID].score += (2 * zombiesArr[i].ScoreShouldBe);
				}
				else {
					playersArr[zombiesArr[i].lastBulletID].score += zombiesArr[i].ScoreShouldBe;
				}
				zombiesArr[i].isDeadCounter = true;
				if ((rand() % 100) + 1 <= 20) {
					deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
				}

			}
			score_ = playersArr[0].score;
			score_2 = playersArr[1].score;

		}

	}
	void draw(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		window.draw(backgroundWoods);
		drawEntities(playersArr, zombiesArr, window, true, true, true);

	};
};
struct ArmyRushMulti {
	Clock deltaClock;
	Clock zombieDeathTimer;
	float dt;
	int mission1_zombies_counter = 0;
	bool missionComplete = false;
	bool isSaved = false;
	float zombieSpawnTime = 3.0f; // Will decrease over time
	const float spawnRampDuration = 120.0f; // Time to reach fastest spawn rate
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
	const int intial_num = 4;

	int armycampscore = 0;//score
	Sprite armycampdead;
	Font font;
	Text scoreText;
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

	PauseableTimer zombie1Spawn;
	PauseableTimer zombie2Spawn;
	const float totalDuration = 180.0f;
	const float phase1Duration = 90.0f;
	const float phase2Duration = 90.0f;
	const float initialSpawnTime = 3.0f;
	const float minSpawnTime = 0.5f;
	ArmyRushMulti(vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr, RenderWindow& window) {
		playersArr.push_back(PLAYER(1920 / 4, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr.push_back(PLAYER(1920 / 6, 1080 / 2, PISTOL, PISTOL, RIFLE, SINGLE_RIFLE, 1, window, 0.35, 0.35, true));
		playersArr[1].secondplayer = true;

		armycampdead.setTexture(armycampdeadTexture);
		armycampdead.setOrigin(armycampdead.getLocalBounds().width / 2, armycampdead.getLocalBounds().height / 2);
		armycampdead.setPosition(960, 540);
		armycampdead.setScale(2, 2);
		font.loadFromFile("img/Caliste.ttf");
		scoreText.setFont(font);
		scoreText.setCharacterSize(50);

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
		View view(window.getDefaultView());

		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, window, -1, true, true, true);
		updateBullets(dt);

		if (!playersArr.empty()) {

			if (!playersArr.empty()) {
				float elapsed = gameTimer.getTime();

				float zombie1SpawnTime;
				if (elapsed <= phase1Duration) {
					float t = elapsed / phase1Duration;
					zombie1SpawnTime = initialSpawnTime - t * (initialSpawnTime - minSpawnTime);
				}
				else {
					zombie1SpawnTime = minSpawnTime;
				}

				if (zombie1Spawn.getTime() >= zombie1SpawnTime) {
					float x, y;
					int side = rand() % 4;
					switch (side) {
					case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
					case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
					case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
					case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
					}
					zombiesArr.push_back(ZOMBIE(x, y, 1));
					zombie1Spawn.reset();
				}

				float zombie2SpawnTime;
				if (elapsed > phase1Duration) {
					float t = (elapsed - phase1Duration) / phase2Duration;
					zombie2SpawnTime = initialSpawnTime - std::min(t, 1.0f) * (initialSpawnTime - minSpawnTime);

					if (zombie2Spawn.getTime() >= zombie2SpawnTime) {
						float x, y;
						int side = rand() % 4;
						switch (side) {
						case 0: x = rand() % (int)mapWidth; y = 1.0f; break;
						case 1: x = mapWidth - 1.0f; y = rand() % (int)mapHeight; break;
						case 2: x = rand() % (int)mapWidth; y = mapHeight - 1.0f; break;
						case 3: x = 1.0f; y = rand() % (int)mapHeight; break;
						}
						zombiesArr.push_back(ZOMBIE(x, y, 2));
						zombie2Spawn.reset();
					}
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
				if (playersArr[zombiesArr[i].lastBulletID].doubleScore) {
					playersArr[zombiesArr[i].lastBulletID].score += (2 * zombiesArr[i].ScoreShouldBe);
				}
				else {
					playersArr[zombiesArr[i].lastBulletID].score += zombiesArr[i].ScoreShouldBe;
				}
				zombiesArr[i].isDeadCounter = true;
				if ((rand() % 100) + 1 <= 20) {
					deathArr.push_back(DeathCircle(zombiesArr[i].shape.getPosition().x, zombiesArr[i].shape.getPosition().y, rand() % 12));
				}

			}
			score_ = playersArr[0].score;
			score_2 = playersArr[1].score;
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
		drawEntities(playersArr, zombiesArr, window, true, true, true);

	}
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
		case 15:
			currentLevel = new CityRush(playersArr, zombiesArr, window);
			break;
		case 16:
			currentLevel = new ArmyRush(playersArr, zombiesArr, window);
			break;
		case 17:
			currentLevel = new BeachRushMulti(playersArr, zombiesArr, window);
			break;
		case 18:
			currentLevel = new DesertroadRushMulti(playersArr, zombiesArr, window);
			break;
		case 19:
			currentLevel = new WoodsRushMulti(playersArr, zombiesArr, window);
			break;
		case 20:
			currentLevel = new CityRushMulti(playersArr, zombiesArr, window);
			break;
		case 21:
			currentLevel = new ArmyRushMulti(playersArr, zombiesArr, window);
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
		case 15:
			((CityRush*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 16:
			((ArmyRush*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 17:
			((BeachRushMulti*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 18:
			((DesertroadRushMulti*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 19:
			((WoodsRushMulti*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 20:
			((CityRushMulti*)currentLevel)->update(playersArr, zombiesArr, window);
			break;
		case 21:
			((ArmyRushMulti*)currentLevel)->update(playersArr, zombiesArr, window);
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
		case 15: return ((CityRush*)currentLevel)->missionComplete;
		case 16: return ((ArmyRush*)currentLevel)->missionComplete;
		case 17: return ((BeachRushMulti*)currentLevel)->missionComplete;
		case 18: return ((DesertroadRushMulti*)currentLevel)->missionComplete;
		case 19: return ((WoodsRushMulti*)currentLevel)->missionComplete;
		case 20: return ((CityRushMulti*)currentLevel)->missionComplete;
		case 21: return ((ArmyRushMulti*)currentLevel)->missionComplete;
		default: return false;
		}
	}

	void setMissionComplete(bool value) {
		switch (id) {
		case 0: ((Beachlevel*)currentLevel)->missionComplete = value; break;
		case 1: ((Desertroad*)currentLevel)->missionComplete = value; break;
		case 2: ((City*)currentLevel)->missionComplete = value; break;
		case 3: ((Safezone1*)currentLevel)->missionComplete = value; break;
		case 4: ((Safezone2*)currentLevel)->missionComplete = value; break;
		case 5: ((Woods*)currentLevel)->missionComplete = value; break;
		case 6: ((Safezone3*)currentLevel)->missionComplete = value; break;
		case 7: ((Army*)currentLevel)->missionComplete = value; break;
		case 8: ((Mission1*)currentLevel)->missionComplete = value; break;
		case 9: ((Mission2*)currentLevel)->missionComplete = value; break;
		case 10: ((Mission3*)currentLevel)->missionComplete = value; break;
		case 11: ((Mission4*)currentLevel)->missionComplete = value; break;
		case 12: ((BeachRush*)currentLevel)->missionComplete = value; break;
		case 13: ((DesertroadRush*)currentLevel)->missionComplete = value; break;
		case 14: ((WoodsRush*)currentLevel)->missionComplete = value; break;
		case 15: ((CityRush*)currentLevel)->missionComplete = value; break;
		case 16: ((ArmyRush*)currentLevel)->missionComplete = value; break;
		case 17: ((BeachRushMulti*)currentLevel)->missionComplete = value; break;
		case 18: ((DesertroadRushMulti*)currentLevel)->missionComplete = value; break;
		case 19: ((WoodsRushMulti*)currentLevel)->missionComplete = value; break;
		case 20: ((CityRushMulti*)currentLevel)->missionComplete = value; break;
		case 21: ((ArmyRushMulti*)currentLevel)->missionComplete = value; break;
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
		case 15:
			((CityRush*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 16:
			((ArmyRush*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 17:
			((BeachRushMulti*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 18:
			((DesertroadRushMulti*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 19:
			((WoodsRushMulti*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 20:
			((CityRushMulti*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		case 21:
			((ArmyRushMulti*)currentLevel)->draw(playersArr, zombiesArr, window);
			break;
		}
	}

	void deleteCurrentLevel() {
		if (currentLevel == nullptr)
			return;

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
		case 15: delete (CityRush*)currentLevel; break;
		case 16: delete (ArmyRush*)currentLevel; break;
		case 17: delete (BeachRushMulti*)currentLevel; break;
		case 18: delete (DesertroadRushMulti*)currentLevel; break;
		case 19: delete (WoodsRushMulti*)currentLevel; break;
		case 20: delete (CityRushMulti*)currentLevel; break;
		case 21: delete (ArmyRushMulti*)currentLevel; break;
		default: break;
		}

		currentLevel = nullptr;
		id = -1;
	}
	~levelHandler() {
		deleteCurrentLevel();
	}
};






void showDeath(vector<PLAYER>& playersArr, RenderWindow& window) {



	Sprite beachdead;

	beachdead.setTexture(beachdeadtexture);

	beachdead.setOrigin(beachdead.getLocalBounds().width / 2, beachdead.getLocalBounds().height / 2);



	beachdead.setPosition(window.mapPixelToCoords({ 1920 / 2,1080 / 2 }));



	beachdead.setScale(2, 2);

	if (playersArr.empty())



	{



		window.draw(beachdead);



	}

}



void showDeath(RenderWindow& window) {
	Sprite beachdead;
	beachdead.setTexture(beachdeadtexture);
	beachdead.setOrigin(beachdead.getLocalBounds().width / 2, beachdead.getLocalBounds().height / 2);
	beachdead.setPosition(window.mapPixelToCoords({ 1920 / 2, 1080 / 2 - 150 }));
	beachdead.setScale(2, 2);

	window.draw(beachdead);
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

//you dead menu

String youdead(RenderWindow& window, Event& event, Font& font, int currentLevelId)
{
	View currentView = window.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();

	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("imgs/pause/main3.jpg")) {
		return "error";
	}
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(4.25f, 4.25f);
	backgroundSprite.setPosition(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f);

	Text tryAgainText("Try Again", font, 48);
	tryAgainText.setFillColor(Color::Black);
	FloatRect tryBounds = tryAgainText.getLocalBounds();
	RectangleShape tryAgainBox(Vector2f(tryBounds.width + 60, tryBounds.height + 30));
	tryAgainBox.setFillColor(Color(139, 0, 0, 200));
	tryAgainBox.setOrigin(tryAgainBox.getSize() / 2.f);
	tryAgainText.setOrigin(tryBounds.left + tryBounds.width / 2.f, tryBounds.top + tryBounds.height / 2.f);

	Text backText("Back to Main Menu", font, 48);
	backText.setFillColor(Color::Black);
	FloatRect backBounds = backText.getLocalBounds();
	float backExtraPaddingX = 85;
	RectangleShape backBox(Vector2f(backBounds.width + 60 + backExtraPaddingX, backBounds.height + 35));
	backBox.setFillColor(Color(139, 0, 0, 200));
	backBox.setOrigin(backBox.getSize() / 2.f);
	backText.setOrigin(backBounds.left + backBounds.width / 2.f, backBounds.top + backBounds.height / 2.f);

	float centerX = window.getSize().x / 2.f;
	float centerY = window.getSize().y / 2.f;
	float spacingBetweenButtons = 90.f;

	float totalWidth = tryAgainBox.getSize().x + backBox.getSize().x + spacingBetweenButtons;

	float buttonsYPosition = viewCenter.y - viewSize.y / 2 + (1080 / 2 - 150) + 250;

	tryAgainBox.setPosition(viewCenter.x - viewSize.x / 2 + centerX - (totalWidth / 2.f) + (tryAgainBox.getSize().x / 2.f), buttonsYPosition);
	tryAgainText.setPosition(tryAgainBox.getPosition());

	backBox.setPosition(tryAgainBox.getPosition().x + (tryAgainBox.getSize().x / 2.f) + spacingBetweenButtons + (backBox.getSize().x / 2.f), buttonsYPosition);
	backText.setPosition(backBox.getPosition());

	FloatRect tryAgainBounds = tryAgainBox.getGlobalBounds();
	FloatRect backBoundsGlobal = backBox.getGlobalBounds();

	int selectedIndex = -1;
	bool usingMouse = false;

	window.setMouseCursorVisible(true);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				return "quit";
			}

			if (event.type == Event::MouseMoved) {
				if (!usingMouse) {
					window.setMouseCursorVisible(true);
					usingMouse = true;
				}
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (tryAgainBounds.contains(mousePos)) selectedIndex = 0;
				else if (backBoundsGlobal.contains(mousePos)) selectedIndex = 1;
				else selectedIndex = -1;
			}
			else if (event.type == Event::KeyPressed) {
				if (usingMouse) {
					window.setMouseCursorVisible(false);
					usingMouse = false;
				}
				if (event.key.code == Keyboard::Left) selectedIndex = (selectedIndex - 1 + 2) % 2;
				else if (event.key.code == Keyboard::Right) selectedIndex = (selectedIndex + 1) % 2;
				else if (event.key.code == Keyboard::Enter) {
					if (selectedIndex == 0) return "try_again";
					if (selectedIndex == 1) return "back_to_main_menu";
				}
			}
			else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				if (!usingMouse) {
					window.setMouseCursorVisible(true);
					usingMouse = true;
				}
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (tryAgainBounds.contains(mousePos)) return "try_again";
				if (backBoundsGlobal.contains(mousePos)) return "back_to_main_menu";
			}
		}

		tryAgainText.setFillColor(Color::Black);
		backText.setFillColor(Color::Black);

		if (selectedIndex == 0)
			tryAgainText.setFillColor(Color::White);
		else if (selectedIndex == 1)
			backText.setFillColor(Color::White);

		window.clear(Color::Black);
		window.draw(backgroundSprite);
		showDeath(window);
		window.draw(tryAgainBox);
		window.draw(tryAgainText);
		window.draw(backBox);
		window.draw(backText);
		window.display();
	}

	window.setMouseCursorVisible(false);
	return "";
}




String youwin(RenderWindow& window, Event& event, Font& font, int currentLevelId)
{
	View currentView = window.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();

	Clock clock;

	// Load Textures
	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("imgs/pause/main3.jpg")) return "error";
	Texture wordTexture;
	if (!wordTexture.loadFromFile("imgs/pause/word.png")) return "error";
	Texture starTexture;
	if (!starTexture.loadFromFile("imgs/pause/star.png")) return "error";

	// Background
	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(4.25f, 4.25f);
	backgroundSprite.setPosition(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f);

	// Win Word
	Sprite wordSprite(wordTexture);
	wordSprite.setScale(2.0f, 2.0f);
	wordSprite.setOrigin(wordSprite.getLocalBounds().width / 2.f, wordSprite.getLocalBounds().height / 2.f);
	wordSprite.setPosition(viewCenter.x, viewCenter.y - 150.f);

	// Middle stars animation
	const int starCount = 5;
	Sprite stars[starCount], glows[starCount];
	Vector2f basePositions[starCount];
	float spacing = 135.f;
	float startX = viewCenter.x - ((starCount - 1) * spacing) / 2.f;

	for (int i = 0; i < starCount; ++i) {
		stars[i].setTexture(starTexture);
		stars[i].setScale(1.5f, 1.5f);
		stars[i].setOrigin(stars[i].getLocalBounds().width / 2.f, stars[i].getLocalBounds().height / 2.f);
		stars[i].setPosition(startX + i * spacing, viewCenter.y - 20.f);
		basePositions[i] = stars[i].getPosition();

		glows[i] = stars[i];
		glows[i].setScale(1.8f, 1.8f);
		glows[i].setColor(Color(255, 255, 0, 100));
	}

	// Side stars (disappear/appear)
	const int sideStarCount = 6;
	Sprite sideStarsLeft[sideStarCount], sideGlowsLeft[sideStarCount];
	Sprite sideStarsRight[sideStarCount], sideGlowsRight[sideStarCount];
	Vector2f sideBasePositionsLeft[sideStarCount], sideBasePositionsRight[sideStarCount];

	float verticalSpacing = 100.f;
	float startY = viewCenter.y - ((sideStarCount - 1) * verticalSpacing) / 2.f;
	float sideXRight = viewCenter.x + viewSize.x / 2.f - 70.f;
	float sideXLeft = viewCenter.x - viewSize.x / 2.f + 70.f;

	for (int i = 0; i < sideStarCount; ++i) {
		// Right stars
		sideStarsRight[i].setTexture(starTexture);
		sideStarsRight[i].setScale(1.5f, 1.5f);
		sideStarsRight[i].setOrigin(sideStarsRight[i].getLocalBounds().width / 2.f, sideStarsRight[i].getLocalBounds().height / 2.f);
		sideStarsRight[i].setPosition(sideXRight, startY + i * verticalSpacing);
		sideBasePositionsRight[i] = sideStarsRight[i].getPosition();

		sideGlowsRight[i] = sideStarsRight[i];
		sideGlowsRight[i].setScale(1.6f, 1.6f);
		sideGlowsRight[i].setColor(Color(255, 255, 0, 100));

		// Left stars
		sideStarsLeft[i].setTexture(starTexture);
		sideStarsLeft[i].setScale(1.5f, 1.5f);
		sideStarsLeft[i].setOrigin(sideStarsLeft[i].getLocalBounds().width / 2.f, sideStarsLeft[i].getLocalBounds().height / 2.f);
		sideStarsLeft[i].setPosition(sideXLeft, startY + i * verticalSpacing);
		sideBasePositionsLeft[i] = sideStarsLeft[i].getPosition();

		sideGlowsLeft[i] = sideStarsLeft[i];
		sideGlowsLeft[i].setScale(1.6f, 1.6f);
		sideGlowsLeft[i].setColor(Color(255, 255, 0, 100));
	}

	// Buttons
	Text nextlevelText("Next Level", font, 48);
	nextlevelText.setFillColor(Color::Black);
	FloatRect tryBounds = nextlevelText.getLocalBounds();
	RectangleShape nextlevelBox(Vector2f(tryBounds.width + 60, tryBounds.height + 33));
	nextlevelBox.setFillColor(Color(139, 0, 0, 200));
	nextlevelBox.setOrigin(nextlevelBox.getSize() / 2.f);
	nextlevelText.setOrigin(tryBounds.left + tryBounds.width / 2.f, tryBounds.top + tryBounds.height / 2.f);

	Text backText("Back to Main Menu", font, 48);
	backText.setFillColor(Color::Black);
	FloatRect backBounds = backText.getLocalBounds();
	float backExtraPaddingX = 80;
	RectangleShape backBox(Vector2f(backBounds.width + 60 + backExtraPaddingX, backBounds.height + 35));
	backBox.setFillColor(Color(139, 0, 0, 200));
	backBox.setOrigin(backBox.getSize() / 2.f);
	backText.setOrigin(backBounds.left + backBounds.width / 2.f, backBounds.top + backBounds.height / 2.f);


	float centerX = viewCenter.x;
	float centerY = viewCenter.y;
	float buttonSpacing = 90.f;
	float buttonsTotalWidth = nextlevelBox.getSize().x + backBox.getSize().x + buttonSpacing;
	nextlevelBox.setPosition(centerX - (buttonsTotalWidth / 2.f) + (nextlevelBox.getSize().x / 2.f), centerY + 120.f);
	nextlevelText.setPosition(nextlevelBox.getPosition());

	backBox.setPosition(nextlevelBox.getPosition().x + (nextlevelBox.getSize().x / 2.f) + buttonSpacing + (backBox.getSize().x / 2.f), centerY + 120.f);
	backText.setPosition(backBox.getPosition());


	FloatRect tryAgainBounds = nextlevelBox.getGlobalBounds();
	FloatRect backBoundsGlobal = backBox.getGlobalBounds();
	int selectedIndex = -1;
	bool usingMouse = false;
	window.setMouseCursorVisible(true);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) return "quit";

			if (event.type == Event::MouseMoved) {
				if (!usingMouse) { window.setMouseCursorVisible(true); usingMouse = true; }
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (tryAgainBounds.contains(mousePos)) selectedIndex = 0;
				else if (backBoundsGlobal.contains(mousePos)) selectedIndex = 1;
				else selectedIndex = -1;
			}
			else if (event.type == Event::KeyPressed) {
				if (usingMouse) { window.setMouseCursorVisible(false); usingMouse = false; }
				if (event.key.code == Keyboard::Left) selectedIndex = (selectedIndex - 1 + 2) % 2;
				else if (event.key.code == Keyboard::Right) selectedIndex = (selectedIndex + 1) % 2;
				else if (event.key.code == Keyboard::Enter) {
					if (selectedIndex == 0) return "next_level";
					if (selectedIndex == 1) return "back_to_main_menu";
				}
			}
			else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (tryAgainBounds.contains(mousePos)) return "next_level";
				if (backBoundsGlobal.contains(mousePos)) return "back_to_main_menu";
			}
		}

		float elapsed = clock.getElapsedTime().asSeconds();

		// Animate middle stars
		for (int i = 0; i < starCount; ++i) {
			float offsetY = sin(elapsed * 3.f + i) * 4.f;
			float rotation = sin(elapsed * 2.f + i) * 10.f;
			Vector2f pos = basePositions[i];
			stars[i].setPosition(pos.x, pos.y + offsetY);
			stars[i].setRotation(rotation);
			glows[i].setPosition(pos.x, pos.y + offsetY);
			glows[i].setRotation(rotation);
		}

		// Animate side stars with fading
		float fadeSpeed = 1.65f;
		for (int i = 0; i < sideStarCount; ++i) {
			float delay = i * 0.4f;
			float fadeValue = sin((elapsed - delay) * fadeSpeed) * 0.5f + 0.5f;
			int alpha = static_cast<int>(fadeValue * 255.f);

			sideStarsRight[i].setColor(Color(255, 255, 255, alpha));
			sideGlowsRight[i].setColor(Color(255, 255, 0, static_cast<int>(alpha * 0.4f)));
			sideStarsLeft[i].setColor(Color(255, 255, 255, alpha));
			sideGlowsLeft[i].setColor(Color(255, 255, 0, static_cast<int>(alpha * 0.4f)));
		}


		window.clear(Color::Black);
		window.draw(backgroundSprite);
		window.draw(wordSprite);

		for (int i = 0; i < sideStarCount; ++i) {
			window.draw(sideGlowsRight[i]);
			window.draw(sideStarsRight[i]);
			window.draw(sideGlowsLeft[i]);
			window.draw(sideStarsLeft[i]);
		}

		for (int i = 0; i < starCount; ++i) {
			window.draw(glows[i]);
			window.draw(stars[i]);
		}

		nextlevelText.setFillColor(selectedIndex == 0 ? Color::White : Color::Black);
		backText.setFillColor(selectedIndex == 1 ? Color::White : Color::Black);
		window.draw(nextlevelBox);
		window.draw(nextlevelText);
		window.draw(backBox);
		window.draw(backText);
		window.display();
	}

	window.setMouseCursorVisible(false);
	return "";
}

String multidead(RenderWindow& window, Event& event, Font& font, int currentLevelId)
{
	View currentView = window.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();

	Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("imgs/pause/main3.jpg"))
		return "error";

	Texture starTexture;
	if (!starTexture.loadFromFile("imgs/pause/star.png"))
		return "error";

	Sprite backgroundSprite(backgroundTexture);
	backgroundSprite.setScale(4.25f, 4.25f);
	backgroundSprite.setPosition(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f);

	Music music;
	if (!music.openFromFile("MainSound.wav")) {
		cout << "Failed to load music.\n";
		return "error";
	}
	music.setLoop(true);
	music.setVolume(musicVolume);
	music.play();

	Text playersText(player1 + "  :  " + player2, font, 35);
	playersText.setFillColor(Color(190, 115, 55, 230));
	playersText.setPosition(viewCenter.x - playersText.getGlobalBounds().width / 2.f, viewCenter.y - 300.f);

	Text scoreText(to_string(score_) + "  :  " + to_string(score_2), font, 40);
	scoreText.setFillColor(Color(180, 130, 35, 240));
	scoreText.setPosition(viewCenter.x - scoreText.getGlobalBounds().width / 2.f, viewCenter.y - 220.f);

	Text winnerText("", font, 48);
	if (score_ > score_2)
		winnerText.setString(player1 + " Wins");
	else if (score_ < score_2)
		winnerText.setString(player2 + " Wins");
	else
		winnerText.setString("Draw");
	winnerText.setFillColor(Color(210, 180, 05, 255));
	winnerText.setPosition(viewCenter.x - winnerText.getGlobalBounds().width / 2.f, viewCenter.y - 140.f);


	Text tryAgainText("Try Again", font, 48);
	tryAgainText.setFillColor(Color::Black);
	FloatRect tryBounds = tryAgainText.getLocalBounds();
	RectangleShape tryAgainBox(Vector2f(tryBounds.width + 60, tryBounds.height + 30));
	tryAgainBox.setFillColor(Color(139, 0, 0, 200));
	tryAgainBox.setOrigin(tryAgainBox.getSize() / 2.f);
	tryAgainText.setOrigin(tryBounds.left + tryBounds.width / 2.f, tryBounds.top + tryBounds.height / 2.f);

	Text backText("Back to Main Menu", font, 48);
	backText.setFillColor(Color::Black);
	FloatRect backBounds = backText.getLocalBounds();
	RectangleShape backBox(Vector2f(backBounds.width + 100, backBounds.height + 35));
	backBox.setFillColor(Color(139, 0, 0, 200));
	backBox.setOrigin(backBox.getSize() / 2.f);
	backText.setOrigin(backBounds.left + backBounds.width / 2.f, backBounds.top + backBounds.height / 2.f);


	float spacingBetweenButtons = 100.f;
	float buttonsY = viewCenter.y + 200.f;

	tryAgainBox.setPosition((viewCenter.x - spacingBetweenButtons / 2.f - tryAgainBox.getSize().x / 2.f) - 80, buttonsY);
	backBox.setPosition(viewCenter.x + spacingBetweenButtons / 2.f + backBox.getSize().x / 2.f, buttonsY);
	tryAgainText.setPosition(tryAgainBox.getPosition());
	backText.setPosition(backBox.getPosition());

	FloatRect tryAgainBounds = tryAgainBox.getGlobalBounds();
	FloatRect backBoundsGlobal = backBox.getGlobalBounds();


	const int starCount = 3;
	Sprite sideStarsLeft[starCount], sideGlowsLeft[starCount];
	Sprite sideStarsRight[starCount], sideGlowsRight[starCount];

	float verticalSpacing = 100.f;
	float startY = viewCenter.y - ((starCount - 1) * verticalSpacing) / 2.f;
	float sideXLeft = viewCenter.x - viewSize.x / 2.f + 80.f;
	float sideXRight = viewCenter.x + viewSize.x / 2.f - 80.f;

	for (int i = 0; i < starCount; ++i) {

		sideStarsLeft[i].setTexture(starTexture);
		sideStarsLeft[i].setScale(1.5f, 1.5f);
		sideStarsLeft[i].setOrigin(sideStarsLeft[i].getLocalBounds().width / 2.f, sideStarsLeft[i].getLocalBounds().height / 2.f);
		sideStarsLeft[i].setPosition(sideXLeft, startY + i * verticalSpacing);

		sideGlowsLeft[i] = sideStarsLeft[i];
		sideGlowsLeft[i].setScale(1.6f, 1.6f);
		sideGlowsLeft[i].setColor(Color(255, 255, 0, 100));

		sideStarsRight[i].setTexture(starTexture);
		sideStarsRight[i].setScale(1.5f, 1.5f);
		sideStarsRight[i].setOrigin(sideStarsRight[i].getLocalBounds().width / 2.f, sideStarsRight[i].getLocalBounds().height / 2.f);
		sideStarsRight[i].setPosition(sideXRight, startY + i * verticalSpacing);

		sideGlowsRight[i] = sideStarsRight[i];
		sideGlowsRight[i].setScale(1.6f, 1.6f);
		sideGlowsRight[i].setColor(Color(255, 255, 0, 100));
	}


	int selectedIndex = -1;
	bool usingMouse = false;
	Clock clock;
	window.setMouseCursorVisible(true);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				return "quit";
			}

			if (event.type == Event::MouseMoved) {
				if (!usingMouse) {
					window.setMouseCursorVisible(true);
					usingMouse = true;
				}
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (tryAgainBounds.contains(mousePos)) selectedIndex = 0;
				else if (backBoundsGlobal.contains(mousePos)) selectedIndex = 1;
				else selectedIndex = -1;
			}
			else if (event.type == Event::KeyPressed) {
				if (usingMouse) {
					window.setMouseCursorVisible(false);
					usingMouse = false;
				}
				if (event.key.code == Keyboard::Left)
					selectedIndex = (selectedIndex - 1 + 2) % 2;
				else if (event.key.code == Keyboard::Right)
					selectedIndex = (selectedIndex + 1) % 2;
				else if (event.key.code == Keyboard::Enter) {
					if (selectedIndex == 0) return "try_again";
					if (selectedIndex == 1) return "back_to_main_menu";
				}
			}
			else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (tryAgainBounds.contains(mousePos)) return "try_again";
				if (backBoundsGlobal.contains(mousePos)) return "back_to_main_menu";
			}
		}


		float elapsed = clock.getElapsedTime().asSeconds();
		float fadeDuration = 0.45f;
		for (int i = 0; i < starCount; ++i) {
			float delay = i * fadeDuration;
			float alpha = 255 * 0.5f * (1 + sin((elapsed - delay) * 3.14f));

			Color starColor = Color(255, 255, 255, static_cast<Uint8>(alpha));
			Color glowColor = Color(255, 255, 0, static_cast<Uint8>(alpha / 1.5f));

			sideStarsLeft[i].setColor(starColor);
			sideGlowsLeft[i].setColor(glowColor);

			sideStarsRight[i].setColor(starColor);
			sideGlowsRight[i].setColor(glowColor);
		}
		tryAgainText.setFillColor(selectedIndex == 0 ? Color::White : Color::Black);
		backText.setFillColor(selectedIndex == 1 ? Color::White : Color::Black);

		window.clear(Color::Black);
		window.draw(backgroundSprite);


		for (int i = 0; i < starCount; ++i) {
			window.draw(sideGlowsLeft[i]);
			window.draw(sideStarsLeft[i]);
			window.draw(sideGlowsRight[i]);
			window.draw(sideStarsRight[i]);
		}


		window.draw(playersText);
		window.draw(scoreText);
		window.draw(winnerText);


		window.draw(tryAgainBox);
		window.draw(tryAgainText);
		window.draw(backBox);
		window.draw(backText);

		window.display();
	}

	window.setMouseCursorVisible(false);
	return "";
}



void pause_menu(RenderWindow& window, Event& event, Font& font, levelHandler& currentLevel, vector<PLAYER>& playersArr, vector<ZOMBIE>& zombiesArr) {
	window.setMouseCursorVisible(true);

	View currentView = window.getView();
	Vector2f viewCenter = currentView.getCenter();
	Vector2f viewSize = currentView.getSize();

	Texture pauseBackgroundTexture;
	pauseBackgroundTexture.loadFromFile("imgs/pause/main3.jpg");
	Sprite pauseBackgroundSprite;
	pauseBackgroundSprite.setTexture(pauseBackgroundTexture);
	pauseBackgroundSprite.setScale(4.25, 4.25);
	pauseBackgroundSprite.setPosition(viewCenter.x - viewSize.x / 2.f, viewCenter.y - viewSize.y / 2.f);

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

	exit.setFont(font);
	exit.setString("Quit");
	exit.setCharacterSize(40);
	exit.setFillColor(Color::White);
	exit.setOrigin(exit.getLocalBounds().width / 2, exit.getLocalBounds().height / 2);
	exit.setPosition(window.mapPixelToCoords({ 1920 / 2, 1080 / 2 + 60 }));

	int PmenuSelection = 0;
	bool usingMouse = false;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			// Mouse moved
			if (event.type == Event::MouseMoved) {
				if (!usingMouse) {
					usingMouse = true;
					window.setMouseCursorVisible(true);  // Show mouse when mouse is used
				}
			}
			else if (event.type == Event::KeyPressed) {
				if (usingMouse) {
					usingMouse = false;
					window.setMouseCursorVisible(false); // Hide mouse when keyboard is used
				}
				if (event.key.code == Keyboard::Up)
					PmenuSelection = (PmenuSelection - 1 + 3) % 3;
				else if (event.key.code == Keyboard::Down)
					PmenuSelection = (PmenuSelection + 1) % 3;
				else if (event.key.code == Keyboard::Enter) {
					if (PmenuSelection == 0)
						return;
					else if (PmenuSelection == 1) {
						isMainmenuTriggerdByPause = true;
						return;
					}
					else if (PmenuSelection == 2) {
						saveScores();
						window.close();
					}
				}
			}

			// Mouse click
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				if (!usingMouse) {
					usingMouse = true;
					window.setMouseCursorVisible(true);  // Show mouse on mouse click
				}
				if (resume.getGlobalBounds().contains(mouseWorldPos)) return;
				if (back.getGlobalBounds().contains(mouseWorldPos)) {
					isMainmenuTriggerdByPause = true;
					return;
				}
				if (exit.getGlobalBounds().contains(mouseWorldPos)) {
					saveScores();
					window.close();
				}
			}
		}

		Vector2i mousePos = Mouse::getPosition(window);
		Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos);

		// Hover effect
		if (usingMouse) {
			resume.setFillColor(resume.getGlobalBounds().contains(mouseWorldPos) ? Color::Red : Color::White);
			back.setFillColor(back.getGlobalBounds().contains(mouseWorldPos) ? Color::Red : Color::White);
			exit.setFillColor(exit.getGlobalBounds().contains(mouseWorldPos) ? Color::Red : Color::White);
		}
		else {
			resume.setFillColor(PmenuSelection == 0 ? Color::Red : Color::White);
			back.setFillColor(PmenuSelection == 1 ? Color::Red : Color::White);
			exit.setFillColor(PmenuSelection == 2 ? Color::Red : Color::White);
		}

		window.clear();
		window.draw(pauseBackgroundSprite);
		window.draw(resume);
		window.draw(back);
		window.draw(exit);
		window.display();
	}

	window.setMouseCursorVisible(false);
}


int main() {

	srand(static_cast<unsigned>(time(0)));

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
	clickSound.setVolume(sfxVolume);

	menuMusic.openFromFile("MainSound.wav");
	menuMusic.setLoop(true);
	menuMusic.play();
	menuMusic.setVolume(musicVolume);

	const int buttonCount = 6;
	MenuButton buttons[buttonCount];

	Image image;
	if (!image.loadFromFile("imgs/cursor48.png")) {
		std::cout << "Failed to load image\n";
	}

	Cursor cursor;
	if (cursor.loadFromPixels(image.getPixelsPtr(), image.getSize(), { 15, 0 })) {
		window.setMouseCursor(cursor);
	}



	const string buttonLabels[buttonCount] = {
		"Story Mode", "Rush Mode", "Leaderboard",
		"Game Controls", "Game Credits", "Quit"
	};

	const string buttonPhrases[buttonCount] = {
		"Live the story of CrimsonLand",
		"Challenge and Get Scores",
		"Top Ten High Scores",
		"Need Help",
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
						clickSound.setVolume(sfxVolume);
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
						clickSound.setVolume(sfxVolume);
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
				tryAgain = false;
			}

			Event event;

			window.setMouseCursorVisible(false);



			//you win menu

			if (currentLevel.getMissionComplete()) {

				if (currentLevel.id < totalLevels - 1 && !unlocked[currentLevel.id + 1]) {
					unlocked[currentLevel.id + 1] = true;
					saveStoryProgress(unlocked, totalLevels);
				}


				if (currentLevel.id == 11 && !endScene && !isEndedEndScene) {

					runEndScene(window);

					isEndedEndScene = true;

				}

				else {
					currentLevel.update(playersArr, zombiesArr, window);

					for (auto& sound : gameSounds)
					{
						sound.stop();
					}

					string winChoice = youwin(window, event, font, currentLevel.id);


					bool sceneEnded = false;
					if (winChoice == "next_level") {
						if (currentLevel.id == 0) sceneEnded = runDesertScene(window);
						else if (currentLevel.id == 2) sceneEnded = runCityScene(window);
						else if (currentLevel.id == 3) sceneEnded = runTrainScene(window);
						else if (currentLevel.id == 5) sceneEnded = runWoodsScene(window);
						else if (currentLevel.id == 6) sceneEnded = runCampScene(window);
						else if (currentLevel.id == 7) sceneEnded = runstartMissionScene(window);
						else if (currentLevel.id == 10) {
							runMissionScene(window);
							sceneEnded = true;
						}
						else sceneEnded = true;
						if (sceneEnded) {
							levelIDMenu = currentLevel.id + 1;
							tryAgain = true;
							currentLevel.deleteCurrentLevel();
							isGameEntered = false;
							gameSounds[1].stop();
							gameSounds[5].stop();
							levelStarted = false;
							isGameEntered = false;

							playersArr.clear();

							zombiesArr.clear();

							deathArr.clear();
							slowEffectActive = false;

							bullets.clear();

						}
					}
					else if (winChoice == "back_to_main_menu") {

						isMainmenuTriggerdByPause = true;


					}

					else if (winChoice == "quit") {

						saveScores();

						window.close();

					}

				}

			}



			//Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) {
					saveScores();
					window.close();
				}
				for (int i = 0; i < playersArr.size(); i++) {
					playersArr[i].emotesStarter(event);
				}
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::P) {
					kk = true;
					currentLevel.setMissionComplete(true);
				}
				if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape || Joystick::isButtonPressed(0, 7))

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

					for (auto& sound : gameSounds)
					{
						if (!sound.isplaying) {
							sound.play();
						}
					}
				}
			}
			else {
				for (auto& sound : gameSounds)
				{
					if (sound.isplaying) {
						sound.stop();
					}
				}
				gameTimer.pause();
				pause_menu(window, event, font, currentLevel, playersArr, zombiesArr);
				for (auto& sound : gameSounds)

				{

					sound.stop();

				}
				pauseGame = false;
			}

			window.clear();



			if (levelStarted) {
				currentLevel.draw(playersArr, zombiesArr, window);
			}
			if (isGameEntered && !currentLevel.getMissionComplete()) {
				if (playersArr.empty())showDeath(playersArr, window);



				if (playersArr.empty())

				{
					currentLevel.update(playersArr, zombiesArr, window);


					for (auto& sound : gameSounds)
					{
						sound.stop();
					}

					//you dead menu

					string deathChoice = youdead(window, event, font, currentLevel.id);


					if (deathChoice == "try_again") {

						levelIDMenu = currentLevel.id;
						tryAgain = true;
						currentLevel.deleteCurrentLevel();
						isGameEntered = false;
						gameSounds[1].stop();
						gameSounds[5].stop();
						levelStarted = false;
						isGameEntered = false;

						playersArr.clear();

						zombiesArr.clear();

						deathArr.clear();
						slowEffectActive = false;

						bullets.clear();


					}

					else if (deathChoice == "back_to_main_menu") {

						isMainmenuTriggerdByPause = true;
					}



					saveScores();

				}

				else {
					if (playersArr.size() > 1) {
						if (playersArr[0].isDead && playersArr[1].isDead) {
							currentLevel.update(playersArr, zombiesArr, window);


							for (auto& sound : gameSounds)
							{
								sound.stop();
							}

							//you dead menu

							string deathChoice = multidead(window, event, font, currentLevel.id);


							if (deathChoice == "try_again") {

								levelIDMenu = currentLevel.id;
								tryAgain = true;
								currentLevel.deleteCurrentLevel();
								isGameEntered = false;
								gameSounds[1].stop();
								gameSounds[5].stop();
								levelStarted = false;
								isGameEntered = false;

								playersArr.clear();

								zombiesArr.clear();

								deathArr.clear();
								slowEffectActive = false;

								bullets.clear();


							}

							else if (deathChoice == "back_to_main_menu") {

								isMainmenuTriggerdByPause = true;
							}
						}

					}
				}
			}

			if (!tryAgain) {
				if (((currentLevel.id == 11) && !endScene && isEndedEndScene) || isMainmenuTriggerdByPause) {
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
					deathArr.clear();
					slowEffectActive = false;
				}

			}

			window.display();


		}

	}

	saveScores();

	return 0;
}