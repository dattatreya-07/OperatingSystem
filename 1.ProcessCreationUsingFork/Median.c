#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
void media(int n, int arr[]) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }
    double median;
    if (n % 2 == 0)
        median = (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
    else
        median = arr[n / 2];
    printf("Median=%.2f\n", median);
}
int main() {
    int n;
    int i;
    int arr[50];
    printf("Enter the value of n: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > 50) {
        printf("Invalid input size.\n");
        return 1;
    }
    printf("Enter %d elements:\n", n);
    for (i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input value.\n");
            return 1;
        }
    }
    int p2 = fork();
    if (p2 == 0) {
        media(n, arr);
    } 
    else if (p2 > 0) {
        wait(NULL);
    } 
    else {
        printf("Invalid process generation (fork failed).\n");
    }
    return 0;
}
