#include <iostream>
#include <vector>
using namespace std;

const int w = 9;
const int h = 9;

int problem[w][h] = {
    {0, 5, 0, 0, 0, 0, 0, 1, 0},
    {0, 7, 0, 0, 0, 0, 0, 2, 0},
    {0, 0, 7, 0, 7, 0, 5, 0, 0},
    {0, 0, 0, 5, 0, 5, 0, 0, 0},
    {0, 7, 0, 0, 0, 0, 0, 3, 0},
    {0, 6, 0, 0, 3, 0, 0, 7, 0},
    {0, 0, 0, 5, 0, 5, 0, 0, 0},
    {1, 2, 0, 4, 0, 4, 0, 2, 1},
    {0, 0, 0, 0, 3, 0, 0, 0, 0},
};

struct pos
{
    int x;
    int y;
};
int available(int x, int y)
{
    if (x < w && x >= 0 && y < h && y >= 0)
    {
        if (problem[x][y] == 0 || problem[x][y] == -1)
        {
            return 1;
        }
    }
    return 0;
}

vector<pos> nearby(int x, int y)
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

void create_region(int x, int y)
{
    int number = problem[x][y];;
    if (number == 1)
    {
        vector<pos> nrb = nearby(x, y);
        if (!nrb.empty())
        {
            for (pos p : nrb)
            {
                problem[p.x][p.y] = -1;
            }
        }
    }
}

void task21()
{
    int n = 1;
    for (int i = 0; i < w; i++) // Перебирання всіх клітинок
    {
        for (int j = 0; j < w; j++)
        {
            if (problem[i][j] == n)
            {
                create_region(i, j);
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
    task21();
    cout << "Solve: " << endl;
    print_problem();
    return 0;
}