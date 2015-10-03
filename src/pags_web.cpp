#include "./pags_web.h"

void resolver_pags_web(
    conf& args, ifstream& ifile, int cant_nodos,
    int cant_aristas, vector<double>& resultado
) {
    MEDIR_TIEMPO_INICIO(args.timer);
    switch (args.alg) {
        case ALG_PAGERANK: {
            matrize data(ifile, cant_nodos, cant_aristas);
            vector<double> inicial (cant_nodos, (double) 1/cant_nodos);
            resultado = data.potencias(inicial, args.c, args.tol, &args.count_iter);
            break;
        }
        case ALG_ALT: {
            resultado = indeg(ifile, cant_nodos, cant_aristas);
            break;
        }
    }
    MEDIR_TIEMPO_FIN(args.timer);
}

matrize::matrize(ifstream& data, int cant_nodos, int cant_aristas) {
    _cant_nodos = cant_nodos;
    int i = 0;
    int j = 0;
    vector<vector<int> > dataPorColumna(_cant_nodos, vector<int>());

    for (int k = 0; k < cant_aristas; k++) {
        data >> j;
        j--;
        data >> i;
        i--;
        dataPorColumna[j].push_back(i);
        // cout << i << "," << j << endl; DEBUG
    }

    for (int i = 0; i < dataPorColumna.size(); i++) {
        sort(dataPorColumna[i].begin(), dataPorColumna[i].end());
    }

    for (int l = 0; l < _cant_nodos; l++) {
        if (dataPorColumna[l].size() == 0) {
            ptr_cols.push_back(-1);
        } else {
            ptr_cols.push_back(vals.size());
            for (int h = 0; h < dataPorColumna[l].size(); h++) {
                int n = dataPorColumna[l].size();
                
                vals.push_back((double) 1 / n);
                ind_filas.push_back(dataPorColumna[l][h]);
            };
        };
    }

    // for (int i = 0; i < vals.size(); i++) {
    //     cout << vals[i] << ",";
    // }
    // cout << endl << endl;

    // for (int i = 0; i < ind_filas.size(); i++) {
    //     cout << ind_filas[i] << ",";
    // }
    // cout << endl << endl;

    // for (int i = 0; i < ptr_cols.size(); i++) {
    //     cout << ptr_cols[i] << ",";
    // }
    // cout << endl << endl;  //DEBUG    
}

vector<double> matrize::potencias(const vector<double>& inicial, double c, double tol, unsigned int* counter) const {
    vector<double> v1 = inicial;
    vector<double> v2 = prod(v1, c);
    SUMAR_ITERACION(counter);
    while (difManhattan(v1, v2) > tol) {
        v1 = v2;
        v2 = prod(v1, c);
        SUMAR_ITERACION(counter);
    }
    return v2;
}

vector<double> matrize::prod(const vector<double>& vec, double c) const {
    vector<double> res(_cant_nodos, 0);
    double dumping = (1 - c) / _cant_nodos;
    for (int j = 0; j < _cant_nodos; j++) {
        double val_vec = vec[j];
        int ptr_actual;
        int ptr_next;
        rango_columna(j, ptr_actual, ptr_next);
        if (ptr_actual == -1) {
            double a = val_vec / _cant_nodos;
            for (int i = 0; i < _cant_nodos; i++) {
                res[i] = res[i] + a;
            }
            // Le sumo a todos 1/n * vec_j
        } else {
            for (int i = 0; i < _cant_nodos; i++) {
                res[i] = res[i] + val_vec * dumping;
            }
            for (int i = ptr_actual; i < ptr_next; i++) { // vamos sumando el valor correspondiente a cada posición no nula de la fila
                res[ind_filas[i]] = res[ind_filas[i]] + val_vec * c * vals[i];
            }
        }
    }

    return res;
}

void matrize::rango_columna(int col, int& ptr_actual, int& ptr_next) const {
    ptr_actual = ptr_cols[col];
    if (ptr_actual != -1) {
        ptr_next = -1;
        for (int j = col + 1; j < _cant_nodos && ptr_next == -1 ; j++) {
            ptr_next = ptr_cols[j];
        }
        if (ptr_next == -1) {
            ptr_next = vals.size();
        }
    } else {
        ptr_next = -1;
    }
}

double matrize::difManhattan(const vector<double>& v1, const vector<double>& v2) const {
    double res = 0;
    for (int i = 0; i < v1.size(); i++) {
        res = res + abs(v1[i] - v2[i]);
    }
    return res;
}

void matrize::imprimir() const {
    for (int j = 0; j < _cant_nodos; j++) {
        int ptr_actual, ptr_next;
        rango_columna(j, ptr_actual, ptr_next);
        if (ptr_actual == -1) {
            for (int j = 0; j < _cant_nodos; j++) {
                cout << "0.000000 ";
            }
        } else {
            for (int i = 0; i < _cant_nodos; i++) {
                int pos = -1;
                for (int k = ptr_actual; k < ptr_next; k++) {
                    if (ind_filas[k] == i) {
                        pos = k;
                    }
                }
                if (pos != -1) {
                    cout << fixed << setprecision(6) << vals[pos] << " ";
                } else {
                    cout << "0.000000 ";
                }
            }
        }
        cout << endl;
    }
}

vector<double> indeg(ifstream& ifile, int cant_nodos, int cant_aristas) {
    int sumaTotal = 0;
    vector<double> resultado (cant_nodos, 0); 
    int i = 0;
    int j = 0;
    for (int k = 0; k < cant_aristas; k++) {
        ifile >> i;
        i--;
        ifile >> j;
        j--;
        resultado[j]++;
        sumaTotal++;
    }

    for (int i = 0; i < cant_nodos; i++){
        resultado[i] = resultado[i] / sumaTotal;
    }

    /* for (int i = 0; i < resultado.size(); i++) {
        cout << resultado[i] << ",";
    }
    cout << endl << endl; */

    return resultado;
}
