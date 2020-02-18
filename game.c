#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>

#define LEFT 1
#define RIGHT 2

int draw(int direction);
WINDOW* start_menu();
void start();
void game_description();
void game_settings();


pthread_t enemy_thread_id;
pthread_t game_start_thread_id;

//aici avem threadul pt bila 
void *enemy_thread(void *vargp){

    int parent_x, parent_y;
    int pos_x, pos_y;
    int pos_player_x;
    int scor = 0;
    int speed = 100000;
    int speed_count = 1;
    int lives = 1;

    box(stdscr, 0, 0);
    getmaxyx(stdscr, parent_y, parent_x);

    pos_x = rand() % parent_x;
    pos_y = 1;

    while( 1 ){
        usleep(speed);
        clear();
        box(stdscr, 0, 0);

        pos_player_x = draw(0);
        mvwprintw(stdscr, pos_y, pos_x, "o");

        pos_y += 1;

        if(pos_x >= pos_player_x && pos_x <= (pos_player_x + 12) && pos_y == (parent_y - 1)){
            scor++;

            if((scor % 5) == 0 && scor != 0){

                speed_count++;
                speed = speed / speed_count;
            }
        }

        if((pos_x < pos_player_x || pos_x > (pos_player_x + 12)) && pos_y == (parent_y - 1)){
            lives -= 1;

            if(lives == 0){
                //pthread_cancel(game_start_thread_id);
                //start_menu();
                int i;
                for( i = 0; i < 3; i++)
                    flash();

                mvwprintw(stdscr, parent_y/2, parent_x/2 - 4, "LOOOOSER");
                wrefresh(stdscr);
                sleep(2);

                pthread_cancel(game_start_thread_id);
                start_menu();
                return;
            }
        }


        if(pos_y == parent_y){
            pos_x = rand() % parent_x;
            pos_y = 0;
        }

        mvwprintw(stdscr, 2, 2, "Scor:  %d", scor);
        mvwprintw(stdscr, 3, 2, "Speed: X%d ", speed_count);
        mvwprintw(stdscr, 4, 2, "Vietz: %dx<3  ", lives);

        wrefresh(stdscr);
    }
}

//aici e desenat playerul
int draw(int direction){

    int parent_x, parent_y;

    initscr();
    //noecho();
    curs_set(0);

    getmaxyx(stdscr, parent_y, parent_x);

    static int pos_x = 1;
    int pos_y = parent_y - 1;

    //box(stdscr, 0, 0);

    if(direction == LEFT && pos_x >= 3){
        pos_x -= 3;
    } else if(direction == RIGHT && pos_x <= parent_x - 16){    // -15 pt ca -3 - lungime player    
        pos_x += 3;
    }

    mvwprintw(stdscr, pos_y - 1, pos_x, "------------");
    //wrefresh(stdscr);

    return pos_x;

}


void *game_start_thread(void *vargp){

    draw(0);

    int game_status = 1;

    while ( game_status != 0 ) {
        int key = getch();

        switch ( key ) {

        case KEY_LEFT:
        case 'a':
            draw(LEFT);
            break;

        case KEY_RIGHT:
        case 'd':
            draw(RIGHT);
            break;

        case 'q':
            start_menu();
            game_status = 0;
            pthread_cancel(enemy_thread_id);
            break;
        }
    }
}


WINDOW* start_menu(){

    int parent_x, parent_y;

    initscr();
    noecho();
    curs_set(2);

    // get our maximum window dimensions
    getmaxyx(stdscr, parent_y, parent_x);

    // set up initial windows
    WINDOW *field = newwin(parent_y, parent_x, 0, 0);

    keypad(field, TRUE);

    wprintw(field,  "Interface Started\n\n");
    wprintw(field,  "Select one of the above: \n");
    wprintw(field,  "1 - Start \n2 - Description \n3 - Settings \n\n0 - Exit \n\n\n");

    wrefresh(field);

    return field;

}

void start(){

    int select = -1;

    WINDOW *field = start_menu();
   
    while(select != 0){
        scanf("%d", &select);
        switch(select){
            case 0:
                wprintw(field, "Exit - See you later! \n");
                wrefresh(field);
                break;

            case 1:
                pthread_create(&enemy_thread_id, NULL, enemy_thread, 0);
                pthread_create(&game_start_thread_id, NULL, game_start_thread, 0);

                pthread_join(enemy_thread_id, NULL);
                pthread_join(game_start_thread_id, NULL);
                break;

            case 2:
                game_description();
                start_menu();
                break;

            case 3:
                game_settings();
                start_menu();
                break;

            default:
                wprintw(field, "Default \n");
                wrefresh(field);
                break;
        }
    }
    delwin(field);
    endwin();
}

void game_description(){

    int parent_x, parent_y, new_x, new_y;
    int select = -1;

    initscr();
    noecho();

    getmaxyx(stdscr, parent_y, parent_x);
   
    WINDOW *description_win = newwin(parent_y, parent_x, 0, 0);
    wprintw(description_win, "Description\n\n");
    wprintw(description_win, "...\n");
    wprintw(description_win, "...\n\n");
    wprintw(description_win, "Back - 0\n");

    wrefresh(description_win);

    while(select != 0){
        scanf("%d", &select);
        switch(select){
            case 0:
                break;
        }
    }
    wrefresh(description_win);
    delwin(description_win);

    return;
}


void game_settings(){

    int parent_x, parent_y, new_x, new_y;
    int select = -1;

    initscr();
    noecho();

    getmaxyx(stdscr, parent_y, parent_x);

    WINDOW *settings_win = newwin(parent_y, parent_x, 0, 0);
    wprintw(settings_win, "Settings\n\n");

    wprintw(settings_win, " Choose from below:\n");
    wprintw(settings_win, "Setup 1\n");
    wprintw(settings_win, "Setup 2\n");
    wprintw(settings_win, "Setup 3\n\n");
    wprintw(settings_win, "Back - 0\n");

    wrefresh(settings_win);

    while(select != 0){
        scanf("%d", &select);
        switch(select){

            case 0:
                wrefresh(settings_win);
                break;

            case 1:
                wprintw(settings_win, "Setup 1 chosen!\n");
                wrefresh(settings_win);
                break;

            case 2:
                wprintw(settings_win, "Setup 2 chosen!\n");
                wrefresh(settings_win);
                break;

            case 3:
                wprintw(settings_win, "Setup 3 chosen!\n");
                wrefresh(settings_win);
                break;

            default:
                wprintw(settings_win, "Default! \n");
                wrefresh(settings_win);
                break;

            }
        }

    wrefresh(settings_win);
    delwin(settings_win);

    return;
}

int main(int argc, char* argv[]){

    start();

    return 0;
}

 
         
