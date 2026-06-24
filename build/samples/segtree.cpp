#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> tree;

// 1. Construção Iterativa - O(N)
void build(int arr_size, const vector<int>& arr) {
    n = arr_size;
    tree.assign(2 * n, 0);

    // Insere os elementos originais nas folhas (segunda metade do vetor tree)
    for (int i = 0; i < n; i++) {
        tree[n + i] = arr[i];
    }

    // Constrói os nós internos subindo na árvore (de trás para frente)
    for (int i = n - 1; i > 0; --i) {
        tree[i] = tree[2 * i] + tree[2 * i + 1]; // Pai = Filho Esquerdo + Filho Direito
    }
}

// 2. Atualização Iterativa de um Ponto - O(log N)
void update(int p, int value) {
    // Modifica o valor na folha correspondente
    tree[p + n] = value;
    p += n;

    // Sobe na árvore atualizando os pais
    for (int i = p; i > 1; i >>= 1) {
        tree[i >> 1] = tree[i] + tree[i ^ 1]; 
        // i >> 1 é o pai (i/2)
        // i ^ 1 é o irmão do nó atual (se i for par, i^1 é ímpar, e vice-versa)
    }
}

// 3. Consulta Iterativa de Intervalo [l, r) - O(log N)
// Nota: O intervalo aqui é semi-aberto, ou seja, inclui 'l' mas não inclui 'r'.
int query(int l, int r) {
    int res = 0;
    // Move os ponteiros para as folhas
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
        // Se l é ímpar, ele é o filho direito do pai dele. 
        // Isso significa que o filho esquerdo está fora do nosso intervalo, então pegamos só l e avançamos.
        if (l & 1) res += tree[l++];
        
        // Se r é ímpar, significa que o limite direito caiu logo após um filho esquerdo.
        // Pegamos o valor anterior (--r) que está dentro do intervalo.
        if (r & 1) res += tree[--r];
    }
    return res;
}

int main() {
    vector<int> arr = {1, 3, 5, 7};
    build(arr.size(), arr);

    cout << "Soma de [0, 2]: " << query(0, 3) << endl;

    update(1, 10);

    cout << "Nova soma de [0, 2]: " << query(0, 3) << endl;

    return 0;
}
