#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#define P 12UL
typedef uint64_t base_t;
#define N 71
#define S 4
#define E 1
#define R 256
typedef base_t elem_t[E];
typedef elem_t array_t[N];

typedef uint64_t table_t[P][P];

table_t st={
{5,3,0,4,1,10,9,2,8,6,11,7}, 
{3,7,5,0,9,6,2,8,10,1,4,11}, 
{0,5,4,11,6,8,1,9,2,10,7,3}, 
{4,0,11,7,10,2,6,1,9,8,3,5},
{1,9,6,10,7,5,11,4,0,3,8,2},
{10,6,8,2,5,4,3,7,11,0,9,1},
{9,2,1,6,11,3,4,0,5,7,10,8},
{2,8,9,1,4,7,0,5,3,11,6,10},
{8,10,2,9,0,11,5,3,7,4,1,6},
{6,1,10,8,3,0,7,11,4,5,2,9},
{11,4,7,3,8,9,10,6,1,2,5,0},
{7,11,3,5,2,1,8,10,6,9,0,4} 
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
	output_random(); // uncomment to output random for randomness tests
}
