#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <cmath>
#include <vector>
#include <map>
#include <string>

#include <random>

using namespace std;
const float speed = 5.0f;
class Ball {
public:
	Ball(float radius = 5.f, const sf::Vector2f& velocity = sf::Vector2f(0.5f, -0.5f))
		: m_radius(radius), m_velocity(velocity) {
		m_circle.setRadius(m_radius);
		m_circle.setFillColor(sf::Color::White);
		m_circle.setOutlineColor(sf::Color::Black);
		m_circle.setOrigin(m_radius, m_radius);
		m_circle.setPosition(400.f, 250.f);
	}

	void update() {

		const float acceleration = 0.000001f;

		m_velocity *= (1 + acceleration);

		m_circle.move(m_velocity);

		// Check collision with walls
		if (m_circle.getPosition().x - m_radius < 0 || m_circle.getPosition().x + m_radius > 900) {
			m_velocity.x *= -1.f;
		}
		if (m_circle.getPosition().y - m_radius < 0 || m_circle.getPosition().y + m_radius > 500) {
			m_velocity.y *= -1.f;
		}
	}

	void draw(sf::RenderWindow& window) const {
		window.draw(m_circle);
	}

	sf::Vector2f getPosition() const {
		return m_circle.getPosition();
	}

	float getRadius() const {
		return m_radius;
	}

	void setVelocity(const sf::Vector2f& velocity) {
		m_velocity = velocity;
	}

	sf::Vector2f getVelocity() const {
		return m_velocity;
	}

	sf::CircleShape& getShape() {
		return m_circle;
	}

	// New methods for cooldown management
	bool canCollideWith(const sf::CircleShape& redCircle) const {
		auto it = collisionCooldowns.find(&redCircle);
		if (it == collisionCooldowns.end()) {
			return true;
		}
		sf::Time elapsed = collisionClock.getElapsedTime() - it->second;
		return elapsed.asSeconds() >= 1.0f;
	}

	void registerCollisionWith(const sf::CircleShape& redCircle) {
		collisionCooldowns[&redCircle] = collisionClock.getElapsedTime();
	}

private:
	sf::CircleShape m_circle;
	float m_radius;
	sf::Vector2f m_velocity;

	// New member variables for managing collision cooldowns
	mutable sf::Clock collisionClock;
	mutable std::map<const sf::CircleShape*, sf::Time> collisionCooldowns;
};

class BlindZone {
public:
	BlindZone(const sf::Texture& texture)
		: m_texture(texture), m_visible(false) {
		m_sprite.setTexture(m_texture);
		m_sprite.setScale(0.5f, 0.5f); // Scale the image to fit the screen
	}

	void update() {
		sf::Time elapsed = m_spawnClock.getElapsedTime();
		if (!m_visible && elapsed.asSeconds() >= 30.0f) {
			spawn();
			m_visible = true;
			m_visibilityClock.restart();
		}

		if (m_visible && m_visibilityClock.getElapsedTime().asSeconds() >= 10.0f) {
			m_visible = false;
			m_spawnClock.restart();
		}
	}

	void draw(sf::RenderWindow& window) const {
		if (m_visible) {
			window.draw(m_sprite);
		}
	}

private:
	void spawn() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> disX(0, 600 - m_sprite.getGlobalBounds().width);
		std::uniform_int_distribution<> disY(0, 440 - m_sprite.getGlobalBounds().height);

		m_sprite.setPosition(disX(gen), disY(gen));
	}

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Clock m_spawnClock;
	sf::Clock m_visibilityClock;
	bool m_visible;
};


float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

