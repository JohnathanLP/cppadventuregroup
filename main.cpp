#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define WIDE 800
#define HIGH 640

#define TILESIZE 32

bool testColl(sf::Sprite a, sf::Sprite b)
{
  return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

bool testWallColl(sf::Sprite a, std::vector<sf::Sprite> walls)
{
  for(int i=0; i<walls.size(); i++)
  {
    if(a.getGlobalBounds().intersects(walls[i].getGlobalBounds()))
    {
      return true;
    }
  }
  return false;
}

int main()
{
  sf::RenderWindow window(sf::VideoMode(WIDE,HIGH),"Adventure!");

  int tileW = WIDE/TILESIZE;
  int tileH = HIGH/TILESIZE;

  std::vector<std::vector<bool> > map;

  for(int i=0; i<tileH; i++)
  {
    std::vector<bool> temp;
    for(int j=0; j<tileW; j++)
    {
      temp.push_back(false);
    }
    map.push_back(temp);
  }

  map[12][3] = true;
  map[4][6] = true;

  for(int i=0; i<map.size(); i++)
  {
    for(int j=0; j<map[i].size(); j++)
    {
      if(map[i][j] == true)
      {
        std::cout << '0';
      }
      else
      {
        std::cout << ' ';
      }
    }
    std::cout << std::endl;
  }

  sf::Texture enemy_texture;
  if(!enemy_texture.loadFromFile("assets/greenblob.png"))
  {
    std::cout << "Error loading enemy texture!" << std::endl;
  }
  sf::Sprite enemy;
  enemy.setTexture(enemy_texture);
  enemy.setScale(sf::Vector2f(2,2));

  sf::Font font;
  if(!font.loadFromFile("assets/ARCADECLASSIC.TTF"))
  {
    std::cout << "Error loading font!" << std::endl;
  }
  sf::Text livesDisp;
  livesDisp.setFont(font);
  livesDisp.setCharacterSize(24);
  livesDisp.setColor(sf::Color::White);

  sf::Texture hero_texture;
  if(!hero_texture.loadFromFile("assets/hero.png"))
  {
    std::cout << "Error loading hero texture!" << std::endl;
  }

  sf::Sprite hero;
  hero.setTexture(hero_texture);
  hero.setScale(sf::Vector2f(2,2));

  sf::Texture wall_texture;
  if(!wall_texture.loadFromFile("assets/rock.png"))
  {
    std::cout << "Error loading wall texture!" << std::endl;
  }

  std::vector<sf::Sprite> wallArr;
  for(int i=0; i<map.size(); i++)
  {
    for(int j=0; j<map[i].size(); j++)
    {
      if(map[i][j] == true)
      {
        sf::Sprite temp;
        temp.setTexture(wall_texture);
        temp.setScale(sf::Vector2f(2,2));
        temp.setPosition(j*TILESIZE,i*TILESIZE);
        wallArr.push_back(temp);
      }
    }
  }

  sf::Texture grass_texture;
  if(!grass_texture.loadFromFile("assets/grass.png"))
  {
    std::cout << "Error loading grass texture!" << std::endl;
  }

  sf::Sprite grass;
  grass.setTexture(grass_texture);
  grass.setScale(sf::Vector2f(2,2));

  int enemyDirection = 0;

  int heroSpeed = 1;

  int lives = 10;
  bool takingDamage = false;

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
      if(testWallColl(hero, wallArr))
      {
        hero.move(0,heroSpeed);
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && hero.getPosition().y < HIGH-TILESIZE)
    {
      hero.move(0,heroSpeed);
      if(testWallColl(hero, wallArr))
      {
        hero.move(0,-heroSpeed);
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && hero.getPosition().x < WIDE-TILESIZE)
    {
      hero.move(heroSpeed,0);
      if(testWallColl(hero, wallArr))
      {
        hero.move(-heroSpeed,0);
      }
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && hero.getPosition().x > 0)
    {
      hero.move(-heroSpeed,0);
      if(testWallColl(hero, wallArr))
      {
        hero.move(heroSpeed,0);
      }
    }

    if(hero.getGlobalBounds().intersects(enemy.getGlobalBounds()))
    {
      if(takingDamage == false)
      {
        lives--;
        takingDamage = true;
      }
    }
    else
    {
      takingDamage = false;
    }

    livesDisp.setString(std::to_string(lives));

    window.clear(sf::Color::Blue);

    //update stuff - This is Aaron
    if(enemyDirection == 0)
    {
      enemy.move(0.5,0);
    }
    else
    {
      enemy.move(-0.5,0);
    }
    if(enemy.getPosition().x > WIDE-TILESIZE)
    {
      enemyDirection = 1;
    }
    if(enemy.getPosition().x < 0)
    {
      enemyDirection = 0;
    }

    //render - This is Max
    for(int i=0; i<tileH ; i++)
    {
      for(int j=0; j<tileW ; j++)
      {
        grass.setPosition(j*TILESIZE, i*TILESIZE);
        window.draw(grass);
      }
    }

    window.draw(enemy);
    for(int i=0; i<wallArr.size(); i++)
    {
      window.draw(wallArr[i]);
    }
    window.draw(hero);
    window.draw(livesDisp);
    window.display();
  }
}
