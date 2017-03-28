#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>

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
  map[0][6] = true;

  for(int i=0; i<tileH; i++)
  {
    map[i][0] = true;
    map[i][tileW-1] = true;
  }
  for(int i=0; i<tileW; i++)
  {
    map[0][i] = true;
    map[tileH-1][i] = true;
  }

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

  sf::Texture homing_enemy_texture;
  if(!homing_enemy_texture.loadFromFile("assets/redblob.png"))
  {
    std::cout << "Error loading enemy texture!" << std::endl;
  }
  sf::Sprite homing_enemy;
  homing_enemy.setTexture(homing_enemy_texture);
  homing_enemy.setScale(sf::Vector2f(2,2));

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

  sf::SoundBuffer grunt_buffer;
  if(!grunt_buffer.loadFromFile("assets/grunt.wav"))
  {
    std::cout << "Error loading grunt sound!" << std::endl;
  }

  sf::Sound grunt;
  grunt.setBuffer(grunt_buffer);
  grunt.setPitch(.8 );

  sf::SoundBuffer music_buffer;
  if(!music_buffer.loadFromFile("assets/Lolita Compiex.wav"))
  {
    std::cout << "Error loading music file!" << std::endl;
  }

  sf::Sound music;
  music.setBuffer(music_buffer);
  music.setLoop(true);

  int enemyDirection = 0;

  int heroSpeed = 1;

  double homingX = 0.5;
  double homingY = 0.5;
  double homingDist = 0;

  int lives = 10;
  bool takingDamage = false;

  hero.setPosition(TILESIZE*2, TILESIZE*2);
  enemy.setPosition(TILESIZE*4, TILESIZE*4);
  homing_enemy.setPosition(TILESIZE*6, TILESIZE*6);

  //music.play();

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

    if(testColl(hero, enemy) || testColl(hero, homing_enemy))
    {
      if(takingDamage == false)
      {
        lives--;
        grunt.play();
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
    if(testWallColl(enemy, wallArr))
    {
      if(enemyDirection == 0)
      {
        enemyDirection = 1;
      }
      else
      {
        enemyDirection = 0;
      }
    }

    homingDist = sqrt(pow(hero.getPosition().x - homing_enemy.getPosition().x, 2) +
          pow(hero.getPosition().y - homing_enemy.getPosition().y, 2));
    homingX = (hero.getPosition().x - homing_enemy.getPosition().x)/homingDist;
    homingY = (hero.getPosition().y - homing_enemy.getPosition().y)/homingDist;
    homing_enemy.move(homingX/2, homingY/2);

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
    window.draw(homing_enemy);
    for(int i=0; i<wallArr.size(); i++)
    {
      window.draw(wallArr[i]);
    }
    window.draw(hero);
    window.draw(livesDisp);
    window.display();
  }
}
