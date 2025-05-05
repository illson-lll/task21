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
    {3, 0, 0, 5, 0},
    {0, 1, 0, 0, 2},
    {3, 4, 0, 2, 0},
    {4, 0, 0, 3, 0},
    {0, 4, 3, 0, 4},
};
// int problem[w][h] = {
//     {0, 0, 0, 3, 3},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 3},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0}};
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
    void operator=(int a)
    {
        problem[x][y] = a;
    }
    friend ostream &operator<<(ostream &stream, const pos &p);
};
ostream &operator<<(ostream &stream, const pos &p)
{
    stream << "Pos: (" << p.x << ":" << p.y << "): " << problem[p.x][p.y] << endl;
    return stream;
}
void mark_safe(vector<pos> &a)
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

void check_subreg(int x, int y, int num)
{
    problem[x][y] = num;

    if((int)check_region(x,y,num).size() > num){
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
        region.clear();
    }
    region.push_back({x, y});

    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if (p == num && !in_list(region, p))
        {

            create_region(p.x, p.y);
        }
        if (p == 0)
        {
            check_subreg(p.x,p.y,num);
            if(p==num)
                create_region(p.x,p.y);
        }
    }

    
    return region;
}

void task21()
{
    for (int n = 10; n != 0; n--)
    {
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < w; j++)
            {
                if (problem[i][j] == n && !safe[i][j])
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