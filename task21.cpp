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

const int w = 5;
const int h = 5;
int problem[w][h] = {
    {4, 0, 3, 3, 3},
    {4, 0, 5, 2, 0},
    {6, 6, 5, 2, 0},
    {6, 6, 5, 5, 0},
    {6, 6, 5, 0, 0},
};

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
        if (problem[x][y] == a)
            return 1;
        return 0;
    }
};
void setsafe(vector<pos> &a)
{
    for (pos p : a)
    {
        safe[p.x][p.y] = 1;
    }
}

int available(int x, int y)
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
    int dx[] = {-1, 1, 0, 0};
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

int check_region(int x, int y, int num = -1) // шукає всі позиції регіона, та повертає його розмір
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

    return checked.size();
}

void create_region(int x, int y,int num = -1)
{
    if (num == -1)
        num = problem[x][y];
    else
        checked.clear(); 
}

void task21()
{
    for (int n = 1; n < 9; n++)
    {
        for (int i = 0; i < w; i++) 
        {
            for (int j = 0; j < w; j++)
            {
                if (problem[i][j] == n && !safe[i][j])
                {
                    int s = check_region(i, j, problem[i][j]);
                    if (s == problem[i][j])
                    {
                        cout << "SAFE " << problem[i][j] << endl;
                        setsafe(checked);
                    }
                    else{
                        cout << "UNSAFE " << problem[i][j] << endl;
                        create_region(i, j);
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
            printf("%2d ", problem[i][j]);
        }
        cout << endl;
    }
}

int main()
{
    cout << "Problem: " << endl;
    print_problem();

    // int b = check_region(0, 0, 4);
    // cout << "B: " << b << endl;
    task21();
    cout << "Solve: " << endl;
    print_problem();
    return 0;
}