// Rodolfo Ferreira Sapateiro 2144328

#include <stdio.h>
#include <stdlib.h>

#define maximo 3
#define minimo 2

struct arvoreB
{
	int item[maximo + 1], cont;
	struct arvoreB *linker[maximo + 1];
};

struct arvoreB *root;

struct arvoreB *criacaoNo(int item, struct arvoreB *filho)
{
	struct arvoreB *novoNo;
	novoNo = (struct arvoreB *)malloc(sizeof(struct arvoreB));
	novoNo->item[1] = item;
	novoNo->cont = 1;
	novoNo->linker[0] = root;
	novoNo->linker[1] = filho;
	return novoNo;
}

void adicionaNo(int item, int pos, struct arvoreB *no, struct arvoreB *filho)
{
	int j = no->cont;
	while (j > pos)
	{
		no->item[j + 1] = no->item[j];
		no->linker[j + 1] = no->linker[j];
		j--;
	}
	no->item[j + 1] = item;
	no->linker[j + 1] = filho;
	no->cont++;
}

void splitNode(int item, int *pvalor, int pos, struct arvoreB *no,
							 struct arvoreB *filho, struct arvoreB **novoNo)
{
	int media, j;

	if (pos > minimo)
		media = minimo + 1;
	else
		media = minimo;

	*novoNo = (struct arvoreB *)malloc(sizeof(struct arvoreB));
	j = media + 1;
	while (j <= maximo)
	{
		(*novoNo)->item[j - media] = no->item[j];
		(*novoNo)->linker[j - media] = no->linker[j];
		j++;
	}
	no->cont = media;
	(*novoNo)->cont = maximo - media;

	if (pos <= minimo)
	{
		adicionaNo(item, pos, no, filho);
	}
	else
	{
		adicionaNo(item, pos - media, *novoNo, filho);
	}
	*pvalor = no->item[no->cont];
	(*novoNo)->linker[0] = no->linker[no->cont];
	no->cont--;
}

int entrarNo(int item, int *pvalor, struct arvoreB *no, struct arvoreB **filho)
{
	int pos;
	if (!no)
	{
		*pvalor = item;
		*filho = NULL;
		return 1;
	}

	if (item < no->item[1])
	{
		pos = 0;
	}
	else
	{
		for (pos = no->cont;
				 (item < no->item[pos] && pos > 1); pos--)
			;
		if (item == no->item[pos])
		{
			printf("Duplicacao nao permitida\n");
			return 0;
		}
	}
	if (entrarNo(item, pvalor, no->linker[pos], filho))
	{
		if (no->cont < maximo)
		{
			adicionaNo(*pvalor, pos, no, *filho);
		}
		else
		{
			splitNode(*pvalor, pvalor, pos, no, *filho, filho);
			return 1;
		}
	}
	return 0;
}

void insercao(int item)
{
	int aux, i;
	struct arvoreB *filho;

	aux = entrarNo(item, &i, root, &filho);
	if (aux)
		root = criacaoNo(i, filho);
}

void copiSucessor(struct arvoreB *noReferencia, int pos)
{
	struct arvoreB *teste;
	teste = noReferencia->linker[pos];

	for (; teste->linker[0] != NULL;)
		teste = teste->linker[0];
	noReferencia->item[pos] = teste->item[1];
}

void removeVal(struct arvoreB *noReferencia, int pos)
{
	int i = pos + 1;
	while (i <= noReferencia->cont)
	{
		noReferencia->item[i - 1] = noReferencia->item[i];
		noReferencia->linker[i - 1] = noReferencia->linker[i];
		i++;
	}
	noReferencia->cont--;
}

void rotaR(struct arvoreB *noReferencia, int pos)
{
	struct arvoreB *x = noReferencia->linker[pos];
	int j = x->cont;

	while (j > 0)
	{
		x->item[j + 1] = x->item[j];
		x->linker[j + 1] = x->linker[j];
	}
	x->item[1] = noReferencia->item[pos];
	x->linker[1] = x->linker[0];
	x->cont++;

	x = noReferencia->linker[pos - 1];
	noReferencia->item[pos] = x->item[x->cont];
	noReferencia->linker[pos] = x->linker[x->cont];
	x->cont--;
	return;
}

void rotaL(struct arvoreB *noReferencia, int pos)
{
	int j = 1;
	struct arvoreB *x = noReferencia->linker[pos - 1];

	x->cont++;
	x->item[x->cont] = noReferencia->item[pos];
	x->linker[x->cont] = noReferencia->linker[pos]->linker[0];

	x = noReferencia->linker[pos];
	noReferencia->item[pos] = x->item[1];
	x->linker[0] = x->linker[1];
	x->cont--;

	while (j <= x->cont)
	{
		x->item[j] = x->item[j + 1];
		x->linker[j] = x->linker[j + 1];
		j++;
	}
	return;
}

