#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.
typedef struct {
    char nome[50];
    char tipo[50];
    int quantidade;
    int prioridade;
} Item;

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).
typedef enum {
    POR_NOME,
    POR_TIPO,
    POR_PRIORIDADE
} CriterioOrdenacao;

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).
#define MAX_ITENS 10
Item mochila[MAX_ITENS];
int numItens = 0;
long long comparacoes = 0;
bool ordenadaPorNome = false;

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela() {
    for (int i = 0; i < 10; i++) {
        printf("\n");
    }
}

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu() {
    printf(" |||-------- Inventario --------||| \n");
    printf(" Status da Mochila: %d/%d itens | Ordenacao por nome: %s\n", 
           numItens, MAX_ITENS, ordenadaPorNome ? "SIM" : "NAO");
    printf("--------------------------------------------\n");
    printf(" 1. Adicionar um item\n");
    printf(" 2. Remover um item\n");
    printf(" 3. Listar todos os itens\n");
    printf(" 4. Ordenar os itens por criterio\n");
    printf(" 5. Realizar busca binaria por nome\n");
    printf(" 0. Sair\n");
    printf("--------------------------------------------\n");
    printf(" Escolha uma opcao: ");
}
void esperarEnter() {
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("\nMOCHILA CHEIA! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novoItem;
    printf("\n--- Adicionar Novo Item ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", novoItem.nome);
    printf("Tipo (ex: Arma, Curativo, Construcao): ");
    scanf(" %[^\n]", novoItem.tipo);
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    printf("Prioridade (1 - baixa, 5 - alta): ");
    scanf("%d", &novoItem.prioridade);

    if (novoItem.prioridade < 1 || novoItem.prioridade > 5) {
        printf("Prioridade invalida. Definida para 3 (media).\n");
        novoItem.prioridade = 3;
    }

    mochila[numItens] = novoItem;
    numItens++;
    ordenadaPorNome = false;
    printf("\nItem '%s' adicionado com sucesso!\n", novoItem.nome);
}

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

void removerItem() {
    char nomeRemover[50];
    printf("\n--- Remover Item ---\n");
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("Digite o nome do item a ser removido: ");
    scanf(" %[^\n]", nomeRemover);

    int indice = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        for (int i = indice; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        ordenadaPorNome = false;
        printf("Item '%s' removido com sucesso.\n", nomeRemover);
    } else {
        printf("Item '%s' nao encontrado na mochila.\n", nomeRemover);
    }
}

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens() {
    printf("\n--- Itens na Mochila ---\n");
    if (numItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("-------------------------------------------------------------------------\n");
    printf("| %-2s | %-20s | %-15s | %-3s | %-10s |\n", 
           "ID", "Nome", "Tipo", "Qtd", "Prioridade");
    printf("-------------------------------------------------------------------------\n");

    for (int i = 0; i < numItens; i++) {
        printf("| %-2d | %-20s | %-15s | %-3d | %-10d |\n", 
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("-------------------------------------------------------------------------\n");
}


int compararItens(Item itemA, Item itemB, CriterioOrdenacao criterio) {
    comparacoes++;
    if (criterio == POR_NOME) {
        return strcasecmp(itemA.nome, itemB.nome);
    } else if (criterio == POR_TIPO) {
        return strcasecmp(itemA.tipo, itemB.tipo);
    } else if (criterio == POR_PRIORIDADE) {
        if (itemA.prioridade > itemB.prioridade) return -1;
        if (itemA.prioridade < itemB.prioridade) return 1;
        return 0;
    }
    return 0;
}


// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("\nERRO: Para a busca binaria, a mochila DEVE estar ordenada por nome.\n");
        printf("Por favor, use a opcao 4 (Ordenar) e escolha o criterio 'Por Nome'.\n");
        return;
    }
    
    if (numItens == 0) {
        printf("\nA mochila esta vazia. Nao ha o que buscar.\n");
        return;
    }

    char nomeBuscado[50];
    printf("\n--- Busca Binaria por Nome ---\n");
    printf("Digite o nome do item que voce esta buscando: ");
    scanf(" %[^\n]", nomeBuscado);
    
    comparacoes = 0;
    
    int inicio = 0;
    int fim = numItens - 1;
    int encontrado = -1;

    while (inicio <= fim) {
        comparacoes++;
        int meio = inicio + (fim - inicio) / 2;

        int resultadoComparacao = strcmp(nomeBuscado, mochila[meio].nome);
        comparacoes++;
        
        if (resultadoComparacao == 0) {
            encontrado = meio;
            break;
        } else if (resultadoComparacao < 0) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }

    printf("\n--- Resultado da Busca ---\n");
    if (encontrado != -1) {
        printf("Item encontrado (ID %d)!\n", encontrado + 1);
        printf("Nome: %s | Tipo: %s | Qtd: %d | Prioridade: %d\n", 
               mochila[encontrado].nome, mochila[encontrado].tipo, mochila[encontrado].quantidade, mochila[encontrado].prioridade);
    } else {
        printf("Item '%s' nao foi encontrado na mochila.\n", nomeBuscado);
    }
    printf("Analise de Desempenho (Total de Comparacoes): %lld\n", comparacoes);
}

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)
void insertionSort(CriterioOrdenacao criterio) {
    if (numItens <= 1) return;

    for (int i = 1; i < numItens; i++) {
        Item itemAtual = mochila[i];
        int j = i - 1;

        while (j >= 0 && compararItens(itemAtual, mochila[j], criterio) < 0) {
            mochila[j + 1] = mochila[j];
            j = j - 1;
        }
        mochila[j + 1] = itemAtual;
    }

    ordenadaPorNome = (criterio == POR_NOME);
}

void menuDeOrdenacao() {
    int opcao;
    comparacoes = 0;

    printf("\n--- Menu de Ordenacao ---\n");
    printf("Escolha o criterio para ordenar:\n");
    printf(" 1. Por Nome (Ordem Alfabetica)\n");
    printf(" 2. Por Tipo (Ordem Alfabetica)\n");
    printf(" 3. Por Prioridade (Mais Alta para a Mais Baixa)\n");
    printf(" Opcao: ");
    if (scanf("%d", &opcao) != 1) {
        // Limpa buffer em caso de erro de leitura
        while (getchar() != '\n'); 
        printf("\nOpcao invalida. Retornando ao menu principal.\n");
        return;
    }

    CriterioOrdenacao criterio;
    char criterioNome[20];

    switch (opcao) {
        case 1:
            criterio = POR_NOME;
            strcpy(criterioNome, "Nome");
            break;
        case 2:
            criterio = POR_TIPO;
            strcpy(criterioNome, "Tipo");
            break;
        case 3:
            criterio = POR_PRIORIDADE;
            strcpy(criterioNome, "Prioridade");
            break;
        default:
            printf("\nOpcao invalida. Retornando ao menu principal.\n");
            return;
    }

    insertionSort(criterio);

    printf("\n--- Ordenacao Concluida ---\n");
    printf("Itens ordenados com sucesso por '%s' usando Insertion Sort.\n", criterioNome);
    printf("Analise de Desempenho (Total de Comparacoes): %lld\n", comparacoes);
    listarItens();
}



int main() {
    int opcao;

    do {
        limparTela();
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
            int c;
            while ((c = getchar()) != '\n' && c != EOF); 
        }

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                menuDeOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("\n--- Fuga concluida! Ate a proxima sobrevivencia. ---\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }
        
        if (opcao != 0) {
            esperarEnter();
        }

    } while (opcao != 0);

    return 0;
}