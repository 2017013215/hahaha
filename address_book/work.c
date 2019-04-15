#include "book.h"

void print()
{
	printf("********************************\n");
	printf("******* 1. ���   2. ɾ�� ******\n");
	printf("******* 3. ����   4. �޸� ******\n");
	printf("*** 5.��ʾȫ��    6.���ȫ�� ***\n");
	printf("*******    7.����������   ******\n");
	printf("*******     0.�˳�����    ******\n");
	printf("********************************\n");
	printf("��ѡ����һ�����еĲ�����");
}

int add_m(struct data* s, int n)
{
	printf("������������\n");
	scanf_s("%s", (s + n)->name,20);
	printf("�������Ա�\n");
	scanf_s("%s", (s + n)->sex,10);
	printf("���������䣺\n");
	scanf_s("%d", &((s + n)->age));
	printf("������绰����\n");
	scanf_s("%d", &((s + n)->phone_number));
	printf("�������ַ��\n");
	scanf_s("%s", (s + n)->address,30);
	return n + 1;
}


void show_m(struct data* s, int n)
{
	if (n != 0)
	{
		int i = 0;
		printf("����            �Ա�       ����      �绰����               ��ַ\n");
		for (i = 0; i < n; i++)
		{
			printf("%-15s,%-9s,%-9d,%-20d,%-9s\n", (s + i)->name, (s + i)->sex, (s + i)->age, (s + i)->phone_number, (s + i)->address);
		}
	}
	else
	{
		printf("�绰����������\n");
	}
}



int find_f(char name_f[20],struct data* s, int n)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < n; i++)
	{
		for(j=0;j<20;j++)
		{
			if (name_f[i] != s->name[i])
				break;
		}
		if (j == 20)
		{
			return i;
		}
	}
	return -1;
}

int mod_m(struct data* s, int f)
{
	int a1 = 0;

	while (1)
	{
		printf("Ҫ�޸ĵ���\n 1.�Ա� 2.���� 3.�绰���� 4.��ַ 0.ȷ���޸�\n");
		scanf_s("%d", &a1);
		switch (a1)
		{
		case 1:
		{
			printf("���������ֵ:");
			scanf_s("%s", (s + f)->sex, 10);
			printf("�޸ĳɹ�\n");
			break;
		}
		case 2:
		{
			printf("���������ֵ:");
			scanf_s("%d", &((s + f)->age));
			printf("�޸ĳɹ�\n");
			break;
		}
		case 3:
		{
			printf("���������ֵ:");
			scanf_s("%d", &((s + f)->phone_number));
			printf("�޸ĳɹ�\n");
			break;
		}
		case 4:
		{
			printf("���������ֵ:");
			scanf_s("%s", (s + f)->address, 30);
			printf("�޸ĳɹ�\n");
			break;
		}
		case 0:
		{
			printf("����޸�\n");
			system("pause");
			return 0;
		}
		}

	}

}


void del_d(struct data* s, int f, int n)
{
	int i = 0;
	int j = 0;
	i = f;
	for (i; i < n; i++)
	{
		for (j = 0; j < 20; j++)
		{
			(s + i)->name[j] = (s + i + 1)->name[j];
		}
		for (j = 0; j < 10; j++)
		{
			(s + i)->sex[j] = (s + i + 1)->sex[j];
		}
		(s + i)->age = (s + i + 1)->age;
		(s + i)->phone_number = (s + i + 1)->phone_number;
		for (j = 0; j < 30; j++)
		{
			(s + i)->address[j] = (s + i + 1)->address[j];
		}

	}
}




void sort_s(struct data* s,int n)
{
	qsort(s, n, sizeof(struct data), struct_cmp);

}


int struct_cmp(const struct data* s1, const struct data* s2)
{
	int i = 0;
	while (i < 20 && (s1->name[i] != '\0') && (s2->name[i] != '\0'))
	{
		if (s1->name[i] != s2->name[i])
		{
			if (s1->name[i] > s2->name[i])
				return 1;
			else if (s1->name[i] < s2->name[i])
				return -1;
		}
		i++;
	}
	return 0;
}



void initial(FILE* pf, const struct data* s,int* n)
{
	errno_t err1;
	err1 = fopen_s(&pf, "book.txt", "rb");
	if (pf != NULL)
	{
		while(fread(s+(*n), sizeof(struct data), 1, pf))
		{
			*n=(*n)+1;
		}
		fclose(pf);
	}
	else
	{
		perror("open file");
		exit(1);
	}
	
	
}
