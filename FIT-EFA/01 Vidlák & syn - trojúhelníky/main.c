#include <stdio.h>
#include <stdlib.h>

/**
 * @source http://geeksquiz.com/merge-sort/
 */
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int * L = (int *) malloc(sizeof(int) * n1);
    int * R = (int *) malloc(sizeof(int) * n2);

    for(i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }

    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

/**
 * @source http://geeksquiz.com/merge-sort/
 */
void mergeSort(int arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

/**
 * @source http://en.literateprograms.org/Binary_search_(C)
 */
int binary_search(int a[], int low, int high, int target)
{
	int middle;

    while (low <= high) {
        middle = low + (high - low) / 2;

        if (target < a[middle]) {
            high = middle - 1;
        } else if (target > a[middle]) {
            low = middle + 1;
        } else {
            return middle;
        }
    }

    return middle;
}

int findOpposite(int i, int n)
{
	int half = n / 2;
	int res = half + i;

	// res = (i > half) ? res - n : res;
	// printf("%d - %d\n", i, res);
	return res;
}

void minMax(int * i, int * j)
{
	int tmp;

	if (* i > * j) {
		tmp = * i;
		* i = * j;
		* j = tmp;
	}
}

int main(int argc, const char * argv[])
{
	long long int result = 0;
	int length, total;
	scanf("%d %d", & length, & total);

	int * trees = (int *) malloc(sizeof(int) * length);
	int * ends = (int *) malloc(sizeof(int) * length);

	int odd = (total % 2 == 0) ? 0 : 1;

	for (int i = 0; i < length; i++) {
		scanf("%d", & trees[i]);
	}

	mergeSort(trees, 0, length - 1);

	for (int i = 0; i < length; i++) {
		ends[i] = findOpposite(trees[i], total);
	}

	for (int i = 0; i < (length / 2) + 1; i++) {
		int min = ends[i];

		int cond = (odd == 1) ? (length / 2) + 1 : length / 2;

		for (int j = i + 1; j <= cond; j++) {
			if (i == 0 && j == cond && odd) {
				continue;
			}

			int max = ends[j];

			// if (min == 0 && ends[i] > (int) (total / 2)) {
				// min = max;
				// max = total;
			// }

			int key1 = binary_search(trees, 0, length - 1, min);
			int key2 = binary_search(trees, 0, length - 1, max);

			minMax(& key1, & key2);

			int sum = key2 - key1;

			if (odd == 0 && (trees[i] != 0 || trees[j] != (total / 2))) {
				// printf("tu\n");
				sum++;
			}

			result += sum;

			// printf("%d %d; %d %d; %d %d; %d\n", trees[i], trees[j], min, max, key1, key2, sum);

			// printf("%d\n", key2 - key1);

			// for (int k = j + 1; k < length; k++) {
			// 	int value = trees[k];
			// 	int cond = (odd == 0) ? (value >= min) : (value > min);
			// 	if (cond && value <= max) {
			// 		result++;
			// 	}
			// }


		}
	}

	printf("%d\n", result);

	free(trees);
	free(ends);

	return 0;
}
