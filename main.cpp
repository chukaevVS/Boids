#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include <cmath>

class Bird {
    public:
        Bird(){
            x = rand() % 700 + 0;
            y = rand() % 500 + 0;
            dx = rand() % 3 + (-3);
            dy = rand() % 3 + (-3);
            accelerationVec = sf::Vector2f(dx - x, dy - y);
        
            triangle.setRadius(15);
            triangle.setPointCount(300);
            triangle.setOutlineColor(sf::Color::Blue);
            triangle.setOutlineThickness(2.f);
            triangle.setFillColor(sf::Color::Green);
            
        }

        sf::CircleShape getBird(){
            return this->triangle;
        }

        void checkBorders() {
            tempX = x + dx;
            tempY = y + dy;

            if (tempY <= 0 || tempY > 570) {
                dy *= -1;
            }

            if (tempX > 750 || tempX < 0) {
                dx *= -1;
            }
        }

        void moveBird() {
            checkBorders();
            x += dx;
            y += dy;
            triangle.setPosition(sf::Vector2f(x, y));
        }


        sf::Vector2f getPos() {
            return sf::Vector2f(x + 15, y + 15);
        }

        sf::Vector2f getAccelerationVec() {
            return sf::Vector2f(dx - x, dy - y);
        }

        sf::Vector2f getAccelerationPoint() {
            return sf::Vector2f(dx, dy);
        }

        void setAccelerationVec(sf::Vector2f newAcceleration) { // Fix Vlad
            accelerationVec += newAcceleration;
            float dist = sqrt(pow(dx - x, 2) + pow(dy - y, 2));
            accelerationVec = accelerationVec / dist;

            dx = (dx + accelerationVec.x / -64);
            dy = (dy + accelerationVec.y / -64);

        }

        void normalizeAccelertion() { // Fix Vlad
            while(dx > 3 && dy > 3) {
                dx -= 1;
                dy -= 1;
            }
        }

        sf::Vertex getDirection() {
            sf::Vertex direction[] = {sf::Vertex(sf::Vector2f(x, y)), sf::Vertex(sf::Vector2f(dx, dy))};

            return *direction;
        }

        float getDx() {
            return dx;
        }

        float getDy() {
            return dy;
        }

        void setDx(float newDx) {
            dx = newDx;
        }

        void setDy(float newDy) {
            dy = newDy;
        }

    private:
        float x, y, dx, dy, tempX, tempY;
        float radius;
        float normDx, normDy;
        sf::Vector2f accelerationVec;
        sf::CircleShape triangle;
    
};

float getDistance(Bird boid_1, Bird boid_2)
{
    float dist = sqrt(pow(boid_1.getPos().x - boid_2.getPos().x, 2) + 
                    pow(boid_1.getPos().y - boid_2.getPos().y, 2));
    return dist;
}

void ruleOfSeparation(int countOfBoids, Bird flock[])
{
    for (int i = 0; i < countOfBoids; i++) {
            std::vector<sf::Vector2f> vectors;
            int birdsInRadius = 0;
            Bird mainBird = flock[i];
            sf::Vector2f vecOfAcceleration = mainBird.getAccelerationVec();
            for (int j = 0; j < countOfBoids; j++) {
                if (mainBird.getPos() != flock[j].getPos()) {
                    float dist = getDistance(flock[i], flock[j]);
                    if (dist < 200) {
                        vectors.push_back(sf::Vector2f(flock[j].getPos().x - mainBird.getPos().x, 
                        flock[j].getPos().y - mainBird.getPos().y));
                        
                        for (sf::Vector2f vec : vectors) {
                            vecOfAcceleration += vec;
                        }
                    } else {
                        flock[i].normalizeAccelertion();
                    }
                }
            }
            if(vectors.size() > 2) {
                flock[i].setAccelerationVec(vecOfAcceleration);
            }
        }
}

int main()
{
    srand(time(0));
    int height = 600;
    int width = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "Boids");
    window.setFramerateLimit(60);

    int countOfBoids = 30; // Set count of boids

    Bird *flock = new Bird[countOfBoids];
    for (int i = 0; i < countOfBoids; i++)
    {
        flock[i] = Bird();
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ruleOfSeparation(countOfBoids, flock); // My rule of separation


        for (int i = 0; i < countOfBoids; i++)
        {
            flock[i].moveBird();
        }
        
    
        window.clear();
        for (int i = 0; i < countOfBoids; i++)
        {
            window.draw(flock[i].getBird());

            sf::VertexArray dir(sf::Lines, 2); // Func that draw the direction of each boid
            dir[0].position = sf::Vector2f(flock[i].getPos().x + flock[i].getAccelerationPoint().x * 10,
                flock[i].getPos().y + flock[i].getAccelerationPoint().y * 10);
            dir[1].position = sf::Vector2f(flock[i].getPos().x, flock[i].getPos().y);

            window.draw(dir);
        }
        window.display();
    }
    delete[] flock;
    return 0;
}
