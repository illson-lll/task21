#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cstring>

using namespace std;

// int problem[w][h] = {
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0},
//     {0, 0, 0, 0, 0}};

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

// const int w = 6;
// const int h = 6;
// int problem[w][h] = {
//     {0, 0, 0, 0, 5, 0},
//     {5, 0, 0, 0, 0, 1},
//     {0, 0, 0, 0, 0, 0},
//     {0, 0, 6, 0, 0, 0},
//     {4, 0, 0, 0, 0, 0},
//     {0, 3, 0, 0, 2, 0},
// };

const int w = 11;
const int h = 11;
int problem[w][h] = {
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
};
int init[w][h] = {
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
};

int safe[w][h] = {}; // Допоміжний масив, зберігаються позиції які не потрібно переглядати.

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

struct pos
{
    int x;
    int y;
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
    int v() const
    {
        return problem[x][y];
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
int index = 0;

struct comppos
{
    bool operator()(const pos &a, const pos &b) const
    {
        if (a.v() != b.v())
            return a > b;
        return a.i > b.i;
    }
};

int available(int x, int y)
{ // Перевіряє чи не вийшли координати за межі границь
    if (x < w && x >= 0 && y < h && y >= 0)
    {
        return 1;
    }
    return 0;
}

vector<pos> nearby(int x, int y)
{ // Повертає позиції сусідніх та доступних клітинок
    vector<pos> r;
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; i++)
    {
        int xn = x + dx[i];
        int yn = y + dy[i];
        if (available(xn, yn))
        {
            r.push_back({xn, yn, 0});
        }
    }
    return r;
}
void mark_safe(vector<pos> &a)
{ // Помічає список заданих позицій безпечними
    for (pos p : a)
    {
        safe[p.x][p.y] = 1;
        // for (pos f : nearby(p.x, p.y))
        // {
        //     if (f == 0)
        //         f = -1;
        // }
    }
}
int contains(vector<pos> &v, const pos &p)
{ // Перевіряє наявність p в масиві
    for (pos g : v)
    {
        if (g.x == p.x && g.y == p.y)
            return 1;
    }
    return 0;
}

// Допоміжний масив, зберігає всі переглянуті позиції регіона.

vector<pos> check_region(int x, int y, vector<pos> checked, int num = -1)
{ // Шукає всі позиції регіона
    if (num == -1)
        num = problem[x][y];
    else
        checked.clear(); // Якщо num задається, переглянуті позиції скидуються

    checked.push_back({x, y, 0});

    vector<pos> nrb = nearby(x, y);
    for (pos p : nrb)
    {
        if (problem[p.x][p.y] == num && !contains(checked, p))
            checked = check_region(p.x, p.y, checked);
    }
    return checked;
}

class reg
{
    vector<pos> region;
    set<pair<int, int>> checked2;
    priority_queue<pos, vector<pos>, comppos> prior;
    vector<pos> conflict;

    pos head;
    int check_difference(int x, int y, int num)
    { // Перевіряє різницю
        vector<pos> nrb = nearby(x, y);
        for (pos p : nrb)
        {
            if (problem[p.x][p.y] > 0 && p != num && abs(p.value() - num) == 1)
            {
                if (!contains(conflict, p))
                {
                    conflict.push_back(p);
                }
                return 0;
            }
        }
        return 1;
    }
    int check_safe(int x, int y, int num)
    { // Перевіряє чи не торкається з безпечними регіоном того ж числа
        vector<pos> nrb = nearby(x, y);
        for (pos p : nrb)
        {
            if (p == num && safe[p.x][p.y])
                return 0;
        }
        return 1;
    }

