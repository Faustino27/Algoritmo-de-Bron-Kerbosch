#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> ler(int *vertices, int *arestas){ 
    // funcao para ler o input do arquivo .txt
    vector<vector<int>> grafo(1000, vector<int>());
    ifstream entrada;
    entrada.open("soc-dolphins.txt", ios::in);
    int tamanho;

    if(entrada.is_open()){
        entrada >> tamanho;
        entrada >> *vertices;
        //salva o numero de vertices
        entrada >> *arestas;
        //salva o numero de arestas
        while(!entrada.eof()){
            int a, b;
            entrada >> a;
            entrada >> b;
            grafo[a].push_back(b);
            grafo[b].push_back(a);

        }
        entrada.close();
    }

    else cout <<"Nao foi possivel abrir o arquivo de entrada.\n";

    return grafo;
}


void printarClique(set<int> R){
    cout << "Clique maximal encontrado com " << R.size() << " vertices, sendo eles :" << endl;
    cout << "{ ";
    for(int i : R)
        cout << i << " ";
    cout << "}" << endl;
}


int maior_Grau(set<int> P, vector<vector<int>> grafo){
    // funcao para descobrir o maior grau dado os vertices de um conjunto
    int maior = 0, pivo = 1;
    for(int i: P){
        if(grafo[i].size() > maior){
            maior = grafo[i].size();
            pivo = i;
        }
    }
    return pivo;
}


int BronKerbosch_SemPivo(set<int> R, set<int> P, set<int> X, vector<vector<int>> grafo){
    //executa o algoritmo de BronKerbosch sem o pivoteamento
    
    int x = 0;
    if(P.empty() and X.empty()){
        printarClique(R);
        return 1;
    }

    else if(P.empty() and not X.empty()) return 0;
    // se P esta vazio e X não quer dizer que não é um clique maximal então a função retorna 0;

    for(int v : P){
        set<int> vizinhos, Pv, Xv;
        for(int i : grafo[v]){//cria o set contendo os vizinhos de v
            vizinhos.insert(i);
        }
        set_intersection(P.begin(), P.end(), vizinhos.begin(), vizinhos.end(), inserter(Pv, Pv.end()));
        // faz a interseção de P e vizinhos e colocar esse novo conjunto em Pv
        set_intersection(X.begin(), X.end(), vizinhos.begin(), vizinhos.end(), inserter(Xv, Xv.end()));
        // faz a interseção de X e vizinhos e coloca esse novo conjunto em Xv
        R.insert(v);
        //inserindo v no conjunto antes de continuar a recursividade
        x += BronKerbosch_SemPivo(R, Pv, Xv, grafo);
        // realizar a busca recursiva
        R.erase(v);
        // depois de inserido o V não é mais necessário manter ele no conjunto, pois ele ja foi enviado na recursão para a proxima iteração.
        X.insert(v);
        //insere v em X
        P.erase(v);
        // remove v de P
        
    }

    return x;// retorna o total de cliques encontrados
}


int BronKerbosch_ComPivo(set<int> R, set<int> P, set<int> X, vector<vector<int>> grafo){
    //executa o algoritmo de BronKerbosch com o pivoteamento
    //funciona quase identicamente ao algoritimo sem pivo, mas agora mandamos um vertice u para ser o pivo
    int x = 0;
    if(P.empty() and X.empty()){
        printarClique(R);
        return 1;
    }

    else if(P.empty() and not X.empty()) return 0;
    // se P esta vazio e X não quer dizer que não é um clique maximal então a função retorna 0;

    set<int> Px, Pn;
    for(int i: X)
        Px.insert(i);
    for(int i: P){
        Px.insert(i);
        Pn.insert(i);
    }
    //realiza a união entre P e X que resulta em Px e realiza uma copia de P chamada Pn que será usada para remover os vertices
    //vizinhos de u em que u é o pivo escolhido(vertice de maior grau em Px)

    int u = maior_Grau(Px, grafo);

    for(int i: grafo[u])
        Pn.erase(i); //remove os vizinhos de u do grafo p
    
    for(int v : Pn){
        set<int> vizinhos, Pv, Xv;
        for(int i : grafo[v]){
            vizinhos.insert(i);
        }
        //cria o set contendo os vizinhos de v
        set_intersection(P.begin(), P.end(), vizinhos.begin(), vizinhos.end(), inserter(Pv, Pv.end()));
        // faz a interseção de P e vizinhos e colocar esse novo conjunto em Pv
        set_intersection(X.begin(), X.end(), vizinhos.begin(), vizinhos.end(), inserter(Xv, Xv.end()));
        // faz a interseção de X e vizinhos e coloca esse novo conjunto em Xv
        R.insert(v);
        x += BronKerbosch_ComPivo(R, Pv, Xv, grafo);
        R.erase(v);
        // depois de inserido o V não é mais necessário manter ele no conjunto, pois ele ja foi enviado na recursão para a proxima iteração.
        X.insert(v);
        //insere v em X
        P.erase(v);
        // remove v de P
        
    }
    return x;// retorna o total de cliques encontrados
}


