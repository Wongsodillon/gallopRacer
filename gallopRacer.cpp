#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

void loadData();
void landing();
int validateName(char name[]);
void login();
void game();
void menuScreen();
void sort();

struct user
{
    char name[100];
    char pass[100];
    int wins;
    int loss;
    char icon;
    int chance;
    double winrate;
};

void menuScreen(user player);

user users[10];
user bots[5];
user player;
int userCount = 0;
int bot1PosY, bot2PosY, bot3PosY, bot4PosY, bot5PosY;
int index;

void loadData()
{
    FILE *file = fopen("users.txt", "r");
    while (fscanf(file, "%[^,],%[^,],%d,%d,%lf\n", users[userCount].name, users[userCount].pass, &users[userCount].wins,
        &users[userCount].loss, &users[userCount].winrate) != EOF)
    {
        userCount++;
    }
    fclose(file);
//     for (int i = 0; i < userCount; i++)
//     {
//         printf("%s %s %d %d %.2lf\n", users[i].name, users[i].pass, users[i].wins, users[i].loss, users[i].winrate);
//     }
}

int validateName(char name[])
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void newGame()
{
    system("cls");
    char name[100];
    char pass[100];
    char confirm[100];
    int flag = 0;
    do
    {
        printf("Enter Username: ");
        scanf("%[^\n]", name); getchar();
        if (validateName(name) != -1)
        {
            printf("Username Taken\n");
        }
    } 
    while (validateName(name) != -1);
    do
    {
        flag = 0;
        printf("Enter Password: ");
        scanf("%s", pass); getchar();
        int len = strlen(pass);
        if (len <= 7)
        {
            flag = 1;
            printf("Password Must be Longer Than 7 Characters\n");
        }
    } 
    while (flag != 0);
    do
    {
        printf("Confirm Password: ");
        scanf("%s", confirm); getchar();
    } 
    while (strcmp(confirm, pass) != 0);
    printf("New Game Created\n");
    printf("Press Enter To Continue..\n");
    FILE *file = fopen("users.txt", "a");
    fprintf(file, "%s,%s,%d,%d,%lf\n", name, pass, 0, 0, 0);
    fclose(file);
    userCount = 0;
    loadData();
    getch();
    landing();
}

void login(void)
{
    system("cls");
    char name[100];
    char pass[100];
    int flag =0;
    do
    {
        system("cls");
        printf("Enter Username: ");
        scanf("%[^\n]", name); getchar();
        if (validateName(name) == -1)
        {
            printf("Username Doesn't Exist\n");
            getch();
        }
        index = validateName(name);
    } 
    while (validateName(name) == -1);   
    do
    {
        system("cls");
        flag = 0;
        printf("Enter Username: %s\n", name);
        printf("Enter Password: ");
        char c = ' ';
        int i = 0;
        while (c != 13)
        {
            c = getch();
            if (c == 13)
            {
                break;
            }
            char stars[100] = "";
            if (c == 8)
            {
                system("cls");
                printf("Enter Username: %s\n", name);
                printf("Enter Password: ");
                if (i > 0)
                {
                    i--;
                }
                else
                {
                    i = 0;
                }
                for (int j = 0; j < i; j++)
                {
                    stars[j] = '*';
                }
                printf("%s", stars);
                continue;
            }
            pass[i] = c;
            printf("*");
            i++;
        }
        pass[i] = '\0';
        if (strcmp(pass, users[index].pass) != 0)
        {
            printf("\nIncorrect Password\n");
            getch();
        }
    } 
    while (strcmp(pass, users[index].pass) != 0);
    player = users[index];
    menuScreen();
}

void landing()
{
	system("cls");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    int menu;
    bla:
    do
    {
        printf(">> ");
        scanf("%d", &menu); getchar();
    } 
    while (menu < 1 || menu > 2);
    if (menu == 1)
    {
        if (userCount == 10)
        {
            printf("Can't create new game\n");
            goto bla;
        }
        else
        {
            newGame();
        }
    }
    else if (menu == 2)
    {
        login();
    }
}

