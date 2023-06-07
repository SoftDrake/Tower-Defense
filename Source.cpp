#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <chrono>

const int fieldheight = 15;
const int fieldwidth = 15;
const int splitsize = 62;

const int towerheight = 68;
const int towerwidth = 48;
const int maxtower = 15;
int countertower = 0;
int towerX;
int towerY;

sf::Vector2i mouseposition;
int money = 1000;
int scorePlayer = 0;
int scoreEnemy = 0;

int NumberOfEnemies = 5;
int counterOfEnemies = 0;

// sf::Texture g;
// sf::Texture tTower;
// sf::Texture tEnemy;
sf::Texture tProjectile;

sf::Font font;
sf::Text moneytext;
sf::Text numbermoney;

// A - трава, B - дорога, T - башня
sf::String TileMap[fieldheight]{
"AAAAAAAAAAAABA",
"ABBBBBBBBBBBBA",
"ABAAAAAAAAAAAA",
"ABBBBBBBBBBBBA",
"AAAAAAAAAAAABA",
"ABBBBBBBBBBBBA",
"ABAAAAAAAAAAAA",
"ABBBBBBBBBBBBA",
"AAAAAAAAAAAABA",
"ABBBBBBBBBBBBA",
"ABAAAAAAAAAAAA",
"ABBBBBBBBBBBBA",
"AAAAAAAAAAAABA",
"ABBBBBBBBBBBBA",
"ABAAAAAAAAAAAA",
};

class Projectile {
public:
    Projectile(){}

    void set(int unittype, float startPosX, float startPosY){
        sprite.setTexture(tProjectile);
        towerPositionX = startPosX;
        towerPositionY = startPosY;
        border = sf::FloatRect(startPosX * splitsize, startPosY * splitsize, splitsize, splitsize);
        switch (unittype)
        {
        case 0:
            sprite.setTextureRect(sf::IntRect(57, 135, 32, 10));
            break;

        default:
            break;
        }
    }

    void flightСalculation(int enemyBorderLeft, int enemyBorderTop) {
        
        int tempX = towerPositionX * splitsize - (splitsize / 2);
        int tempY = towerPositionY * splitsize - (splitsize / 2);

        std::cout << "EnemyY " << enemyBorderTop << "\t" << "EnemyX " << enemyBorderLeft << std::endl;

        float radians = atan((tempY - enemyBorderTop) / (tempX - enemyBorderLeft));
        float degrees = (180 / 3.14159265) * radians;

        std::cout << "degrees " << fabs(degrees) << std::endl;

        if (tempY > enemyBorderTop && tempX > enemyBorderLeft) { std::cout << "Enemy: Up + Left" << std::endl; speedX = speed * (fabs(degrees) * 100 / 90) / 100; speedY = speedX - speed; speedX *= - 1; }
        else if (tempY > enemyBorderTop && tempX < enemyBorderLeft) { std::cout << "Enemy: Up + Right" << std::endl; speedX = speed * (fabs(degrees) * 100 / 90) / 100; speedY = speedX - speed;}
        else if (tempY < enemyBorderTop && tempX > enemyBorderLeft) { std::cout << "Enemy: Down + Left" << std::endl; speedX = speed * (fabs(degrees) * 100 / 90) / 100; speedY = speed - speedX; speedX *= - 1;}
        else if (tempY < enemyBorderTop && tempX < enemyBorderLeft) { std::cout << "Enemy: Down + Right " << std::endl; speedX = speed * (fabs(degrees) * 100 / 90) / 100; speedY = speed - speedX;}

        // speedX = speed * (degrees * 100 / 90) / 100; speedY = speed - speedX;
        // std::cout << "EnemyY " << enemyBorderTop << "\t" << "EnemyX " << enemyBorderLeft << std::endl;
        std::cout << "speedY " << speedY << "\t" << "speedX " << speedX << std::endl << std::endl;
    }

    void endOfFlight(){
        float speedX = 0;
        float speedY = 0;
        border.left = towerPositionX * splitsize;
        border.top = towerPositionY * splitsize;
    }

    void update(float time) { 
        border.left += speedX; // time
        border.top += speedY; // time
        sprite.setPosition(border.left, border.top);
    }
    
    sf::Sprite GetSprite() { return sprite; }

private:
    sf::Sprite sprite;
    sf::FloatRect border;
    
    float speed = 0.35;
    float speedX = 0;
    float speedY = 0;
    int towerPositionX = 0;
    int towerPositionY = 0;
};

class Enemy {

public:
    Enemy (){}

    void set(int unittype, sf::Texture& image) {
        sprite.setTexture(image);
        border = sf::FloatRect(12 * splitsize, 0, splitsize, splitsize);
        switch (unittype)
        {
        
        case 0:
            sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            break;
        
        default:
            break;
        }
    }

