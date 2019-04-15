#include "book.h"

int main()
{
	int i = 0;
	int a = 0;
	int n = 0;
	int b = 5;
	int f = 0;
	char name_f[20] = "";
	FILE* pf = NULL;
	errno_t err;
	struct data* p = (struct data*)malloc(sizeof(struct data)*b);
	initial(pf, p, &n);
	while (1)
	{
		system("cls");
		print();
		scanf_s("%d", &a);
		system("cls");
		switch (a)
		{
		case add:
		{
			if (n != b)
			{
				n = add_m(p, n);
			}
			else
			{
				b = b + 5;
				struct data* p2 = (struct data*) realloc(p, sizeof(struct data)*b);
				if (p2 != NULL)
				{
					struct data* p = p2;
				}
				n = add_m(p, n);
			}
			break;
		}
		case del:
		{
			printf("������Ҫɾ�����˵�����:");
			scanf_s("%s", name_f, 20);
			f = find_f(name_f, p, n);
			if (f != -1)
			{
				del_d(p, f, n);
				n = n - 1;
				printf("ɾ���ɹ�\n");
				system("pause");
			}
			else
			{
				printf("���޴���\n");
				system("pause");
			}
			break;
		}
		case find:
		{
			printf("������Ҫ���ҵ��˵�����:");
			scanf_s("%s", name_f, 20);
			f = find_f(name_f, p, n);
			if (f != -1)
			{
				printf("%-9s,%-9s,%-9d,%-9d,%-9s\n", (p + f)->name, (p + f)->sex, (p + f)->age, (p + f)->phone_number, (p + f)->address);
			}
			else
			{
				printf("\n���޴���\n\n");
			}
			system("pause");
			break;
		}
		case mod:
		{
			printf("������Ҫ�޸ĵ��˵�����:");
			scanf_s("%s", name_f, 20);
			f = find_f(name_f, p, n);
			if (f != -1)
			{
				mod_m(p, f);
			}
			else
			{
				printf("\n���޴���\n\n");
				system("pause");
			}
			break;
		}
		case show:
		{
			show_m(p, n);
			system("pause");
			break;
		}
		case emp:
		{
			free(p);
			b = 5;
			n = 0;
			struct data* p = (struct data*)malloc(sizeof(struct data)*b);
			printf("����ɹ�\n");
			break;
		}
		case sort:
		{
			sort_s(p, n);
			break;
		}
		case exi:
		{
			err = fopen_s(&pf, "book.txt", "wb");
			if (pf != NULL)
			{
				fwrite((p + i), sizeof(struct data), n, pf);
				fclose(pf);
			}
			else
			{
				perror("open file");
				exit(1);
			}
			free(p);
			p = NULL;
			return 0;
		}
		default:
		{
			printf("������Ч������������һ��\n");
			system("pause");
		}
		}

	}


}