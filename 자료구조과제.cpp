#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SUB 30 //과목 수
#define MAX_SUB_LEN 30 //과목 이름 크기
#define MAX_NAME_LEN 30
#define MAX_STU 100 //학생 수
#define MAX_MAJOR 15 //학과 길이
/*

선수과목 순서대로 출력이 무슨 뜻인지 예제를 보아도 이해가 잘 안 갑니다.
과제에 대한 질문은 받지 않는다 하셔서, 일단 제 임의대로 출력했습니다.
미수강 과목을 듣기 위해선, 어떤 과목(그 과목의 수강하지 않은 선수과목)들을 들어야 하는 지 출력하는 식으로 하였습니다.

그리고 선수과목이 없는 과목들은 따로 모아서 출력했습니다.

*/
struct Student {
	char name[MAX_NAME_LEN];
	char major[MAX_MAJOR];
	float grade;
	int num = 0;
	int year;
	int visited_sub[MAX_SUB_LEN] = { 0 };
};

struct Node {
	char code[5]; // code
	char name[MAX_NAME_LEN];
	Node *next;
};

struct Graph {
	int n;
	Node list[MAX_SUB];
};


Student table[MAX_STU];
int stu_n = 0;
Graph sub_list;


int hash_func(int d) {
	int value = 0;
	while (d) {
		value += d % 10;
		d /= 10;
	}
	return value % MAX_STU;
}

void read_sub(){
	FILE *f = NULL;

	char tmp[100];
	char *ptr;
	Node *newnode;
	if ((f = fopen("교과목록.csv", "r")) == NULL) {
		printf("교과목록.csv 파일이 존재하지 않습니다.\n\n");
		exit(0);
	}
	
	sub_list.n = 0;

	fgets(tmp, 100, f);

	while(fgets(tmp, 100, f) != NULL){
		//printf("%s\n", tmp);
		ptr = strtok(tmp, ",");
		sub_list.list[sub_list.n].next = NULL;
		
		strcpy(sub_list.list[sub_list.n].code, ptr);
		ptr = strtok(NULL, ",");
		strcpy(sub_list.list[sub_list.n].name, ptr);
		
		

		while ((ptr = strtok(NULL, ", \n\"")) != NULL) {
			//printf("%s\n", ptr);
			if (strncmp(ptr, "None",4) == 0) {break; }

			newnode = (Node*)malloc(sizeof(Node));
			strcpy(newnode->code, ptr);
			newnode->next = sub_list.list[sub_list.n].next;
			sub_list.list[sub_list.n].next = newnode;
		}
		sub_list.n += 1;
	}	
	fclose(f);
}

void read_stu() {
	FILE *f = NULL;

	char tmp[100];
	char a[100];
	char *ptr;
	int num,idx;
	if ((f = fopen("학생명단.csv", "r")) == NULL) {
		printf("학생명단.csv 파일이 존재하지 않습니다.\n\n");
		exit(0);
	}

	

	fgets(tmp, 100, f);
	
	while (fgets(tmp, 100, f) != NULL) {
		ptr = strtok(tmp, ",");
		num = atoi(ptr);
		idx = hash_func(num);
		
		while (table[idx].num != 0)
			idx = (idx+1)%MAX_STU;
		table[idx].num = num;
		

		ptr = strtok(NULL, ",");
		strcpy(table[idx].name, ptr);

		ptr = strtok(NULL, ",");
		strcpy(table[idx].major, ptr);

		ptr = strtok(NULL, ",");
		table[idx].year = atoi(ptr);
		
		ptr = strtok(NULL, ",");
		table[idx].grade = atof(ptr);

		while ((ptr = strtok(NULL, ", \n\"")) != NULL) {
			
			for (int i = 0; i < sub_list.n; i++) {
				if (strcmp(sub_list.list[i].code, ptr)==0) {
					table[idx].visited_sub[i] = 1;
					break;
				}
			}
			
		}
		stu_n += 1;
	}
	fclose(f);
}

