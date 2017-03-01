#include <SFML/Graphics.hpp>
#include <iostream>

#define WIDE 800
#define HIGH 600

int main()
{
  sf::RenderWindow window(sf::VideoMode(WIDE,HIGH),"Adventure!");

  sf::CircleShape circle(20);
  circle.setFillColor(sf::Color::Yellow);
  circle.setPosition(100,100);

  sf::RectangleShape hero(sf::Vector2f(10,10));
  hero.setFillColor(sf::Color::Red);
  hero.setPosition(400,300);

  int circleDirection = 0;

  int heroSpeed = 1;

  int points = 10;

  //Game Loop
  while(window.isOpen())
  {
    //Handles Events
    sf::Event event;
    while(window.pollEvent(event))
    {
      //Close Event
      if(event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && hero.getPosition().y > 0)
    {
      hero.move(0,-heroSpeed);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && hero.getPosition().y < HIGH-hero.getSize().y)
    {
      hero.move(0,heroSpeed);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && hero.getPosition().x < WIDE-hero.getSize().x)
    {
      hero.move(heroSpeed,0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && hero.getPosition().x > 0)
    {
      hero.move(-heroSpeed,0);
    }

    if(hero.getGlobalBounds().intersects(circle.getGlobalBounds()))
    {
      std::cout << "Touching!" << std::endl;
    }
    else
    {
      std::cout << "Not Touching!" << std::endl;
    }

    window.clear(sf::Color::Blue);

    //update stuff - This is Aaron
    if(circleDirection == 0)
    {
      circle.move(0.5,0);
    }
    else
    {
      circle.move(-0.5,0);
    }
    if(circle.getPosition().x > 600)
    {
      circleDirection = 1;
    }
    if(circle.getPosition().x < 0)
    {
      circleDirection = 0;
    }

    //render - This is Max
    window.draw(hero);
    window.draw(circle);
    window.display();
  }
}
