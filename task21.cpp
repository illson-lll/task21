/* ----------------------------------------------------------------<Header>-
 Name: task21.cpp
 Title: Завдання №21.
 Group: ТВ-41
 Student: Бобровнік І.А.
 Written: 2025-05-12
 Revised: 2025-05-17
 Description:
Розробіть алгоритм вирішення задачі та реалізуйте його у вигляді
програми мовою ANSI C++.
Напишіть у ĸожній порожній ĸлітині число; ĸожне число на
ігровому полі має стати частиною безперервної області, ĸільĸість
ĸлітин яĸої відповідає значенню цього числа. Область вважається
безперервною, яĸщо дві ĸлітини стиĸаються одна з одною
стороною. Дві області з однаĸовою ĸільĸістю ĸлітин не можуть
стиĸатися одна з одною.
 ------------------------------------------------------------------</Header>-*/

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cstring>
#include <climits>
using namespace std;

const int W = 11;
const int H = 11;
int problem[W][H] = {
    {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 4, 0, 0, 0, 0, 0, 21, 0},
    {0, 0, 0, 20, 20, 0, 0, 0, 0, 0, 0},
    {0, 4, 20, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 22, 3, 0},
    {0, 0, 0, 0, 0, 0, 22, 22, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 6, 0, 5, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
}; // Задвання
int init[W][H] = {}; // Допоміжний масив для зберігання початкового стану
int g_max;           // Глобальна змінна, зберігає найбільше число в масиві
int safe[W][H] = {}; // Допоміжний масив, зберігаються позиції які не потрібно переглядати.

void print_grid(int m[W][H])
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            if (problem[i][j] != 0)
                printf("%2d ", m[i][j]);
            else
                printf(" - ");
        }
        cout << endl;
    }
}
/* ---------------------------------------------------------------------[<]-
 Function: print_grid
 Synopsis: Виводить заданий масив в консоль у відформатованому вигляді.
 ---------------------------------------------------------------------[>]-*/

void copy(int a[W][H], int b[W][H])
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            a[i][j] = b[i][j];
        }
    }
}
/* ---------------------------------------------------------------------[<]-
 Function: copy
 Synopsis: Копією масив.
 ---------------------------------------------------------------------[>]-*/

struct pos
{
    int x, y;
    int i;
    int operator==(const pos &b) const
    {
        if (x == b.x && y == b.y)
            return 1;
        return 0;
    }
    int operator==(int a) const
    {
        return problem[x][y] == a;
    }

    int operator!=(int a) const
    {
        return problem[x][y] != a;
    }
    void operator=(int a)
    {
        problem[x][y] = a;
    }
    int operator>(const pos &a) const
    {
        return problem[x][y] > problem[a.x][a.y];
    }
    int operator<(const pos &a) const
    {
        return problem[x][y] < problem[a.x][a.y];
    }
    int value() const
    {
        return problem[x][y];
    }
    int &value()
    {
        return problem[x][y];
    }
};

struct pos_comp
{
    bool operator()(const pos &a, const pos &b) const
    {
        if (a.value() != b.value())
            return a > b;
        return a.i > b.i;
    }
};

/* ---------------------------------------------------------------------[<]-
 Struct: pos
 Synopsis: Структура для полегшення синтаксису, зберігає координати
 та індекс(детальніше в пояснювальній записці).
 ---------------------------------------------------------------------[>]-*/

int is_available(int x, int y)
{ //
    if (x < W && x >= 0 && y < H && y >= 0)
    {
        return 1;
    }
    return 0;
}
/* ---------------------------------------------------------------------[<]-
 Function: is_available
 Synopsis: Перевіряє чи не вийшли заданні координати за межі границь
 ---------------------------------------------------------------------[>]-*/
vector<pos> get_nearby(int x, int y)
{
    vector<pos> r;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; i++)
    {
        int xn = x + dx[i];
        int yn = y + dy[i];
        if (is_available(xn, yn))
        {
            r.push_back({xn, yn, 0});
        }
    }
    return r;
}
/* ---------------------------------------------------------------------[<]-
 Function: get_nearby
 Synopsis: Повертає позиції сусідніх та доступних клітинок.
 ---------------------------------------------------------------------[>]-*/

