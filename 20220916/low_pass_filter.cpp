#include <stdio.h>

float array;

float low_pass_filter(float pre_avg, float new_value, float alpha);

int main(void)
{
	float new_value, pre_avg = 0, alpha;
	
	printf("가중치 : ");
	scanf("%f", &alpha);

	while(1)
	{
		printf("새로운 데이터: ");
		scanf("%f", &new_value);
		
		pre_avg = low_pass_filter(pre_avg, new_value, alpha);
		
		printf("\n 평균 : %.4f\n", pre_avg);
	}
	
	return 0;
}

float low_pass_filter(float pre_avg, float new_value, float alpha)
{
	float average = 0;
	
	array = new_value;
	
	average = alpha * pre_avg + (1 - alpha) * new_value;
	
	return average;
}
