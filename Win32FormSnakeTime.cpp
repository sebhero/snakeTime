// Snake time.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32FormSnakeTime.h"
#include <string>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

//game engine
std::unique_ptr<SnakeEngine> game(new SnakeEngine);

//pen used to paint length of snake and points
auto penPlayer = CreateSolidBrush(RGB(0,0,255));//blue

//all the images
static HBITMAP bmpSource=nullptr;
static HDC hdcSource = nullptr;
static BITMAP bitmap;

static HBITMAP bmpSourceWall=nullptr;
static HDC hdcSourceWall = nullptr;
static BITMAP bitmapWall;

static HBITMAP bmpSourceSnake=nullptr;
static HDC hdcSourceSnake = nullptr;
static BITMAP bitmapSnake;

static HBITMAP bmpSourceFruit=nullptr;
static HDC hdcSourceFruit = nullptr;
static BITMAP bitmapFruit;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//paus and unpause the game, needs the mainwindow
//true for pauseing the game
static void PausGame(bool pauseTheGame, HWND hWnd)
{
	if(pauseTheGame)
	{
		KillTimer(hWnd,1);
	}
	else
	{
		//speeds up the snake when eats a fruit
		int speed=(50*game->currentSnake->snakeTail.size());
		if(375<speed)
		{
			speed=375;
		}
		SetTimer(hWnd,1,400-speed,nullptr);
	}
}
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SNAKETIME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKETIME));


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKETIME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SNAKETIME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   	//load the game
	if(game->InitGame(0))
	{
		//can start the game
		PausGame(false, hWnd);
	}

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	string highscoreString;
	std::string tempPlayerInfoText;
	
	//size of the blocks
	int blockSize=30;
	
	int leftRect=blockSize;
	int topRect=blockSize;

	switch (message)
	{
	case WM_TIMER:
		//OutputDebugString(L"timer go\n");
			//Update the game, check for events.
		if(game->currentGameResult != GameState::LOSE)
		{
			//call game update
			if(GameState::LOSE==game->updateGame())
			{
				PausGame(true,hWnd);
				//Get the final highscore.
				string endText = "End of the game!\nyour score was: "+to_string(game->currentSnake->playerPoints);
				MessageBoxA(hWnd,endText.c_str(),"End",0);
				break;//or return;
			}

			//play ate sound.
			if(game->currentGameResult == GameState::ATE_FRUIT)
			{
				//PlaySound(L"fruitEaten.WAV",hInst, SND_ASYNC | SND_FILENAME);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2),hInst,SND_RESOURCE| SND_ASYNC);
				//speeds up the snake when eats a fruit
				int speed=(50*game->currentSnake->snakeTail.size());
				if(375<speed)
				{
					speed=375;
				}
				SetTimer(hWnd,1,400-speed,nullptr);
				
			}
			InvalidateRect(hWnd, 0, FALSE);
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_LEVEL_LEVEL1:
			//MessageBox(hWnd,L"LEvel 1",L"Level",0);
			game.reset();
			game= unique_ptr<SnakeEngine>(new SnakeEngine());
			//start first level
			game->InitGame(0);//vec starts on 0
			InvalidateRect(hWnd,0,0);
			//starts the timer,unpauses the game
			PausGame(false, hWnd);
			break;
		case ID_LEVEL_LEVEL2:
			PausGame(true, hWnd);
			game.reset();
			game= unique_ptr<SnakeEngine>(new SnakeEngine());
			//start first level
			game->InitGame(1);//vec starts on 0
			InvalidateRect(hWnd,0,0);
			//starts the timer,unpauses the game
			PausGame(false, hWnd);
			break;
		case IDM_ABOUT:
			//display about the game.
			//paus the game
			if(game->currentGameResult == GameState::CONTINUE)
				PausGame(true, hWnd);
			//show info
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			//unpause
			if(game->currentGameResult == GameState::CONTINUE)
				PausGame(false, hWnd);
			break;
		case IDM_NEWGAME:
			//starta a new game
			//deletes current game
			game.reset();
			game= unique_ptr<SnakeEngine>(new SnakeEngine());
			//start first level
			game->InitGame(0);
			InvalidateRect(hWnd,0,0);
			//starts the timer,unpauses the game
			PausGame(false, hWnd);
			break;
		case IDM_HIGHSCORE:
			//displays a highscore
			//pause
			if(game->currentGameResult == GameState::CONTINUE)
				PausGame(true, hWnd);
			//gets the string of highscores from gameengine
			//due to problems for me getting string vector work in win32 i create just one string to display.
			for(auto var: game->gameHighscore->GetStringVec())
			{
				highscoreString +=var+"\n";
			}
			//display highscore
			MessageBoxA(hWnd,highscoreString.c_str(),"Highscore",0);
			//unpause game
			if(game->currentGameResult == GameState::CONTINUE)
			{
				PausGame(false, hWnd);
			}
			InvalidateRect(hWnd,0,0);
			break;
		case IDM_EXIT:
			OutputDebugString(L"Calling Exit\n");
			game->gameHighscore->SaveHighscore();
			//game.reset();//maybe call destroy.
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//loopar igenom alla kart bitar och ritar ut dem po skarmen.
		//kallad varje gang man flyttar spelaren.
		//varje bild krymper jag ner till blocksize, som storleken jag bestämt för att använda mig utav.
			for(int y=0; y<20; y++)
			{
				for(int x=0; x<20 ;x++)
				{
					switch (game->getGamePieceAtPosition(y,x))
					{
					case 's':
						GetObject(bmpSourceSnake, sizeof(BITMAP), &bitmapSnake);
						StretchBlt(hdc, leftRect, topRect, blockSize,blockSize, hdcSourceSnake,0,0, bitmapSnake.bmWidth,bitmapSnake.bmHeight,SRCCOPY);
						break;
					case '1':
						GetObject(bmpSourceWall, sizeof(BITMAP), &bitmapWall);
						StretchBlt(hdc, leftRect, topRect, blockSize,blockSize, hdcSourceWall,0,0, bitmapWall.bmWidth,bitmapWall.bmHeight,SRCCOPY);
						break;
					case '0':
						GetObject(bmpSource, sizeof(BITMAP), &bitmap);
						StretchBlt(hdc, leftRect, topRect, blockSize,blockSize, hdcSource,0,0, bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
						break;
					case 'f':
						GetObject(bmpSourceFruit, sizeof(BITMAP), &bitmapFruit);
						StretchBlt(hdc, leftRect, topRect, blockSize,blockSize, hdcSourceFruit,0,0, bitmapFruit.bmWidth,bitmapFruit.bmHeight,SRCCOPY);
						break;
					default:
						break;
					}
					//sätter nästa position av x
					leftRect = leftRect+blockSize;
				}
				//nollställer x positionen
				leftRect=blockSize;
				//ökar y positionen, namnen kommer från att jag använde Rect() innan.
				topRect=topRect+blockSize;
			}
		//end of loops
		//Draw highscore and length
		tempPlayerInfoText = "Points = "+std::to_string(game->currentSnake->playerPoints)+"     ";//moste gora den langre for 10,100,1000...
		TextOutA(hdc, 650,30,tempPlayerInfoText.c_str(),tempPlayerInfoText.length());
		tempPlayerInfoText ="snake Size: "+std::to_string(game->currentSnake->snakeTail.size());
		TextOutA(hdc, 650,80,tempPlayerInfoText.c_str(),tempPlayerInfoText.length());
		SelectObject(hdc,penPlayer);

		EndPaint(hWnd, &ps);
		break;
		case WM_KEYDOWN:
			//keyboard handling. try to moveplayer and then repaint the winform.
			//same as the buttons.
			switch(wParam)
			{
			case VK_UP:
				game->ChangeDirection(Direction::NORTH);
				break;
			case VK_DOWN:
				game->ChangeDirection(Direction::SOUTH);
				break;
			case VK_LEFT:
				game->ChangeDirection(Direction::WEST);
				break;
			case VK_RIGHT:
				game->ChangeDirection(Direction::EAST);
				break;
			}
		break;
	case WM_DESTROY:
		//sparar undan highscore.
		game->gameHighscore->SaveHighscore();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		//skapa bild länkarna.
		bmpSource = (HBITMAP) LoadImage(nullptr,L"floor2.bmp", IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
		if(bmpSource == nullptr)
		{
				    MessageBox(NULL, L"Bitmap not loaded- Ensure the file 'floor2.bmp' is present in the project folder",L"Error",MB_OK);
		}
		hdcSource = CreateCompatibleDC(GetDC(0));
		SelectObject(hdcSource, bmpSource);

		bmpSourceWall = (HBITMAP) LoadImage(nullptr,L"wall.bmp", IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
		if(bmpSourceWall == nullptr)
		{
				    MessageBox(NULL, L"Bitmap not loaded- Ensure the file 'wall.bmp' is present in the project folder",L"Error",MB_OK);
		}
		hdcSourceWall = CreateCompatibleDC(GetDC(0));
		SelectObject(hdcSourceWall, bmpSourceWall);

		bmpSourceSnake = (HBITMAP) LoadImage(nullptr,L"snake.bmp", IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
		if(bmpSourceSnake == nullptr)
		{
				    MessageBox(NULL, L"Bitmap not loaded- Ensure the file 'snake.bmp' is present in the project folder",L"Error",MB_OK);
		}
		hdcSourceSnake = CreateCompatibleDC(GetDC(0));
		SelectObject(hdcSourceSnake, bmpSourceSnake);

		bmpSourceFruit = (HBITMAP) LoadImage(nullptr,L"fruit.bmp", IMAGE_BITMAP,0,0, LR_LOADFROMFILE);
		if(bmpSourceFruit == nullptr)
		{
				    MessageBox(NULL, L"Bitmap not loaded- Ensure the file 'fruit.bmp' is present in the project folder",L"Error",MB_OK);
		}
		hdcSourceFruit = CreateCompatibleDC(GetDC(0));
		SelectObject(hdcSourceFruit, bmpSourceFruit);

		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}