int calculaTriangulos(vector<bool> visitados, set<int> vertice, vector<vector<int>> grafo, int no){
    int contador = 0;
    for(int v: vertice){
        for(int k: grafo[v]){
            if(vertice.find(k) != vertice.end() and not visitados[k])
                contador++;
        }
    }
    /* 
    #para cada vizinho de I(presente no set "vertice") conferimos os viznhos desses vizinho, se por acaso eles tiverem um vizinho
    #em comum com o I(ou seja se ele pertencer ao set) concluimos q temos um triangulo no grafo, tabém conferimos se ja nao calculamos
    #o triangulo em questao anteriormente utilizando o vetor visitados.   
    */
   cout << no << "contador :" << contador << " ";
    return contador;
}


double coeficienteAglomeracao(vector<bool> visitados, vector<vector<int>> grafo, set<int> P){
    double somatorio = 0;
    //guarda a soma dos coeficientes de aglomeracao;
    for(int i: P){
        set<int> aux;
        for(int v: grafo[i])//salva os vizinhos do vetice I num set auxiliar para ser usado na funcao de calcular triangulos
            aux.insert(v);
        visitados[i] = true;
        //colocamos no vetor que ja passamos por aquele vertices para n conta-lo 2 vezes
        //quando formos fazer uma contagem iniciando de outro vertice viznho
        double quantidade_real = calculaTriangulos(visitados, aux, grafo, i); //pega a quantidade real de triangulos do vertice I
        double n = grafo[i].size(), quantidade_max =(n*(n-1))/2; // pega a quantidade maxima possivel de triangulos no vertice I
        double Ci =(quantidade_max == 0)? 0 : quantidade_real/quantidade_max; // calcula o coeficiente de aglomeracao
        somatorio += Ci;
    }

    return somatorio;
}


int main(){
    int n_vertices, n_arestas;// armazenam o numero de vertices e arestas inseridos
    vector<vector<int>> grafo = ler(&n_vertices, &n_arestas);// inicializar o grafo;
    set<int> R, P, X;
    /*  
        # conjuntos que vão armazenar os vertices
        # R é vazio inicialmente, P contem todos os vertices que tem ligaçao com todos os vertices de R(candidatos) inicializado com todos os lementos do grafo;
        # X é o conjunto dos vertices ja analisados que nao levam a uma extensao do conjunto R(usado para evitar comparacao excessiva
        # e inicia vazio)
    */

    for(int i = 1;i <= n_vertices;i++){ // insere os vertices em P
        P.insert(i);
    }
    cout << "Realizando a busca com Bron Kerbosch sem pivo: " << endl;

    int total_sem = BronKerbosch_SemPivo(R, P, X, grafo); // mostras os cliques e salva a quantidade total numa variavel

    cout << "total de cliques encontrados pelo algoritmo de Bron Kerbosch sem pivo: "<< total_sem << endl;
    cout << "Realizando a busca com Bron Kerbosch com pivo: "<< endl;

    int total_com = BronKerbosch_ComPivo(R,P,X,grafo); //mostra os clicques e salva a quantidade total numa variavel

    cout << "total de cliques encontrados pelo algoritmo de Bron Kerbosch com pivo: "<< total_com << endl;
    vector<bool> visitados(n_vertices, false);
    //criamos um vetor de visitados para saber se ja passamos calculamos um triangulo e nao conta-lo 2 vezes
    double x = coeficienteAglomeracao(visitados, grafo, P);
    cout << "O coeficiente de aglomeracao medio do grafo: " << x/n_vertices;
    return 0;
}