#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
// const int w = 9;
// const int h = 9;

// int problem[w][h] = {
//     {0, 5, 0, 0, 0, 0, 0, 1, 0},
//     {0, 7, 0, 0, 0, 0, 0, 2, 0},
//     {0, 0, 7, 0, 7, 0, 5, 0, 0},
//     {0, 0, 0, 5, 0, 5, 0, 0, 0},
//     {0, 7, 0, 0, 0, 0, 0, 3, 0},
//     {0, 6, 0, 0, 3, 0, 0, 7, 0},
//     {0, 0, 0, 5, 0, 5, 0, 0, 0},
//     {1, 2, 0, 4, 0, 4, 0, 2, 1},
//     {0, 0, 0, 0, 3, 0, 0, 0, 0},
// };

// const int w = 5;
// const int h = 5;
// int problem[w][h] = {
//     {3, 0, 0, 5, 0},
//     {0, 1, 0, 0, 2},
//     {3, 4, 0, 2, 0},
//     {4, 0, 0, 3, 0},
//     {0, 4, 3, 0, 4},
// };
// int problem[w][h] = {
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0}};


const int w = 6;
const int h = 6;
int problem[w][h] = {
    {0, 0, 0, 0, 5, 0},
    {5, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 6, 0, 0, 0},
    {4, 0, 0, 0, 0, 0},
    {0, 3, 0, 0, 2, 0},
};

// const int w = 11;
// const int h = 11;
// int problem[w][h] = {
//     {5, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 1, 0, 4, 0, 0, 0, 0, 21, 0},
//     {0, 0, 0, 20, 20, 0, 0, 0, 0, 0},
//     {0, 4, 20, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 20, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 22, 0, 0},
//     {0, 0, 0, 0, 0, 0, 0, 22, 3, 0},
//     {0, 0, 0, 0, 0, 22, 22, 0, 0, 0},
//     {0, 21, 0, 0, 0, 0, 6, 0, 5, 0},
//     {0, 0, 0, 0, 0, 0, 0, 0, 0, 7}};

int safe[w][h] = {}; // Допоміжний масив, зберігаються позиції які не потрібно переглядати.

struct pos             
{
    int x;
    int y;
    int operator==(const pos &b)
    {
        if (x == b.x && y == b.y)
            return 1;
        return 0;
    }
    int operator==(int a)
    {
        return problem[x][y] == a;
    }

    int operator!=(int a)
    {
        return problem[x][y] != a;
    }
    void operator=(int a)
    {
        problem[x][y] = a;
    }
    int operator>(pos a)
    {
        return problem[x][y] > problem[a.x][a.y];
    }
    int operator<(pos a)
    {
        return problem[x][y] < problem[a.x][a.y];
    }
    int &value()
    {
        return problem[x][y];
    }
    friend ostream &operator<<(ostream &stream, const pos &p);
};
ostream &operator<<(ostream &stream, const pos &p)
{
    stream << "Pos: (" << p.x << ":" << p.y << "): " << problem[p.x][p.y] << endl;
    return stream;
}

void mark_safe(vector<pos> &a) // Помічає список заданих позицій безпечними
{
    for (pos p : a)
    {
        safe[p.x][p.y] = 1;
    }
}

int available(int x, int y) // Перевіряє чи не вийшли координати за межі границь
{
    if (x < w && x >= 0 && y < h && y >= 0)
    {
        return 1;
    }
    return 0;
}

vector<pos> nearby(int x, int y) // Повертає позиції сусідніх та доступних клітинок
{
    vector<pos> r;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; i++)
    {
        int xn = x + dx[i];
        int yn = y + dy[i];
        if (available(xn, yn))
        {
            r.push_back({xn, yn});
        }
    }
    return r;
}

int in_list(vector<pos> &a, const pos &p)
{
    if (find(a.begin(), a.end(), p) != a.end())
        return 1;
    return 0;
}

