#include <stdio.h>

//M^2 - B^1 - K

float x[] = {0.0};
float v[] = {0.0};

float K_x1, K_x2, K_x3, K_x4, dX ,dV;
float K_v1, K_v2, K_v3, K_v4;

float func(float B, float K, float M, float x, float v);
float dxdv_F (float B, float M, float K, int cnt, float h);

int main(void)
{
	float K, M, B, T0, T1, V0, X0, h;

	printf("---M---\n");
	scanf("%f", &M);
	printf("---B---\n");
	scanf("%f", &B);
	printf("---K---\n");
	scanf("%f", &K);
	printf("---h---\n");
	scanf("%f", &h);
	printf("---V0---\n");
	scanf("%f", &V0);
	printf("---X0---\n");
	scanf("%f", &X0);
	printf("---T0---\n");
	scanf("%f", &T0);
	printf("---T1---\n");
	scanf("%f", &T1);
	
	x[0] = X0;
	v[0] = V0;

	int cnt = (T1 - T0);

	dxdv_F (B, M, K, cnt, h);
	
	return 0;
}

float func(float B, float K, float M, float v, float x)
{
	
	return -((B*v)/M)-((K*x)/M);
}

float dxdv_F (float B, float M, float K, int cnt, float h)
{
	
	for(int n = 0; n < cnt; n++)
	{
		float XX = x[n];
		float VV = v[n];
		
		K_x1 = VV*h;
		K_v1 = func( B, K, M, VV, XX )*h;
			
		K_x2 = (VV + (K_v1/2))*h ;
		K_v2 = func( B, K, M, VV+K_v1/2, XX+K_x1/2 )*h;
		
		K_x3 = (VV + (K_v2/2))*h;
		K_v3 = func( B, K, M, VV+K_v2/2, XX+K_x2/2 )*h;
		
		K_x4 = (VV + K_v3)*h;
		K_v4 = func( B, K, M, VV+K_v3, XX+K_x3 )*h;

		dX = h*(K_x1 + 2*K_x2 + 2*K_x3 + K_x4)/6.0;
		dV = h*(K_v1 + 2*K_v2 + 2*K_v3 + K_v4)/6.0;
			
		v[n+1] = VV + dV;
		x[n+1] = XX + dX;
		
		printf("속도[%d] : %f\n", n, VV);
		printf("변위[%d] : %f\n\n", n, XX);
	
	}
}
