#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#define ROW_SIZE 15
#define COL_SIZE 15

#define NORTH_BOUNDARY 0
#define SOUTH_BOUNDARY 14
#define WEST_BOUNDARY 0
#define EAST_BOUNDARY 14

#define ARROW_KEYS 224
#define UP 72
#define DOWN 80 
#define LEFT 75
#define RIGHT 77

typedef enum tag_YesNo {
	NO = 0,
	YES = 1
} YesNo;

typedef enum tag_Direction {
	MOVE_STOP = 0,
	MOVE_UP = 1,
	MOVE_DOWN = 2,
	MOVE_RIGHT = 3,
	MOVE_LEFT = 4
} Direction;

typedef struct tag_GameObject {
	int x;
	int y;
	char object_symbol;
} GameObject;

typedef struct tag_CheckCollision {
	YesNo boundary_collission;
	YesNo food_collission;
	YesNo body_collision;
} CheckCollision;

GameObject change_direction(Direction direction, GameObject snake_head, GameObject snake_body[196], int snake_body_parts);
CheckCollision check_body_collision(CheckCollision game, GameObject snake_head, GameObject snake_body[196], int snake_body_parts);
CheckCollision check_boundary_collision(CheckCollision game, GameObject snake_head);
CheckCollision check_food_collision(CheckCollision game, GameObject snake_head, GameObject food);
GameObject generate_random_coords(GameObject food, GameObject snake_head);
void keyboard_movement(Direction* direction);
void print_game_board(char game_board_array[COL_SIZE][ROW_SIZE]);
void set_game_board(char game_board_array[COL_SIZE][ROW_SIZE], GameObject snake_head, GameObject food, GameObject snake_body[196], int snake_body_size);

int main(void) 
{
	srand((int)time(0));

	// Game Board
	char game_board_array[COL_SIZE][ROW_SIZE] =
	{
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
	};	

	// Check collision variables
	CheckCollision game;
	game.boundary_collission = NO;
	game.food_collission = NO;
	game.body_collision = NO;

	// Snake Head
	GameObject snake_head;
	snake_head.x = 5;
	snake_head.y = 5;
	snake_head.object_symbol = '#';

	// Snake body
	GameObject snake_body[196];

	for (int i = 0; i < 196; i++)
	{
		snake_body[i].x = -1;
		snake_body[i].y = -1;
		snake_body[i].object_symbol = '#';
	}

	int snake_body_parts = 0;

	// Food
	GameObject food;
	food.x = -1;
	food.y = -1;
	food.object_symbol = 'A';
	food = generate_random_coords(food, snake_head);

	// Move direction
	Direction direction = MOVE_STOP;

	while (1)
	{
		// Refresh screen
		system("cls");

		// Snake movement 
		keyboard_movement(&direction);
		snake_head = change_direction(direction, snake_head, snake_body, snake_body_parts);

		// Set and print game board.
		set_game_board(game_board_array, snake_head, food, snake_body, snake_body_parts);
		print_game_board(game_board_array);

		// Check for collision to food.
		game = check_food_collision(game, snake_head, food);

		if (game.food_collission == YES)
		{
			food = generate_random_coords(food, snake_head);
			game.food_collission = NO;
			snake_body_parts++;
		}

		// Check for collision to boundaries.
		game = check_boundary_collision(game, snake_head);

		if (game.boundary_collission == YES)
		{
			break;
		}

		// Check for collison to body.
		game = check_body_collision(game, snake_head, snake_body, snake_body_parts);

		if (game.body_collision == YES)
		{
			break;
		}

		// Game speed.
		Sleep(200);
	}


	return 0;
}

GameObject change_direction(Direction direction, GameObject snake_head, GameObject snake_body[196], int snake_body_parts)
{
	for (int body = snake_body_parts - 1; body > 0; body--)
	{
		snake_body[body].x = snake_body[body - 1].x;
		snake_body[body].y = snake_body[body - 1].y;
	}

	if (snake_body_parts > 0)
	{
		snake_body[0].x = snake_head.x;
		snake_body[0].y = snake_head.y;
	}

	if (direction == MOVE_UP && direction != MOVE_DOWN && snake_head.y > NORTH_BOUNDARY)
	{
		snake_head.y--;
	}
	else if (direction == MOVE_DOWN && direction != MOVE_UP && snake_head.y < SOUTH_BOUNDARY)
	{
		snake_head.y++;
	}
	else if (direction == MOVE_RIGHT && direction != MOVE_LEFT && snake_head.x < EAST_BOUNDARY)
	{
		snake_head.x++;
	}
	else if (direction == MOVE_LEFT && direction != MOVE_RIGHT && snake_head.x > WEST_BOUNDARY)
	{
		snake_head.x--;
	}



	return snake_head;
}