bool checkCollisionWithRedCircles(const Ball& ball, const sf::CircleShape& redCircle) {
	sf::Vector2f ballCenter = ball.getPosition() + sf::Vector2f(ball.getRadius(), ball.getRadius());
	sf::Vector2f circleCenter = redCircle.getPosition() + sf::Vector2f(redCircle.getRadius(), redCircle.getRadius());

	float distance = sqrt(pow(ballCenter.x - circleCenter.x, 2) + pow(ballCenter.y - circleCenter.y, 2));

	return distance <= ball.getRadius() + redCircle.getRadius();
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(930, 500), "Pong202");
	Menu menu;
	menu.show(window);

	sf::RectangleShape player1(sf::Vector2f(95, 16));
	player1.setFillColor(sf::Color::White);
	player1.setPosition(700, 7);
	player1.setOutlineColor(sf::Color::Black);
	sf::CircleShape CirclePlayer11(10.f);
	CirclePlayer11.setFillColor(sf::Color::Red);
	CirclePlayer11.setPosition(700.f, 5.f);
	sf::CircleShape CirclePlayer12(10.f);
	CirclePlayer12.setFillColor(sf::Color::Red);
	CirclePlayer12.setPosition(700.f, 5.f);
	window.setFramerateLimit(600);
	sf::RectangleShape player2(sf::Vector2f(95, 16));
	player2.setFillColor(sf::Color::White);
	player2.setPosition(6, 477);
	player2.setOutlineColor(sf::Color::Black);

	sf::CircleShape CirclePlayer21(10.f);
	CirclePlayer21.setFillColor(sf::Color::Red);
	CirclePlayer21.setPosition(5.f, 475.f);
	sf::CircleShape CirclePlayer22(10.f);
	CirclePlayer22.setFillColor(sf::Color::Red);
	CirclePlayer22.setPosition(5.f, 475.f);


	sf::RectangleShape grayScreen(sf::Vector2f(130, 500));
	grayScreen.setFillColor(sf::Color(150, 150, 150));
	grayScreen.setPosition(800, 0);

	std::vector<Ball> balls;
	balls.push_back(Ball(5.f, sf::Vector2f(0.5f, -0.5f)));

	int player1Lives = 3;
	int player2Lives = 3;

	sf::Texture heartTexture;
	if (!heartTexture.loadFromFile("heart.png")) {
		return EXIT_FAILURE;
	}

	sf::Sprite player1Heart;
	player1Heart.setTexture(heartTexture);
	player1Heart.setPosition(810, 50);
	player1Heart.setScale(0.5f, 0.5f);

	sf::Sprite player2Heart;
	player2Heart.setTexture(heartTexture);
	player2Heart.setPosition(810, 100);
	player2Heart.setScale(0.5f, 0.5f);

	sf::Font font;
	if (!font.loadFromFile("Platypi-Italic-VariableFont_wght.ttf")) {
		return EXIT_FAILURE;
	}

	// Timer setup for adding new balls
	sf::Clock gameClock;
	sf::Text timerText;
	timerText.setFont(font);
	timerText.setCharacterSize(20);
	timerText.setFillColor(sf::Color::White);
	timerText.setPosition(810, 200);

	// New timer setup
	sf::Clock newTimerClock;
	sf::Text newTimerText;
	newTimerText.setFont(font);
	newTimerText.setCharacterSize(20);
	newTimerText.setFillColor(sf::Color::White);
	newTimerText.setPosition(810, 250); // Position it below the first timer

	sf::Texture blindZoneTexture;
	if (!blindZoneTexture.loadFromFile("image1.png")) {
		return EXIT_FAILURE;
	}

	BlindZone blindZone(blindZoneTexture);
	int t = 0;
	bool gameOver = false;
	while (window.isOpen() && !gameOver)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		if (mousePos.x > 5 && mousePos.x + player2.getSize().x < 800) {
			player2.setPosition(static_cast<float>(mousePos.x), player2.getPosition().y);
			CirclePlayer21.setPosition(static_cast<float>(mousePos.x) - 9, CirclePlayer21.getPosition().y);
			CirclePlayer22.setPosition(static_cast<float>(mousePos.x) + 85, CirclePlayer22.getPosition().y);
		}

		float minDistanceX = 999999.f;
		sf::Vector2f closestBallPosition;

		for (const auto& ball : balls) {
			// ќбчислюЇмо в≥дстань по ос≥ x в≥д м'€ча до верхнього краю екрану
			float distanceToTop = std::abs(ball.getPosition().y - player1.getPosition().y);
			if (distanceToTop < minDistanceX) {
				minDistanceX = distanceToTop;
				closestBallPosition = ball.getPosition();
			}
		}

		
			float newPlayer1X = lerp(player1.getPosition().x, closestBallPosition.x - 30, speed * 0.01f); // 0.01f - це невеликий крок дл€ плавного перем≥щенн€
			float newCirclePlayer11X = lerp(CirclePlayer11.getPosition().x, closestBallPosition.x - 40, speed * 0.01f);
			float newCirclePlayer12X = lerp(CirclePlayer12.getPosition().x, closestBallPosition.x + 55, speed * 0.01f);
			if(newCirclePlayer11X > 0 && newCirclePlayer12X < 785){
			player1.setPosition(newPlayer1X, player1.getPosition().y);
			CirclePlayer11.setPosition(newCirclePlayer11X, CirclePlayer11.getPosition().y);
			CirclePlayer12.setPosition(newCirclePlayer12X, CirclePlayer12.getPosition().y);
			}
		for (auto& ball : balls) {
			// Check collision with CirclePlayer21
			if (checkCollisionWithRedCircles(ball, CirclePlayer21) && ball.canCollideWith(CirclePlayer21)) {
				sf::Vector2f ballCenter = ball.getPosition() + sf::Vector2f(ball.getRadius(), ball.getRadius());
				sf::Vector2f circleCenter = CirclePlayer21.getPosition() + sf::Vector2f(CirclePlayer21.getRadius(), CirclePlayer21.getRadius());

				sf::Vector2f collisionVector = ballCenter - circleCenter;
				float distance = sqrt(collisionVector.x * collisionVector.x + collisionVector.y * collisionVector.y);
				sf::Vector2f normal = collisionVector / distance;
				sf::Vector2f newVelocity = ball.getVelocity() - 2.f * (ball.getVelocity().x * normal.x + ball.getVelocity().y * normal.y) * normal;
				ball.setVelocity(newVelocity);

				// Register the collision to start cooldown
				ball.registerCollisionWith(CirclePlayer21);
			}

			// Check collision with CirclePlayer22
			if (checkCollisionWithRedCircles(ball, CirclePlayer22) && ball.canCollideWith(CirclePlayer22)) {
				sf::Vector2f ballCenter = ball.getPosition() + sf::Vector2f(ball.getRadius(), ball.getRadius());
				sf::Vector2f circleCenter = CirclePlayer22.getPosition() + sf::Vector2f(CirclePlayer22.getRadius(), CirclePlayer22.getRadius());

				sf::Vector2f collisionVector = ballCenter - circleCenter;
				float distance = sqrt(collisionVector.x * collisionVector.x + collisionVector.y * collisionVector.y);
				sf::Vector2f normal = collisionVector / distance;
				sf::Vector2f newVelocity = ball.getVelocity() - 2.f * (ball.getVelocity().x * normal.x + ball.getVelocity().y * normal.y) * normal;
				ball.setVelocity(newVelocity);

				// Register the collision to start cooldown
				ball.registerCollisionWith(CirclePlayer22);
			}

			if (ball.getPosition().y + ball.getRadius() > player2.getPosition().y &&
				ball.getPosition().y - ball.getRadius() < player2.getPosition().y + player2.getSize().y &&
				ball.getPosition().x + ball.getRadius() > player2.getPosition().x &&
				ball.getPosition().x - ball.getRadius() < player2.getPosition().x + player2.getSize().x) {
				ball.setVelocity(sf::Vector2f(ball.getVelocity().x, -std::abs(ball.getVelocity().y)));
			}

			if (ball.getPosition().y - ball.getRadius() < player1.getPosition().y + player1.getSize().y &&
				ball.getPosition().y + ball.getRadius() > player1.getPosition().y &&
				ball.getPosition().x + ball.getRadius() > player1.getPosition().x &&
				ball.getPosition().x - ball.getRadius() < player1.getPosition().x + player1.getSize().x) {
				ball.setVelocity(sf::Vector2f(ball.getVelocity().x, std::abs(ball.getVelocity().y)));
			}

			if (ball.getPosition().x + ball.getRadius() > grayScreen.getPosition().x &&
				ball.getPosition().x - ball.getRadius() < grayScreen.getPosition().x + grayScreen.getSize().x &&
				ball.getPosition().y + ball.getRadius() > grayScreen.getPosition().y &&
				ball.getPosition().y - ball.getRadius() < grayScreen.getPosition().y +
				grayScreen.getSize().y) {
				ball.setVelocity(sf::Vector2f(-std::abs(ball.getVelocity().x), ball.getVelocity().y));
			}

			if (ball.getPosition().y + ball.getRadius() > window.getSize().y) {
				ball.getShape().setPosition(400.f, 250.f);
				ball.setVelocity(sf::Vector2f(0.5f, -0.5f));
				player1Lives--;
			}

			if (ball.getPosition().y - ball.getRadius() < 0) {
				ball.getShape().setPosition(400.f, 250.f);
				ball.setVelocity(sf::Vector2f(0.5f, -0.5f));
				player2Lives--;
			}

			ball.update();
		}

		if (player1Lives <= 0 || player2Lives <= 0) {
			gameOver = true;
		}

		// Update the timer text
		sf::Time elapsedTime = gameClock.getElapsedTime();
		int remainingTime = 20 - static_cast<int>(elapsedTime.asSeconds());
		timerText.setString("New ball: " + to_string(remainingTime));

		// Check if time is up to add a new ball
		if (remainingTime <= 0) {
			balls.push_back(Ball(5.f, sf::Vector2f(0.5f, -0.5f)));
			gameClock.restart(); // Reset the timer
		}

		// Update the new timer text
		sf::Time newElapsedTime = newTimerClock.getElapsedTime();
		int newRemainingTime =  30 + t - static_cast<int>(newElapsedTime.asSeconds());
		newTimerText.setString("BlindZone:\n" + to_string(newRemainingTime));
		
		// Check if time is up for the second timer
		if (newRemainingTime <= 0) {
			// Perform any actions needed when the timer reaches 0
			newTimerClock.restart(); // Reset the timer
			t = 10;

		}
		blindZone.update();
		window.clear();
		sf::Text endText;
		endText.setFont(font);
		if (player1Lives <= 0)
			endText.setString("Game Over");
		else if (player2Lives <= 0)
			endText.setString("You Win");
		endText.setCharacterSize(50);
		endText.setFillColor(sf::Color::White);
		endText.setPosition(300, 350);
		window.draw(endText);
		window.draw(CirclePlayer11);
		window.draw(CirclePlayer12);
		window.draw(CirclePlayer21);
		window.draw(CirclePlayer22);
		window.draw(player1);
		window.draw(player2);
		window.draw(grayScreen);
		window.draw(timerText);
		window.draw(newTimerText); // Draw the new timer
		for (int i = 0; i < player1Lives; ++i) {
			player1Heart.setPosition(810 + 40 * i, 50);
			window.draw(player1Heart);
		}

		for (int i = 0; i < player2Lives; ++i) {
			player2Heart.setPosition(810 + 40 * i, 100);
			window.draw(player2Heart);
		}

		for (const auto& ball : balls) {
			ball.draw(window);
		}
		blindZone.draw(window);
		window.display();
	}

	return 0;
}