void set_safe(vector<pos> &a)
{
    for (pos p : a)
    {
        safe[p.x][p.y] = 1;
    }
}
/* ---------------------------------------------------------------------[<]-
 Function: set_safe
 Synopsis: Помічає список заданих позицій безпечними.
 ---------------------------------------------------------------------[>]-*/
int is_contains(vector<pos> &v, const pos &p)
{
    for (pos g : v)
    {
        if (g.x == p.x && g.y == p.y)
            return 1;
    }
    return 0;
}
/* ---------------------------------------------------------------------[<]-
 Function: is_contains
 Synopsis: Перевіряє чи присутня задана позиція в списку.
 ---------------------------------------------------------------------[>]-*/

vector<pos> get_region(int x, int y, vector<pos> &checked)
{
    int num = problem[x][y];

    checked.push_back({x, y, 0});

    for (pos p : get_nearby(x, y))
    {
        if (problem[p.x][p.y] == num && !is_contains(checked, p))
            checked = get_region(p.x, p.y, checked);
    }
    return checked;
}
/* ---------------------------------------------------------------------[<]-
 Function: get_region
 Synopsis: Повертає список всіх позицій безперервної зони.
 ---------------------------------------------------------------------[>]-*/

class region
{
    /* ---------------------------------------------------------------------[<]-
    Class: region
    Synopsis: Відповідає за створення та розширення безперерверної зони розміром відповідного числа.
    Має допоміжні поля та методи для необхідних перевірок.
    ---------------------------------------------------------------------[>]-*/

    vector<pos> region_v;        // Список позицій регіону.
    set<pair<int, int>> checked; // Допоміжний set для priority, для уникнення дублікату.
    priority_queue<pos, vector<pos>, pos_comp> priority;
    /*
    Приріотетна черга з позицій для розширення регіону,
    сортування працює через оператор pos_comp
    */
    vector<pos> conflict;
    /*
    Допоміжний список, слугує в разі не вдачі створення регіону.
    Зберігає позиції сусідніх регіонів.
    */
    vector<pos> unwanted;
    /*
    Допоміжний список з неохочих нових позицій для розширення регіону
    */
    int index;

    int check_difference(int x, int y, int num)
    {
        for (pos p : get_nearby(x, y))
        {
            if (problem[p.x][p.y] > 0 && p != num && abs(p.value() - num) == 1)
            {
                if (!is_contains(conflict, p))
                {
                    conflict.push_back(p);
                }
                return 0;
            }
        }
        return 1;
    }
    /* ---------------------------------------------------------------------[<]-
    Function: check_difference
    Synopsis: Перевіря різницю нового числа та сусідніх регіонів.
    ---------------------------------------------------------------------[>]-*/

    int check_safe(int x, int y, int num)
    { //
        for (pos p : get_nearby(x, y))
        {
            if (p == num && safe[p.x][p.y])
                return 0;
        }
        return 1;
    }
    /* ---------------------------------------------------------------------[<]-
    Function: check_safe
    Synopsis: Перевіряє чи не торкається з безпечними регіоном того ж числа регіону.
    ---------------------------------------------------------------------[>]-*/

    int check_priority(int x, int y, int num)
    {
        for (pos p : get_nearby(x, y))
        {
            if ((p == num && !is_contains(region_v, p)) || (p == -1 && !checked.count({p.x, p.y})))
                return 1;
        }
        return 0;
    }
    /* ---------------------------------------------------------------------[<]-
    Function: check_priority
    Synopsis: Переглядає приріотетність позиції.
    ---------------------------------------------------------------------[>]-*/

    int check_valid(int x, int y, int num)
    {
        if (!check_difference(x, y, num))
        {
            return 0;
        }
        if (!check_safe(x, y, num))
        {
            return 0;
        }
        problem[x][y] = num;
        vector<pos> t;
        t = get_region(x, y, t);
        if ((int)t.size() > num)
        {
            problem[x][y] = 0;
            return 0;
        }
        return 1;
    }
    /* ---------------------------------------------------------------------[<]-
    Function: check_valid
    Synopsis: Переглядає валідність розширення регіону на заданній позиції.
    ---------------------------------------------------------------------[>]-*/