void sort()
{
	for (int i = 0; i < userCount; i++)
	{
		float winrate = 0;
		if (users[i].loss != 0 && users[i].wins != 0)
		{
			winrate = (users[i].wins / (float)(users[i].loss + users[i].wins)) * 100;
		}
		else if (users[i].loss == 0 && users[i].wins != 0)
		{
			winrate = 100;
		}
		users[i].winrate = winrate;
	}
	user tmp;
	for (int i = 0; i < userCount - 1; i++)
	{
		for (int j = 0; j < userCount - i - 1; j++)
		{
			if (users[j].winrate <= users[j+1].winrate)
			{
				tmp = users[j];
				users[j] = users[j+1];
				users[j+1] = tmp;
			}
		}
	}
}

void leaderBoards(void)
{
	system("cls");
	sort();
	printf("Leaderboard\n");
	printf("=====================================\n");
	printf("| Rank | %-15s | %8s |\n", "Name", "Win Rate");
	printf("=====================================\n");
	for (int i = 0; i < userCount; i++)
	{
		printf("| %4i | %-15s | %7.2f%% |\n", i+1, users[i].name, users[i].winrate);
	}
	printf("=====================================\n");
	printf("Press enter to continue...");
	getch();
	menuScreen();
}

void menuScreen()
{
    system("cls");
    printf("Hello %s\n", player.name);
    printf("Wins: %d\n", player.wins);
    printf("Loss: %d\n", player.loss);
    printf("=================================\n");
    printf("1. Play Game\n");
    printf("2. View Leaderboards\n");
    printf("3. Exit\n");
    int menu;
    do 
    {
    	printf(">> ");
    	scanf("%d", &menu); getchar();
	}
	while(menu < 1 || menu > 3);
	if (menu == 1)
	{
		game();
	}
	else if (menu == 2)
	{
		leaderBoards();
	}
	else if (menu == 3)
	{
		users[index] = player;
		FILE *file = fopen("users.txt", "w");
		for (int i = 0; i < userCount; i++)
		{
			fprintf(file, "%s,%s,%d,%d,%lf\n", users[i].name, users[i].pass, users[i].wins, users[i].loss, users[i].winrate);
		}
		return;
	}
}

char chooseCharacter(char characters[])
{
	char input;
	int i = 0;
	while (true)
	{
		system("cls");
		printf("\n");
		printf("  %c  \n", characters[i]);
		printf("\n");
		printf("<--->\n");
		printf("Press enter to select character\n");
		printf("Press a / d to choose character\n");
		input = getch();
		if (input == 'd' && i != 4)
		{
			i++;
		}
		else if (input == 'a' && i != 0)
		{
			i--;
		}
		else if (input == 13)
		{
			printf("Character Selected %c\n", characters[i]);
			getch();
			return characters[i];
		}
	}
}
void printMap(char map[][100])
{
	int bot1PosX = 1;
	int bot2PosX = 3;
	int bot3PosX = 5;
	int bot4PosX = 7;
	int bot5PosX = 9;
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (j == bot5PosY && i == bot5PosX)
			{
				printf("%c", bots[4].icon);
			}
			else if (i == bot1PosX && j == bot1PosY)
			{
				printf("%c", bots[0].icon);
			}
			else if (i == bot2PosX && j == bot2PosY)
			{
				printf("%c", bots[1].icon);
			}
			else if (i == bot3PosX && j == bot3PosY)
			{
				printf("%c", bots[2].icon);
			}
			else if (i == bot4PosX && j == bot4PosY)
			{
				printf("%c", bots[3].icon);
			}
			else 
			{
				printf("%c", map[i][j]);
			}	
		}
		printf("\n");
	}
}