vector<pos> checked; // Допоміжний масив, зберігає всі переглянуті позиції регіона.

vector<pos> check_region(int x, int y, int num = -1) // шукає всі позиції регіона
{
    if (num == -1)
        num = problem[x][y];
    else
        checked.clear(); // Якщо num задається, переглянуті позиції скидуються

    checked.push_back({x, y});

    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if (problem[p.x][p.y] == num && !in_list(checked, p))
            check_region(p.x, p.y);
    }
    return checked;
}

vector<pos> region;

int check_difference(int x, int y, int num) //Перевіряє різницю
{
    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if (p != 0 && p != num && abs(p.value() - num) == 1)
            return 0;
    }
    return 1;
}
int check_safe(int x, int y, int num){ //Перевіряє чи не торкається з безпечними регіоном того ж числа
    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if(p == num && safe[p.x][p.y])
            return 0;
    }
    return 1;
}

int check_subreg(int x, int y, int num){
    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if(p == num && !in_list(region,p))
            return 1; 
    }
    return 0;
}

void check_valid(int x, int y, int num) //Перевіряє чи можна ставити число замість нуля
{
    if (!check_difference(x, y, num) || !check_safe(x,y,num))
        return;
    problem[x][y] = num;
    if ((int)check_region(x, y, num).size() > num)
    {
        problem[x][y] = 0;
    }
}


vector<pos> create_region(int x, int y, int num = -1)
{
    if (num == -1)
    {
        num = problem[x][y];
    }
    else
    {
        region = check_region(x, y, num);
    }
    if (!in_list(region,{x, y}))
        region.push_back({x, y});
    if ((int)region.size() >= num)
        return region;
    vector<pos> priority;
    
    
    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if (p == num && !in_list(region, p))
        {

            create_region(p.x, p.y);
        }
        else if(p == 0){
            if(check_subreg(x,y,num))
                p = -3;
            priority.push_back(p);
        }
        else if (p == -1 || p == -2)
        {
            priority.push_back(p);
        }
    }
    sort(priority.begin(), priority.end());
    for (pos p : priority)
    {
        if (p == -1 || p == -2)
        {
            p = 0;
        }
        check_valid(p.x, p.y, num);
        if (p == num)
            create_region(p.x, p.y);       
    }
    return region;
}

void task21()
{
    if (!problem[0][0])
        problem[0][0] = -2;
    if (!problem[w - 1][0])
        problem[w - 1][0] = -2;
    if (!problem[0][h - 1])
        problem[0][h - 1] = -2;
    if (!problem[w - 1][h - 1])
        problem[w - 1][h - 1] = -2;

    for (int n = 1; n <= 22; n++)
    {
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < w; j++)
            {
                if (problem[i][j] == 1)
                {
                    safe[i][j] = 1;
                    vector<pos> nrb = nearby(i, j);
                    for (pos p : nrb)
                    {
                        if (p == 0)
                            p = -1;
                    }
                }
                else if (problem[i][j] == n && !safe[i][j])
                {
                    vector<pos> s;
                    s = check_region(i, j, problem[i][j]);
                    if ((int)s.size() == problem[i][j])
                    {
                        mark_safe(s);
                    }
                    else
                    {
                        s = create_region(i, j, problem[i][j]);
                        if ((int)s.size() == problem[i][j])
                            mark_safe(s);
                    }
                }
            }
        }
    }
}

void print_problem()
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (problem[i][j] != 0)
                printf("%2d ", problem[i][j]);
            else
                printf(" - ");
        }
        cout << endl;
    }
}
void print_marked()
{
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            if (safe[i][j] != 0)
                printf("%2d ", safe[i][j]);
            else
                printf(" - ");
        }
        cout << endl;
    }
}

int main()
{
    cout << "Problem: " << endl;
    print_problem();

    task21();
    cout << "Solve: " << endl;
    print_problem();
    cout << "Safe: " << endl;
    print_marked();

    return 0;
}