    void set_init(vector<pos> &l)
    {
        for (pos p : l)
        {
            p = init[p.x][p.y];
            safe[p.x][p.y] = 0;
        }
    }
    /* ---------------------------------------------------------------------[<]-
    Function: set_init
    Synopsis: Повертає значеннях заданих позицій до початкового стану (init).
    ---------------------------------------------------------------------[>]-*/

    void destroy(pos p)
    {
        vector<pos> s;
        s = get_region(p.x, p.y, s);
        set_init(s);
    }
    /* ---------------------------------------------------------------------[<]-
    Function: destroy
    Synopsis: Повертає регіон (до якого належить задана позиція) до початкового стану.
    ---------------------------------------------------------------------[>]-*/

    vector<pos> expand_region(int x, int y)
    {
        vector<pos> t;
        int num = problem[x][y];
        if (!is_contains(region_v, {x, y, 0}))
            region_v = get_region(x, y, t);
        if ((int)region_v.size() >= num)
            return region_v;

        for (pos p : get_nearby(x, y))
        {
            if (p != num && problem[p.x][p.y] > 1)
            {
                if (!is_contains(conflict, p))
                {
                    if (priority.empty())
                        conflict.insert(conflict.begin(), p);
                    else
                        conflict.push_back(p);
                    if (!is_contains(unwanted, {p.x, p.y, 0}))
                        unwanted.push_back({p.x, p.y, 0});
                }
            }
            else if (p == num)
            {
                if (!checked.count({p.x, p.y}))
                {
                    checked.insert({p.x, p.y});
                    p.i = index++;
                    priority.push(p);
                }
            }
            else if (p == 0 || p == -1 || p == -2)
            {
                if (!checked.count({p.x, p.y}))
                {

                    checked.insert({p.x, p.y});

                    if (check_priority(p.x, p.y, num))
                        p = -1;

                    p.i = index++;

                    if (is_contains(unwanted, {p.x, p.y, 0}))
                    {
                        p = 0;
                        p.i *= 10;
                    }
                    priority.push(p);
                }
            }
        }
        pos p;
        int is_expanded = 0;
        do
        {

            if (!priority.empty())
            {
                p = priority.top();
                priority.pop();
            }
            else
            {
                return region_v;
            }

            if (check_valid(p.x, p.y, num))
            {
                expand_region(p.x, p.y);
                is_expanded = 1;
            }
        } while (!is_expanded);

        return region_v;
    }
    /* ---------------------------------------------------------------------[<]-
    Function: expand_region
    Synopsis: Розширює регіон на заданій позиції.
    ---------------------------------------------------------------------[>]-*/
    void clear(int x, int y)
    {
        while (!priority.empty())
            priority.pop();
        checked.clear();
        vector<pos> t;
        index = 0;
        region_v = get_region(x, y, t);
    }
    /* ---------------------------------------------------------------------[<]-
    Function: clear
    Synopsis: Скидує всі поля класу.
    ---------------------------------------------------------------------[>]-*/
public:
    int create_region(int x, int y, int num, int allowed = 1)
    {
        clear(x, y);
        expand_region(x, y);

        if ((int)region_v.size() != num)
        {
            if (!allowed)
            {
                set_init(region_v);
                return 0;
            }

            for (pos c : conflict)
            {
                if (c != 0)
                {
                    destroy(c);
                    set_init(region_v);
                    clear(x, y);
                    expand_region(x, y);
                    if ((int)region_v.size() == num)
                    {
                        set_safe(region_v);
                        return 1;
                    }
                }
            }
        }

        if ((int)region_v.size() == num)
        {
            set_safe(region_v);
            return 1;
        }
        return 0;
    }
    /* ---------------------------------------------------------------------[<]-
    Function: create_region
    Synopsis: Створює та розширює регіон, якщо при заваді та при дозволі(allowed)
    знищує конфліктові регіони та перестворює його ще раз.
    В разі невдачі, відновлює всі позиції до початкового стану та повертає 0.
    ---------------------------------------------------------------------[>]-*/
};
/* ---------------------------------------------------------------------[<]-
Class: region
Synopsis: Відповідає за створення та розширення безперерверної зони розміром відповідного числа.
Має допоміжні поля та методи для необхідних перевірок.
---------------------------------------------------------------------[>]-*/

