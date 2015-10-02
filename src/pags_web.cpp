#include "./pags_web.h"

void resolver_pags_web(
    conf& args, ifstream& ifile, int cant_nodos,
    int cant_aristas, double resultado[]
) {
    MEDIR_TIEMPO_INICIO(args.timer);
    matrize data(ifile, cant_nodos, cant_aristas);
    MEDIR_TIEMPO_FIN(args.timer);
}

matrize::matrize(ifstream& data, int cant_nodos, int cant_aristas) {
    _cant_nodos = cant_nodos;
    int i = 0;
    int j = 0;
    for (int k = 0; k < cant_aristas; k++) {
        data >> i;
        data >> j;
        cout << "Datos: " << i << ", " << j << endl;
    }
}

vector<double> matrize::potencias(const vector<double>& inicial, double c, double tol) const {
    vector<double> v1 = inicial;
    vector<double> v2;
    while (difManhattan(v1, v2) > tol) {
        v2 = prod(v1, c);
    }
    return v2;
}

vector<double> matrize::prod(const vector<double>& vec, double c) const {
    vector<double> res;

    double prom = 0;
    for (int i = 0; i < _cant_nodos; i++) {
        prom = prom + vec[i];
    }
    prom = prom / _cant_nodos;
    double dumping = prom * c;
    
    for (int i = 0; i < _cant_nodos; i++) {
        int ptr_actual;
        int ptr_next;
        rango_fila(i, ptr_actual, ptr_next);
        double acum = 0;
        if (ptr_actual == -1) {
            acum = prom;
        } else {
            acum = dumping;  // arranco con el valor de dumping
            for (int j = ptr_actual; j < ptr_next; j++) { // vamos sumando el valor correspondiente a cada posición no nula de la fila
                acum = acum + (1 - c) * vals[j] * vec[ind_cols[j]];
            }
        }
        res[i] = acum;
    }

    return res;
}

void matrize::rango_fila(int fila, int& ptr_actual, int& ptr_next) const {
    ptr_actual = ptr_filas[fila];
    if (ptr_actual != -1) {
        ptr_next = -1;
        for (int j = fila + 1; j < _cant_nodos && ptr_next == -1 ; j++) {
            ptr_next = ptr_filas[j];
        }
        if (ptr_next == -1) {
            ptr_next = _cant_nodos;
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