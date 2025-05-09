#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>


const double PI = 3.14159265f;

using namespace sf;
using namespace std;


enum FireType {
	SINGLE,
	BURST,
	AUTO
};


// Weapon types (IDs)

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
}


struct Weapon {
	WeaponID id;
	string name;
	int clipSize;
	int currentClipSize;
	float damage;
	float bulletDelay;
	bool isPlasma;
	FireType fireType;
	int currentSprite;

	Weapon(WeaponID index) {
		id = index;
		isPlasma = false;

		switch (id)
		{
		case KNIFE:
			name = "Knife";
			clipSize = 0;
			damage = 35.0f;
			bulletDelay = 0.4f;
			fireType = SINGLE;
			currentSprite = 0;
			break;

		case PISTOL:
			name = "Pistol";
			clipSize = 17;
			damage = 50.0f;
			bulletDelay = 0.45f;
			fireType = SINGLE;
			currentSprite = 1;
			break;

		case RIFLE:
			name = "Rifle (Auto)";
			clipSize = 30;
			damage = 75.0f;
			bulletDelay = 0.1f;
			fireType = AUTO;
			currentSprite = 2;
			break;

		case BURST_RIFLE:
			name = "Rifle (Burst)";
			clipSize = 24;
			damage = 75.0f;
			bulletDelay = 1.0f;
			fireType = BURST;
			currentSprite = 2;
			break;

		case SINGLE_RIFLE:
			name = "Rifle (Single)";
			clipSize = 20;
			damage = 100.0f;
			bulletDelay = 0.5f;
			fireType = SINGLE;
			currentSprite = 2;
			break;

		case SHOTGUN:
			name = "Shotgun";
			clipSize = 7;
			damage = 50.0f;
			bulletDelay = 0.65f;
			fireType = SINGLE;
			currentSprite = 3;
			break;

		case PLASMA_PISTOL:
			name = "Plasma Pistol";
			clipSize = 17;
			damage = 100.0f;
			bulletDelay = 0.45f;
			fireType = SINGLE;
			currentSprite = 1;
			isPlasma = true;
			break;

		case PLASMA_RIFLE:
			name = "Plasma Rifle";
			clipSize = 30;
			damage = 100.0f;
			bulletDelay = 0.1f;
			fireType = AUTO;
			currentSprite = 2;
			isPlasma = true;
			break;

		case PLASMA_SHOTGUN:
			name = "Plasma Shotgun";
			clipSize = 7;
			damage = 50.0f;
			bulletDelay = 0.65f;
			fireType = SINGLE;
			currentSprite = 3;
			isPlasma = true;
			break;

		default:
			name = "Unknown";
			clipSize = 0;
			damage = 0.0f;
			bulletDelay = 1.0f;
			fireType = SINGLE;
			currentSprite = 0;
			break;
		}

		currentClipSize = clipSize;
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





struct PLAYER {

	float const FRAME_WIDTH = 280.0f;
	float const FRAME_HEIGHT = 220.0f;


	float health = 100.0f; // Starting With The full Health (100%)


	CROSSHAIR crosshair;
	Weapon currentWeapon; // 

	float lastFireTime = 0;
	float now = 0;

	int currentSprite = currentWeapon.currentSprite; // Index from 0 to n (n = number of the weapons (0 refers to knife , 1 to handgun , 2 to rifle , 3 to shotgun))
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

	float animationDelay = 0.025f;


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


	PLAYER(float x, float y, WeaponID weaponIndex, RenderWindow& window, float localscaleX = 0.35, float localscaleY = 0.35) : currentWeapon(weaponIndex), crosshair(crosshairTexture, window) {
		/*

		// first and second for spawning coordinates ,
		third and forth for texture and weapone (Read Up) ,
		fifth and sixth for the scale (it's optional ,u can change it after creating the player (like in changing animations state))

		*/

		scaleX = localscaleX;
		scaleY = localscaleY;
		//shape.
		//shape.setSize(Vector2f(FRAME_WIDTH, FRAME_HEIGHT)); // DON'T CHANGE IT !! , if u want to resize the shape use setScale(); (try to make all sprites with the scale not stretched as u can)
		shape.setScale(Vector2f(scaleX, -scaleY));
		shape.setTexture(SpriteTextures[currentSprite]);
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

			if (currentSprite != 0) {
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
			shape.setScale(scaleX + 0.15, scaleY + 0.15);
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

		bool isMoving = false;

		if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::D)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(diagonalSpeed, -diagonalSpeed);
			shape.setRotation(-45);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::W) && Keyboard::isKeyPressed(Keyboard::A)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(-diagonalSpeed, -diagonalSpeed);
			shape.setRotation(-135);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::D)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(diagonalSpeed, diagonalSpeed);
			shape.setRotation(45);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S) && Keyboard::isKeyPressed(Keyboard::A)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(-diagonalSpeed, diagonalSpeed);
			shape.setRotation(135);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::W)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(0, -speed);
			shape.setRotation(-90);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(0, speed);
			shape.setRotation(90);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::A)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(-speed, 0);
			shape.setRotation(180);
			isMoving = true;
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			if (!(currentState == 2 || currentState == 3)) { changeState(0); }
			shape.move(speed, 0);
			shape.setRotation(0);
			isMoving = true;
		}

		if (!isMoving) {
			if (!(currentState == 2 || currentState == 3)) { changeState(1); }
		}
	}
	void playerMouse(Vector2f MouseGlobalPos) {
		Vector2f aimDirection = MouseGlobalPos - shape.getPosition();
		shape.setRotation(atan2(aimDirection.y, aimDirection.x) * 180 / PI);
	}


	void emotesStarter(Event event) {
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
			if (!(currentState == 2 || currentState == 3) && (currentSprite != 0)) {
				changeState(2);
			}
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::F) {
			if (!(currentState == 2 || currentState == 3)) {
				changeState(3);
			}
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

		if (Mouse::isButtonPressed(Mouse::Left) && ((currentWeapon.clipSize == currentWeapon.currentClipSize) || ((now - lastFireTime) >= currentWeapon.bulletDelay))) {

			Vector2f playerPos = shape.getPosition();
			Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
			Vector2f dir = getDirection(playerPos, mousePos);

			if (currentWeapon.currentClipSize > 0) {
				if (currentWeapon.id == SHOTGUN || currentWeapon.id == PLASMA_SHOTGUN) { // Shotgun
					for (int i = -2; i <= 2; ++i) {
						float spread = i * 9.5f;
						Bullet b = createBullet(playerPos, shape.getRotation(), currentWeapon, window);
						b.shape.rotate(spread);
						b.velocity = rotateVector(b.velocity, spread);
						bullets.push_back(b);
					}
					currentWeapon.currentClipSize--;

				}
				else if (currentWeapon.id == BURST_RIFLE) {
					if (!isBursting && currentWeapon.currentClipSize >= 3) {
						isBursting = true;
						burstShotsFired = 0;
						nextBurstTime = now;
						currentWeapon.currentClipSize -= 3;
						cout << currentWeapon.currentClipSize << endl;
					}
				}
				else {
					bullets.push_back(createBullet(playerPos, shape.getRotation(), currentWeapon, window));
					currentWeapon.currentClipSize--;
				}


			}

			lastFireTime = now;

		}
	}


	void endState() {
		if (reloadIndex == 19) {
			currentWeapon.currentClipSize = currentWeapon.clipSize;
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
				Bullet b = createBullet(shape.getPosition(), shape.getRotation(), currentWeapon, window);
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



struct ZOMBIE_3 {

	float FRAME_WIDTH = 318.0f;
	float FRAME_HEIGHT = 311.0f;

	float health = 200.0f; // Starting Health
	float speed = 3.0f; // Zombies are usually slower
	float diagonalSpeed = abs(speed) / sqrt(2); // For diagonal movement

	Sprite shape; // Main shape
	Clock animationClock;

	float animationDelay = 0.05f; // Zombies are a bit slower in animating

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

	bool isFar = true;

	int movingDirection = 1;
	int idleDirection = 1;

	bool isDead = false;

	float zombieAttackDistance = 50.0f;
	float playerAvoidDistance = 45.0f;


	float scaleX;
	float scaleY;

	Clock zombieDeathTimer;
	bool isTimerTriggerd = false;
	bool shoudBeErased = false;

	ZOMBIE_3(float x, float y, int localIndex, float localscaleX = 0.35f, float localscaleY = 0.35f) {

		index = localIndex;

		if (index == 0) {
			FRAME_WIDTH = 256.0f;
			FRAME_HEIGHT = 256.0f;
			scaleX = 1.15;
			scaleY = 1.15;

			zombieAttackDistance = 100.0f;
			playerAvoidDistance = 85.0f;
			health = 100.0f;
		}
		else if (index == 1) {
			scaleX = localscaleX;
			scaleY = localscaleY;
		}
		shape.setScale(Vector2f(scaleX, -scaleY));

		shape.setTexture(ZombieTextures[index]); // You'll need to define ZombieTexture globally
		shape.setPosition(x, y);

		shape.setTextureRect(IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
		shape.setOrigin(shape.getLocalBounds().width / 2, shape.getLocalBounds().height / 2);


	}

	void changingFrames() {
		if (animationClock.getElapsedTime().asSeconds() < animationDelay)
			return;
		animationClock.restart();

		if (index == 0) {
			if (currentState == 0) { // Moving

				movingIndex += movingDirection;
				if (movingIndex >= 31) {
					movingIndex = 30;
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
				if (idleIndex >= 31) {
					idleIndex = 30;
					idleDirection = -1;
				}
				else if (idleIndex <= 0) {
					idleIndex = 1;
					idleDirection = 1;
				}
				shape.setTextureRect(IntRect(idleIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 2) { // Attacking
				attackIndex = (attackIndex + 1) % 20;
				shape.setTextureRect(IntRect(attackIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 3) { // Dying
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
			if (currentState == 0) { // Moving

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
			else if (currentState == 1) { // Idle

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
			else if (currentState == 2) { // Attacking
				attackIndex = (attackIndex + 1) % 9;
				shape.setTextureRect(IntRect(attackIndex * FRAME_WIDTH, currentState * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
			}
			else if (currentState == 3) { // Dying
				if (dyingIndex < 4) {
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
		/*
			0: Moving
			1: Idle
			2: Attacking
			3: Dying
		*/
		changingFrames();
	}





	void zombieMoveTowards(Vector2f targetPosition, float  distance) {
		if ((distance < 50.0f) || (currentState == 3)) {
			return; // No movement while attacking or dying
		}
		else {
			if (attackIndex < 8 && currentState == 2) {
				isFar = true;
				return;
			}
			changeState(0);
		}


		Vector2f direction = targetPosition - shape.getPosition();
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);

		if (length != 0) {
			direction /= length; // Normalize
		}

		shape.move(direction.x * speed, direction.y * speed);

		// Rotate towards player
		shape.setRotation(atan2(direction.y, direction.x) * 180 / PI);
		changeState(0); // Moving state

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


	void avoidOtherZombies(vector<ZOMBIE_3>& zombies) {
		for (int i = 0; i < zombies.size(); ++i) {
			if (&zombies[i] == this || zombies[i].isDead)
				continue;



			Vector2f otherPos = zombies[i].shape.getPosition();
			float dist = sqrt(pow(shape.getPosition().x - otherPos.x, 2) + pow(shape.getPosition().y - otherPos.y, 2));

			if (dist < zombies[i].playerAvoidDistance && dist > 0.0f) {
				Vector2f away = shape.getPosition() - otherPos;

				float length = sqrt(away.x * away.x + away.y * away.y);
				if (length != 0) {
					away /= length;
					shape.move(away.x * 1.5f, away.y * 1.5f);
				}
			}
		}
	}


	void update(vector<PLAYER>& players, vector<ZOMBIE_3>& zombies) {
		if (isDead && !isTimerTriggerd) {
			isTimerTriggerd = true;
			zombieDeathTimer.restart().asSeconds();
		}


		if (isDead)
		{
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
				changeState(1); // Idle animation
				changingFrames();
				return;
			}

			Vector2f nearestPlayerPos = findNearestPlayerPosition(shape.getPosition(), players);

			float distance = sqrt(pow(shape.getPosition().x - nearestPlayerPos.x, 2) +
				pow(shape.getPosition().y - nearestPlayerPos.y, 2));

			if (distance < zombieAttackDistance) {
				// Attack distance
				isFar = false;
				zombieAttack();
			}
			else {
				zombieMoveTowards(nearestPlayerPos, distance);
			}

		}

		avoidOtherZombies(zombies);




		changingFrames();



	}


};



void playersAvoidOtherZombies(vector<PLAYER>& playersArr, vector<ZOMBIE_3>& zombiesArr) {
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


void bulletIntersection(vector<Bullet>& bullets, vector<PLAYER>& playersArr, vector<ZOMBIE_3>& zombiesArr) { // should also take vector of bulidings 
	for (int i = 0; i < bullets.size(); ++i) {
		Vector2f bulletPos = bullets[i].shape.getPosition();
		bool isBulletGone = false;
		/*for (int j = 0; j < zombiesArr.size(); ++j) {
			if (bullets[i].shape.getGlobalBounds().intersects(zombiesArr[j].shape.getGlobalBounds())) {
				zombiesArr[j].health -= bullets[i].damage;
				cout << zombiesArr[j].health << endl;
				bullets.erase(bullets.begin() + i);
				--i;
				isBulletGone = true;
				break;
			}
		}*/
		for (int j = 0; j < zombiesArr.size(); ++j) {
			if (zombiesArr[j].isDead) continue;

			Vector2f zombiePos = zombiesArr[j].shape.getPosition();
			float dist = sqrt(pow(bulletPos.x - zombiePos.x, 2) + pow(bulletPos.y - zombiePos.y, 2));

			if (dist < 30.0f && dist > 0.0f) {
				Vector2f away = bulletPos - zombiePos;
				zombiesArr[j].health -= bullets[i].damage;

				bullets.erase(bullets.begin() + i);
				isBulletGone = true;
				cout << zombiesArr[j].health << endl;
				break;

			}
		}

		if (isBulletGone) continue;
		/*for (int j = 0; j < zombiesArr.size(); ++j) {
		*	we should add the same loop but for the buildings
		*
			if (bullets[i].shape.getGlobalBounds().intersects(zombiesArr[j].shape.getGlobalBounds())) {
				zombiesArr[j].health -= bullets[i].damage;
				bullets.erase(bullets.begin() + i);
				--i;
				isBulletGone = true;
			}
		}*/
	}
}

void updateEntities(vector<PLAYER>& playersArr, vector<ZOMBIE_3>& zombiesArr, vector<Bullet>& bullets, Clock zombieDeathTimer, bool zombieActionTriggered, RenderWindow& window) {

	for (int i = 0; i < playersArr.size(); i++) playersArr[i].update(window);
	for (int i = 0; i < zombiesArr.size(); i++) zombiesArr[i].update(playersArr, zombiesArr);
	playersAvoidOtherZombies(playersArr, zombiesArr);
	bulletIntersection(bullets, playersArr, zombiesArr);
	//cout << "size is " << zombiesArr.size() << endl;

	for (int i = 0; i < zombiesArr.size(); i++) {
		if (zombiesArr[i].shoudBeErased) {
			zombiesArr.erase(zombiesArr.begin() + i);
			--i;
		}
	}


}

void drawEntities(vector<PLAYER>& playersArr, vector<ZOMBIE_3>& zombiesArr, RenderWindow& window) {


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
		window.draw(playersArr[i].crosshair.shape);
	}
}


int main() {
	loadingAssets(); // insure we have loaded all textures correctly

	vector<PLAYER> playersArr;
	vector<ZOMBIE_3> zombiesArr;
	bool zombieActionTriggered = false;

	RenderWindow window(VideoMode::getDesktopMode(), "My Game");
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	playersArr.push_back(PLAYER(rand() % 1920, rand() % 1080, PISTOL, window));
	//playersArr.push_back(PLAYER(100 / 2, 1080 / 2, PLASMA_SHOTGUN, window));


	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 0));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));
	zombiesArr.push_back(ZOMBIE_3(rand() % 1920, rand() % 1080, 1));



	Clock deltaClock;
	Clock zombieDeathTimer;

	while (window.isOpen()) {
		float dt = deltaClock.restart().asSeconds();



		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			for (int i = 0; i < playersArr.size(); i++) {
				playersArr[i].emotesStarter(event);
			}
		}



		updateEntities(playersArr, zombiesArr, bullets, zombieDeathTimer, zombieActionTriggered, window);
		updateBullets(dt);

		window.clear();
		drawEntities(playersArr, zombiesArr, window);
		window.display();

	}

	return 0;
}