CheckCollision check_body_collision(CheckCollision game, GameObject snake_head, GameObject snake_body[196], int snake_body_parts)
{
	for (int body = 0; body < snake_body_parts; body++)
	{
		if (snake_body[body].x == snake_head.x && snake_body[body].y == snake_head.y)
		{
			game.body_collision = YES;
			break;
		}
	}

	return game;
}

CheckCollision check_boundary_collision(CheckCollision game, GameObject snake_head)
{
	if (snake_head.x == EAST_BOUNDARY || snake_head.x == WEST_BOUNDARY)
	{
		game.boundary_collission = YES;
	}

	if (snake_head.y == NORTH_BOUNDARY || snake_head.y == SOUTH_BOUNDARY)
	{
		game.boundary_collission = YES;
	}

	return game;
}

CheckCollision check_food_collision(CheckCollision game, GameObject snake_head, GameObject food )
{
	// Food
	if (snake_head.x == food.x && snake_head.y == food.y)
	{
		game.food_collission = YES;
	}
	else
	{
		game.food_collission = NO;
	}

	return game;
}

GameObject generate_random_coords(GameObject food, GameObject snake_head)
{
	do
	{
		food.x = (rand() % (ROW_SIZE - 2)) + 1;
		food.y = (rand() % (COL_SIZE - 2)) + 1;
		food.object_symbol = (rand() % ('Z' - 'A' + 1)) + 'A';
	} while (food.x == snake_head.x && food.y == snake_head.y);

	return food;
}

void keyboard_movement(Direction* direction)
{
	if (_kbhit())
	{
		int key_press = _getch();

		if (key_press == ARROW_KEYS)
		{
			int arrow_key = _getch();

			if (arrow_key == UP && *direction != MOVE_DOWN)
			{
				*direction = MOVE_UP;
			}
			else if (arrow_key == DOWN && *direction != MOVE_UP)
			{
				*direction = MOVE_DOWN;
			}
			else if (arrow_key == LEFT && *direction != MOVE_RIGHT)
			{
				*direction = MOVE_LEFT;
			}
			else if (arrow_key == RIGHT && *direction != MOVE_LEFT)
			{
				*direction = MOVE_RIGHT;
			}
		}
	}
}

void print_game_board(char game_board_array[COL_SIZE][ROW_SIZE]) {
	for (int col = 0; col < COL_SIZE; col++)
	{
		for (int row = 0; row < ROW_SIZE; row++)
		{
			printf("%c", game_board_array[col][row]);
		}
		printf("\n");
	}

}

void set_game_board(char game_board_array[COL_SIZE][ROW_SIZE], GameObject snake_head, GameObject food, GameObject snake_body[196], int snake_body_size)
{
	for (int col = 0; col < COL_SIZE; col++)
	{
		for (int row = 0; row < ROW_SIZE; row++)
		{
			if (row == WEST_BOUNDARY || row == EAST_BOUNDARY || col == NORTH_BOUNDARY || col == SOUTH_BOUNDARY)
			{
				game_board_array[col][row] = '@';
			}
			else if (snake_head.x == row && snake_head.y == col)
			{
				game_board_array[col][row] = snake_head.object_symbol;
			}
			else if (food.x == row && food.y == col)
			{
				game_board_array[col][row] = food.object_symbol;
			}
			else
			{
				game_board_array[col][row] = ' ';
			}

			for (int body = 0; body <= snake_body_size; body++)
			{
				if (snake_body[body].x == row && snake_body[body].y == col)
				{
					game_board_array[col][row] = snake_body[body].object_symbol;
				}
			}
		}
	}
}

// TODO Score
// -Increment 1 per eatean food. 

// TODO Save Score to File
// -Check if game over score is higher than saved highest score then save new high score.

// Testing with git terminal.