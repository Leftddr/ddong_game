from tkinter import *
import tkinter as tk
import numpy as np
from tkinter import messagebox
import sys
from PIL import ImageTk, Image
import time

root_window = None
frame = None
label = None
btn = None
game_board = []
bomb_cnt = 20
row = None
col = None
btn_size = 0
radio_val = None
radio_top = None
top_label = None

def make_window(win_height = 600, win_width = 600):
    global root_window
    root_window = Tk()
    root_window.title("지뢰찾기")
    root_window.geometry("%dx%d" % (win_height, win_width))

def make_frame(win_height = 600, win_width = 600, btn_size = 30):
    global root_window
    global frame
    global btn
    global row
    global col
    global label
    global top_label

    top_frame = Frame(root_window, height = 10, width = 600)
    top_frame.pack_propagate(False)
    top_label = Label(top_frame)
    top_label['text'] = '남은 지뢰 수 : ' + str(bomb_cnt)
    top_label.pack(fill = tk.BOTH, expand = tk.YES)

    loop_row = int(win_height / btn_size)
    loop_col = int(win_width / btn_size)

    frame = [[0] * loop_row for _ in range(loop_col)]
    btn = [[0] * loop_row for _ in range(loop_col)]
    label = [[0] * loop_row for _ in range(loop_col)] 

    make_game_board(loop_row, loop_col)
    row = loop_row
    col = loop_col

    for i in range(0, loop_row):
        for j in range(0, loop_col):
            frame[i][j] = Frame(root_window, height = btn_size, width = btn_size, padx = 0, pady = 0, relief='sunken', bd = 2)
            frame[i][j].pack_propagate(False)
            frame[i][j].grid(row = i + 1, column = j)

            btn[i][j] = Button(frame[i][j], command = (lambda x_ = i, y_ = j : right_btn_click(x_, y_)))
            #버튼을 frame에 맞게 꽉 맞추는 코드이다.
            btn[i][j].bind('<Button-3>', (lambda event, x_ = i, y_ = j : left_btn_click(x_, y_)))
            btn[i][j].pack(fill = tk.BOTH, expand = tk.YES)

            #이미지를 보여주기 위한 코드
            label[i][j] = Label(frame[i][j])

def make_radio_button():
    global radio_val
    global radio_top

    radio_top = Tk()
    radio_top.title("지뢰 찾기")
    radio_top.geometry("300x200")

    radio_val = IntVar()
    lb = Label(radio_top, text = "난이도를 선택하세요")
    lb.pack()

    #상, 중, 하 여서 3개
    for i in range(0, 3):
        string = ""
        if i == 0:
            string = "상 30 x 30 크기에, 지뢰가 200개"
        elif i == 1:
            string = "중 20 x 20 크기에, 지뢰가 100개"
        else:
            string = "하 10 x 10 크기에, 지뢰가 30개"
        r = Radiobutton(radio_top, text = string, variable = radio_val, value = str(i + 1), command = select)
        r.pack(anchor = W)
    
    radio_top.mainloop()

def select():
    global radio_val
    global btn_size
    global bomb_cnt
    global radio_top

    if radio_val.get() == 1:
        btn_size = 20
        bomb_cnt = 200
    elif radio_val.get() == 2:
        btn_size = 30
        bomb_cnt = 100
    elif radio_val.get() == 3:
        btn_size = 60
        bomb_cnt = 30
    
    radio_top.destroy()

def make_game_board(row, col):
    global game_board
    global label
    global frame

    game_board = [[0] * row for _ in range(col)]
    flag_cnt = bomb_cnt

    pair = set()
    while len(list(pair)) <= (bomb_cnt + flag_cnt):
        x = np.random.randint(row)
        y = np.random.randint(col)
        pair.add((x, y))
    
    pair = list(pair)
    for idx, p in enumerate(pair):
        if idx > bomb_cnt:
            game_board[p[0]][p[1]] = -2
            label[p[0]][p[1]] = Label(frame[p[0]][p[1]], image = tk.PhotoImage('./flag.png'))
        else:
            game_board[p[0]][p[1]] = -1
            label[p[0]][p[1]] = Label(frame[p[0]][p[1]], image = tk.PhotoImage('./bomb.png'))

    cnt_bomb(row, col)

def cnt_bomb(row, col):
    global game_board

    for i in range(row):
        for j in range(col):
            if i - 1 >= 0 and game_board[i - 1][j] == -1:
                game_board[i][j] += 1
            if i + 1 < row and game_board[i + 1][j] == -1:
                game_board[i][j] += 1
            if j - 1 >= 0 and game_board[i][j - 1] == -1:
                game_board[i][j] += 1
            if j + 1 < col and game_board[i][j + 1] == -1:
                game_board[i][j] += 1

            if i - 1 >= 0 and j - 1 >= 0 and game_board[i - 1][j - 1] == -1:
                game_board[i][j] += 1
            if i + 1 < row and j - 1 >= 0 and game_board[i + 1][j - 1] == -1:
                game_board[i][j] += 1
            if i - 1 >= 0 and j + 1 < col and game_board[i - 1][j + 1] == -1:
                game_board[i][j] += 1
            if i + 1 < row and j + 1 < col and game_board[i + 1][j + 1] == -1:
                game_board[i][j] += 1

