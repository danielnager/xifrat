#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#define P 13UL
typedef uint64_t base_t;
#define N 69
#define S 4
#define E 1
#define R 64
typedef base_t elem_t[E];
typedef elem_t array_t[N];

typedef uint64_t table_t[P][P];

table_t st={
 { 5,  3,  0, 12, 11,  4,  9, 10,  8,  1,  6,  7,  2},
 { 3,  7,  2, 11,  9, 10,  5,  6,  0, 12,  8,  4,  1},
 { 0,  2,  3, 10,  6, 12,  8, 11,  5,  4,  9,  1,  7},
 {12, 11, 10,  0,  2,  5,  1,  3,  4,  8,  7,  9,  6},
 {11,  9,  6,  2,  1,  3, 12,  7, 10,  0,  4,  5,  8},
 { 4, 10, 12,  5,  3,  8,  7,  0,  1,  9,  2,  6, 11},
 { 9,  5,  8,  1, 12,  7, 11,  4,  6,  2, 10,  3,  0},
 {10,  6, 11,  3,  7,  0,  4,  2, 12,  5,  1,  8,  9},
 { 8,  0,  5,  4, 10,  1,  6, 12,  9,  7, 11,  2,  3},
 { 1, 12,  4,  8,  0,  9,  2,  5,  7,  6,  3, 11, 10},
 { 6,  8,  9,  7,  4,  2, 10,  1, 11,  3, 12,  0,  5},
 { 7,  4,  1,  9,  5,  6,  3,  8,  2, 11,  0, 10, 12},
 { 2,  1,  7,  6,  8, 11,  0,  9,  3, 10,  5, 12,  4}
};

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

	memcpy(c,a,sizeof(c));

	for(i=0;i<N;i++)
		f(c[i],c[i],c[(i+N-1)%N]);

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

#if 1
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
#if 0
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
	output_random(); // uncomment to output random for randomness tests
}