void add_stu() {
	int tmp1,j, idx, i = 0;
	char tmp2[MAX_SUB_LEN]; //or NAME_LEN
	float tmp3;

	printf("학번을 입력하세요: "); scanf(" %d", &tmp1);
	idx = hash_func(tmp1);
	while (table[idx].num != 0) {
		if (table[idx].num == tmp1){
			printf("이미 존재하는 학번입니다.\n\n");
			return;
		}
		else if (i == MAX_STU) {
			printf("더 이상 저장할 수 없습니다.\n\n");
			return;
		}
		i++;
		idx = (idx + 1) % MAX_STU;
	}
	table[idx].num = tmp1;
	
	printf("이름을 입력하세요 : "); scanf(" %s", tmp2);
	strcpy(table[idx].name, tmp2);

	printf("학과를 입력하세요 : "); scanf(" %s", tmp2);
	strcpy(table[idx].major, tmp2);

	printf("학년을 입력하세요 : "); scanf(" %d", &tmp1);
	table[idx].year = tmp1;

	printf("학점을 입력하세요 : "); scanf(" %f", &tmp3);
	table[idx].grade = tmp3;

	printf("이수한 과목의 수를 입력하세요 : "); scanf(" %d", &tmp1);
	printf("이수한 과목코드 %d개를 입력하세요 \n", tmp1);
	for (i = 0; i < tmp1; i++) {
		scanf(" %s", tmp2);
		for (j = 0; j < sub_list.n; j++) {
			if (strcmp(sub_list.list[j].code, tmp2) == 0) {
				table[idx].visited_sub[j] = 1;
				break;
			}
		}
	}
	stu_n += 1;
	printf("추가되었습니다.\n\n");
}

void del_stu() {
	int d, value, i = 0;
	printf("삭제할 학생의 학번을 입력하세요 : "); scanf(" %d", &d);
	value = hash_func(d);

	while (table[value].num != 0 && table[value].num != d && i != MAX_STU) {
		value = (value + 1) % MAX_STU;
		i++;
	}


	if (table[value].num == d) {
		table[value].num = 0;
		for (i = 0; i < sub_list.n; i++) {
			table[value].visited_sub[i] = 0;
		}
		printf("삭제되었습니다.\n\n");
	}
	else
		printf("학번이 %d인 학생은 존재하지 않습니다.\n\n", d);
}

void print_sub(int value) {
	int tmp,tmp2, i,j, top, front, rear;
	int st[MAX_SUB];//print
	int qu[MAX_SUB];
	int visited[MAX_SUB] = { 0 };
	Node* tmp_node;

	
	for (i = 0; i < sub_list.n; i++) {
		if (table[value].visited_sub[i] == 1) {
			visited[i] = 1;
		}
	}

	printf("미수강 과목 리스트:\n");
	top = -1;
	for (i = 0; i < sub_list.n; i++) {
		if (sub_list.list[i].next == NULL && visited[i] == 0) {
			st[++top] = i;
		}
	}
	if (top == -1) {}
	else {
		printf("선수 과목 없는 과목들 : ");
		for (i = top; i >= 0; i--) {
			printf("%s ", sub_list.list[st[i]].name);
		}
		printf("\n-----------------------\n\n");
	}
	
	for (i = 0; i < sub_list.n; i++) {

		
		if (sub_list.list[i].next != NULL && visited[i] == 0) {
			front = 0; rear = 0, top = -1;
			tmp2 = i;
			qu[rear] = i;
			rear = ++rear%MAX_SUB;
			
			while (rear!= front) {
				tmp = qu[front]; front = ++front %  MAX_SUB;
				tmp_node = sub_list.list[tmp].next;
				st[++top] = tmp;
				while (tmp_node!=NULL) {
					for (j = 0; j < sub_list.n; j++) {
						if (strcmp(tmp_node->code, sub_list.list[j].code) == 0) break;
					}
					
					if (visited[j] == 0) {
						qu[rear] = j; rear = ++rear%MAX_SUB;
					}
					tmp_node = tmp_node->next;
				}
			}

			printf("%s\n\n", sub_list.list[tmp2].name);
			if (top == 0) printf("수강해야 할 선수과목은 없습니다.\n\n");
			else {
				printf("수강해야 할 선수과목: ");
				while (top > 0) {
					printf("*%s* ", sub_list.list[st[top--]].name);
				}
				printf("\n");
			}
			printf("-----------------------\n\n");
		}
		
		
	}
	
}

