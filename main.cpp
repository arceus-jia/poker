//
//  main.cpp
//  poker
//
//  Created by arceus on 2018/8/7.
//  Copyright © 2018年 arceus. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

// A : joker, joker,2,2,k,k,j,j
// B: 2,2,10,10,6,6,6,6
// 排序

string mp[6] = {"6","10","j","k","2","joker"};
int a[8] = {2,2,3,3,4,4,5,5};
int b[8] = {0,0,0,0,1,1,4,4};

const int N = 1 << 8;
int dp[N][N][N][2];

void print(int discard, int cards[])
{
    for (int i = 0; i < 8; i ++)
    {
        if ((discard >> i) & 1 )
            cout << mp[cards[i]] <<" ";
    }
    cout <<endl;
}

bool ableToDiscard (int cards[], int now,int discard)
{
    vector<int>vt;
    //没有这些牌，无法出
    for (int i = 0; i < 8; i ++)
    {
        if (((discard >> i) & 1))
        {
            if (!((now >> i) & 1 ))
                return false;
            vt.push_back(cards[i]);
        }
    }
    //多张牌不同，无法出
    for (int i = 1; i < vt.size(); i ++)
    {
        if (vt[i] != vt[0])
            return false;
    }
    return true;
}

bool ableToDefeat(vector<int>enemy, int cards[], int now, int discard)
{
    //敌人大小王，必负
    if (enemy.size() == 2 && enemy[0] == 5)  return false;
    
    //无法出牌
    if (!ableToDiscard(cards, now, discard))
    {
        return false;
    }
    vector<int> vt;
    for (int i = 0; i < 8; i ++)
    {
        if ((discard >> i) & 1)
        {
            vt.push_back(cards[i]);
        }
    }
    //自己大小王，必胜
    if (vt.size() == 2 && vt[0] == 5)  return true;
    //敌人炸，必负
    if (enemy.size() == 4) return false;
    //自己炸，必胜
    if (vt.size() == 4) return true;
    //数量不同，无法出
    if (enemy.size() != vt.size()) return false;
    //数量相同，点数大，赢了
    if (vt[0] > enemy[0]) return true;
    
    return false;
}

bool dfs(int m, int n,int k, bool a_turn)
{
    if (m == 0) return dp[m][n][k][a_turn] = 1;
    if (n == 0) return dp[m][n][k][a_turn] = 0;
    if (dp[m][n][k][a_turn] != -1) return dp[m][n][k][a_turn];
    
    bool win = 0;
    if (a_turn)
    {
        if (!ableToDiscard(a, m, k))
            return dp[m][n][k][1] = 0;
        if (m == k)
            return dp[m][n][k][1] = 1;
        vector<int>vt;
        for (int i = 0; i < 8; i ++)
        {
            if ((k >> i) & 1)
                vt.push_back(a[i]);
        }
        win = 1; //b的所有下招都失败才能赢
        for (int discard = 1; discard <= n; discard ++)
        {
            //b出牌
            if (ableToDefeat(vt, b, n, discard))
            {
                win &= dfs(m-k,n,discard,0);
            }
        }
        //b在所有策略都失败的情况下会选择不出牌 a下一步必须出一个能赢的
        if (win)
        {
            win = 0;
            for (int next_card = 1; next_card <= m-k; next_card ++)
            {
                win |= dfs(m - k,n,next_card,1);
            }
        }
    }
    else
    {
        if (!ableToDiscard(b, n, k))
            return dp[m][n][k][0] = 1;
        if (n == k)
            return dp[m][n][k][0] = 0;
        vector<int>vt;
        for (int i = 0; i < 8; i ++)
        {
            if ((k >> i) & 1)
                vt.push_back(b[i]);
        }
        win = 0; //a的所有下招都失败才算输
        for (int discard = 1; discard <= m; discard ++)
        {
            //a出牌
            if (ableToDefeat(vt, a, m, discard))
            {
                win |= dfs(m,n-k,discard,1);
            }
        }
        //a在所有策略都失败的情况下会选择不出牌 b下一步必须出一个能赢的才行
        if (!win)
        {
            win = 1;
            for (int next_card = 1; next_card <= n-k; next_card ++)
                win &= dfs(m,n-k,next_card,0);
        }
    }
    return dp[m][n][k][a_turn] = win;
}

void run(int m, int n)
{
    for (int discard = 1; discard <= m; discard ++)
    {
        if (ableToDiscard(a, m, discard))
        {
            bool win = dfs(m,n,discard,1);
//            cout << discard <<" "<<win<<endl;
            if (win)
            {
                print(discard,a);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    memset(dp,-1,sizeof(dp));
    run(255, 255);
    return 0;
}

