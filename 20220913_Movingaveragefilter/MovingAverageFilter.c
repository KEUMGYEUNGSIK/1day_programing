#include <stdio.h>

#define FILTERSIZE 10

int array[FILTERSIZE] = {0,};

float mov_filter(float pre_avg, float new_value);

int main(void)
{
	float new_value;
	float pre_avg;

	while(1)
	{
		printf("데이터를 입력하세요 : ");
		scanf("%f", &new_value);
		
		pre_avg = mov_filter(pre_avg, new_value);
		
		printf("평균 : %.4f\n", pre_avg);
	}
	
	return 0;
}

float mov_filter (float pre_avg, float new_value)
{
	float average = 0;
	
	array[FILTERSIZE] = new_value;
	
	average = pre_avg + array[FILTERSIZE]/4 - array[FILTERSIZE-4]/4;
	
	for(int i = 1 ; i < FILTERSIZE+1 ; i++)
	{
		array[i-1] = array[i];
	}
	array[FILTERSIZE] = 0;
	
	return average;
}

