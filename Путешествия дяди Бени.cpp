#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>   
#include <sstream>
#include <set>
#include <iterator>
#include <map>

using namespace std;

long long int MAX_ROAD = 20000000000;
long long int LONGEST_ROAD = 15000000;
long long int INF = 400000000000000000;

class Branch
{
public:
    Branch(vector<vector<pair<long long int, pair<int, int>>>> _mass, int _from_city,
        int _to_city, bool _is_cross_road, long long int cost);
    Branch();
    vector<vector<pair<long long int, pair<int, int>>>> mass;
    int from_city;
    int to_city;
    long long int cost;
    bool is_cross_road;
    vector<pair<int, int>> from_to;
};

Branch::Branch (vector<vector<pair<long long int, pair<int, int>>>> _mass, int _from_city,
    int _to_city, bool _is_cross_road, long long int _cost)
{
    mass = _mass;
    from_city = _from_city;
    to_city = _to_city;
    is_cross_road = _is_cross_road;
    cost = _cost;
}

Branch::Branch()
{
}

vector<long long int> Min_in_str(vector<vector<pair<long long int, pair<int, int>>>> m)
{
    int n = m.size();
    vector<long long int> d(n, INF);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (d[i] > m[i][j].first)
                d[i] = m[i][j].first;
        }
    }
    return d;
}

vector<vector<pair<long long int, pair<int, int>>>> Red_in_str(vector<vector<pair<long long int, 
    pair<int, int>>>> m, vector<long long int> d)
{
    int n = m.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            m[i][j].first -= d[i];
        }
    }
    return m;
}

vector<long long int> Min_in_row(vector<vector<pair<long long int, pair<int, int>>>> m)
{
    int n = m.size();
    vector<long long int> d(n, INF);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (d[j] > m[i][j].first)
                d[j] = m[i][j].first;
        }
    }
    return d;
}

vector<vector<pair<long long int, pair<int, int>>>> Red_in_row
(vector<vector<pair<long long int, pair<int, int>>>> m, vector<long long int> d)
{
    int n = m.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            m[i][j].first -= d[j];
        }
    }
    return m;
}

long long int Low_sqrt(vector<long long int> di, vector<long long int> dj)
{
    long long int low_sqrt = 0;
    for (int i = 0; i < di.size(); i++)
    {
        low_sqrt += di[i] + dj[i];
    }
    return low_sqrt;
}

vector<vector<long long int>> Zeros_marks(vector<vector<pair<long long int, pair<int, int>>>> m)
{
    int n = m.size();
    vector<long long int> first_str(n, INF);
    vector<long long int> second_str(n, INF);
    vector<long long int> first_row(n, INF);
    vector<long long int> second_row(n, INF);
    vector<vector<long long int>> mass_zeros(n, vector<long long int>(n, -1));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (first_str[i] >= m[i][j].first)
            {
                second_str[i] = first_str[i];
                first_str[i] = m[i][j].first;
            }
            else if (second_str[i] > m[i][j].first)
            {
                second_str[i] = m[i][j].first;
            }
            if (first_row[j] >= m[i][j].first)
            {
                second_row[j] = first_row[j];
                first_row[j] = m[i][j].first;
            }
            else if (second_row[j] > m[i][j].first)
            {
                second_row[j] = m[i][j].first;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (m[i][j].first == 0)
            {
                mass_zeros[i][j] = second_str[i] + second_row[j];
            }
        }
    }
    return mass_zeros;
}

pair<Branch, Branch> Red_matrix(Branch br, 
    pair<int, int> Opt)
{
    vector<vector<pair<long long int, pair<int, int>>>> m = br.mass;
    int n = m.size();
    vector<vector<pair<long long int, pair<int, int>>>> new_mass(n - 1, vector<pair<long long int, pair<int, int>>>(n - 1));
    for (int i = 0, i_1 = 0; i < n; i++, i_1++)
    {
        if (i == Opt.first)
        {
            i_1--;
            continue;
        }
        for (int j = 0, j_1 = 0; j < n; j++, j_1++)
        {
            if (j != Opt.second)
            {
                new_mass[i_1][j_1] = m[i][j];
            }
            else
            {
                j_1--;
            }
        }
    }

    Branch new_branch_in, new_branch_out;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (m[Opt.first][Opt.second].second.first == new_mass[i][j].second.second && 
                m[Opt.first][Opt.second].second.second == new_mass[i][j].second.first)
            {
                new_mass[i][j].first = MAX_ROAD;
            }
        }
    }
    new_branch_in = Branch(new_mass, m[Opt.first][Opt.second].second.first,
        m[Opt.first][Opt.second].second.second, true, 0);
    m[Opt.first][Opt.second].first = MAX_ROAD;
    new_branch_out = Branch(m, m[Opt.first][Opt.second].second.first,
        m[Opt.first][Opt.second].second.second, false, 0);
    new_branch_in.from_to = br.from_to;
    new_branch_in.from_to.push_back({ new_branch_in.from_city, new_branch_in.to_city });
    new_branch_out.from_to = br.from_to;
    return {  new_branch_out, new_branch_in };
}

