#include <SFML/Graphics.hpp>
#include<vector>
#include<iostream>
using namespace std;
class Bricks;
class paddle{
private:
public:
    sf::RectangleShape rect;
    float speed=6;
    int positionX=660;
    int positionY=695;

    paddle(){
        sf::Vector2f rectanglePosition(positionX,positionY);
        rect.setPosition(rectanglePosition);
        rect.setSize(sf::Vector2f(75,25));
    }
int paddle_move() {
    // Move the paddle left if the left arrow is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (rect.getPosition().x > 0) {  // Prevent moving left off the screen
            rect.move(-speed, 0.f);
        }
    }

    // Move the paddle right if the right arrow is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (rect.getPosition().x < 1280 - rect.getSize().x) {  // Prevent moving right off the screen
            rect.move(speed, 0.f);
        }
    }

    // Handle paddle wraparound logic:
    // If the paddle moves off the left side, reset it to the right side
    if (rect.getPosition().x + rect.getSize().x < 0) {
        rect.setPosition(1280 - rect.getSize().x, 695);  // Wrap to the right side
    }

    // If the paddle moves off the right side, reset it to the left side
    if (rect.getPosition().x > 1280) {
        rect.setPosition(0, 695);  // Wrap to the left side
    }

    return 0;
}


};


class Bricks{
    public:
    float positionX;
    float positionY;
    int counter=0;
    sf::RectangleShape block;
    Bricks(float posx,float posy){
        positionX=posx;
        positionY=posy;
        sf::Vector2f Block_position(positionX,positionY);
        block.setPosition(Block_position);
        block.setSize((sf::Vector2f(100,40)));
        counter++;
    }
    void draw(sf::RenderWindow& window) {
        window.draw(block);
    }



};

class ball_class{
    private:
    public:
        sf::CircleShape ball;
        float xVelocity=3;
        float yVelocity=3;

        ball_class(){
            ball.setPosition(640.f, 360.f);
            ball.setRadius(25.f);
        }
        int ball_move(paddle& pad){
            sf::Vector2f circlePosition=ball.getPosition();
            circlePosition.x+=xVelocity;
            circlePosition.y+=yVelocity;
            ball.setPosition(circlePosition);
            if(circlePosition.x<0 || circlePosition.x>1280-50)xVelocity*=-1;
            if(circlePosition.y<0 || circlePosition.y>720-50)yVelocity*=-1;

            if (ball.getGlobalBounds().intersects(pad.rect.getGlobalBounds())) {
            // Bounce the ball off the paddle by reversing the vertical direction
            yVelocity *= -1;

            // Optional: Change the ball's horizontal direction based on the paddle's position
            // This will make the bounce angle more dynamic depending on where it hits the paddle
            float paddleCenter = pad.rect.getPosition().x + pad.rect.getSize().x / 2;
            float ballCenter = ball.getPosition().x + ball.getRadius();
            xVelocity = (ballCenter - paddleCenter) / 10;  // Modify this factor to adjust the bounce effect
        }

        // Update the ball position
        ball.setPosition(circlePosition);

            return 0;
        }
    bool brick_collision(Bricks& brick) {
        if (ball.getGlobalBounds().intersects(brick.block.getGlobalBounds())) {
            sf::Vector2f ballPosition = ball.getPosition();
            sf::Vector2f brickPosition = brick.block.getPosition();
            sf::Vector2f brickSize = brick.block.getSize();

            if (ballPosition.x + ball.getRadius() <= brickPosition.x || ballPosition.x >= brickPosition.x + brickSize.x) {
                xVelocity = -xVelocity; // Reverse horizontal direction
            } else {
                yVelocity = -yVelocity; // Reverse vertical direction
            }

            return true; // Collision detected
        }
        return false; // No collision
    }
    bool game_over() {
        return ball.getPosition().y + ball.getRadius() * 2 >= 720; // Ball's bottom edge crosses below screen
    }

};

int main() {
    ball_class a;
    paddle pad;
    sf::RenderWindow window(sf::VideoMode(1280,720),"MY program");
    window.setFramerateLimit(60);

     vector<Bricks> bricks;
    int row=3;
    int column=12;

    for (int i = 0; i <=row; ++i) {
        for (int j = 0; j <=column; ++j){
            bricks.push_back(Bricks(j * 106.8f, i*50.f+50));  // Adjust spacing as needed
        }
    }
    bool gameOver = false;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed)window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close();
        }
    if (!gameOver) {
        window.clear();

    a.ball_move(pad);
    pad.paddle_move();
    for (size_t i = 0; i < bricks.size();) {
            if (a.brick_collision(bricks[i])) {
                bricks.erase(bricks.begin() + i); // Remove brick if hit
            } else {
                ++i;
            }
        }

        window.clear();
        for (auto& brick : bricks) {
            brick.draw(window);
        }
        if(a.game_over()==true){
            gameOver=true;
        }
        window.draw(a.ball);
        window.draw(pad.rect);
        window.display();
    }
    else {
            // Game Over: Display Game Over text
            sf::Font font;
    if (!font.loadFromFile("C:\\Users\\haseebay\\Desktop\\Arkanoid\\Arkanoid\\Sansation_Bold.ttf")) {  // Use your own font file
        std::cerr << "Error: Unable to load font!" << std::endl;
        return -1;
    }

    // Create a text object
    sf::Text text;
    text.setFont(font);               // Set the font
    text.setString("Game Over!");     // Set the text string
    text.setCharacterSize(50);        // Set the character size
    text.setFillColor(sf::Color::Red); // Set the text color
    text.setPosition(500.f, 300.f);

            window.clear();
            window.draw(text);
            window.display();
        }
    }

    return 0;
}
