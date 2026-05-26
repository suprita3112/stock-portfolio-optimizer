#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Data Structure: Struct to hold stock attributes
typedef struct {
    char name[50];
    int price;
    int profit;
} Stock;

// Utility function to find maximum of two integers
int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

// 0/1 Knapsack using Dynamic Programming
void optimizePortfolio(int budget, Stock stocks[], int n) {
    int i, w;

    // Allocate memory for DP table
    int **K = (int **)malloc((n + 1) * sizeof(int *));

    if (K == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    for (i = 0; i <= n; i++) {
        K[i] = (int *)malloc((budget + 1) * sizeof(int));
    }

    // Build DP table
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= budget; w++) {

            if (i == 0 || w == 0) {
                K[i][w] = 0;
            }

            else if (stocks[i - 1].price <= w) {

                K[i][w] = maxInt(
                    stocks[i - 1].profit +
                    K[i - 1][w - stocks[i - 1].price],

                    K[i - 1][w]
                );
            }

            else {
                K[i][w] = K[i - 1][w];
            }
        }
    }

    int maxProfit = K[n][budget];

    // Backtracking to find selected stocks
    int res = maxProfit;
    w = budget;
    int totalSpent = 0;

    printf("\n========================================\n");
    printf("   PORTFOLIO OPTIMIZATION RESULTS\n");
    printf("========================================\n");

    printf("Selected Stocks to Buy:\n");

    for (i = n; i > 0 && res > 0; i--) {

        if (res == K[i - 1][w]) {
            continue;
        }

        else {

            printf("- %s (Cost: Rs.%d | Expected Profit: Rs.%d)\n",
                   stocks[i - 1].name,
                   stocks[i - 1].price,
                   stocks[i - 1].profit);

            totalSpent += stocks[i - 1].price;

            res = res - stocks[i - 1].profit;
            w = w - stocks[i - 1].price;
        }
    }

    // Financial metrics
    int totalValue = totalSpent + maxProfit;

    float roi = 0.0;

    if (totalSpent > 0) {
        roi = ((float)maxProfit / totalSpent) * 100.0;
    }

    printf("----------------------------------------\n");
    printf("Total Budget Spent : Rs.%d / Rs.%d\n",
           totalSpent, budget);

    printf("Unused Cash        : Rs.%d\n",
           budget - totalSpent);

    printf("Total Profit Gen   : Rs.%d\n",
           maxProfit);

    printf("Total Est. Value   : Rs.%d\n",
           totalValue);

    printf("Expected ROI       : %.2f%%\n",
           roi);

    printf("========================================\n");

    // Free memory
    for (i = 0; i <= n; i++) {
        free(K[i]);
    }

    free(K);
}

int main() {

    int n, totalBudget;

    printf("========================================\n");
    printf("   STOCK PORTFOLIO OPTIMIZER (DP)\n");
    printf("========================================\n");

    printf("Enter your total investment budget (INR): Rs.");
    scanf("%d", &totalBudget);

    printf("Enter the number of available stocks: ");
    scanf("%d", &n);

    // Allocate memory for stocks
    Stock *market = (Stock *)malloc(n * sizeof(Stock));

    if (market == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Input stock details
    for (int i = 0; i < n; i++) {

        printf("\n--- Enter details for Stock %d ---\n", i + 1);

        printf("Name: ");
        scanf(" %[^\n]", market[i].name);

        printf("Price (INR): Rs.");
        scanf("%d", &market[i].price);

        printf("Expected Profit (INR): Rs.");
        scanf("%d", &market[i].profit);
    }

    printf("\nRunning Dynamic Programming Algorithm...\n");

    clock_t start_time = clock();

    optimizePortfolio(totalBudget, market, n);

    clock_t end_time = clock();

    double time_taken =
        ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Algorithm Execution Time: %f seconds\n",
           time_taken);

    free(market);

    return 0;
}