pair<int, int> Opt_zero(vector<vector<long long int>> mass_zeros)
{
    int n = mass_zeros.size();
    pair <int, int> Opt_pair;
    long long int max = -1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (mass_zeros[i][j] > max)
            {
                Opt_pair = { i, j };
                max = mass_zeros[i][j];
            }
        }
    }
    return Opt_pair;
}

Branch Low_first_sqrt(Branch br, long long int old_cost)
{
    int n = br.mass.size();
    auto di = Min_in_str(br.mass);
    br.mass = Red_in_str(br.mass, di);
    auto dj = Min_in_row(br.mass);
    br.mass = Red_in_row(br.mass, dj);
    long long int new_cost = old_cost;
    for (int i = 0; i < n; i++)
    {
        new_cost += di[i] + dj[i];
    }
    br.cost = new_cost;
    br.is_cross_road = true;
    return br;
}

Branch Low_second_sqrt(Branch br, long long int old_cost, long long int Pij)
{
    br.cost = old_cost + Pij;
    br.is_cross_road = false;
    return br;
}

int Min_cost_branch(vector<Branch> Brs)
{
    int n = Brs.size();
    int index = 0;
    long long int cost = INF;
    for (int i = 0; i < n; i++)
    {
        if (cost > Brs[i].cost)
        {
            cost = Brs[i].cost;
            index = i;
        }
    }
    return index;
}

