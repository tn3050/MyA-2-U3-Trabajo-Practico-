#include "Game.h"
#include "Box2DHelper.h"

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
	wnd = new RenderWindow(VideoMode(ancho, alto), titulo); // Crea una ventana con las dimensiones y título especificados
	wnd->setVisible(true); // Hace visible la ventana
	fps = 60; // Establece el límite de cuadros por segundo (FPS)
	wnd->setFramerateLimit(fps); // Aplica el límite de FPS a la ventana
	frameTime = 1.0f / fps; // Calcula el tiempo por cuadro en segundos
	SetZoom(); // Configura el "zoom" o vista de la cámara
	InitPhysics(); // Inicializa la simulación de física
}

void Game::Loop()
{
	while (wnd->isOpen()) // Bucle principal del juego que se ejecuta mientras la ventana esté abierta
	{
		wnd->clear(clearColor); // Limpia la ventana con el color de fondo
		DoEvents(); // Maneja los eventos (input del usuario)
		CheckCollitions(); // Verifica colisiones (a implementar)
		UpdatePhysics(); // Actualiza la simulación de física
		DrawGame(); // Dibuja los elementos del juego
		wnd->display(); // Muestra los cambios en la ventana
	}
}

void Game::UpdatePhysics()
{
	phyWorld->Step(frameTime, 8, 8); // Avanza la simulación de física un paso
	phyWorld->ClearForces(); // Limpia las fuerzas acumuladas
	phyWorld->DebugDraw(); // Dibuja la representación de debug de la simulación
	SimulateElasticBounce();
}


void Game::DrawGame()
{
	// Función para dibujar los elementos del juego (a implementar)
}

void Game::DoEvents()
{
	Event evt;
	while (wnd->pollEvent(evt)) // Procesa todos los eventos acumulados
	{
		switch (evt.type)
		{
		case Event::Closed: // Si se solicita cerrar la ventana
			wnd->close(); // Cierra la ventana
			break;
		case Event::MouseButtonPressed: // Si se presiona un botón del ratón
			// Crea un cuerpo dinámico con forma triangular en la posición del clic
			b2Body* body = Box2DHelper::CreateTriangularDynamicBody(phyWorld, b2Vec2(0.0f, 0.0f), 10.0f, 1.0f, 4.0f, 0.1f);
			Vector2f pos = wnd->mapPixelToCoords(Vector2i(evt.mouseButton.x, evt.mouseButton.y)); // Transforma la posición del clic a coordenadas del mundo
			body->SetTransform(b2Vec2(pos.x, pos.y), 0.0f); // Mueve el cuerpo a la posición del clic
			break;
		}
	}
}

void Game::CheckCollitions()
{
	// Verificación de colisiones (a implementar)
}

void Game::SetZoom()
{
	View camara;
	camara.setSize(100.0f, 100.0f); // Establece el tamaño de la vista
	camara.setCenter(50.0f, 50.0f); // Centra la vista en un punto del mundo
	wnd->setView(camara); // Aplica la vista a la ventana
}

void Game::SimulateElasticBounce()
{
	const float k = 310.0f; // Constante del resorte (rigidez)
	b2Vec2 pos = pelota->GetPosition();

	b2Vec2 fuerzaTotal(0.0f, 0.0f);

	// Izquierda
	if (pos.x < 10.0f) {
		float x = 10.0f - pos.x;
		fuerzaTotal.x += k * x;
	}

	// Derecha
	if (pos.x > 90.0f) {
		float x = pos.x - 90.0f;
		fuerzaTotal.x -= k * x;
	}

	// Arriba
	if (pos.y < 10.0f) {
		float y = 10.0f - pos.y;
		fuerzaTotal.y += k * y;
	}

	// Abajo
	if (pos.y > 90.0f) {
		float y = pos.y - 90.0f;
		fuerzaTotal.y -= k * y;
	}

	pelota->ApplyForceToCenter(fuerzaTotal, true);
}

void Game::InitPhysics()
{
	// Inicializa el mundo de Box2D con una gravedad hacia abajo
	phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

	debugRender = new SFMLRenderer(wnd); // Crea un renderizador de debug para SFML
	debugRender->SetFlags(UINT_MAX); // Configura el renderizador para dibujar todas las formas de debug
	phyWorld->SetDebugDraw(debugRender); // Establece el renderizador de debug para el mundo de Box2D

	// Crea cuerpos estáticos para simular el suelo y las paredes
	// También crea cuerpos dinámicos (como círculos) y los une con resortes al techo para demostrar la simulación física
	b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);

	b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);

	b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
	rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);

	// Creamos un techo
	b2Body* topWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
	topWallBody->SetTransform(b2Vec2(50.0f, 0.0f), 0.0f);



	pelota = Box2DHelper::CreateCircularDynamicBody(phyWorld, 2.0f, 1.0f, 1.0f, 0.1f); // radio, masa, restitución, fricción
	pelota->SetTransform(b2Vec2(50.0f, 50.0f), 0.0f);
	pelota->SetLinearVelocity(b2Vec2(100.0f, -15.0f));
	// Atamos la esfera mediante un resorte al techo


	// Creamos un circulo que ataremos al resorte derecho



}




Game::~Game(void)
{
	// Destructor de la clase Game (a implementar si es necesario)
}