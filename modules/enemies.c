#include "game_screen.h"
#include "enemies.h"
#include "missile.h"
#include "set_utils.h"
#include "ADTList.h"
#include "ADTSet.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

List state_enemies(Set enemies, float y_from, float y_to) {
	List list = list_create(NULL);	// create list


	Enemy enemy1 = malloc(sizeof(Enemy));	// allocate memory
	enemy1->rect.y = y_from;	// give y coordinate to enemy1 
							// (required for the next functions)

	Enemy enemy2 = malloc(sizeof(Enemy));	// allocate memory
	enemy2->rect.y = y_to;	// give y coordinate to enemy2
							// (required for the next functions)

	Set set = enemies;

	// Find first and last enemies between y_from and y_to coordinates
	Enemy enemy_first = set_find_eq_or_greater(set, enemy1);
	Enemy enemy_last = set_find_eq_or_smaller(set, enemy2);

	// If there are no enemies between y_from and y_to,
	// return empty list
	if (enemy_first == NULL || enemy_last == NULL) {
		return list;
	}

	// Iterate set from enemy_first to enemy_last and add
	// enemies to the list we created

	SetNode first_node = set_find_node(set, enemy_first);
	SetNode last_node = set_find_node(set, enemy_last);
	
	for(SetNode node = first_node;
		node != set_next(set, last_node);
		node = set_next(set, node)) {

		Enemy enemy = set_node_value(set, node);			// recover enemy
		list_insert_next(list, list_last(list), enemy);	// insert enemy to list
	}

	return list;
}

// CompareFunc comparing enemies (used for set)

int enemies_comparefunc(Pointer a, Pointer b) {
	Enemy enemy_a = a;
	Enemy enemy_b = b;

	// comparing coordinates of y axis
	if (enemy_a->rect.y < enemy_b->rect.y) {
		return 1;
	} else if (enemy_a->rect.y > enemy_b->rect.y) {
		return -1;
	} else {

		// comparing memory addresses
		if (a < b) {
			return 1;
		} else if (a > b) {
			return -1;
		} else {
			return 0;
		}
	}
}

static Enemy create_enemy(EnemyType type, float x, float y, float width, float height, Game game) {
	Enemy enemy = malloc(sizeof(*enemy));
	enemy->type = type;
	enemy->rect.x = x;
	enemy->rect.y = y;
	enemy->rect.width = width;
	enemy->rect.height = height;

	// to make timing between bulllets random from enemy to enemy
	float rand_time = 0.5 * (float)((float)GetRandomValue(0, 10) / (float)10);

	if (game->difficulty != 4)
		rand_time = GetRandomValue(0,1) ? rand_time : -rand_time;

	// interval between bullets depends on difficulty
	if (game->difficulty == 1)
		enemy->bullet_cooldown = 4.5 + rand_time;
	else if (game->difficulty == 2)
		enemy->bullet_cooldown = 3.5 + rand_time;
	else if (game->difficulty == 3)
		enemy->bullet_cooldown = 2 + rand_time;
	else
		enemy->bullet_cooldown = 2 - rand_time;
	
	return enemy;
}

void add_enemies(Game game, float start_y) {
	// Προσθέτουμε ENEMY_ROWS φορές τους εχθρούς.

	for (int i = 0; i < ENEMY_ROWS; i++) {
		// Add motherships
		Enemy mothership;
		int random = GetRandomValue(0, 1); // ένα αριστερά, ένα δεξιά
		for (int j = 0; j < 2; j++) {
			mothership = create_enemy (
				MOTHERSHIP,
				random ? 0 : SCREEN_W_G * 0.7,
				start_y - 21 * SPACING * (i+1) + j * SPACING * 5,
				264,
				SPACING * 5,
				game
			);
			mothership->right = random ? false : true;
			
			// Add mothership defenders
			for (int j = 0; j < 3; j++) {
				Enemy enemy = create_enemy (
					MOTHERSHIP_DEFENDER,
					(SCREEN_W_G - 70) / 2,
					mothership->rect.y + (j+1) * mothership->rect.height / 4,
					70,
					30,
					game
				);
				set_insert(game->enemies, enemy);
			}
			set_insert(game->enemies, mothership);

			random = random ? 0 : 1;
		}

		// Add 10 enemies before motherships
		for (int j = 0; j < 10; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = mothership->rect.y + mothership->rect.height + (j+1)*SPACING;

			Enemy enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε crab και longhorn
				? create_enemy(CRAB,    (SCREEN_W_G - 83)/2, y, 90, 40, game)		// οριζόντιο κεντράρισμα
				: create_enemy(LONGHORN, (SCREEN_W_G - 70)/2, y, 100, 30, game);
			enemy->right = GetRandomValue(0, 1);	// Τυχαία αρχική κατεύθυνση

			set_insert(game->enemies, enemy);
		}
	}
}

