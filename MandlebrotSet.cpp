#include <iostream>
#include <SFML/Graphics.hpp>
#include <ccomplex>

const int WIDTH = 100;
const int HEIGHT = 75;
const int MAX_ITER = 100;

sf::Color getColor(int iterations) {
    if (iterations == MAX_ITER) {
        return sf::Color::Black;
    }
    int hue = 255 * iterations / MAX_ITER;
    //set colours from blue to white
    return sf::Color(hue, hue, 255 - hue); 
}
//generate Mandelbrot set using SFML
static void generateMandelbrot(sf::Image& image) {
    double xmin = -2.5, xmax = 1.5;
    double ymin = -1.5, ymax = 1.5;

//OpenMP for parallel computation
//specifies to use dynamic scheduling for threads
#pragma omp parallel for schedule(dynamic)
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            //map pixel to complex plane
            std::complex<double> c(
                xmin + (xmax - xmin) * x / WIDTH,
                ymin + (ymax - ymin) * y / HEIGHT
            );
            std::complex<double> z = 0;
            int iterations = 0;

            //mandelbrot calculation
            while (std::abs(z) < 2.0 && iterations < MAX_ITER) {
                z = z * z + c;
                iterations++;
            }
            //set the pixel color based on the number of iterations
            sf::Color color = getColor(iterations);
            image.setPixel(x, y, color);
        }
    }
}

//generate Mandelbrot set using symbols and display on console
static void generateSetOnConsole() {
    //defines the space for the complex numbers
    double xmin = -2.5, xmax = 1.5;
    double ymin = -1.5, ymax = 1.5;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            std::complex<double> c(
                xmin + (xmax - xmin) * x / WIDTH,
                ymin + (ymax - ymin) * y / HEIGHT
            );
            std::complex<double> z = 0;
            int iterations = 0;

            while (std::abs(z) < 2.0 && iterations < MAX_ITER) {
                z = z * z + c;
                iterations++;
            }
            if (iterations == MAX_ITER) {
                std::cout << "#";
            }
            else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

int main()
{
    //console
    generateSetOnConsole();
    
    //using SFML
    sf::Image image;
    image.create(WIDTH, HEIGHT);
    generateMandelbrot(image);

    //creates window to display it
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot Set");
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}