def right_btn_click(x, y):
    global game_board
    global btn
    global label

    if game_board[x][y] == -9:
        return

    #이게 지뢰를 밟은 경우
    if game_board[x][y] == -1:
        btn[x][y].pack_forget()
        label[x][y].pack()
        '''
        msg_box = tk.messagebox.askyesno('You Lose', 'Restart Game?')
        if msg_box == True:
            make_clean()
            restart_app()
        else:
            sys.exit(1)
        '''
        return

    #이게 플래그를 밟은 경우이다.
    elif game_board[x][y] == -2:
        btn[x][y]['bg'] = 'blue'
        click_flag(x, y)
        return

    btn[x][y]['text'] = str(game_board[x][y])
    game_board[x][y] = -9

def left_btn_click(x, y):
    global game_board
    global btn

    if game_board[x][y] == -9:
        return

    #이게 지뢰를 밟은 경우
    if game_board[x][y] == -1:
        btn[x][y]['text'] = 'F' 
        return

    #이게 플래그를 밟은 경우이다.
    elif game_board[x][y] == -2:
        btn[x][y]['bg'] = 'blue'
        click_flag(x, y)
        return

    btn[x][y]['text'] = str(game_board[x][y])
    game_board[x][y] = -9

def click_flag(i, j):
    global game_board
    global btn
    global row
    global col

    #우선 -9로 맞추어준다.
    game_board[i][j] = -9

    if i - 1 >= 0 and game_board[i - 1][j] == -2:
        click_flag(i - 1, j)
    if i + 1 < row and game_board[i + 1][j] == -2:
        click_flag(i + 1, j)
    if j - 1 >= 0 and game_board[i][j - 1] == -2:
        click_flag(i, j - 1)
    if j + 1 < col and game_board[i][j + 1] == -2:
        click_flag(i, j + 1)

    if i - 1 >= 0 and j - 1 >= 0 and game_board[i - 1][j - 1] == -2:
        click_flag(i - 1, j - 1)
    if i + 1 < row and j - 1 >= 0 and game_board[i + 1][j - 1] == -2:
        click_flag(i + 1, j - 1)
    if i - 1 >= 0 and j + 1 < col and game_board[i - 1][j + 1] == -2:
        click_flag(i - 1, j + 1)
    if i + 1 < row and j + 1 < col and game_board[i + 1][j + 1] == -2:
        click_flag(i + 1, j + 1)
    
    show_bomb_cnt(i, j)

def show_bomb_cnt(i, j):
    global row
    global col

    if i - 1 >= 0 and game_board[i - 1][j] != -9:
        btn[i - 1][j]['text'] = str(game_board[i - 1][j])
        game_board[i - 1][j] = -9
    if i + 1 < row and game_board[i + 1][j] != -9:
        btn[i + 1][j]['text'] = str(game_board[i + 1][j])
        game_board[i + 1][j] = -9
    if j - 1 >= 0 and game_board[i][j - 1] != -9:
        btn[i][j - 1]['text'] = str(game_board[i][j - 1])
        game_board[i][j - 1] = -9
    if j + 1 < col and game_board[i][j + 1] != -9:
        btn[i][j + 1]['text'] = str(game_board[i][j + 1])
        game_board[i][j + 1] = -9

    if i - 1 >= 0 and j - 1 >= 0 and game_board[i - 1][j - 1] != -9:
        btn[i - 1][j - 1]['text'] = str(game_board[i - 1][j - 1])
        game_board[i - 1][j - 1] = -9
    if i + 1 < row and j - 1 >= 0 and game_board[i + 1][j - 1] != -9:
        btn[i + 1][j - 1]['text'] = str(game_board[i + 1][j - 1])
        game_board[i + 1][j - 1] = -9
    if i - 1 >= 0 and j + 1 < col and game_board[i - 1][j + 1] != -9:
        btn[i - 1][j + 1]['text'] = str(game_board[i - 1][j + 1])
        game_board[i - 1][j + 1] = -9
    if i + 1 < row and j + 1 < col and game_board[i + 1][j + 1] != -9:
        btn[i + 1][j + 1]['text'] = str(game_board[i + 1][j + 1])
        game_board[i + 1][j + 1] = -9

def make_clean():
    global game_board
    global btn
    global frame

    game_board = []
    btn = []
    frame = None

def restart_app():
    make_frame()

if __name__ == "__main__":
    make_radio_button()
    make_window()
    make_frame(btn_size = btn_size)
    root_window.mainloop()