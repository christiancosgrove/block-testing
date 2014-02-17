#include "MainGame.h"
#include "WorldGenerator.h"

MainGame::MainGame(void)
	: Window(sf::VideoMode(World::DEFAULT_WINDOW_WIDTH, World::DEFAULT_WINDOW_HEIGHT), "Block Testing - Early Version", sf::Style::Default, sf::ContextSettings(0, 0, 0, 3, 3)),
	time(std::clock()), frames(0)
{
	/*glm::vec2 vec = WorldGenerator::getVector(13771264, 4,2);
	printf("Rand vec X: %f, Y: %f\n\n", vec.x, vec.y);*/
	font.loadFromFile(std::string("C:\\Users\\Christian\\Documents\\Visual Studio 2012\\Projects\\BlockTesting\\Release\\arial.ttf"));
	std::cout << "Executable Path: " << ExePath() << std::endl;
	Window.setFramerateLimit(FRAME_LIMIT);
	initGL();
	glManager.CompileShader(std::string("C:\\Users\\Christian\\Documents\\visual studio 2012\\Projects\\BlockTesting\\Debug\\vertexShader.glsl"), "vertexShader", GL_VERTEX_SHADER);
	glManager.CompileShader(std::string("C:\\Users\\Christian\\Documents\\visual studio 2012\\Projects\\BlockTesting\\Debug\\fragmentShader.glsl"), "fragmentShader", GL_FRAGMENT_SHADER);
	glManager.InitProgram();
	world.PrepareToDraw(glManager);
}

void MainGame::initGL() 
{

	Window.setMouseCursorVisible(false);
	Window.display();
	
	glEnable(GL_TEXTURE_2D_ARRAY);
	if (glewInit()!=GLEW_OK)
	{
		printf("GLEW initialization error");
	}
	glewGetExtension("GL_VERSION_3_3");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void MainGame::Draw() 
{
	if (frames > 1e4 )
	{
		//printf("Framerate: %f\n", (float)frames * (float)CLOCKS_PER_SEC/((float)(std::clock() - time)));
		frames = 0;
		time = std::clock();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.39f, 0.58f, 0.93f, 1.0f);
	world.Draw();
	Window.display();
	frames++;
}

void MainGame::HandleWindowChanges()
{
	sf::Event event;
	while (Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			Window.close();
		}
		if (event.type == sf::Event::Resized)
		{
			glViewport(0, 0, event.size.width, event.size.height);
			world.ResizedWindow(event.size.width, event.size.height);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		Window.close();
	}
}

MainGame::~MainGame(void)
{
}

std::string MainGame::ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos) + "\\";
}