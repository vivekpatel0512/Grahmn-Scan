/*
 * 
 *
 * project: hw3
 * name: Vivek Patel
 * user??
 * date: 
 * file: hw3.c
 */


#include "hw3.h"


Point largest_number;

typedef struct 
{
	Point p;
	double angle;
} PolarPoints;

//****************************************Stack definition*********************************************************************************

Point NULL_POINT = {-1,-1};

struct Stack {
    int top;
    unsigned capacity;
    Point* array;
};

struct Stack* createStack(unsigned capacity)
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (Point*)malloc(stack->capacity * sizeof(Point));
    return stack;
}


int isFull(struct Stack* stack)
{
    return stack->top == stack->capacity - 1;
}


int isEmpty(struct Stack* stack)
{
    return stack->top == -1;
}


void push(struct Stack* stack, Point item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
   
}


Point pop(struct Stack* stack)
{
    if (isEmpty(stack))
        return NULL_POINT;
    return stack->array[stack->top--];
}



Point peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return NULL_POINT;
    return stack->array[stack->top];
}


Point prevPoint(struct Stack* stack){
    if (isEmpty(stack))
        return NULL_POINT;

    Point tope = pop(stack);
    Point prev = peek(stack);

    push(stack,tope);

    return prev;
}
int size(struct Stack* stack){
    return stack ->top;
}

//*****************************************************************************************************************************************

void swap(Point *p1, Point *p2){

	Point temp = *p1;
	*p1=*p2;
	*p2=temp;
}

int leftOrRightTurn(Point p1,Point p2, Point p3){

	float crossProduct = (p2.x - p1.x)*(p3.y -p1.y) - (p3.x - p1.x)*(p2.y - p1.y);
    return crossProduct > 0 ? -1:1;

}

int cmp_points(const void *pa, const void *pb){

	Point *p1 = (Point *)pa;                                   
												 
	Point *p2 = (Point *)pb;                          
	
	return atan2(( p1->y - largest_number.y ),(  p1->x - largest_number.x )) - atan2((p2->y - largest_number.y ),( p2->x - largest_number.x )) > 0? 1:-1;
	

}



void sorting_angle(Points points)
{  /*
		getting the point with the maximum value of y and placing it at index 0
    */
	 double y_max=-12345;
	 double x_max=-12345;
	 int max;

	for (int i = 0; i < points.length; i++)
	{	
		if(points.data[i].y >y_max){

			x_max = points.data[i].x;
			y_max = points.data[i].y;
			max=i;
		}
		if(points.data[i].y == y_max){

			if(points.data[i].x < x_max){
				x_max = points.data[i].x;
			    y_max = points.data[i].y;
				max=i;
			}	
		}

	}
	swap(&points.data[max],&points.data[0]);
	largest_number = points.data[0];    // The largest number is at index 0 of the data array of the points.
	qsort(&points.data[1],points.length-1 ,sizeof(Point),cmp_points);  // sort the points by their angle value from data[1] - data[length-1]
	
}




Points convex(Points points)
{	
	
	struct Stack* stack = createStack(1000);
     push(stack,points.data[0]);
     push(stack,points.data[1]);
	
	
     for(int i=2; i<points.length; i++){

         while( size(stack)>=2 && leftOrRightTurn(prevPoint(stack),peek(stack),points.data[i])!=-1){
        
		    pop(stack);
			
         }
		
        push(stack,points.data[i]);
     }

	Point* data = (Point*)malloc(sizeof(Point) * 100);
	Points points2 = {data, 100};
	int leng=0;
	
	while(!isEmpty(stack)) 
	{	
		Point p = pop(stack);

		points2.data[leng].x = p.x;
		points2.data[leng].y = p.y;
		leng++;
	}
	
	points2.data[leng++] = points2.data[0];
	points2.length = leng;
	data = (Point*)realloc(data,sizeof(Point) * (leng+1));
     
    return points2;

}


void write2csv(Points points, char *file)
{
    FILE *f = fopen(file, "w");
	TEST_FILE(f,"write2csv",file);
	Point *data = points.data;
	for (int i = 0; i < points.length; ++i)	
		fprintf(f, "%f,%f\n",data[i].x,data[i].y);	
	fclose(f);
}



Points createData(int seed,int num)
{
	
	srand48(seed); /* seeding the pseudorandom number generator */		
	Point* data = (Point*)malloc(sizeof(Point) * num);
	Points points = {data, num};

	for (int i = 0; i < num; i++) 
	{
		data[i].x = drand48();
		data[i].y = drand48();
	}
	return points;
}

void freePoints(Points points)
{
	free(points.data);
}