void game()
{
	int winnerCount = 0;
	int flag = 0;
	char characters[5] = {'!', '^', '?', '$', '@'};
	system("cls");
	player.icon = chooseCharacter(characters);
	system("cls");
	int chance1, chance2, chance3, chance4, chance5;
	time_t t1; 
	srand ( (unsigned) time (&t1));
	int start = rand() % 5;
	user tmpList[5];
	tmpList[0] = bots[4];
	tmpList[1] = bots[0];
	tmpList[2] = bots[1];
	tmpList[3] = bots[2];
	tmpList[4] = bots[3];
	int chanceList[5] = {0};
	user winner[10];
	bot5PosY = bot1PosY = bot2PosY = bot3PosY = bot4PosY = 13;
	for (int i = 0; i < 5; i++)
	{
		bots[i].icon = characters[i];
	}
	strcpy(bots[0].name, "Horse 1");
	strcpy(bots[1].name, "Horse 2");
	strcpy(bots[2].name, "Horse 3");
	strcpy(bots[3].name, "Horse 4");
	strcpy(bots[4].name, "Horse 5");
	char map[][100] = {
		"################################################################", 
		"# Horse 1   |                                                  #",
		"#--------------------------------------------------------------#",
		"# Horse 2   |                                                  #",
		"#--------------------------------------------------------------#",
		"# Horse 3   |                                                  #",
		"#--------------------------------------------------------------#",
		"# Horse 4   |                                                  #",
		"#--------------------------------------------------------------#",
		"# Horse 5   |                                                  #",
		"################################################################", 
	};
	printMap(map);
	printf("Press Enter To Start The Game...");
	getch();
	printf("Icon Selected: %c\n", player.icon);
	bool winCondition = false;
	while (winCondition == false)
	{
		int i = 0;
		system("cls");
		printMap(map);
		chanceList[0] = rand() % 100;
		chanceList[1] = rand() % 100;
		chanceList[2] = rand() % 100;
		chanceList[3] = rand() % 100;
		chanceList[4] = rand() % 100;
		while (i < 5)
		{
			tmpList[(start + i) % 5].chance = chanceList[i];
			i++;
		}
//		printf("%d %d %d\n", start, tmpList[2].chance, tmpList[1].chance);
		if (tmpList[0].chance > 50)
		{
			bot5PosY++;
			if (bot5PosY == 63)
			{
				winner[winnerCount] = bots[4];
				winnerCount++;
			}
		}
		if (tmpList[1].chance > 50)
		{
			bot1PosY++;
			if (bot1PosY == 63)
			{
				winner[winnerCount] = bots[0];
				winnerCount++;
			}
		}
		if (tmpList[2].chance > 50)
		{
			bot2PosY++;
			if (bot2PosY == 63)
			{
				winner[winnerCount] = bots[1];
				winnerCount++;
			}
		}
		if (tmpList[3].chance > 50)
		{
			bot3PosY++;
			if (bot3PosY == 63)
			{
				winner[winnerCount] = bots[2];
				winnerCount++;
			}
		}
		if (tmpList[4].chance > 50)
		{
			bot4PosY++;
			if (bot4PosY == 63)
			{
				winner[winnerCount] = bots[3];
				winnerCount++;
			}
		}
		if (bot5PosY == 63 || bot1PosY == 63 || bot2PosY == 63 || bot3PosY == 63 || bot4PosY == 63)
		{
			winCondition = true;
		}
		sleep(1);
	}	
	printf("Race Ended!\n");
	printf("Winner: ");
	for (int j = 0; j < winnerCount; j++)
	{
		printf("%c ", winner[j].icon);
	}
	printf("\n");
	printf("Your Horse: %c\n", player.icon);
	if (winner[0].icon == player.icon)
	{
		player.wins++;
	}
	else
	{
		player.loss++;
	}
	player.winrate = player.wins/(player.wins+player.loss);
	getch();
	menuScreen();
}

int main(void)
{
    loadData();
    landing();
}
