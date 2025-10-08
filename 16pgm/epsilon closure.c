#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 50

bool epstable[MAX][MAX] = {false};
int numstates, numtrans;

void get_enfa()
{
	char transition[MAX], *p;

	printf("Enter the number of states n, states will be named from q0 to q(n-1)\n");
	scanf("%d", &numstates);

	printf("Enter the total number of transistions\n");
	scanf("%d", &numtrans);

	printf("Enter all the transitions as state symbol state(no space in between), eg q0aq1, use # for epsilon\n");
	for (int i = 0; i < numtrans; i++)
	{
		scanf("%s", transition);
//		printf("%s", transition);

		if ((p = strchr(transition, '#')) != NULL)
		{
			int i, j;
			sscanf(transition, "q%d", &i);
			j = atoi(p+2);
			epstable[i][j] = true;
		}
	}
}


void findeclose(int state, bool visited[])
{
	visited[state] = true;

	for (int i = 0; i < numstates; i++)
	{
		if(epstable[state][i] && !visited[i])
		{
			findeclose(i, visited);
		}
	}
}

void printeclose()
{
	for(int i = 0; i < numstates; i++)
	{
		bool visited[MAX] = {false};
		char outstr[MAX] = {'\0'}, tempstr[MAX] = {'\0'};
		findeclose(i, visited);
		printf("e-closure(q%d) = {", i);
		for(int j = 0; j < numstates; j++)
		{
			if(visited[j])
			{
				sprintf(tempstr, "q%d,", j);
				strcat(outstr, tempstr);
			}
		}
		outstr[strlen(outstr)-1] = '\0';
		printf("%s}\n", outstr);
	}
}

int main()
{
	get_enfa();
	printeclose();
	return 0;
}
