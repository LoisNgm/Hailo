
//  Module:             Gameplay Programming
//  Assignment1:		Hailo
//  Student Name:       Ngm Hui Min, Lois	| Jessica Tan Hwee Ching	| Pang Jin Xiang
//  Student Number:     S10158786C			| S10156827G				| S10157119D

#include "highscore.h"
//construtor
Highscore::Highscore()
{
	dxFontForScore = new TextDX();
	displaying = false;
	getScores();
}

void Highscore::draw()
{
	highscore.draw();
	dxFontForScore->setFontColor(graphicsNS::WHITE);
	getScores();
	int i = 0;
	while (highScore[i] != NULL)
	{
		dxFontForScore->print(highscoreName.at(i), 200, GAME_HEIGHT / 4 + (i * 25));
		dxFontForScore->print(to_string(highScore[i]),400, GAME_HEIGHT/4+(i*25));
		i++;
	}
}

void Highscore::initialize(Graphics *graphics)
{
	// highscore page texture
	if (!highscoreTexture.initialize(graphics, HIGHSCORE_PAGE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing highscore page texture"));

	// highscore page
	if (!highscore.initialize(graphics, 0, 0, 0, &highscoreTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing"));

	//intialize text for highscore page
	if (dxFontForScore->initialize(graphics, 30, true, false, "Calibri") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));
}

void Highscore::getScores()
{
	int lineNum = 0;
	int arrayNum = 0;
	string line="";
	ifstream myfile("Highscore.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{			
				if (lineNum % 2 == 0)
				{
					highscoreName.push_back(line);
				}
				else
				{
					highScore[arrayNum] = atoi(line.c_str());
					arrayNum++;
				}
				lineNum++;
			}
		
	}
	myfile.close();	
}
void Highscore::setScores(int p1score, int p2score)
{
	checkingscore(p1score);
	checkingscore(p2score);
	ofstream myfile;
	myfile.open("Highscore.txt");
	int iterate=0;
	while (iterate<numOfTopScore)
	{
		if (highScore[iterate] != NULL && highScore[iterate] >0 )
		{
			//myfile << highscoreName.at(iterate) << endl;
			myfile << "name"<< endl;
			myfile << highScore[iterate] << endl;
		}
		iterate++;
	}
	myfile.close();
}

void Highscore::setDisplayStatus(bool ifDisplaying)
{
	if (ifDisplaying)
	{
		displaying = true;
	}
	else
	{
		displaying = false;
	}
}
bool Highscore::getDisplayStatus()
{
	return displaying;
}

void Highscore::checkingscore(int score)
{

	bool checkHighScore = true;
	int i = 0;
	while (checkHighScore)
	{
		if (highScore[i] != NULL)
		{
			//check if it is in highscore for p1
			if (score >= highScore[i])
			{
				for (int j = numOfTopScore; j < i; j++)
				{
					if (highScore[j - 1] != NULL&& highScore[j - 1] > 0)
					{
						highScore[j - 1] = highScore[j];
					}
				}
				highScore[i] = score;
				checkHighScore = false;
			}
		}
		else
		{
			highScore[i] = score;
			checkHighScore = false;
		}
		if (i > sizeof(highScore))
		{
			checkHighScore = false;
		}
		i++;
	}
}

