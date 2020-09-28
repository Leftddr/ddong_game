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
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', '3', '.', 'e', 'x', 'i', 't', ' '},
    {'*', ' ', ' ', '-', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', '(', 'q', ')'},
    {'*', ' ', '/', ' ', '\\', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '|', ' ', ' ', ' ', '|', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', '\\', ' ', '/', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', '-', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', ' ', ' ', ' ', ' ', ' ', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'*', '*', '*', '*', '*', '*', '*', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
};

//스레드를 만든다.
void *down_thread(void*);
void *print_arr(void*);
void *input_key(void*);
void start_menu_print();
void show_score();
void print();
char ddong = '-';

//세마포어를 위한 것
sem_t semaphore;
sem_t for_print;
atomic<int> ticket(12);
int cur_p_x = ROW - 2, cur_p_y = COL / 2;
bool game_state = false;

//random 함수를 쓰기 위한 것이다.
random_device rd;
mt19937_64 gen(rd());

//점수를 저장하기 위한 벡터
vector<string> score;
//점수를 포함하기 위한것
atomic<int> atomic_score(0);

int main(int argc, char *argv[]){
    //세마포어 초기화
    sem_init(&semaphore, 0, ticket);
    sem_init(&for_print, 0, 1);

    //random함수 초기화
    uniform_int_distribution<int> dist(1, COL - 2);
    uniform_int_distribution<int> s_time(1, 2);

    //시작 메뉴를 보여준다.
    while(true){
        while(true){
            system("clear");
            start_menu_print();
            char key = getch_();

            if(key == '2'){
                cout << "점수 보여주기" << endl;
                show_score();
            }
            else if(key == 'q'){
                return 0;
            }
            else if(key =='1'){
                cout << "GAME START" << endl;
                sleep(2);
                break;
            }
        }

        //사람 초기화
        arr[cur_p_x][cur_p_y] = 'A';
        //필요 스레드, 입력스레드, 출력스레드 초기화
        pthread_t for_print_thread;
        pthread_create(&for_print_thread, NULL, print_arr, NULL);
        pthread_t for_input_key;
        pthread_create(&for_input_key, NULL, input_key, NULL);
        
        while(true){
            if(ticket == 0) continue;
            else{
                int col = dist(gen);
                pthread_t thread1;
                pthread_create(&thread1, NULL, down_thread, (void*)&col);
                //pthread_join(thread1, NULL);
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

void *down_thread(void *arg){
    ticket--;
    sem_wait(&semaphore);
    int col = *(int*)arg;
    int row = 0;

    uniform_int_distribution<long long> d_time(1000, 1000000);
    long long down_time = d_time(gen);

    while(row < ROW - 1){
        if(row > 0){
            arr[row - 1][col] = ' ';
        }
        if(arr[row][col] == 'A'){
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
        arr[row][col] = ddong;
        row++;
        usleep(down_time);
        
        //떨어지는 속도를 각기 다르게 한다.
    }
    arr[row - 1][col] = ' ';
    ticket++;
    atomic_score += 10;
    sem_post(&semaphore);
}

void *print_arr(void *arg){
    while(true){
        if(game_state) return NULL;
        print();
        system("clear");
    }
}

void *input_key(void *arg){
    while(true){
        if(game_state) return NULL;
        char ch = getch_();
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
}

void print(){
    for(int i = 0 ; i < ROW ; i++){
        for(int j = 0 ; j < COL ; j++){
            cout << arr[i][j];
        }
        cout << endl;
    }
}