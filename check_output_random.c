#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#define P 16UL
typedef uint64_t base_t;
#define N 64
#define S 4
#define E 1
#define R (64)
typedef base_t elem_t[E];
typedef elem_t array_t[N];

typedef uint64_t table_t[P][P];

#if 1
table_t st={
{ 7, 9,13,10,15, 2, 0, 6, 3,12, 8, 4, 1, 5,14,11},
{ 1,15, 6, 3, 9, 4,11,13,10, 5,14, 2, 7,12, 8, 0},
{ 3, 0,12, 1,11, 8, 9, 5, 7,13, 2,14,10, 6, 4,15},
{ 4, 6,15, 8,13, 1, 5, 9,14,11,10, 7, 2, 0, 3,12},
{ 0, 3, 8,15,10,12, 7,14, 9, 2,13, 5,11, 4, 6, 1},
{10,11, 5, 7, 0,14,15,12, 1, 6, 4, 8, 3,13, 2, 9},
{ 5,14,10,13, 8,11, 4, 3, 6, 1,15, 0,12, 7, 9, 2},
{15, 1, 4, 0, 7, 6,10, 2,11,14, 5,13, 9, 8,12, 3},
{12, 8, 3, 6,14, 0, 2,10,13, 7, 9,11, 5, 1,15, 4},
{13, 2, 7, 5, 4, 9, 8, 1,12, 3, 0,15, 6,10,11,14},
{ 6, 4, 1,12, 2,15,14, 7, 5,10,11, 9,13, 3, 0, 8},
{ 9, 7, 2,11, 1,13, 3, 4, 0, 8,12, 6,15,14, 5,10},
{11,10,14, 9, 3, 5, 1, 8,15, 4, 6,12, 0, 2,13, 7},
{14, 5,11, 2,12,10, 6, 0, 4,15, 1, 3, 8, 9, 7,13},
{ 8,12, 0, 4, 5, 3,13,11, 2, 9, 7,10,14,15, 1, 6},
{ 2,13, 9,14, 6, 7,12,15, 8, 0, 3, 1, 4,11,10, 5}
};
#endif

void f(elem_t r,elem_t a,elem_t b)
{
        int i,j;
        base_t c;
        elem_t t;

        for(i=0;i<E;i++)
                t[i]=st[a[i]][b[i]];

        memcpy(r,t,sizeof(t));  
}

void gk(array_t r,array_t a,array_t b)
{
	int i;
	array_t c;

	for(i=0;i<N;i++)
		f(c[i],a[i],b[i]);

	memcpy(r,c,sizeof(c));	
}

void gm(array_t r,array_t a)
{
	int i;
	array_t c;
	base_t e,d;

	memcpy(c,a,sizeof(c));

	srand(0);

	e=rand()%N;
	for(i=0;i<R*N;i++){
		d=rand()%N;
		if(d==e)d=(d+1)%N;
		f(c[d],c[d],c[e]);
		e=d;
	}

	memcpy(r,c,sizeof(c));	
}

void m(array_t r,array_t a,array_t b)
{
	int i;
	array_t c,d;

	memcpy(c,a,sizeof(c));
	memcpy(d,b,sizeof(c));

	for(i=0;i<R;i++){
		gk(c,c,d);
		gm(c,c);
	}

	memcpy(r,c,sizeof(c));
}

FILE *rn;

uint64_t rnd()
{
	uint64_t r,i;

	i=fread(&r,8,1,rn);
	if(i==0)
		printf("***error***\n");

	return r;
}

uint64_t a_out=0;
uint64_t a_bit=0;

void write_b(base_t v,int b)
{
	int s=sizeof(a_out)*8;
	unsigned char r;
	
	while(a_bit+b>=s){
		r=a_out;
		fwrite(&r,1,1,stdout);
		a_bit-=8;
		a_out>>=8;
	}
	a_out=a_out^(v<<a_bit);
	a_bit+=b;
}

void write_array(array_t a)
{
	int i,j;
	base_t r;

	for(i=0;i<N;i++)
	for(j=0;j<E;j++){
		r=rnd()%(1UL<<S);
		if(r>=P)
			write_b(r,S);
		else
			write_b(a[i][j],S);
	}

}

void rnd_e(elem_t a)
{
	int i;

	for(i=0;i<E;i++)
		a[i]=rnd()%P;
}

void rnd_a(array_t a, int n)
{
	int i;

	for(i=0;i<n;i++)
		rnd_e(a[i]);
}

void output_random()
{
	base_t i,j;
	array_t t={0},k={0},c;


	for(i=0;i<N;i++)
	for(j=0;j<E;j++){
		k[i][j]=0;
		t[i][j]=0;
	}



	for(;;){

#if 0
		i=0;j=0;
		t[i][j]++;
		while(t[i][j]>=P){
			t[i][j]=0;
			j++;
			if(j==E){
				j=0;
				i++;
			}
			t[i][j]++;
		}
#endif
#if 1
		rnd_a(t,N/2);
#endif
		m(c,t,k);

		write_array(c);
	}

}

void check()
{
	elem_t a,b,c,d,s1,s2,s3;

	rnd_e(a);
	rnd_e(b);
	rnd_e(c);
	rnd_e(d);

	f(s1,a,b);
	f(s2,c,d);
	f(s1,s1,s2);

	f(s2,a,c);
	f(s3,b,d);
	f(s2,s2,s3);

	if(memcmp(s1,s2,sizeof(s1))!=0)
		printf("*error*");

	int i;
	array_t e[4],t1,t2,t3;

	for(i=0;i<4;i++)
		rnd_a(e[i],N);

	m(t1,e[0],e[1]);
	m(t2,e[2],e[3]);
	m(t1,t1,t2);

	m(t2,e[0],e[2]);
	m(t3,e[1],e[3]);
	m(t2,t2,t3);

	if(memcmp(t1,t2,sizeof(t1))!=0){
		printf("***ERROR***\n");
		exit(0);
	}
}

int main()
{
	rn=fopen("/dev/urandom","r");
	check();
	//output_random(); // uncomment to output random for randomness tests
}
