#include <bits/stdc++.h>
using namespace std;

pair<int, vector<int>> Knapsack(vector<int> weight, vector<int> value, int N, int W) {
    // Step 1: Initialize DP table
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    // Step 2: Fill DP table
    for (int i = 1; i <= N; i++) {
        for (int w = 1; w <= W; w++) {
            if (weight[i - 1] <= w)
                dp[i][w] = max(value[i - 1] + dp[i - 1][w - weight[i - 1]], dp[i - 1][w]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    // Step 3: Maximum utility
    int maxUtility = dp[N][W];

    // Step 4: Traceback to find included items
    int w = W;
    vector<int> includedItems;
    for (int i = N; i > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            includedItems.push_back(i);  // store 1-based index
            w -= weight[i - 1];
        }
    }

    reverse(includedItems.begin(), includedItems.end());
    return {maxUtility, includedItems};
}

int main() {
    int N, W;
    cout << "Enter number of items and capacity: ";
    cin >> N >> W;

    vector<int> weight(N), value(N);
    cout << "Enter weights:\n";
    for (int i = 0; i < N; i++) cin >> weight[i];
    cout << "Enter values:\n";
    for (int i = 0; i < N; i++) cin >> value[i];

    auto result = Knapsack(weight, value, N, W);
    cout << "\nMaximum Utility: " << result.first << "\nItems Included: ";
    for (int item : result.second) cout << item << " ";
    cout << "\n";
    return 0;
}


