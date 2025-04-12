#include "Game.h"
#include <tchar.h>

using namespace sf;

class Pelota
{
private:
    // Sprite y su imagen para representaci�n gr�fica en el juego
    Sprite* _sprite;
    Texture* _image;
    RenderWindow* wnd;

    // Body para representaci�n f�sica en la simulaci�n Box2D
    b2Body* _body;

public:
    // Constructor que inicializa la pelota con un cuerpo f�sico y una representaci�n gr�fica
    Pelota(b2World* _world, RenderWindow* _wnd)
    {
        // Guardamos puntero a ventana para poder dibujar la pelota m�s tarde
        wnd = _wnd;
        _image = new Texture();
        // Cargamos la imagen de la pelota desde un archivo
        _image->loadFromFile("Pelota.png");
        // Cargamos el sprite con la imagen cargada
        _sprite = new Sprite(*_image);

        // Definimos las propiedades del cuerpo f�sico y lo creamos en el mundo Box2D
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody; // El cuerpo es din�mico, puede moverse y ser afectado por fuerzas
        bodyDef.position.Set(100.0f, 0.0f); // Posici�n inicial del cuerpo
        _body = _world->CreateBody(&bodyDef);

        // Definimos la forma de colisi�n del cuerpo, en este caso una caja cuadrada
        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox(20.0f, 20.0f);

        // Creamos el fixture (propiedades f�sicas) del cuerpo
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &dynamicBox; // Forma de la colisi�n
        fixtureDef.density = 10.0f; // Densidad
        fixtureDef.friction = 0.3f; // Fricci�n
        fixtureDef.restitution = 1.0f; // Restituci�n (rebote)
        _body->CreateFixture(&fixtureDef); // Asignamos el fixture al cuerpo
    }

    // M�todo para actualizar la posici�n del sprite basada en la posici�n del cuerpo f�sico
    void ActualizarPosiciones()
    {
        b2Vec2 pos = _body->GetPosition();
        _sprite->setPosition(pos.x, pos.y);
    }

    // M�todo para dibujar el sprite en la ventana de renderizado
    void Dibujar()
    {
        wnd->draw(*_sprite);
    }

    // Otros m�todos relacionados al personaje podr�an ser implementados aqu�
};

int _tmain(int argc, _TCHAR* argv[])
{
    // Creaci�n del juego y bucle principal
    Game* Juego;
    Juego = new Game(800, 600, "Ejercicio 2 - MAVII");
    Juego->Loop(); // Inicia el bucle principal del juego

    return 0;
}