int is_safe_numbers()
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            if (problem[i][j] > 1 && !safe[i][j])
                return 0;
        }
    }
    return 1;
}
/* ---------------------------------------------------------------------[<]-
Function: is_safe_numbers
Synopsis: Перевіряє чи всі регіони чисел створились.
---------------------------------------------------------------------[>]-*/
int is_safe_zero()
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            if (problem[i][j] == 0)
                return 0;
        }
    }
    return 1;
}
/* ---------------------------------------------------------------------[<]-
Function: is_safe_zero
Synopsis: Перевіряє присутність незаповнених клітинок.
---------------------------------------------------------------------[>]-*/
int solve_numbers()
{

    for (int n = 2; n <= g_max; n++)
    {
        for (int i = 0; i < W; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (problem[i][j] == n && !safe[i][j]) //
                {
                    vector<pos> s;
                    s = get_region(i, j, s);
                    if ((int)s.size() == problem[i][j])
                    {
                        set_safe(s);
                    }
                    else
                    {
                        region newreg;
                        newreg.create_region(i, j, problem[i][j]);
                    }
                }
            }
        }
    }
    return is_safe_numbers();
}
/* ---------------------------------------------------------------------[<]-
Function: solve_numbers
Synopsis: Створює та розширює регіони. Повертає is_safe_numbers().
---------------------------------------------------------------------[>]-*/
int solve_zeros()
{
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (problem[i][j] == -1 || problem[i][j] == -2)
            {
                problem[i][j] = 0;
            }
            if (problem[i][j] == 0)
            {
                vector<pos> s;
                s = get_region(i, j, s);
                int size = s.size();
                for (int num = size; num >= 1; num--)
                {
                    problem[i][j] = num;
                    region newreg;
                    if (newreg.create_region(i, j, num, 0))
                    {
                        break;
                    }
                }
            }
        }
    }
    return is_safe_zero();
}
/* ---------------------------------------------------------------------[<]-
Function: solve_zeros
Synopsis: Заповнює пусті клітинки. Повертає is_safe_zeros().
---------------------------------------------------------------------[>]-*/

int task21()
{
    if (!problem[0][0])
        problem[0][0] = -2;
    if (!problem[W - 1][0])
        problem[W - 1][0] = -2;
    if (!problem[0][H - 1])
        problem[0][H - 1] = -2;
    if (!problem[W - 1][H - 1])
        problem[W - 1][H - 1] = -2;

    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (problem[i][j] == 1)
            {
                safe[i][j] = 1;
                for (pos p : get_nearby(i, j))
                {
                    if (p == 0)
                        p = -1;
                }
            }
        }
    }

    copy(init, problem);

    g_max = INT_MIN;
    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < H; j++)
        {
            if (problem[i][j] > g_max)
            {
                g_max = problem[i][j];
            }
        }
    }

    int iter = 0;
    while (!solve_numbers())
    {
        iter++;
        if (iter == 20)
            break;
    }

    int zero_iter = 0;

    while (!solve_zeros())
    {
        zero_iter++;
        if (zero_iter == 5)
            break;
    }
    return iter;
}
/* ---------------------------------------------------------------------[<]-
Function: task21
Synopsis: Робить кути та всі нулі навколо 1 приріотетними.
Ітераційно заповнює весь массив числами.
---------------------------------------------------------------------[>]-*/
int main()
{
    cout << "Problem: " << endl;
    print_grid(problem);
    int iter = task21();
    cout << "Iteration count: " << iter << endl;
    cout << "Solve: " << endl;
    print_grid(problem);
    // cout << "Safe: " << endl;
    // print_grid(safe);
    return 0;
}
/* ---------------------------------------------------------------------[<]-
Function: main
Synopsis: Виводить задачу, її рішення, та кількість необхідних ітерацій.
---------------------------------------------------------------------[>]-*/