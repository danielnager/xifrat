#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define P 16


uint8_t eq_table[P*P*P*P][4];
uint64_t max_eq_table=0;

uint64_t subs_table[P][P];
uint64_t subs_stack[P*P][P][P];
uint64_t top_subs_stack=0;

int repeated_subs_table(int row,int col)
{
	int i;
	
	for(i=0;i<row;i++)
	if(subs_table[i][col]!=P&&subs_table[i][col]==subs_table[row][col])
		return 1;
	for(i=0;i<col;i++)
	if(subs_table[row][i]!=P&&subs_table[row][i]==subs_table[row][col])
		return 1;

	return 0;
}

int new_number(uint64_t a,uint64_t b,uint64_t v)
{
	subs_table[a][b]=v;
	if(repeated_subs_table(a,b))
		return 0;
	return 1;
}

int add_numbers()
{
	uint64_t eq;
	uint64_t a,b,c,d,w,x,y,z;
	uint64_t af;
	
	do {
		af=0; 
		for(eq=0;eq<max_eq_table;eq++){
			a=eq_table[eq][0];
			b=eq_table[eq][1];
			c=eq_table[eq][2];
			d=eq_table[eq][3];	
			w=subs_table[a][b];
			x=subs_table[c][d];
			y=subs_table[a][c];
			z=subs_table[b][d];
			if(w!=P&&x!=P&&y!=P&&z!=P){
				if(subs_table[w][x]==P&&subs_table[y][z]!=P){
					if(!new_number(w,x,subs_table[y][z]))
						return 0;
					af++;
				}else if(subs_table[w][x]!=P&&subs_table[y][z]==P){
					if(!new_number(y,z,subs_table[w][x]))
						return 0;
					af++;
				}else if(subs_table[w][x]!=P&&subs_table[y][z]!=P){
					if(subs_table[w][x]!=subs_table[y][z])
						return 0;
				}
			}
		}
	} while (af!=0);

	return 1;	
}

int test_coherencia(uint64_t a,uint64_t b)
{
	uint64_t eq;
	uint64_t w,x,y,z;

	for(eq=0;eq<max_eq_table;eq++){
		w=eq_table[eq][0];
		x=eq_table[eq][1];
		y=eq_table[eq][2];
		z=eq_table[eq][3];
		if(subs_table[w][x]!=P&&subs_table[y][z]!=P&&
			subs_table[w][y]!=P&&subs_table[x][z]!=P&&
			subs_table[subs_table[w][x]][subs_table[y][z]]!=P&&
			subs_table[subs_table[w][y]][subs_table[x][z]]!=P&&
			subs_table[subs_table[w][x]][subs_table[y][z]]!=
			subs_table[subs_table[w][y]][subs_table[x][z]])
				return 0;
	}
	return 1;
}

void print_subs_table()
{
	int row,col;
	
	for(row=0;row<P;row++){
		for(col=0;col<P;col++)
			printf("%2lx ",subs_table[row][col]); 
		printf("\n");
	}
	printf("\n");
}

int coherent()
{
	int i,j,k,l;
	
	for(i=0;i<P;i++)
	for(j=0;j<P;j++)
	for(k=0;k<P;k++)
	for(l=0;l<P;l++)
	if(subs_table[subs_table[i][j]][subs_table[k][l]]!=
			subs_table[subs_table[i][k]][subs_table[j][l]])
		return 0;
	return 1;
}

void gen_sequency(uint64_t seq[P])
{
	int i,j;
	
	for(i=0;i<P;i++){
		do {
			seq[i]=rand()%P;
			for(j=0;j<i;j++)
			if(seq[j]==seq[i])
				break;
		} while(j<i);
	}
}

void testar()
{
	int col,row,i;
	uint64_t seq[P];

	col=0;row=0;
	while(subs_table[row][col]!=P){
		if(row<col)
			row++;
		else if (col>0)
			col--;
		else {
			col=row+1;
			row=0;
			if(col==P){
				print_subs_table();
				return;			
			}
		}
	}

	memcpy(subs_stack[top_subs_stack++],subs_table,sizeof(subs_table));

	gen_sequency(seq);

	for(i=0;i<P;i++){
		memcpy(subs_table,subs_stack[top_subs_stack-1],sizeof(subs_table));
		new_number(row,col,seq[i]);
		if(repeated_subs_table(row,col))
			continue;
		if(!add_numbers())
			continue;
		if(!test_coherencia(row,col))
			continue;
		testar();
	}
	
	memcpy(subs_table,subs_stack[--top_subs_stack],sizeof(subs_table));
	
}

int repeated_eq_table(uint64_t a,uint64_t b,uint64_t c,uint64_t d)
{
	int i,e,f,g,h;	
	
	for(i=0;i<max_eq_table;i++){
		e=eq_table[i][0];
		f=eq_table[i][1];
		g=eq_table[i][2];
		h=eq_table[i][3];
		if(a==e&&b==f&&c==g&&d==h ||
		   a==e&&c==f&&b==g&&d==h)
		   	return 1;
	}
	return 0;
}

void fill_eq_table()
{
	int i,j,k,l,m;
	
	for(i=0;i<P;i++)
	for(j=0;j<P;j++)
	for(k=0;k<P;k++)
	for(l=0;l<P;l++)
	if(j!=k&&i!=l)
	if(!repeated_eq_table(i,j,k,l)){
		eq_table[max_eq_table][0]=i;
		eq_table[max_eq_table][1]=j;
		eq_table[max_eq_table][2]=k;
		eq_table[max_eq_table][3]=l;
		max_eq_table++;
	}
}

void empty_subs_table()
{
	int row,col;
	
	for(row=0;row<P;row++)
	for(col=0;col<P;col++)
		subs_table[row][col]=P;
}

void main()
{
	fill_eq_table();
	empty_subs_table();

	testar();
}
