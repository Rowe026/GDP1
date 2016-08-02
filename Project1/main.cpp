#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cButton.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 1024;
    const int windowHeight = 768;
    const int windowBPP = 16;



    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 5; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };

	//theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fonts
	// Load Fonts
	LPCSTR gameFonts[2] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);

	// Create vector array of textures
	//List of background textures
	vector<cTexture*> textureBkgList;
	LPCSTR bkgTexturesToUse[] = { "Images/menuBkGrd.png", "Images/mainBkGrd.png" };
	for (int tCount = 0; tCount < 2; tCount++)
	{
		textureBkgList.push_back(new cTexture());
		textureBkgList[tCount]->createTexture(bkgTexturesToUse[tCount]);
	}

	cTexture transSprite;
	transSprite.createTexture("Images/blank.png");

	//Menu screens texture
	cBkGround spriteMenuBkgd;
	spriteMenuBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteMenuBkgd.setTexture(textureBkgList[0]->getTexture());
	spriteMenuBkgd.setTextureDimensions(textureBkgList[0]->getTWidth(), textureBkgList[0]->getTHeight());

	//Main screen texture
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgList[1]->getTexture());
	spriteBkgd.setTextureDimensions(textureBkgList[1]->getTWidth(), textureBkgList[1]->getTHeight());

	//Setting up asteriod spawn conditions
	for (int astro = 0; astro < 5; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(windowWidth / (rand() % 5 + 1), windowHeight / (rand() % 5 + 1)));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((rand() % 4 + 1), (rand() % 4 + 1)));
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}


	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\rocketSprite.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(512.0f, 380.0f));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));





	// Create vector array of button textures  ..............................................................................
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/playBtn.png"};
	for (int tCount = 0; tCount < 2; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	//Exit button
	// exitButton;
	//.attachInputMgr(theInputMgr);
	//exitButton.setTexture(btnTextureList[0]->getTexture());
	//exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());

	//Play button
	//cButton playButton;
	//.attachInputMgr(theInputMgr);
	//.setTexture(btnTextureList[1]->getTexture());
	//.setTextureDimensions(btnTextureList[1]->getTWidth(), btnTextureList[1]->getTHeight());

	//Strings for text display  .............................................................................................
	string outputMsg;
	string strMsg[] = { "UFO ATTACK", "Avoid the enemies while trying", "to distroy them and get points.", "Score:", "Congratulations!", "Your final score is: " };

	//Starting game state ...................................................................................................
	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;


	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Used to switch between game states
		switch (theGameState)
		{
			//First gamestate - Start menu
		case MENU:

			//Renders the start menu background
			spriteMenuBkgd.render();

			//Renders the menu buttons
			//.setSpritePos(glm::vec2(400.0f, 400.0f));
			//.setSpritePos(glm::vec2(400.0f, 475.0f));
			//.render();
			//.render();

			//Updates gamestate when play button pressed
			//theGameState = playButton.update(theGameState, PLAYING);
			//exitButton.update();

			//Text output for start menu
			outputMsg = strMsg[0];
			theFontMgr->getFont("Font1")->printText(outputMsg.c_str(), FTPoint(100, 200, 0.0f));
			outputMsg = strMsg[1];
			theFontMgr->getFont("Font2")->printText(outputMsg.c_str(), FTPoint(200, 250, 0.0f));
			outputMsg = strMsg[2];
			theFontMgr->getFont("Font2")->printText(outputMsg.c_str(), FTPoint(190, 300, 0.0f));

			//Closes window if exit button pressed
			// (exitButton.getClicked())
			//{
			//	SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			//}

			break;

			//Second gamestate - Main level
		case PLAYING:

			//Renders main level background
			spriteBkgd.render();

			//Updates rocket's position
			rocketSprite.update(elapsedTime);

			{
				vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
				while (asteroidIterator != theAsteroids.end())
				{
					//Destroys asteroids if collision occurs
					if ((*asteroidIterator)->isActive() == false)
					{
						asteroidIterator = theAsteroids.erase(asteroidIterator);
					
					}
					//Renders asteroids new position
					else
					{
						(*asteroidIterator)->update(elapsedTime);
						(*asteroidIterator)->render();
						++asteroidIterator;
					}
				}
			}

			// Ends game if all enemies are distroyed
			{
				vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
				if (asteroidIterator == theAsteroids.end())
				{
					theGameState = END;
				}
			}

			//Ends game if enemy collides with player
			for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
			{
				if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), rocketSprite.getBoundingRect()))
				{
					theGameState = END;
				}
			}

			//Renders rocket
			rocketSprite.render();

			break;

			//Third gamestate - End menu
		case END:

			//Renders End menu background
			spriteMenuBkgd.render();

			//playButton.setClicked(false);
			//exitButton.setClicked(false);

			//Renders the menu buttons
			//playButton.setSpritePos(glm::vec2(400.0f, 300.0f));
			//exitButton.setSpritePos(glm::vec2(400.0f, 375.0f));
			//playButton.render();
			//exitButton.render();

			//Updates gamestate when play button pressed
			//theGameState = playButton.update(theGameState, PLAYING);
			//exitButton.update();

			//Text output for end menu
			outputMsg = strMsg[4];
			theFontMgr->getFont("Font3")->printText(outputMsg.c_str(), FTPoint(200, 200, 0.0f));
			outputMsg = strMsg[5];
			theFontMgr->getFont("Font2")->printText(outputMsg.c_str(), FTPoint(200, 250, 0.0f));

			//Closes window if exit button pressed
			//if (exitButton.getClicked())
			//{
			//	SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			//}
			break;
		}

		//Clears buffers
		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);
	}

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