    void update(float time) {
        
        int tempY = border.top / splitsize;
        int tempX = border.left / splitsize;

        if (border.top >= 0 && border.top < 854){
        
            if (rotate == 0 && TileMap[tempY + 1][tempX] == 'A') { speedY = 0; speedX = -0.05; rotate = 1; }
            else if (rotate == 1 && TileMap[tempY][tempX - 1] == 'A') { speedX = 0; speedY = 0.05; rotate = 2; }
            else if (rotate == 2 && TileMap[tempY + 1][tempX] == 'A') { speedY = 0; speedX = 0.05; rotate = 3; }
            else if (rotate == 3 && TileMap[tempY][tempX + 1] == 'A') { speedX = 0; speedY = 0.05; rotate = 0; }
        }
        else { isDead = true; scoreEnemy++; }

        border.left += speedX * time;
        border.top += speedY * time;

        sprite.setPosition(border.left, border.top);
    }

    void Dead (){ bool isDead = true; }
    bool checkDead() { return isDead; }
    sf::Sprite GetSprite() { return sprite; }
    int GetBorderLeft() { return border.left; }
    int GetBorderTop() { return border.top; }

    void check(){ 
        std::cout << "isDead: " << isDead << std::endl;
        std::cout << "X: " << border.left << std::endl;
        std::cout << "Y: " << border.top << std::endl;
    }

private:
    float speedX = 0;
    float speedY = 0.05;
    sf::FloatRect border;
    sf::Sprite sprite;
    int rotate = 0; // 0 - иду вниз хочу влево. 1 - иду влево. 2 - иду вниз, хочу вправо, 3 - иду вправо.
    float currentFrame = 2; // пока нет анимации, не использую
    float hp = 100;
    bool isDead = false;
};

class Tower {

public:
    Tower(){}
    
    void set(sf::Texture& image, int unittype, int posX, int posY){
        sprite.setTexture(image);
        switch (unittype)
        {
        case 0:
            border = sf::FloatRect(posX * splitsize, posY * splitsize, splitsize, splitsize);
            sprite.setTextureRect(sf::IntRect(splitsize + 22, 104 - splitsize, towerwidth, towerheight));
            std::cout << "StartX " << "\t" << border.left << "\t" << "StartY " << "\t" << border.top << std::endl << std::endl;
            break;

        default:
            break;
        }
    }

    bool attack(bool checkDead, int enemyBorderLeft, int enemyBorderTop) {

        if (!reload || castingSpeed <= attackClock.getElapsedTime().asSeconds()) {
            reload = false;
            if (!checkDead) {
                float distance = sqrt(pow((border.left - enemyBorderLeft), 2) + pow((border.top - enemyBorderTop), 2));
                if (range >= distance) {
                    reload = true;
                    attackClock.restart();
                    return true;
                }
                else { return false; }
            }
            else { return false; }
        }
        else { return false; }
        // float d = sqrt(pow((Xa - Xb), 2) + pow((Ya - Yb), 2)); длина между точками на плоскости
    }

    void update(float time) {sprite.setPosition(border.left, border.top);}
    void Dead() { bool isDead = true; }
    bool checkDead() { return isDead; }
    sf::Sprite GetSprite() { return sprite; }
    float getTime() { return attackClock.getElapsedTime().asSeconds(); }

private:
    sf::FloatRect border;
    sf::Sprite sprite;
    sf::Clock attackClock;
    Projectile projectile;