void main()
{
    int n, m, k;
    /*n = 5;
    m = 20;
    k = 0;
    vector<vector<pair<long long int, pair<int, int>>>> mass;
    for (int i = 0; i < n; i++)
    {
        mass.push_back(vector<pair<long long int, pair<int, int>>>(n, { MAX_ROAD, {0, 0} }));
    }*/
    /*mass[0][0].first = MAX_ROAD;
    mass[0][0].second.first = 0;
    mass[0][0].second.second = 0;
    mass[0][1].first = 20;
    mass[0][1].second.first = 0;
    mass[0][1].second.second = 1;
    mass[0][2].first = 18;
    mass[0][2].second.first = 0;
    mass[0][2].second.second = 2;
    mass[0][3].first = 12;
    mass[0][3].second.first = 0;
    mass[0][3].second.second = 3;
    mass[0][4].first = 8;
    mass[0][4].second.first = 0;
    mass[0][4].second.second = 4;
    mass[1][0].first = 5;
    mass[1][0].second.first = 1;
    mass[1][0].second.second = 0;
    mass[1][1].first = MAX_ROAD;
    mass[1][1].second.first = 1;
    mass[1][1].second.second = 1;
    mass[1][2].first = 14;
    mass[1][2].second.first = 1;
    mass[1][2].second.second = 2;
    mass[1][3].first = 7;
    mass[1][3].second.first = 1;
    mass[1][3].second.second = 3;
    mass[1][4].first = 11;
    mass[1][4].second.first = 1;
    mass[1][4].second.second = 4;
    mass[2][0].first = 12;
    mass[2][0].second.first = 2;
    mass[2][0].second.second = 0;
    mass[2][1].first = 18;
    mass[2][1].second.first = 2;
    mass[2][1].second.second = 1;
    mass[2][2].first = MAX_ROAD;
    mass[2][2].second.first = 2;
    mass[2][2].second.second = 2;
    mass[2][3].first = 6;
    mass[2][3].second.first = 2;
    mass[2][3].second.second = 3;
    mass[2][4].first = 11;
    mass[2][4].second.first = 2;
    mass[2][4].second.second = 4;
    mass[3][0].first = 11;
    mass[3][0].second.first = 3;
    mass[3][0].second.second = 0;
    mass[3][1].first = 17;
    mass[3][1].second.first = 3;
    mass[3][1].second.second = 1;
    mass[3][2].first = 11;
    mass[3][2].second.first = 3;
    mass[3][2].second.second = 2;
    mass[3][3].first = MAX_ROAD;
    mass[3][3].second.first = 3;
    mass[3][3].second.second = 3;
    mass[3][4].first = 12;
    mass[3][4].second.first = 3;
    mass[3][4].second.second = 4;
    mass[4][0].first = 5;
    mass[4][0].second.first = 4;
    mass[4][0].second.second = 0;
    mass[4][1].first = 5;
    mass[4][1].second.first = 4;
    mass[4][1].second.second = 1;
    mass[4][2].first = 5;
    mass[4][2].second.first = 4;
    mass[4][2].second.second = 2;
    mass[4][3].first = 5;
    mass[4][3].second.first = 4;
    mass[4][3].second.second = 3;
    mass[4][4].first = MAX_ROAD;
    mass[4][4].second.first = 4;
    mass[4][4].second.second = 4;*/
    cin >> n >> m >> k;
    map <string, int> country_num;
    vector<vector<pair<long long int, pair<int, int>>>> mass;
    map <string, int> :: iterator iterator_1, iterator_2;
    for (int i = 0; i < n; i++)
    {
        string str;
        cin >> str;
        country_num.insert(make_pair(str, i));
        mass.push_back(vector<pair<long long int, pair<int, int>>>(n + 1, { MAX_ROAD, {0, 0} }));
    }
    mass.push_back(vector<pair<long long int, pair<int, int>>>(n + 1, { MAX_ROAD, {0, 0} }));
    for (int i = 0; i < m; i++)
    {
        string c_1, c_2;
        int ml;
        cin >> c_1 >> c_2 >> ml;
        iterator_1 = country_num.find(c_1);
        iterator_2 = country_num.find(c_2);
        mass[iterator_1->second + 1][iterator_2->second + 1] = { ml, {iterator_1->second + 1, iterator_2->second + 1} };
        mass[iterator_2->second + 1][iterator_1->second + 1] = { ml, {iterator_2->second + 1, iterator_1->second + 1} };
    }
    for (int i = 1; i <= n; i++)
    {
        mass[i][0] = { 0, {i, 0} };
        mass[0][i] = { 0, {0, i} };
    }
    if (k >= n - 1)
    {
        cout << 0;
        return;
    }
    n++;
    vector<vector<pair<long long int, pair<int, int>>>> temp_mass = mass;
    auto di = Min_in_str(temp_mass);
    temp_mass = Red_in_str(temp_mass, di);
    auto dj = Min_in_row(temp_mass);
    temp_mass = Red_in_row(temp_mass, dj);
    Branch first_br = Branch(temp_mass, 0, 0, true,  Low_sqrt(di, dj));
    vector<Branch> completed;
    vector<Branch> in_processed;
    vector<pair<int, int>> road;
    in_processed.push_back(first_br);
    while (true)
    {
        int lowest_num = Min_cost_branch(in_processed);
        Branch temp = in_processed[lowest_num];
        if (temp.mass.size() == 1)
        {
            road = temp.from_to;
            road.push_back({ temp.mass[0][0].second.first, temp.mass[0][0].second.second });
            multiset<long long int> roads_ml;
            for (int i = 0; i < road.size(); i++)
            {
                roads_ml.insert(mass[road[i].first][road[i].second].first);
            }
            multiset <long long int> ::iterator it = roads_ml.end();
            it--;
            while (k != 0)
            {
                k--;
                it--;
                if (it == roads_ml.begin())
                {
                    cout << 0;
                    return;
                }
            }
            long long int size_road = 0;
            do
            {
                size_road += *it;
            } while (it-- != roads_ml.begin());
            if (size_road > LONGEST_ROAD)
                cout << -1;
            else
                cout << size_road;
            return;
        }
        Branch br = Branch(temp.mass, temp.from_city, temp.to_city, temp.is_cross_road, temp.cost);
        br.from_to = temp.from_to;
        completed.push_back(br);
        auto di = Min_in_str(br.mass);
        br.mass = Red_in_str(br.mass, di);
        auto dj = Min_in_row(br.mass);
        br.mass = Red_in_row(br.mass, dj);
        vector<vector<long long int>> zeros = Zeros_marks(br.mass);
        pair <int, int> opt_zero = Opt_zero(zeros);
        pair <Branch, Branch> branchs = Red_matrix(br, opt_zero);
        branchs.first = Low_second_sqrt(branchs.first, in_processed[lowest_num].cost, zeros[opt_zero.first][opt_zero.second]);
        branchs.second = Low_first_sqrt(branchs.second, in_processed[lowest_num].cost);
        in_processed.push_back(branchs.first);
        in_processed.push_back(branchs.second);
        in_processed.erase(in_processed.begin() + lowest_num);
    }
}