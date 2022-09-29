#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define SIZE 4

// 출력함수
void printMatrix(int m[][SIZE]) { 
	printf("----- 출력 -----\n");
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%5d", m[i][j]);
		}
		printf("\n");
	}
}
// 입력함수
void setMatrix(int m[][SIZE]) { 
	int i, j;
	for (i = 0; i < SIZE; i++) {  
		for (j = 0; j < SIZE; j++) {
			printf("[%d][%d] : 입력\n", i, j);
			scanf("%d", &m[i][j]);
		}
		if (j != SIZE)
			printf("mA[%d][%d] : 입력\n", i, j);
	}
}
//계산 함수 
void calcMatrix(int a[][SIZE],int b[][SIZE], int c[][SIZE]){  
	int k, temp;

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			temp = 0;
			for (k = 0; k < SIZE; k++) {
				temp += (a[i][k] * b[k][j]);
			}
			c[i][j] = temp;
		}
	}
}
int main(void) {
	int mA[SIZE][SIZE] = { 0, };
	int mB[SIZE][SIZE] = { 0, };
	int mC[SIZE][SIZE] = { 0, };

	while (1) {
		int s = NULL;
		printf("프로그램을 계속 진행시키고 싶으시면 1을 입력해주세요\n");
		printf("프로그램 종료는 0을 입력해주세요");
		scanf("%d", &s);
		if (s == 0) break;
		
		printf("mA 입력\n");
		setMatrix(mA);
		printf("mB 입력\n");
		setMatrix(mB);

		printf("mA 출력\n");
		printMatrix(mA);
		printf("mB 출력\n");
		printMatrix(mB);
		printf("mC 계산\n");
		calcMatrix(mA, mB, mC);

		printf("mC 출력\n");
		printMatrix(mC);
	}
	return 0;
}