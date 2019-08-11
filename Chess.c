#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<pthread.h>
#include<windows.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
typedef struct block		// Each block structure template
{
	char pos[3];
	int side_code;
	int color;
	int player_code;
	char piece_name[10];
	struct block *N,*S,*E,*W,*NE,*NW,*SE,*SW;		
}NODE;
NODE *a[9],*b[9],*c[9],*d[9],*e[9],*f[9],*g[9],*h[9],*head;		// Blocks declared globally
NODE* create();
void linking();
void initial_position();
void player_position(int);
void piece_location();
NODE* source_address(char s[3]);
NODE* destination_address(char d[3]);
int PAWN(NODE* src,NODE* dst,int);	//if int = 2 or 7. If int=2 valid move is SW,S,NW.if int=7 valid move is NW,N,NE
int ROOK(NODE* src,NODE* dst);
int KNIGHT(NODE* src,NODE* dst);
int BISHOP(NODE* src,NODE* dst);
int KING(NODE* src,NODE* dst);
int QUEEN(NODE* src,NODE* dst);
int replace(NODE* src,NODE* dst); 	//function to replace destination piece with source piece and make source empty block
void display();
void position_address();
void clean();
void color_code();

int main(int argc, char const *argv[])
{	int i;
	int success;	// success = 1 ,if(piece move is successful) else 0
	char source_position[3];char destination_position[3];
	NODE *source,*dest;
	head = create();
	for( i=1; i<=8; i++)
	{
		a[i] = create();b[i] = create();c[i] = create();d[i] = create();e[i] = create();f[i] = create();
		g[i] = create();h[i] = create();
	}
	linking();initial_position();player_position(0);piece_location();position_address();color_code();
	printf("Starts from WHITE side \n");
//	printf("Enter source,destination position : \n");
	display();
	for(;;)
	{
		scanf("%s",source_position);scanf("%s",destination_position);
		printf("%s : %s\n",source_position,destination_position);
		source = source_address(source_position);
		dest = destination_address(destination_position);
		if(source==NULL)
			{printf("Source position is wrong, Re-Enter Src and Dst\n");continue;}
		if(dest==NULL)
			{printf("Destination position is wrong, Re-Enter Src and Dst");continue;}
//		printf("\t%d\t%d\n",source->side_code,dest->side_code);	
		if(strcmp(source->piece_name,"PAWN")==0)
		success = PAWN(source,dest,source->side_code);
		if(strcmp(source->piece_name,"ROOK")==0)
		success = ROOK(source,dest);
		if(strcmp(source->piece_name,"KNIGHT")==0)
		success = KNIGHT(source,dest);
		if(strcmp(source->piece_name,"BISHOP")==0)
		success = BISHOP(source,dest);
		if(strcmp(source->piece_name,"KING")==0)
		success = KING(source,dest);
		if(strcmp(source->piece_name,"QUEEN")==0)
		success = QUEEN(source,dest);
		if(success==0)
			{printf("Invalid move !! Re-Enter \n");continue;}
		else {system("cls");
		display();continue;}
			
	}
	return 0;
}
//*********************
NODE* create()		//Node create function with default direction to NULL
{
	NODE* newnode = (NODE*)malloc(sizeof(struct block));	//memory allocation
	newnode -> N = NULL;
	newnode -> S = NULL;
	newnode -> E = NULL;
	newnode -> W = NULL;
	newnode -> NE = NULL;
	newnode -> NW = NULL;
	newnode -> SE = NULL;
	newnode -> SW = NULL;
	return newnode;
}
//*********************
void linking()		// Block direction to blocks function
{
	a[1]->E=b[1];a[1]->S=a[2];a[1]->SE=b[2];	
	b[1]->W=a[1];b[1]->SW=a[2];b[1]->S=b[2];b[1]->SE=c[2];b[1]->E=c[1]; 
	c[1]->W=b[1];c[1]->SE=b[2];c[1]->S=c[2];c[1]->SE=d[2];c[1]->E=d[1];		
	d[1]->W=c[1];d[1]->S=d[2];d[1]->SW=c[2];d[1]->E=e[1];d[1]->SE=e[2]; 
	e[1]->E=f[1];e[1]->SE=f[2];e[1]->S=e[2];e[1]->W=d[1];e[1]->SW=d[2];	
	f[1]->W=e[1];f[1]->SW=e[2];f[1]->S=f[2];f[1]->SE=g[2];f[1]->E=g[1];
	g[1]->W=f[1];g[1]->SW=f[2];g[1]->S=g[2];g[1]->SE=h[2];g[1]->E=h[1];		
	h[1]->W=g[1];h[1]->SW=g[2];h[1]->S=h[2];
	
	for(int ax=1,by=2,cz=3,i=1; i<=6; i++,ax++,by++,cz++)
	{
	a[by]->N=a[ax];a[by]->NE=b[ax];a[by]->E=b[by];a[by]->SE=b[cz];a[by]->S=a[cz];		
	b[by]->N=b[ax];b[by]->NE=c[ax];b[by]->E=c[by];b[by]->SE=c[cz];b[by]->S=b[cz];b[by]->SW=a[cz];b[by]->W=a[by];b[by]->NW=a[ax];
	c[by]->N=c[ax];c[by]->NE=d[ax];c[by]->E=d[by];c[by]->SE=d[cz];c[by]->S=c[cz];c[by]->SW=b[cz];c[by]->W=b[by];c[by]->NW=b[ax];	
	d[by]->N=d[ax];d[by]->NE=e[ax];d[by]->E=e[by];d[by]->SE=e[cz];d[by]->S=d[cz];d[by]->SW=c[cz];d[by]->W=c[by];d[by]->NW=c[ax];	
	e[by]->N=e[ax];e[by]->NE=f[ax];e[by]->E=f[by];e[by]->SE=f[cz];e[by]->S=e[cz];e[by]->SW=d[cz];e[by]->W=d[by];e[by]->NW=d[ax];	
	f[by]->N=f[ax];f[by]->NE=g[ax];f[by]->E=g[by];f[by]->SE=g[cz];f[by]->S=f[cz];f[by]->SW=e[cz];f[by]->W=e[by];f[by]->NW=e[ax];
	g[by]->N=g[ax];g[by]->NE=h[ax];g[by]->E=h[by];g[by]->SE=h[cz];g[by]->S=g[cz];g[by]->SW=f[cz];g[by]->W=f[by];g[by]->NW=f[ax];	
	h[by]->N=h[ax];h[by]->SW=g[cz];h[by]->S=h[cz];h[by]->NW=g[ax];h[by]->W=g[by];
	}
	
	a[8]->N=a[7];a[8]->NE=b[7];a[8]->E=b[8];
	b[8]->N=b[7];b[8]->NE=c[7];b[8]->E=c[8];b[8]->W=a[8];b[8]->NW=a[7];
	c[8]->N=c[7];c[8]->NE=d[7];c[8]->E=d[8];c[8]->W=b[8];c[8]->NW=b[7];
	d[8]->N=d[7];d[8]->NE=e[7];d[8]->E=e[8];d[8]->W=c[8];d[8]->NW=c[7];
	e[8]->N=e[7];e[8]->NE=f[7];e[8]->E=f[8];e[8]->W=d[8];e[8]->NW=d[7];
	f[8]->N=f[7];f[8]->NE=g[7];f[8]->E=g[8];f[8]->W=e[8];f[8]->NW=e[7];
	g[8]->N=g[7];g[8]->NE=h[7];g[8]->E=h[8];g[8]->W=f[8];g[8]->NW=f[7];
	h[8]->N=h[7];h[8]->W=g[8];h[8]->NW=g[7];
	head->E=a[1];	
}
//*********************
void initial_position()
{
	char rook[10]="ROOK",knight[10]="KNIGHT",bishop[10]="BISHOP",king[10]="KING",queen[10]="QUEEN",pawn[10]="PAWN";
	strcpy(a[1]->piece_name,rook);strcpy(a[8]->piece_name,rook);strcpy(h[1]->piece_name,rook);strcpy(h[8]->piece_name,rook);
	strcpy(b[1]->piece_name,knight);strcpy(b[8]->piece_name,knight);strcpy(g[1]->piece_name,knight);strcpy(g[8]->piece_name,knight);
	strcpy(c[1]->piece_name,bishop);strcpy(c[8]->piece_name,bishop);strcpy(f[1]->piece_name,bishop);strcpy(f[8]->piece_name,bishop);
	strcpy(d[1]->piece_name,king);strcpy(d[8]->piece_name,king);
	strcpy(e[1]->piece_name,queen);strcpy(e[8]->piece_name,queen);
	strcpy(a[2]->piece_name,pawn);strcpy(b[2]->piece_name,pawn);strcpy(c[2]->piece_name,pawn);strcpy(d[2]->piece_name,pawn);
	strcpy(e[2]->piece_name,pawn);strcpy(f[2]->piece_name,pawn);strcpy(g[2]->piece_name,pawn);strcpy(h[2]->piece_name,pawn);
	strcpy(a[7]->piece_name,pawn);strcpy(b[7]->piece_name,pawn);strcpy(c[7]->piece_name,pawn);strcpy(d[7]->piece_name,pawn);
	strcpy(e[7]->piece_name,pawn);strcpy(f[7]->piece_name,pawn);strcpy(g[7]->piece_name,pawn);strcpy(h[7]->piece_name,pawn);
}
//*********************
void player_position(int x)
{	
	int pos,pos_def=0;
	if(x==0) pos=5;
	else pos=6;
	for(int i=1; i<=2; i++)
	{	a[i]->player_code=pos;b[i]->player_code=pos;c[i]->player_code=pos;d[i]->player_code=pos;
		e[i]->player_code=pos;f[i]->player_code=pos;g[i]->player_code=pos;h[i]->player_code=pos;}
	if(x==0) pos=6;
	else pos=5;
	for(int i=7; i<=8; i++)
	{	a[i]->player_code=pos;b[i]->player_code=pos;c[i]->player_code=pos;d[i]->player_code=pos;
		e[i]->player_code=pos;f[i]->player_code=pos;g[i]->player_code=pos;h[i]->player_code=pos;}
	for(int i=3; i<=6; i++)
	{	a[i]->player_code=pos_def;b[i]->player_code=pos_def;c[i]->player_code=pos_def;d[i]->player_code=pos_def;
		e[i]->player_code=pos_def;f[i]->player_code=pos_def;g[i]->player_code=pos_def;h[i]->player_code=pos_def;}	
}
//*********************
void piece_location()
{
	a[2]->side_code=2;b[2]->side_code=2;c[2]->side_code=2;d[2]->side_code=2;
	e[2]->side_code=2;f[2]->side_code=2;g[2]->side_code=2;h[2]->side_code=2;
	a[7]->side_code=7;b[7]->side_code=7;c[7]->side_code=7;d[7]->side_code=7;
	e[7]->side_code=7;f[7]->side_code=7;g[7]->side_code=7;h[7]->side_code=7;
	for(int i=3; i<=6; i++)
	{a[i]->side_code=-5;b[i]->side_code=-5;c[i]->side_code=-5;d[i]->side_code=-5;
	e[i]->side_code=-5;f[i]->side_code=-5;g[i]->side_code=-5;h[i]->side_code=-5;}
	a[1]->side_code=-5;b[1]->side_code=-5;c[1]->side_code=-5;d[1]->side_code=-5;
	e[1]->side_code=-5;f[1]->side_code=-5;g[1]->side_code=-5;h[1]->side_code=-5;
	a[8]->side_code=-5;b[8]->side_code=-5;c[8]->side_code=-5;d[8]->side_code=-5;
	e[8]->side_code=-5;f[8]->side_code=-5;g[8]->side_code=-5;h[8]->side_code=-5;
}
//*********************
NODE* source_address(char source_position[3]) //check if source address comes inside chess block and return exact source block address
{
	int x,y;
	NODE *s;
	s = create();
	s = a[1];
	x=source_position[0]-97;
	y=source_position[1]-49;
	if(x<0 || x>=8)
	return NULL;
	if(y<0 || y>=8)
	return NULL;
	while(x>0)
	{
		s = s->E;
		x--;
	}
	while(y>0)
	{
		s = s->S;
		y--;
	}
	return s;
}
//*********************
NODE* destination_address(char destination_position[3])//check if destination address comes inside chess block and return exact destination block address
{
	int x,y;
	NODE *d;
	d = create();
	d = a[1];
	x=destination_position[0]-97;
	y=destination_position[1]-49;
	if(x<0 || x>=8)
	return NULL;
	if(y<0 || y>=8)
	return NULL;
	while(x>0)
	{
		d = d->E;
		x--;
	}
	while(y>0)
	{
		d = d->S;
		y--;
	}
	return d;
}
//*********************
int PAWN(NODE* src,NODE* dst,int side)
{
	NODE* step;
	step = create();
	step = src;
/*	printf("%s\n",src->piece_name);
	printf("%s\n",dst->piece_name);
	printf("%d\n",step->player_code);*/
	if(side==7)
	{	
		if(step->NW!=NULL && step->NW->player_code!=step->player_code && step->NW->player_code!=0 && step->NW==dst)
		{replace(src,dst);return 1;}
		step = src;
		if(step->NE!=NULL && step->NE->player_code!=step->player_code && step->NE->player_code!=0 && step->NE==dst)
		{replace(src,dst);return 1;}
		step = src;
		if(step->N!=NULL && step->N->player_code!=5 && step->N->player_code!=6 && step->N==dst)
		{replace(src,dst);
	//	printf("%d\n",src->player_code);
		return 1;}	
	}
	if(side==2)
	{
		if(step->SW!=NULL && step->SW->player_code!=step->player_code && step->SW->player_code!=0 && step->SW==dst)
		{replace(src,dst);return 1;}
		step = src;
		if(step->SE!=NULL && step->SE->player_code!=step->player_code && step->SE->player_code!=0 && step->SE==dst)
		{replace(src,dst);return 1;}
		step = src;
		if(step->S!=NULL && step->S->player_code!=5 && step->S->player_code!=6 && step->S==dst)
		{replace(src,dst);return 1;}	
	}
	return 0;
}
//*********************
int ROOK(NODE* src,NODE* dst)
{
	NODE* step;
	step = create();
/*	printf("%s\n",src->piece_name);
	printf("%s\n",dst->piece_name);*/
	step = src;
	while(step->E!=NULL && step->E->player_code!=src->player_code)
	{
		step=step->E;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->W!=NULL && step->W->player_code!=src->player_code)
	{
		step=step->W;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->N!=NULL && step->N->player_code!=src->player_code)
	{
		step=step->N;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->S!=NULL && step->S->player_code!=src->player_code)
	{
		step=step->S;
		if(step==dst){replace(src,dst);return 1;}
	}
	return 0;
}
//*********************
int KNIGHT(NODE* src,NODE* dst)
{
	NODE* step;
	step = create();
	step = src;
	if(step->N!=NULL)
	{	if(step->N->N!=NULL)
		{	if(step->N->N->W!=NULL)
			{	step=step->N->N->W;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->N!=NULL)
	{	if(step->N->N!=NULL)
		{	if(step->N->N->E!=NULL)
			{	step=step->N->N->E;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->E!=NULL)
	{	if(step->E->E!=NULL)
		{	if(step->E->E->N!=NULL)
			{	step=step->E->E->N;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->E!=NULL)
	{	if(step->E->E!=NULL)
		{	if(step->E->E->S!=NULL)
			{	step=step->E->E->S;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->S!=NULL)
	{	if(step->S->S!=NULL)
		{	if(step->S->S->E!=NULL)
			{	step=step->S->S->E;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->S!=NULL)
	{	if(step->S->S!=NULL)
		{	if(step->S->S->W!=NULL)
			{	step=step->S->S->W;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->W!=NULL)
	{	if(step->W->W!=NULL)
		{	if(step->W->W->S!=NULL)
			{	step=step->W->W->S;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}
	step=src;			
	if(step->W!=NULL)
	{	if(step->W->W!=NULL)
		{	if(step->W->W->N!=NULL)
			{	step=step->W->W->N;
				if(step->player_code!=src->player_code && step==dst)
				{
					replace(src,dst);return 1;
				}}}}																					
	return 0;
}
//*********************
int BISHOP(NODE* src,NODE* dst)
{
	NODE* step;
	step = create();
/*	printf("%s\n",src->piece_name);
	printf("%s\n",dst->piece_name);*/
	step=src;
	while(step->NE!=NULL && step->NE->player_code!=src->player_code)
	{
		step=step->NE;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->NW!=NULL && step->NW->player_code!=src->player_code)
	{
		step=step->NW;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->SW!=NULL && step->SW->player_code!=src->player_code)
	{
		step=step->SW;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->SE!=NULL && step->SE->player_code!=src->player_code)
	{
		step=step->SE;
		if(step==dst){replace(src,dst);return 1;}
	}
	return 0;
//*********************	
}
int KING(NODE* src,NODE* dst)
{
	NODE* step;
	step = create();
/*	printf("%s\n",src->piece_name);
	printf("%s\n",dst->piece_name);*/
	step = src;
	if(step->N==dst && step->N->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->NE==dst && step->NE->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->E==dst && step->E->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->SE==dst && step->SE->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->S==dst && step->S->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->SW==dst && step->SW->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->W==dst && step->W->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	step=src;
	if(step->NW==dst && step->NW->player_code!=src->player_code)
	{replace(src,dst);return 1;}
	return 0;
}
//*********************
int QUEEN(NODE* src,NODE* dst)
{
	NODE* step;
	step = create();
	printf("%d : ",src->player_code);
	printf("%d\n",dst->player_code);
	step = src;
	while(step->E!=NULL && step->E->player_code!=src->player_code)
	{
		step=step->E;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->W!=NULL && step->W->player_code!=src->player_code)
	{
		step=step->W;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->N!=NULL && step->N->player_code!=src->player_code)
	{
		step=step->N;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->S!=NULL && step->S->player_code!=src->player_code)
	{
		step=step->S;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->NE!=NULL && step->NE->player_code!=src->player_code)
	{
		step=step->NE;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->NW!=NULL && step->NW->player_code!=src->player_code)
	{
		step=step->NW;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->SW!=NULL && step->SW->player_code!=src->player_code)
	{
		step=step->SW;
		if(step==dst){replace(src,dst);return 1;}
	}
	step=src;
	while(step->SE!=NULL && step->SE->player_code!=src->player_code)
	{
		step=step->SE;
		if(step==dst){replace(src,dst);return 1;}
	}
	return 0;
}
//*********************
int replace(NODE* src,NODE* dst)
{
	if(dst!=NULL)
	printf("\t\tOut : %s\n",dst->piece_name);
	strcpy(dst->piece_name,src->piece_name);
	dst->side_code = src->side_code;
	dst->player_code = src->player_code;
	strcpy(src->piece_name,"");
	src->player_code = 0;
	src->side_code = 0;
	return 1;
}
void display()
{
	printf("Enter source,destination position : \n");
	for(int i=1; i<=8; i++)
	{
		if(a[i]->player_code!=0)
		{
			if(a[i]->player_code==5)
			{
				printf("ANSI_COLOR_RED");
  				printf("%4s\t\t\n",a[i]->piece_name);
  				printf("ANSI_COLOR_RESET");
			}
			else if(a[i]->player_code==6)
			{
				printf("ANSI_COLOR_RED");
  				printf("%4s\t\t\n",a[i]->piece_name);
  				printf("ANSI_COLOR_RESET");
			}
		}
		else if(a[i]->color==1)
		{
			printf("\033[[0;31m");
  				printf("X\t\t\n");
  				printf("\033[0m;");
		}
		else if(a[i]->color==2)
		{
			printf("\033[[0;32m");
  				printf("X\t\t\n");
  				printf("\033[0m;");
		}
	//	Sleep(1);
	//	fflush(stdout);
	}
}
void position_address()
{
	strcpy(a[1]->pos,"a1");strcpy(b[1]->pos,"b1");strcpy(c[1]->pos,"c1");strcpy(d[1]->pos,"d1");
	strcpy(e[1]->pos,"e1");strcpy(f[1]->pos,"f1");strcpy(g[1]->pos,"g1");strcpy(h[1]->pos,"h1");
	strcpy(a[2]->pos,"a2");strcpy(b[2]->pos,"b2");strcpy(c[2]->pos,"c2");strcpy(d[2]->pos,"d2");
	strcpy(e[2]->pos,"e2");strcpy(f[2]->pos,"f2");strcpy(g[2]->pos,"g2");strcpy(h[2]->pos,"h2");
	strcpy(a[3]->pos,"a3");strcpy(b[3]->pos,"b3");strcpy(c[3]->pos,"c3");strcpy(d[3]->pos,"d3");
	strcpy(e[3]->pos,"e3");strcpy(f[3]->pos,"f3");strcpy(g[3]->pos,"g3");strcpy(h[3]->pos,"h3");
	strcpy(a[4]->pos,"a4");strcpy(b[4]->pos,"b4");strcpy(c[4]->pos,"c4");strcpy(d[4]->pos,"d4");
	strcpy(e[4]->pos,"e4");strcpy(f[4]->pos,"f4");strcpy(g[4]->pos,"g4");strcpy(h[4]->pos,"h4");
	strcpy(a[5]->pos,"a5");strcpy(b[5]->pos,"b5");strcpy(c[5]->pos,"c5");strcpy(d[5]->pos,"d5");
	strcpy(e[5]->pos,"e5");strcpy(f[5]->pos,"f5");strcpy(g[5]->pos,"g5");strcpy(h[5]->pos,"h5");
	strcpy(a[6]->pos,"a6");strcpy(b[6]->pos,"b6");strcpy(c[6]->pos,"c6");strcpy(d[6]->pos,"d6");
	strcpy(e[6]->pos,"e6");strcpy(f[6]->pos,"f6");strcpy(g[6]->pos,"g6");strcpy(h[6]->pos,"h6");
	strcpy(a[7]->pos,"a7");strcpy(b[7]->pos,"b7");strcpy(c[7]->pos,"c7");strcpy(d[7]->pos,"d7");
	strcpy(e[7]->pos,"e7");strcpy(f[7]->pos,"f7");strcpy(g[7]->pos,"g7");strcpy(h[7]->pos,"h7");
	strcpy(a[8]->pos,"a1");strcpy(b[8]->pos,"b1");strcpy(c[8]->pos,"c1");strcpy(d[8]->pos,"d1");
	strcpy(e[8]->pos,"e1");strcpy(f[8]->pos,"f1");strcpy(g[8]->pos,"g1");strcpy(h[8]->pos,"h1");
}
void color_code()
{
	for(int i=1; i<=7; i=i+2)
	{a[i]->color=1;b[i]->color=2;c[i]->color=1;d[i]->color=2;e[i]->color=1;f[i]->color=2;g[i]->color=1;h[i]->color=2;}
	for(int i=2; i<=8; i=i+2)
	{a[i]->color=2;b[i]->color=1;c[i]->color=2;d[i]->color=1;e[i]->color=2;f[i]->color=1;g[i]->color=2;h[i]->color=1;}
}