    int check_subreg(int x, int y, int num)
    { // Перевіряє чи не торкається з іншим регіоном
        vector<pos> nrb = nearby(x, y);
        for (pos p : nrb)
        {
            if ((p == num && !contains(region, p)) || (p == -1 && !checked2.count({p.x, p.y})))
                return 1;
        }
        return 0;
    }
    void copy(int a[w][h], int b[w][h])
    {
        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                a[i][j] = b[i][j];
            }
        }
    }
    int check_block(int x, int y, int num)
    {
        problem[x][y] = num;
        for (pos p : nearby(x, y))
        {
            if (p != num && problem[p.x][p.y] > 1 && !safe[p.x][p.y])
            {
                int temp[w][h];
                int temp_safe[w][h];
                copy(temp, problem);
                copy(temp_safe, safe);
                mark_safe(region);
                reg newreg;
                vector<pos> nreg = newreg.create_region(p.x, p.y, problem[p.x][p.y]);
                copy(problem, temp);
                copy(safe, temp_safe);

                if ((int)nreg.size() != problem[p.x][p.y])
                {
                    problem[x][y] = 0;
                    return 0;
                }
            }
        }
        return 1;
    }
    int check_valid(int x, int y, int num)
    { // Перевіряє чи можна ставити число замість нуля
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
        t = check_region(x, y, t, num);
        if ((int)t.size() > num)
        {
            problem[x][y] = 0;
            return 0;
        }
        return 1;
    }
    void set_init(vector<pos> l)
    {
        for (pos p : l)
        {
            p = init[p.x][p.y];
            safe[p.x][p.y] = 0;
        }
    }
    vector<pos> destroy(pos p)
    {
        vector<pos> s;
        s = check_region(p.x, p.y, s, problem[p.x][p.y]);
        set_init(s);
    }
    vector<pos> expand_region(int x, int y)
    {
        vector<pos> t;
        int num = problem[x][y];
        if (!contains(region, {x, y, 0}))
            region = check_region(x, y, t, num);
        if ((int)region.size() >= num)
            return region;

        vector<pos> nrb = nearby(x, y);
        for (pos p : nrb)
        {
            if (p != num && problem[p.x][p.y] > 1)
            {
                if (!contains(conflict, p))
                {
                    conflict.push_back(p);
                }
            }
            else if (p == num)
            {
                if (!checked2.count({p.x, p.y}))
                {
                    checked2.insert({p.x, p.y});
                    p.i = index++;
                    prior.push(p);
                }
            }
            else if (p == 0 || p == -1 || p == -2)
            {
                if (!checked2.count({p.x, p.y}))
                {
                    if (check_subreg(p.x, p.y, num))
                        p = -1;
                    checked2.insert({p.x, p.y});
                    p.i = index++;
                    prior.push(p);
                }
            }
        }
        pos p;
        int b = 0;
        do
        {

            if (!prior.empty())
            {
                p = prior.top();
                prior.pop();
            }
            else
            {
                return region;
            }

            // if (p == -1 || p == -2 || p == -3)
            // {
            //     p = 0;
            // }

            if (check_valid(p.x, p.y, num))
            {
                expand_region(p.x, p.y);
                b = 1;
            }
        } while (b != 1);

        return region;
    }

public:
    vector<pos> create_region(int x, int y, int num)
    {
        head = {x, y, 0};
        while (!prior.empty())
            prior.pop();
        checked2.clear();
        vector<pos> t;
        region = check_region(x, y, t, num);
        expand_region(x, y);
        vector<pos> heads;
        if ((int)region.size() != num)
        {
            for (pos c : conflict)
            {
                if (c != 0)
                {
                    destroy(c);
                    set_init(region);
                    while (!prior.empty())
                        prior.pop();
                    checked2.clear();
                    region = check_region(x, y, t, num);
                    expand_region(x, y);
                    if ((int)region.size() == num)
                    {
                        mark_safe(region);
                        return region;
                    }
                }
            }

            // for (pos p : heads)
            // {
            //     cout << "-------------" << endl;
            //     vector<pos> s = create_region(p.x, p.y, problem[p.x][p.y]);
            //     cout << "CREATE SUB REGION: " << problem[p.x][p.y] << " " << s.size() << endl;
            //     print_problem();
            // }
        }
        if ((int)region.size() == num)
        {
            mark_safe(region);
        }

        return region;
    }
};

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
    // for (int i = 0; i < w; i++)
    // {
    //     for (int j = 0; j < w; j++)
    //     {
    //         if ((i == w - 1 || i == 0) && problem[i][j] == 0)
    //             problem[i][j] = -1;
    //         else if ((j == h - 1 || j == 0) && problem[i][j] == 0)
    //             problem[i][j] = -1;
    //     }
    // }
    for (int j = 0; j < w; j++)
    {
        for (int i = 0; i < w; i++)
        {

            for (int n = 1; n <= 22; n++)
            {

                if (problem[i][j] == 1)
                {
                    safe[i][j] = 1;
                    for (pos p : nearby(i, j))
                    {
                        if (p == 0)
                            p = -1;
                    }
                }
                else if (problem[i][j] == n && !safe[i][j]) //
                {
                    vector<pos> s;
                    s = check_region(i, j, s, problem[i][j]);
                    if ((int)s.size() == problem[i][j])
                    {
                        mark_safe(s);
                    }
                    else
                    {
                        reg newreg;
                        s = newreg.create_region(i, j, problem[i][j]);
                        cout << "CREATE REGION: " << n << " " << s.size() << endl;
                        print_problem();
                        cout << "-------------" << endl;
                        // if ((int)s.size() == problem[i][j])
                        //     mark_safe(s);
                    }
                }
            }
        }
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
    cout << "2" << endl
         << endl;
    cout << "Problem: " << endl;
    print_problem();
    task21();
    cout << "Solve: " << endl;
    print_problem();
    cout << "Safe: " << endl;
    print_marked();

    cout << "2" << endl
         << endl;
    cout << "Problem: " << endl;
    print_problem();
    task21();
    cout << "Solve: " << endl;
    print_problem();
    cout << "Safe: " << endl;
    print_marked();
    return 0;
}