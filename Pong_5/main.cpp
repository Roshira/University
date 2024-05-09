#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

using namespace std;

class Ball {
public:
    Ball(float radius = 10.f, const sf::Vector2f& velocity = sf::Vector2f(0.2f, -0.2f))
        : m_radius(radius), m_velocity(velocity) {
        m_circle.setRadius(m_radius);
        m_circle.setFillColor(sf::Color::White);
        m_circle.setOutlineColor(sf::Color::Black);
        m_circle.setOutlineThickness(2.f);
        m_circle.setOrigin(m_radius, m_radius);
        m_circle.setPosition(400.f, 250.f);
    }

    void update() {
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

private:
    sf::CircleShape m_circle;
    float m_radius;
    sf::Vector2f m_velocity;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(930, 500), "Pong2024 vs Bot");

    sf::RectangleShape player1(sf::Vector2f(95, 20));
    player1.setFillColor(sf::Color::White);
    player1.setPosition(700, 5);
    player1.setOutlineColor(sf::Color::Black);
    player1.setOutlineThickness(5);

    sf::RectangleShape player2(sf::Vector2f(95, 20));
    player2.setFillColor(sf::Color::White);
    player2.setPosition(5, 475);
    player2.setOutlineColor(sf::Color::Black);
    player2.setOutlineThickness(5);

    sf::RectangleShape grayScreen(sf::Vector2f(130, 500));
    grayScreen.setFillColor(sf::Color(150, 150, 150));
    grayScreen.setPosition(800, 0);

    std::vector<Ball> balls;
    balls.push_back(Ball(10.f, sf::Vector2f(0.2f, -0.2f)));

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
        if (mousePos.x > 5 && mousePos.x + player2.getSize().x < 800)
            player2.setPosition(static_cast<float>(mousePos.x), player2.getPosition().y);

        // Player 1 follows closest ball
        float minDistance = 999999.f; // Initialize with a large value
        sf::Vector2f closestBallPosition;
        for (const auto& ball : balls) {
            float distance = std::sqrt(std::pow(ball.getPosition().x - player1.getPosition().x, 2) + std::pow(ball.getPosition().y - player1.getPosition().y, 2));
            if (distance < minDistance) {
                minDistance = distance;
                closestBallPosition = ball.getPosition();
            }
        }
        player1.setPosition(closestBallPosition.x - 30, player1.getPosition().y);

        
        for (auto& ball : balls) {
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
                ball.getPosition().y - ball.getRadius() < grayScreen.getPosition().y + grayScreen.getSize().y) {
                ball.setVelocity(sf::Vector2f(-std::abs(ball.getVelocity().x), ball.getVelocity().y));
            }
            if (ball.getPosition().y + ball.getRadius() > window.getSize().y) {
                // Player 1 missed the ball, reset the ball position and direction
                ball.getShape().setPosition(400.f, 250.f);
                ball.setVelocity(sf::Vector2f(0.2f, -0.2f)); // Reset constant velocity
                player1Lives--;
            }

            if (ball.getPosition().y - ball.getRadius() < 0) {
                ball.getShape().setPosition(400.f, 250.f);
                ball.setVelocity(sf::Vector2f(0.2f, -0.2f));
                player2Lives--;
            }

            ball.update();
        }

        if (player1Lives <= 0 || player2Lives <= 0) {
            gameOver = true;
        }

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

        window.draw(player1);
        window.draw(player2);
        window.draw(grayScreen);

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
        window.display();
    }

    return 0;
}