void quit() {
	FILE *f;
	int i,j;
	f=fopen("student.txt", "w");

	fprintf(f,"%-18s%-30s%-16s%-7s%-7s%s\n", "학번", "이름", "학과","학년","학점","이수한 과목코드");
	for (i = 0; i < MAX_STU; i++) {
		if (table[i].num!=0) {
			fprintf(f, "%-15d%-30s%-15s%-7d%-5.2f ", table[i].num, table[i].name, table[i].major, table[i].year, table[i].grade);
			for (j = 0; j < sub_list.n; j++) {
				if (table[i].visited_sub[j] != 0) {
					fprintf(f, "%s ", sub_list.list[j].code);
				}
			}
			fprintf(f,"\n");
		}
	}

	printf("프로그램이 종료되었습니다\n");
	exit(0);
}

void search() {
	int d, value, i = 0;
	printf("검색할 학생의 학번을 입력하세요 : "); scanf(" %d", &d);
	value = hash_func(d);

	while (table[value].num != 0 && table[value].num != d && i != MAX_STU) {
		value = (value + 1) % MAX_STU; i++;
	}
	
	

	if (table[value].num == d) {
		printf("학번 : %d\n", d);
		printf("이름 : %s\n",table[value].name);
		printf("학과 : %s\n", table[value].major);
		printf("학년 : %d\n", table[value].year);
		printf("학점 : %.2f\n", table[value].grade);
		print_sub(value);
	}
	else
		printf("학번이 %d인 학생은 존재하지 않습니다.\n\n",d);
}

void sort_grade() {
	int i,j,k = 0,tmp_g, tmp_m, target, tmp, s_idx, f_idx;
	int grade_list[MAX_STU];
	char tmp_n[MAX_NAME_LEN];

	printf("검색할 학년을 입력하세요 : "); scanf(" %d", &target);

	for (i = 0; i < MAX_STU; i++) {
		if (table[i].num != 0 && table[i].year == target) {
			grade_list[k++] = i;
		}
	}
	
	for (i = 0; i < k-1; i++) {
		tmp_m = i;

		for (j = i + 1; j < k; j++) {
			if (table[grade_list[tmp_m]].grade < table[grade_list[j]].grade) {
				tmp_m = j;
			}
		}
		tmp = grade_list[i];
		grade_list[i] = grade_list[tmp_m];
		grade_list[tmp_m] = tmp;
	}
	
	
	s_idx = 0; f_idx = 0;
	for (i = 0; i < k; i++) {
		tmp_g = table[grade_list[i]].grade;
		s_idx = i;

		i += 1;
		while (1) {
			if (i == k) { f_idx = i - 1;  break; }
			
			if (table[grade_list[i]].grade != tmp_g) {
				tmp_g = table[grade_list[i]].grade;
				f_idx = i-1;
				break;
			}
			i++;
		}
		if (s_idx != f_idx) {
			for (s_idx = 0; s_idx <= f_idx-1 ; s_idx++) {
				tmp_m = s_idx;

				for (j = s_idx + 1; j <= f_idx; j++) {
					if (strcmp(table[grade_list[tmp_m]].name, table[grade_list[j]].name) > 0) {
						tmp_m = j;
					}
				}
				strcpy(tmp_n, table[grade_list[tmp_m]].name);
				strcpy(table[grade_list[tmp_m]].name, table[grade_list[s_idx]].name);
				strcpy(table[grade_list[s_idx]].name, tmp_n);
			}
		}
		
	}


	printf("%-15s%-30s%-5s\n", "학번", "이름","학점");
	for (i = 0; i < k; i++) {
		printf("%-15d%-30s%.2f\n", table[grade_list[i]].num, table[grade_list[i]].name, table[grade_list[i]].grade);
	}
	printf("\n\n");
}


int main() {
	int flag;
	

	printf("학생정보관리프로그램\n\n");

	read_sub();
	read_stu();
	


	
	while (1) {
		printf("====================\n");
		printf("1. 학생정보검색\n");
		printf("2. 학생정보추가\n");
		printf("3. 학생정보삭제\n");
		printf("4. 학년별 학점순위\n");
		printf("5. 프로그램 종료\n");
		printf("====================\n\n");

		printf("사용할 번호를 입력하세요 : "); scanf(" %d", &flag);
		switch (flag) {
			case 1:
				search();
				break;
			case 2:
				add_stu();
				break;
			case 3:
				del_stu();
				break;
			case 4:
				sort_grade();
				break;
			case 5:
				quit();
				break;
		}
	}

	return 0;
}