// Checks if an enemy is about to collide with a terain enemy 
// If it does change its direction, else don't

void enemy_collision(Enemy enemy, Set set) {

	if (enemy->type != MOTHERSHIP_DEFENDER) {
		// If enemy is about to collide with screen border change its direction
		if (enemy->rect.x < 0 || enemy->rect.x + enemy->rect.width > SCREEN_W_G)
			enemy->right = !enemy->right;
	} else {

		// Find the first mothership enemy after the enemy
		Enemy mothership;
		SetNode node = set_find_node(set, enemy);
		do {
			node = set_next(set, node);
			mothership = set_node_value(set, node);
		} while (mothership->type != MOTHERSHIP);

		// Find x coordinates the enemy can move between
		// before touching mothership

		float x_left;
		float x_right;
		if (mothership->right) {
			x_left = 0;
			x_right = mothership->rect.x;
		} else {
			x_left = mothership->rect.width;
			x_right = SCREEN_W_G;
		}

		// If enemy is about to collide with mothership change its direction
		if (enemy->rect.x < x_left || enemy->rect.x + enemy->rect.width > x_right)
			enemy->right = !enemy->right;
	}


}

// Move enemies depending on the the direction
// they are facing and on the game's speed

void enemy_movement(Enemy enemy, float speed) {
	int pixels;
	if (enemy->type == LONGHORN)	// enemies move differently
		pixels = 4;	// helictopers move 4 pixels
	else if (enemy->type == CRAB)
		pixels = 3;	// warships move 3 pixels
	else
		pixels = 5;	// mothership defenders move 5 pixels

	if (enemy->right)	// depending on their direction
						// they move left or right
		enemy->rect.x += pixels * speed;	// move "pixels" right multiplied by game's speed
	else
		enemy->rect.x -= pixels * speed;	// move "pixels" left multiplied by game's speed
}

void enemy_missile(Game game, Enemy enemy) {
	enemy->bullet_cooldown -= GetFrameTime();
	if(enemy->bullet_cooldown <= 0) {
		MissileType m_type;
		if (enemy->type == CRAB)
			m_type = C_MISSILE;
		else if (enemy->type == LONGHORN)
			m_type = LH_MISSILE;
		else
			m_type = M_MISSILE;
		
		missile_create(game, enemy->rect, m_type);

		// to make timing between bulllets random from enemy to enemy
		float rand_time = 0.5 * (float)((float)GetRandomValue(0, 10) / (float)10);

		if (game->difficulty != 4)
			rand_time = GetRandomValue(0,1) ? rand_time : -rand_time;

		// interval between bullets depends on difficulty
		if (game->difficulty == 1)
			enemy->bullet_cooldown = 4.5 + rand_time;
		else if (game->difficulty == 2)
			enemy->bullet_cooldown = 3.5 + rand_time;
		else if (game->difficulty == 3)
			enemy->bullet_cooldown = 2 + rand_time;
		else
			enemy->bullet_cooldown = 2 - rand_time;
	}
}

// Finds current last enemy

static Enemy find_last_enemy(Set set) {
	Enemy enemy;
	SetNode node = set_last(set);	// recover last node of set
	enemy = set_node_value(set, node);	// recover last enemy

	return enemy;
}

void enemies_update(Game game) {
    Set set = game->enemies;

	///// UPDATE MISSILES /////
    List list = state_enemies(	// list of enemies from screen till the last enemy
		game->enemies,
		game->camera_y + SCREEN_HEIGHT,
		find_last_enemy(game->enemies)->rect.y	// SPACING * 5 ύψος του ψυλότερου αντικειμένου στο παιχνίδι
	); 

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Enemy enemy = list_node_value(list, node);	// recover enemy
			
		if (enemy->type != MOTHERSHIP_DEFENDER)
			enemy_missile(game, enemy);
	}

	list_destroy(list);

	///// UPDATE EVERYTHING ELSE//
	list = state_enemies(
		game->enemies,
		game->camera_y + SCREEN_HEIGHT,
		game->camera_y - 150
	); 

	for	(ListNode node = list_first(list);	// iterate list
    	node != LIST_EOF;
    	node = list_next(list, node)) {

		Enemy enemy = list_node_value(list, node);	// recover enemy

		if (enemy->type != MOTHERSHIP) {
			
			enemy_collision(enemy, set);
			enemy_movement(enemy, game->speed_factor);
		}
	}

	list_destroy(list);

	// check if new enemies must be added
	Enemy last_enemy = find_last_enemy(set);
	float last_enemy_y  = last_enemy->rect.y;
	if (abs((int)(last_enemy_y - game->camera_y)) < SCREEN_HEIGHT) {
		add_enemies(game, last_enemy_y);
	}
	
}