    float hp = 100;
    float range = 150;
    float castingSpeed = 2;
    bool isDead = false;
    int price = 100;
    bool reload = false;
    int towerPositionX = 0;
    int towerPositionY = 0;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(fieldwidth * splitsize, fieldheight * splitsize), "Tower Defense");
    
    sf::Clock clock;
    sf::Clock spawnclock;

    sf::Texture g;
    g.loadFromFile("F:/Устан/Для проектов/ground.png");
    sf::Sprite ground(g);

    sf::Texture tTower;
    tTower.loadFromFile("F:/Устан/Для проектов/Tiles1.jpg");
    
    sf::Texture tEnemy;
    tEnemy.loadFromFile("F:/Устан/Для проектов/Super Mario/Game/Paint/enemy.png");
    
    tProjectile.loadFromFile("F:/Устан/Для проектов/projectile.jpg");

    sf::Texture win;
    win.loadFromFile("F:/Устан/Для проектов/Пакман/Paint/youwin.png");
    sf::Sprite youwin(win);
    youwin.setPosition(200, 410);

    sf::Texture lose;
    lose.loadFromFile("F:/Устан/Для проектов/Пакман/Paint/youlose.png");
    sf::Sprite youlose(lose);
    youlose.setPosition(200, 410);

    std::string m = std::to_string(money);
    if (font.loadFromFile("arial.ttf")) { 
        moneytext.setFont(font);
        moneytext.setPosition(16, 12);
        moneytext.setString("Money = ");
        moneytext.setCharacterSize(32);
        numbermoney.setFont(font);
        numbermoney.setPosition(150, 16);
        numbermoney.setString(m);
        numbermoney.setCharacterSize(28);
    }

    Enemy* pArrayOfEnemies = new Enemy[NumberOfEnemies]; // delete[]pArrayOfEnemies;
    Tower* pArrayOfTowers = new Tower[maxtower]; // delete[]pArrayOfTowers;
    Projectile* pArrayOfProjectile = new Projectile[maxtower]; // delete[]pArrayOfProjectile;

    // std::vector<Enemy> ArrayOfEnemies;
    // ArrayOfEnemies[3].set(e, 0);
    // ArrayOfEnemies.erase(ArrayOfEnemies.begin() + 2);
    // std::vector<Enemy>::iterator it = ArrayOfEnemies.begin();

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds(); // запись времени в переменную
        if (time >= 3000) { clock.restart(); }
        time = time / 800;

        float spawntime = spawnclock.getElapsedTime().asSeconds();
        if (spawntime >= 2) { 
            spawnclock.restart();

            if (counterOfEnemies <= NumberOfEnemies) {
                pArrayOfEnemies[counterOfEnemies].set(0, tEnemy);
                
                if (counterOfEnemies < NumberOfEnemies) { counterOfEnemies++; }
            }
            
        }
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && scoreEnemy <= 3 && scorePlayer <= NumberOfEnemies - scoreEnemy) {

                mouseposition = sf::Mouse::getPosition(window);
                int positionY = mouseposition.y / splitsize;
                int positionX = mouseposition.x / splitsize;

                if (countertower <= maxtower && TileMap[positionY][positionX] == 'A' && positionX < fieldwidth - 2) {
                    pArrayOfTowers[countertower].set(tTower, 0, positionX, positionY);
                    money -= 50;
                    TileMap[positionY][positionX] = 'T';
                    pArrayOfProjectile[countertower].set(0, positionX, positionY);
                    if (countertower < maxtower) { countertower++; }
                }

            }
        }

        window.clear(sf::Color::Black);
        
        /* Отрисовка карты */
        for (int i = 0; i < fieldheight; i++)
        {
            for (int j = 0; j < fieldwidth; j++)
            {
                if (TileMap[i][j] == 'A') {
                    ground.setTextureRect(sf::IntRect(splitsize + 88, 0, splitsize, splitsize));
                }
                if (TileMap[i][j] == 'B') {
                    ground.setTextureRect(sf::IntRect(splitsize + 88, splitsize + 12, splitsize, splitsize));
                }
                if (TileMap[i][i] == 'T') { continue; }


                ground.setPosition(j * splitsize, i * splitsize);
                window.draw(ground);
                
            }
        }
        
        /* Обновление башен и отрисовка */
        for (int i = 0; i < countertower; i++)
        {
            pArrayOfTowers[i].update(time);
            window.draw(pArrayOfTowers[i].GetSprite());
        }
        
        /* Башни атакуют */
        for (int i = 0; i < countertower; i++)
        {
            for (int j = 0; j < counterOfEnemies; j++)
            {
                if (!pArrayOfEnemies[j].checkDead()) {
                    if (scoreEnemy <= 3 && scorePlayer <= NumberOfEnemies - scoreEnemy) {
                        if (pArrayOfTowers[i].attack(pArrayOfEnemies[j].checkDead(), pArrayOfEnemies[j].GetBorderLeft(), pArrayOfEnemies[j].GetBorderTop())) {
                            pArrayOfProjectile[i].flightСalculation(pArrayOfEnemies[j].GetBorderLeft(), pArrayOfEnemies[j].GetBorderTop());
                            j = counterOfEnemies;
                            std::cout << "Attack enemy " << j << std::endl << std::endl;
                        }
                    }
                }
            }
        }
        
        /* Обновление и отрисовка снарядов */
        for (int i = 0; i < countertower; i++)
        {
            pArrayOfProjectile[i].update(time);
            window.draw(pArrayOfProjectile[i].GetSprite());
            if (0.1 >= pArrayOfTowers[i].getTime()) { pArrayOfProjectile[i].endOfFlight(); }
        }

        /* Обновление противников и отрисовка */
        for (int i = 0; i < counterOfEnemies; i++)
        {
            if (!pArrayOfEnemies[i].checkDead()) {
                if (scoreEnemy <= 3 && scorePlayer <= NumberOfEnemies - scoreEnemy) {
                    pArrayOfEnemies[i].update(time);
                }
                window.draw(pArrayOfEnemies[i].GetSprite());
            }
        }

        /* Отрисовка текстов*/
        window.draw(moneytext);
        std::string m = std::to_string(money);
        numbermoney.setString(m);
        window.draw(numbermoney);
        
        /*Условия победы или поражения*/
        if (scoreEnemy >= 3) { window.draw(youlose);}
        if (scorePlayer >= NumberOfEnemies - scoreEnemy){ window.draw(youwin); }

        
        window.display();
    }
    
    // delete[]pArrayOfTowers;
    delete[]pArrayOfEnemies;
    return 0;
}