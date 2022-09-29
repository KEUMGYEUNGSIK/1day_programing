#include <stdio.h>
#include <math.h>

float data = 0.0f;

FILE* fp;


int main(void)
{
	double t = 0;
	double y_t = 0;
	double frequency_n = 0, zeta = 0, frequency_d = 0;

	// s^2 + a*s + b = 0

	fp = fopen("Automatic Control.txt", "w");
		
	if(fp == NULL)
	{
		printf("파일 열기 실패\n");
	}
	else
	{
		printf("파일 열기 성공\n");
	}

	printf("frequency_n : ");
	scanf("%f", &data);
	frequency_n = data;
	printf("zeta : ");
	scanf("%f", &data);
	zeta = data;

	frequency_d = frequency_n * sqrt(1 - zeta * zeta);

	for (t = 0.0; t < 3.0; t += 0.001)
	{
		//printf("%f", t);
		// 0 < zeta  < 1
		if (zeta > 0 && zeta < 1)
		{
			y_t = 1 - (exp(-zeta * frequency_n * t) / sqrt(1 - zeta * zeta)) * sin(frequency_d * t + atan(sqrt(1 - zeta * zeta) / zeta));
			fprintf(fp, "%f %.8f\n",t, y_t);
			printf("%.8f \n", y_t);
		}
		// zeta = 0
		else if (zeta == 1)
		{
			y_t = 1 - exp(-frequency_n * t) - frequency_n * t * exp(-frequency_n * t);
			fprintf(fp, "%f %.8f\n", t, y_t);
			printf("%.8f \n", y_t);
		}
		// zeta > 1
		else if (zeta > 1)
		{
			y_t = 1 - (0.5) * (zeta / sqrt(zeta * zeta - 1) + 1) * exp(-frequency_n * (zeta - sqrt(zeta * zeta - 1)) * t) + (0.5) * (zeta / sqrt(zeta * zeta - 1) - 1) * exp(-frequency_n * (zeta + sqrt(zeta * zeta - 1)) * t);
			fprintf(fp, "%f %.8f\n", t, y_t);
			printf("%.8f \n", y_t);
		}
		else if (zeta == 0)
		{
			y_t = 1.0 - cos(frequency_n * t);
			fprintf(fp, "%f %.8f\n", t, y_t);
			printf("%.8f \n", y_t);
		}
		else
		{
			printf("시스템이 불안정하게 된다.");
		}
	}

	fclose(fp);

	return 0;
}