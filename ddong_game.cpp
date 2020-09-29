#include <iostream>
#include <curses.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <random>
#include <unistd.h>
#include <atomic>
#include "getch.h"
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <vector>
using namespace std;

#define ROW 20
#define COL 15
#define FILE_NAME "GAME_SCORE.txt"

char arr[ROW][COL] = {
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}
};

char com_vs_map[ROW][COL * 2 - 1] = {
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '*'},
    {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
};

char start_menu[ROW][COL] = {
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '-', '-', '-', '-', '-', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '|', ' ', '|', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '|', ' ', '|', ' ', ' ', ' ', '*', '1', '.', 's', 't', 'a', 'r', 't'},
    {'*', '|', ' ', '|', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '-', '-', '-', '-', '-', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', '|', ' ', ' ', ' ', '*', '2', '.', 's', 'c', 'o', 'r', 'e'},
    {'*', ' ', ' ', '|', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '-', '-', '-', '-', '-', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '3', '.', 'c', 'o', 'm', ' ', ' '},
    {'*', ' ', ' ', '-', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', '/', ' ', '\\', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '|', ' ', ' ', ' ', '|', ' ', '*', '4', '.', 'e', 'x', 'i', 't', ' '},
    {'*', ' ', '\\', ' ', '/', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', '-', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '*', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};

//게임을 구동하는데 필요한 스레드이다.
void *user_down_thread(void*);
void *com_down_thread(void*);
void *user_input_key(void*);
void *com_input_key(void*);
//화면을 출력해주기 위한 것
void start_menu_print();
void show_score();
void print_user_map();
void print_com_vs_map();
void *print_arr(void*);
char ddong = '-';

//세마포어를 위한 것
sem_t semaphore;
sem_t for_com_vs;
atomic<int> ticket(12);
atomic<int> com_vs_ticket(12);
int cur_p_x = ROW - 2, cur_p_y = COL / 2;
int cur_com_x = ROW - 2, cur_com_y = 21;
bool game_state = false;
bool com_vs_processing = false;
bool com_win = false, user_win = false;

//random 함수를 쓰기 위한 것이다.
random_device rd;
mt19937_64 gen(rd());

//점수를 저장하기 위한 벡터
vector<string> score;
//점수를 포함하기 위한것
atomic<int> atomic_score(0);
//컴퓨터가 계산하기 위해 global한 변수를 둔다.
atomic<int> global_x(0);
atomic<int> global_y(0);

int main(int argc, char *argv[]){
    //세마포어 초기화
    sem_init(&semaphore, 0, ticket);
    sem_init(&semaphore, 0, com_vs_ticket);
    //random함수 초기화
    uniform_int_distribution<int> dist(1, COL - 2);
    uniform_int_distribution<int> s_time(1, 2);
    uniform_int_distribution<int> com_vs_dist(15, 27);
    uniform_int_distribution<int> user_vs_dist(1, 13);

    //시작 메뉴를 보여준다.
    while(true){
        char key;
        while(true){
            system("clear");
            start_menu_print();
            key = getch_();
            if(key =='1'){
                cout << "GAME START" << endl;
                sleep(2);
                break;
            }
            else if(key == '2'){
                cout << "점수 보여주기" << endl;
                show_score();
            }
            else if(key == '3'){
                com_vs_processing = true;
                break;
            }
            else if(key == '4'){
                cout << "GAME EXIT" << endl;
                sleep(2);
                return 0;
            }
        }

        //사람 초기화
        if(!com_vs_processing)
            arr[cur_p_x][cur_p_y] = 'A';
        else
            com_vs_map[cur_p_x][cur_p_y] = 'A';
        //필요 스레드, 입력스레드, 출력스레드 초기화
        pthread_t for_print_thread;
        pthread_create(&for_print_thread, NULL, print_arr, NULL);
        pthread_t for_input_key;
        pthread_create(&for_input_key, NULL, user_input_key, NULL);
        if(com_vs_processing){
            pthread_t for_com_input_key;
            pthread_create(&for_com_input_key, NULL, com_input_key, NULL);
        }
        
        while(true){
            if(ticket > 0){
                int col;
                if(!com_vs_processing)
                    col = dist(gen);
                else
                    col = user_vs_dist(gen);
                pthread_t thread1;
                pthread_create(&thread1, NULL, user_down_thread, (void*)&col);
                //pthread_join(thread1, NULL);
            }

            if(com_vs_ticket > 0 && com_vs_processing){
                int col = com_vs_dist(gen);
                pthread_t thread1;
                pthread_create(&thread1, NULL, com_down_thread, (void*)&col);
            }
            sleep(s_time(gen));
            //다 지우므로써 제자리에 있는 효과를 보여준다.
            if(game_state) break;
        }

        if(game_state){
            game_state = false;
            FILE *fp = fopen(FILE_NAME, "a+");
            fprintf(fp, "%d\n", atomic_score.load());
            fclose(fp);
        }

        //컴퓨터랑 붙은거면 누가 이겼는지 검사한다.
        if(com_vs_processing && com_win){
            cout << "COMPUTER WIN" << endl;
            sleep(3);
        }
        else if(com_vs_processing && user_win){
            cout << "USER WIN" << endl;
            sleep(3);
        }

        //초기화 해놓는다.
        com_vs_processing = false;
        com_win = false; user_win = false;
        game_state = false;
    }
    cout << "GAME END" << endl;
    return 0;
}

void show_score(){
    FILE *fp = fopen(FILE_NAME, "r");
    char buffer[15];
    while(fgets(buffer, sizeof(buffer), fp) != NULL){
        score.push_back(buffer);
    }

    sort(score.begin(), score.end(), greater<string>());

    for(int i = 0 ; i < score.size() ; i++)
        cout << to_string(i + 1) << "." << score[i];
    
    score.clear();

    cout << "Press Any Key" << endl;
    char key = getch_();
}

void start_menu_print(){
    for(int i = 0 ; i < ROW ; i++){
        for(int j = 0 ; j < COL ; j++){
            cout << start_menu[i][j];
        }
        cout << endl;
    }
}

void *user_down_thread(void *arg){
    ticket--;
    sem_wait(&semaphore);
    int col = *(int*)arg;
    int row = 0;

    uniform_int_distribution<long long> d_time(1000, 1000000);
    long long down_time = d_time(gen);

    while(row < ROW - 1){
        if(row > 0){
            if(!com_vs_processing)
                arr[row - 1][col] = ' ';
            else
                com_vs_map[row - 1][col] = ' ';
        }
        if(arr[row][col] == 'A' && !com_vs_processing){
            game_state = true;
            ticket++;
            sem_post(&semaphore);
            return NULL;
        }
        else if(com_vs_map[row][col] == 'A' && com_vs_processing){
            if(!user_win && !com_win) com_win = true;
            game_state = true;
            ticket++;
            sem_post(&semaphore);
            return NULL;
        }
        //game_state는 게임이 끝났는지 판단하는 변수이다.
        if(game_state){
            ticket++;
            sem_post(&semaphore);
            return NULL;
        }
        if(!com_vs_processing)
            arr[row][col] = ddong;
        else
            com_vs_map[row][col] = ddong;
        row++;
        usleep(down_time);
        
        //떨어지는 속도를 각기 다르게 한다.
    }
    if(!com_vs_processing)
        arr[row - 1][col] = ' ';
    else
        com_vs_map[row - 1][col] = ' ';
    ticket++;
    atomic_score += 10;
    sem_post(&semaphore);
}

void *user_input_key(void *arg){
    while(true){
        if(game_state) return NULL;
        char ch = getch_();
        if(!com_vs_processing){
            if(ch == 'A' || ch == 'a'){
                if(arr[cur_p_x][cur_p_y - 1] == '*') continue;
                arr[cur_p_x][cur_p_y] = ' ';
                cur_p_y--;
                arr[cur_p_x][cur_p_y] = 'A';
            }
            else if(ch == 'D' || ch == 'd'){
                if(arr[cur_p_x][cur_p_y + 1] == '*') continue;
                arr[cur_p_x][cur_p_y] = ' ';
                cur_p_y++;
                arr[cur_p_x][cur_p_y] = 'A';
            }
            else if(ch == 'Q' || ch == 'q'){
                game_state = true;
                break;
            }
        }
        else{
            if(ch == 'A' || ch == 'a'){
                if(com_vs_map[cur_p_x][cur_p_y - 1] == '*') continue;
                com_vs_map[cur_p_x][cur_p_y] = ' ';
                cur_p_y--;
                com_vs_map[cur_p_x][cur_p_y] = 'A';
            }
            else if(ch == 'D' || ch == 'd'){
                if(com_vs_map[cur_p_x][cur_p_y + 1] == '*') continue;
                com_vs_map[cur_p_x][cur_p_y] = ' ';
                cur_p_y++;
                com_vs_map[cur_p_x][cur_p_y] = 'A';
            }
            else if(ch == 'Q' || ch == 'q'){
                game_state = true;
                break;
            }
        }
    }
}

void *com_down_thread(void *arg){
    com_vs_ticket--;
    sem_wait(&for_com_vs);
    int col = *(int*)arg;
    int row = 0;

    uniform_int_distribution<long long> d_time(1000, 1000000);
    long long down_time = d_time(gen);

    while(row < ROW - 1){
        if(row > 0){
            com_vs_map[row - 1][col] = ' ';
        }
        if(com_vs_map[row][col] == 'A'){
            if(!user_win && !com_win) user_win = true;
            game_state = true;
            com_vs_ticket++;
            sem_post(&for_com_vs);
            return NULL;
        }
        //game_state는 게임이 끝났는지 판단하는 변수이다.
        if(game_state){
            com_vs_ticket++;
            sem_post(&for_com_vs);
            return NULL;
        }
        com_vs_map[row][col] = ddong;
        if(row > global_x.load()){
            global_x.store(row, memory_order_relaxed);
            global_y.store(col, memory_order_relaxed);
        }
        row++;
        usleep(down_time);
    }

    com_vs_map[row - 1][col] = ' ';
    if(global_x.load() >= ROW - 1){
        global_x.store(0, memory_order_relaxed);
        global_y.store(15, memory_order_relaxed);
    }
    com_vs_ticket++;
    sem_post(&for_com_vs);
}

void *com_input_key(void *arg){
    //컴퓨터가 움직이기 위한 시간을 정의한다.
    uniform_int_distribution<long long> d_time(1000, 1000000);
    uniform_int_distribution<int> move(1, 2);
    long long down_time = d_time(gen);

    while(true){
        //ddong을 피하게 한다.
        if(game_state) return NULL;
        int move_dir = move(gen);
        if(cur_com_y == global_y.load()){
            move_dir = move(gen);
            if(move_dir == 1 && com_vs_map[cur_com_x][cur_com_y - 1] != '*'){
                com_vs_map[cur_com_x][cur_com_y] = ' ';
                cur_com_y--;
                com_vs_map[cur_com_x][cur_com_y] = 'C';
            }
            else if(move_dir == 2 && com_vs_map[cur_com_x][cur_com_y + 1] != '*'){
                com_vs_map[cur_com_x][cur_com_y] = ' ';
                cur_com_y++;
                com_vs_map[cur_com_x][cur_com_y] = 'C';
            }
            else if(com_vs_map[cur_com_x][cur_com_y - 1] != '*'){
                com_vs_map[cur_com_x][cur_com_y] = ' ';
                cur_com_y--;
                com_vs_map[cur_com_x][cur_com_y] = 'C';
            }
            else if(com_vs_map[cur_com_x][cur_com_y + 1] != '*'){
                com_vs_map[cur_com_x][cur_com_y] = ' ';
                cur_com_y++;
                com_vs_map[cur_com_x][cur_com_y] = 'C';
            }
        }
        //계속 움직이게 하기 위한 코드
        else{
            if(move_dir == 1 && com_vs_map[cur_com_x][cur_com_y - 1] != '*'){
                com_vs_map[cur_com_x][cur_com_y] = ' ';
                cur_com_y--;
                com_vs_map[cur_com_x][cur_com_y] = 'C';
            }
            else if(move_dir == 2 && com_vs_map[cur_com_x][cur_com_y + 1] != '*'){
                com_vs_map[cur_com_x][cur_com_y] = ' ';
                cur_com_y++;
                com_vs_map[cur_com_x][cur_com_y] = 'C';
            }
        }
        usleep(down_time);
    }
}

void *print_arr(void *arg){

    while(true){
        if(game_state) return NULL;
        if(!com_vs_processing)
            print_user_map();
        else
            print_com_vs_map();
        system("clear");
    }
}

void print_user_map(){
    for(int i = 0 ; i < ROW ; i++){
        for(int j = 0 ; j < COL ; j++){
            cout << arr[i][j];
        }
        cout << endl;
    }
}

void print_com_vs_map(){
    for(int i = 0 ; i < ROW ; i++){
        for(int j = 0 ; j < COL * 2 - 1 ; j++){
            cout << com_vs_map[i][j];
        }
        cout << endl;
    }
}