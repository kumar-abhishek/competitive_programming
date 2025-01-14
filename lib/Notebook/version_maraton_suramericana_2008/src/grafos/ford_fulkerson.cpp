/*
  cap[i][j] = Capacidad de la arista (i, j).
  prev[i] = Predecesor del nodo i en un camino de aumentación.
 */
int cap[MAXN+1][MAXN+1], prev[MAXN+1];

vector<int> g[MAXN+1]; //Vecinos de cada nodo.
inline void link(int u, int v, int c){ cap[u][v] = c; g[u].push_back(v), g[v].push_back(u); }
/*
  Notar que link crea las aristas (u, v) && (v, u) en el grafo g. Esto es necesario
  porque el algoritmo de Edmonds-Karp necesita mirar el "back-edge" (j, i) que se crea
  al bombear flujo a través de (i, j). Sin embargo, no modifica cap[v][u], porque se
  asume que el grafo es dirigido. Si es no-dirigido, hacer cap[u][v] = cap[v][u] = c.
 */


/*
  Método 1:

  Mantener la red residual, donde residual[i][j] = cuánto flujo extra puedo inyectar
  a través de la arista (i, j).

  Si empujo k unidades de i a j, entonces residual[i][j] -= k y residual[j][i] += k
  (Puedo "desempujar" las k unidades de j a i).

  Se puede modificar para que no utilice extra memoria en la tabla residual, sino
  que modifique directamente la tabla cap.
*/

int residual[MAXN+1][MAXN+1];
int fordFulkerson(int n, int s, int t){
  memcpy(residual, cap, sizeof cap);

  int ans = 0;
  while (true){
    fill(prev, prev+n, -1);
    queue<int> q;
    q.push(s);
    while (q.size() && prev[t] == -1){
      int u = q.front();
      q.pop();
      vector<int> &out = g[u];
      for (int k = 0, m = out.size(); k<m; ++k){
        int v = out[k];
        if (v != s && prev[v] == -1 && residual[u][v] > 0)
          prev[v] = u, q.push(v);
      }
    }

    if (prev[t] == -1) break;

    int bottleneck = INT_MAX;
    for (int v = t, u = prev[v]; u != -1; v = u, u = prev[v]){
      bottleneck = min(bottleneck, residual[u][v]);
    }
    for (int v = t, u = prev[v]; u != -1; v = u, u = prev[v]){
      residual[u][v] -= bottleneck;
      residual[v][u] += bottleneck;
    }
    ans += bottleneck;
  }
  return ans;
}



/*
  Método 2:

  Mantener la red de flujos, donde flow[i][j] = Flujo que, err, fluye
  de i a j. Notar que flow[i][j] puede ser negativo. Si esto pasa,
  es lo equivalente a decir que i "absorve" flujo de j, o lo que es
  lo mismo, que hay flujo positivo de j a i.

  En cualquier momento se cumple la propiedad de skew symmetry, es decir,
  flow[i][j] = -flow[j][i]. El flujo neto de i a j es entonces flow[i][j].

*/

int flow[MAXN+1][MAXN+1];
int fordFulkerson(int n, int s, int t){
  //memset(flow, 0, sizeof flow);
  for (int i=0; i<n; ++i) fill(flow[i], flow[i]+n, 0);
  int ans = 0;
  while (true){
    fill(prev, prev+n, -1);
    queue<int> q;
    q.push(s);
    while (q.size() && prev[t] == -1){
      int u = q.front();
      q.pop();
      vector<int> &out = g[u];
      for (int k = 0, m = out.size(); k<m; ++k){
        int v = out[k];
        if (v != s && prev[v] == -1 && cap[u][v] > flow[u][v])
          prev[v] = u, q.push(v);
      }
    }

    if (prev[t] == -1) break;

    int bottleneck = INT_MAX;
    for (int v = t, u = prev[v]; u != -1; v = u, u = prev[v]){
      bottleneck = min(bottleneck, cap[u][v] - flow[u][v]);
    }
    for (int v = t, u = prev[v]; u != -1; v = u, u = prev[v]){
      flow[u][v] += bottleneck;
      flow[v][u] = -flow[u][v];
    }
    ans += bottleneck;
  }
  return ans;
}