void mergeNodes(struct arvoreB *noReferencia, int pos)
{
	int j = 1;
	struct arvoreB *x1 = noReferencia->linker[pos], *x2 = noReferencia->linker[pos - 1];

	x2->cont++;
	x2->item[x2->cont] = noReferencia->item[pos];
	x2->linker[x2->cont] = noReferencia->linker[0];

	while (j <= x1->cont)
	{
		x2->cont++;
		x2->item[x2->cont] = x1->item[j];
		x2->linker[x2->cont] = x1->linker[j];
		j++;
	}

	j = pos;
	while (j < noReferencia->cont)
	{
		noReferencia->item[j] = noReferencia->item[j + 1];
		noReferencia->linker[j] = noReferencia->linker[j + 1];
		j++;
	}
	noReferencia->cont--;
	free(x1);
}

void ajeitaNo(struct arvoreB *noReferencia, int pos)
{
	if (!pos)
	{
		if (noReferencia->linker[1]->cont > minimo)
		{
			rotaL(noReferencia, 1);
		}
		else
		{
			mergeNodes(noReferencia, 1);
		}
	}
	else
	{
		if (noReferencia->cont != pos)
		{
			if (noReferencia->linker[pos - 1]->cont > minimo)
			{
				rotaR(noReferencia, pos);
			}
			else
			{
				if (noReferencia->linker[pos + 1]->cont > minimo)
				{
					rotaL(noReferencia, pos + 1);
				}
				else
				{
					mergeNodes(noReferencia, pos);
				}
			}
		}
		else
		{
			if (noReferencia->linker[pos - 1]->cont > minimo)
				rotaR(noReferencia, pos);
			else
				mergeNodes(noReferencia, pos);
		}
	}
}

int deletaNo(int item, struct arvoreB *noReferencia)
{
	int pos, aux = 0;
	if (noReferencia)
	{
		if (item < noReferencia->item[1])
		{
			pos = 0;
			aux = 0;
		}
		else
		{
			for (pos = noReferencia->cont; (item < noReferencia->item[pos] && pos > 1); pos--)
				;
			if (item == noReferencia->item[pos])
			{
				aux = 1;
			}
			else
			{
				aux = 0;
			}
		}
		if (aux)
		{
			if (noReferencia->linker[pos - 1])
			{
				copiSucessor(noReferencia, pos);
				aux = deletaNo(noReferencia->item[pos], noReferencia->linker[pos]);
				if (aux == 0)
				{
					printf("Dados nao presente na arvore B\n");
				}
			}
			else
			{
				removeVal(noReferencia, pos);
			}
		}
		else
		{
			aux = deletaNo(item, noReferencia->linker[pos]);
		}
		if (noReferencia->linker[pos])
		{
			if (noReferencia->linker[pos]->cont < minimo)
				ajeitaNo(noReferencia, pos);
		}
	}
	return aux;
}

void deleta(int item, struct arvoreB *noReferencia)
{
	struct arvoreB *tmp;
	if (!deletaNo(item, noReferencia))
	{
		printf("Not present\n");
		return;
	}
	else
	{
		if (noReferencia->cont == 0)
		{
			tmp = noReferencia;
			noReferencia = noReferencia->linker[0];
			free(tmp);
		}
	}
	root = noReferencia;
	return;
}

void busca(int item, int *pos, struct arvoreB *noReferencia)
{
	if (!noReferencia)
	{
		return;
	}

	if (item < noReferencia->item[1])
	{
		*pos = 0;
	}
	else
	{
		for (*pos = noReferencia->cont;
				 (item < noReferencia->item[*pos] && *pos > 1); (*pos)--)
			;
		if (item == noReferencia->item[*pos])
		{
			printf("%d presente na arvore B", item);
			return;
		}
	}
	busca(item, pos, noReferencia->linker[*pos]);
	return;
}

void imprimeArvore(struct arvoreB *noReferencia)
{
	int i;
	if (noReferencia)
	{
		for (i = 0; i < noReferencia->cont; i++)
		{
			imprimeArvore(noReferencia->linker[i]);
			printf("%d ", noReferencia->item[i + 1]);
		}
		imprimeArvore(noReferencia->linker[i]);
	}
}

int main()
{
	int item, ch;

	insercao(10);
	insercao(11);
	insercao(12);
	insercao(13);
	insercao(17);
	insercao(18);
	insercao(19);
	insercao(20);
	insercao(22);
	insercao(25);

	imprimeArvore(root);

	deleta(22, root);
	printf("\n");
	imprimeArvore(root);
}