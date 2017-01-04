
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
	dxFontForScore->print("Position", 270, GAME_HEIGHT / 4 + (i * 25));
	dxFontForScore->print("Name", 450, GAME_HEIGHT / 4 + (i * 25));
	dxFontForScore->print("Score", 700, GAME_HEIGHT / 4 + (i * 25));
	while (highScore[i] != NULL)
	{
		if (i < 10)
		{
			dxFontForScore->print(to_string((i + 1)), 300, GAME_HEIGHT / 4 + ((i + 1) * 25));
			dxFontForScore->print(scoreName[i], 450, GAME_HEIGHT / 4 + ((i + 1) * 25));
			dxFontForScore->print(to_string(highScore[i]), 700, GAME_HEIGHT / 4 + ((i + 1) * 25));
		}
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
					scoreName[lineNum/2] = line;
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
void Highscore::setScores(int p1score, int p2score, string p1name, string p2name)
{
	checkingscore(p1score, p1name);
	checkingscore(p2score, p2name);
	ofstream myfile;
	myfile.open("Highscore.txt");
	int iterate=0;
	while (iterate<numOfTopScore)
	{
		if (highScore[iterate] != NULL && highScore[iterate] >0 )
		{		
			myfile << scoreName[iterate] << endl;
			if (iterate != 9)
				myfile << highScore[iterate] << endl;
			else
				myfile << highScore[iterate];
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

void Highscore::checkingscore(int score,string name)
{

	bool checkHighScore = true;
	int i = 0;
	string n = name;
	if (score > 0)
	{
		while (checkHighScore)
		{
			//if score is not the first one recorded(means text file not empty)
			if (highScore[i] != NULL)
			{
				if (score >= highScore[i])
				{
					for (int j = numOfTopScore; j < i; j--)
					{
						if (highScore[j - 1] != NULL&& highScore[j - 1] > 0)
						{
							scoreName[j - 1] = scoreName[j];
							highScore[j - 1] = highScore[j];
						}
					}
					scoreName[i] = n;
					highScore[i] = score;
					checkHighScore = false;
				}
			}
			else
			{
				scoreName[i] = n;
				highScore[i] = score;
				checkHighScore = false;
			}
			if (i > 10)
			{
				checkHighScore = false;
			}
			i++;
		}
	}
}

