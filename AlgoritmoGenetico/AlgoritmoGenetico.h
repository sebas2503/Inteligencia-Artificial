#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>
#include <random>
using namespace std;

#define city 3;

int get_random(int low, int high) { // Funcion que genera numeros aleatorios para hacer las pruebas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(low, high);
    return distribution(gen);
}

float probabilidad() { // Funcion que genera numeros aleatorios para hacer las pruebas
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    return dis(gen);
}

struct Ciudad
{
    int id;
    float x;
    float y;

    bool operator==(const Ciudad& c) const {
        return x == c.x && y == c.y;
    }
};

struct Individuo
{
    vector<Ciudad> individuo;
    float fitness;
    Individuo() { fitness = 0; };
    Individuo(vector<Ciudad> ind, float fit)
    {
        individuo.resize(ind.size());
        copy(ind.begin(), ind.end(), individuo.begin()); // copio los valores de un individuo a este para instanciarlo
        fitness = fit;
    }
};

struct TSP
{
    vector<Ciudad> ciudades;
    int cantidad;
    int individuos;// = 10; // prueba
    vector<vector<float>> adyacencia; // matriz de adyacencia
    vector<Individuo> poblacion;
    float fitness_total;


    TSP(int num, int ind)
    {
        individuos = ind; // prueba, cant de individuos
        ciudades.resize(num);
        cantidad = num; // CANTIDAD DE CIUDADES
        adyacencia.resize(cantidad, vector<float>(cantidad)); // matriz de adyacencia
        poblacion.resize(individuos); // caminos aleatorios, cantidad + 1  para llegar al origen
        fitness_total = 0; // suma de los fitness de la poblacion actual
    }
    ///

    vector<Individuo> CX(Individuo padre, Individuo madre) {
        //int particion = padre.individuo.size() / 2;
        // particion aleatoria
        int particion = get_random(1, padre.individuo.size() - 1);

        // creamos dos hijos, el mejor se elige

        vector<Ciudad> p1(padre.individuo.begin(), padre.individuo.begin() + particion);
        vector<Ciudad> p2(padre.individuo.begin() + particion, padre.individuo.end());

        vector<Ciudad> m1(madre.individuo.begin(), madre.individuo.begin() + particion);
        vector<Ciudad> m2(madre.individuo.begin() + particion, madre.individuo.end());

        vector<Ciudad> hijo; // hijo tiene mismo tamanio que el padre
        vector<Ciudad> hijo2; // hijo 2 para hacer competencia

        // HIJO 1
        for (const auto& a : p1)
        {
            hijo.push_back(a);

        }

        for (const auto& a : m2)
        {
            if (find(hijo.begin(), hijo.end(), a) == hijo.end()) { // Si la ciudad a no se encuentra en el hijo, se agrega
                hijo.push_back(a);
            }
        }

        for (const auto& a : m1)
        {
            if (find(hijo.begin(), hijo.end(), a) == hijo.end()) { // Si la ciudad a no se encuentra en el hijo, se agrega
                hijo.push_back(a);
            }
        }

        // HIJO 2
        for (const auto& a : m1)
        {
            hijo2.push_back(a);

        }

        for (const auto& a : p2)
        {
            if (find(hijo2.begin(), hijo2.end(), a) == hijo2.end()) { // Si la ciudad a no se encuentra en el hijo, se agrega
                hijo2.push_back(a);
            }
        }

        for (const auto& a : p1)
        {
            if (find(hijo2.begin(), hijo2.end(), a) == hijo2.end()) { // Si la ciudad a no se encuentra en el hijo, se agrega
                hijo2.push_back(a);
            }
        }

        // MUTACION
        float prob_mutacion = probabilidad();
        if (prob_mutacion < 0.1) // habra mutacion
        {
            int mutacion_hijo = get_random(0, 1);
            if (mutacion_hijo == 0) // muta el primer hijo
            {
                mutacion(hijo);
            }
            else
            {
                mutacion(hijo2);
            }
        }


        // al final se agrega nuevamente la ciudad inicial
        hijo.push_back(ciudades[0]);
        hijo2.push_back(ciudades[0]);

        vector<Individuo> hijos(2);
        hijos[0] = Individuo(hijo, fitness(hijo));
        hijos[1] = Individuo(hijo2, fitness(hijo2));

        return hijos;//Individuo(hijo, fitness(hijo));
        // crear un nuevo individuo con el hijo y su fitness
        //return Individuo(hijo, fitness(hijo));
    }




    ///
    void CrearPoblacion() // crea a las ciudades
    {
        for (int i = 0; i < cantidad; i++) {
            ciudades[i].id = i;
            ciudades[i].x = (float)get_random(-10000, 10000) / 10000.0;  // valor escalado para opengl
            ciudades[i].y = (float)get_random(-10000, 10000) / 10000.0;
        }
    }

    void printGrafo()
    {
        /*cout << "CIUDADES: " << endl;
        vector<Ciudad>::iterator it;
        for (it = ciudades.begin(); it != ciudades.end(); ++it)
        {
            cout << "Ciudad: " << (*it).id << ": (" << (*it).x << "," << (*it).y << ")" << endl;
        }*/
    }

    float distanciaEuclidiana(Ciudad a, Ciudad b)
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));

    }

    void LinkCiudades()
    {
        const float umbral = 0.5; // umbral máximo de distancia para conectar ciudades

        // llenar matriz de adyacencia
        for (int i = 0; i < cantidad; i++) {
            for (int j = 0; j < cantidad; j++) {
                if (i != j) {
                    adyacencia[i][j] = distanciaEuclidiana(ciudades[i], ciudades[j]);

                }
            }
        }


    }

    void printdistancias()
    {
        cout << "Distancias: " << endl;
        for (int i = 0; i < cantidad; i++) {
            for (int j = 0; j < cantidad; j++) {
                cout << adyacencia[i][j] << " ";
            }cout << endl;
        }
    }

    void generarPoblacion()
    {

        vector<Ciudad> aux(cantidad); //va a copiar los elementos de ciudades para hacer permutaciones
        copy(ciudades.begin(), ciudades.end(), aux.begin());
        aux.push_back(ciudades[0]); //agrego el origen al final
        //cout << "AUX:\n";
        vector<Ciudad>::iterator it;
        /*for (it = aux.begin(); it != aux.end(); ++it)
        {
            cout << "Ciudad: " << (*it).id << ": (" << (*it).x << "," << (*it).y << ")" << endl;
        }*/


        for (int i = 0; i < individuos; i++) {
            random_shuffle(aux.begin() + 1, aux.end() - 1); // +1 y -1 para mantener el origen y destino
            // se crea un individuo con aux
            //Individuo indiv(aux, fitness(aux));// creo el individuo y su fitness al mismo tiempo 

            //cout << aux.size();
            poblacion[i] = Individuo(aux, fitness(aux)); // agrego el nuevo individuo a la poblacion
        }
    }

    void printPoblacion()
    {

        /*cout << "Poblacion: " << endl;
        for (int i = 0; i < poblacion.size(); i++) {

            for (int j = 0; j < poblacion[0].individuo.size(); j++) {
                cout << poblacion[i].individuo[j].id << " "; //poblacion[i][j].id << " "; //fit = fitness(poblacion[i]);
            } cout << "fitness: " << poblacion[i].fitness << endl;//fit = fitness(poblacion[i]); cout << "fitness: " << fit;  cout << endl;
        }*/
    }

    float fitness(vector<Ciudad> individuo) // calculo el fitness de cada individuo
    {
        float fit = 0;
        for (int i = 0, j = i + 1; i < cantidad; i++, j++) // sumo las distancias de 2 en 2
        {
            //cout << individuo[i].id;
            fit += adyacencia[individuo[i].id][individuo[j].id];
            // sumo las distancias y le saco la inversa
            //cout << " * " << adyacencia[individuo[i].id][individuo[j].id] << " * " << endl;
        }
        //fit = 1 / fit; // inversa, para que la menor distancia tenga la mayor probabilidad y viceversa
        //return fit;
        return fit;
    }

    float suma_fit(vector<Individuo> poblacion)
    {
        for (int i = 0; i < poblacion.size(); i++)
        {
            fitness_total += poblacion[i].fitness;
        }
        return fitness_total;
    }

    float suma_fit_inverso(vector<Individuo> poblacion)
    {
        for (int i = 0; i < poblacion.size(); i++)
        {
            fitness_total += 1 / (poblacion[i].fitness);
        }
        return fitness_total;
    }

    void llenar_ruleta(vector<Individuo> poblacion, vector<float>& ruleta, float fit_total)
    {
        for (int i = 0; i < poblacion.size(); i++)
        {
            ruleta[i] = (1 / poblacion[i].fitness) / fit_total;
        }
    }

    int girar_ruleta(vector<float> ruleta, float probabilidad) //retorna el indice del individuo seleccionado
    {
        float suma_prob = 0;
        int indice = -1;
        for (int i = 0; i < ruleta.size(); i++)
        {
            suma_prob += ruleta[i];
            if (probabilidad <= suma_prob) // pare
            {
                indice = i;
                break;
            }
        }
        return indice;
    }

    void swap(Ciudad& a, Ciudad& b)
    {
        Ciudad aux = a;
        a = b;
        b = aux;
    }

    void cambio() // mutación generada si todas las soluciones se mantienen iguales
    {
        //cout << "TODO ES IGUAL";
        int indice = get_random(0, poblacion.size() - 2); // -2 porque va a tener el 0 al final
        vector<Ciudad> mut = poblacion[indice].individuo;
        mutacion2(mut);
    }

    void verificar() // verifico si se quedó estancado
    {
        float fit = poblacion[0].fitness;
        bool flag = true;
        for (int i = 0; i < poblacion.size(); i++)
        {
            if (poblacion[i].fitness != fit)// si es distinto, no se estancó
            {
                flag = false;
            }
        }
        if (flag)
        {
            for (int j = 0; j < poblacion.size() * 100; j++) // varias mutaciones
            {
                cambio(); // si todos son iguales, muta obliatoriamente
            }
        }
    }

    void mejor_camino()
    {
        cout << "CAMINO: ";
        for (auto& a : poblacion[0].individuo)
        {
            cout << a.id << " ";
        }cout << " FITNESS: " << poblacion[0].fitness << endl;
    }

    void mutacion2(vector<Ciudad>& hijo) // muta al individuo, en cambio()
    {
        /*
        cout << "MUTACION" << endl;
        for (auto& a : hijo)
        {
            cout << a.id << " ";
        }cout << endl;*/
        int indice_mutacion1 = get_random(1, hijo.size() - 2); // obtengo el indice para el swap
        int indice_mutacion2 = get_random(1, hijo.size() - 2); // no puede hacer swap el 0

        if (indice_mutacion1 == indice_mutacion2)// si son iguales, obtenerlos otra vez
        {
            while (indice_mutacion1 == indice_mutacion2)
            {
                indice_mutacion2 = get_random(1, hijo.size() - 2);
            }
        }

        // swap
        swap(hijo[indice_mutacion1], hijo[indice_mutacion2]); // se hizo la mutación

        /*
        cout << "MUTADO: " << endl;
        for (auto& a : hijo)
        {
            cout << a.id << " ";
        }cout << endl;*/
    }

    void mutacion(vector<Ciudad>& hijo) // muta al individuo
    {
        //cout << "MUTACION" << endl;
        /*for (auto& a : hijo)
        {
            cout << a.id << " ";
        }cout << endl;*/
        int indice_mutacion1 = get_random(1, hijo.size() - 1); // obtengo el indice para el swap
        int indice_mutacion2 = get_random(1, hijo.size() - 1); // no puede hacer swap el 0

        if (indice_mutacion1 == indice_mutacion2)// si son iguales, obtenerlos otra vez
        {
            while (indice_mutacion1 == indice_mutacion2)
            {
                indice_mutacion2 = get_random(1, hijo.size() - 1);
            }
        }

        // swap
        swap(hijo[indice_mutacion1], hijo[indice_mutacion2]); // se hizo la mutación
        /*
        cout << "MUTADO: " << endl;
        for (auto& a : hijo)
        {
            cout << a.id << " ";
        }cout << endl;*/
    }

    void seleccion()
    {


        // WHILE PRINCIPAL 100 GENERACIONES
        int generacion = 0;
        //while (generacion < 100)
        //{
            // ordenamiento
        sort(poblacion.begin(), poblacion.end(), [](const Individuo& a, const Individuo& b) {
            return a.fitness < b.fitness; // ordena en orden descendente de fitness
            });

        // elige los dos mejores individuos
        printPoblacion();
        float fit_total = suma_fit(poblacion);
        fitness_total = 0;
        float fit_total_inverso = suma_fit_inverso(poblacion);
        fitness_total = 0; // una vez se usa fitness_total, se resetea su valor a 0 para la siguiente iteracion
        //cout << " SUMA: " << fit_total << endl;
        //cout << "SUMA INVERSA: " << fit_total_inverso << endl;

        vector<float> ruleta(individuos); // va a tener los resultados de la ruleta
        llenar_ruleta(poblacion, ruleta, fit_total_inverso);
        /*for (int i = 0; i < ruleta.size(); i++)
        {
            cout << "RULETA: " << ruleta[i] << endl;
        }*/

        //float prob = probabilidad();

        //cout << "prob: " << prob << endl;
        int indice1 = girar_ruleta(ruleta, probabilidad()); // retorna el indice elegido para seleccionar a los padres
        //cout << "INDICE: " << indice1 << endl;
        // agrego los datos a la ruleta


        int indice2 = girar_ruleta(ruleta, probabilidad()); // retorna el indice elegido para seleccionar a los padres
        //cout << "INDICE: " << indice2 << endl;


        // verifico que los indices sean diferentes
        if (indice1 == indice2)
        {
            while (indice1 == indice2)
            {
                indice2 = girar_ruleta(ruleta, probabilidad());
            }
        }



        // SELECCION
        Individuo padre(poblacion[indice1].individuo, poblacion[indice1].fitness); // obtengo al individuo padre y madre
        Individuo madre(poblacion[indice2].individuo, poblacion[indice2].fitness);
        //Individuo hijo = CX(padre, madre);

        // CRUZAMIENTO
        vector<Individuo> hijos = CX(padre, madre); // retorna dos hijos 
        /*cout << "\nPADRE: ";
        for (const auto& a : padre.individuo)
        {
            cout << a.id << " ";
        }cout << "fit: " << padre.fitness << endl;
        cout << "MADRE: ";
        for (const auto& a : madre.individuo)
        {
            cout << a.id << " ";
        }cout << "fit: " << madre.fitness << endl;

        cout << "HIJO 1: ";
        for (const auto& a : hijos[0].individuo)
        {
            cout << a.id << " ";
        }cout << "fit: " << hijos[0].fitness << endl;

        cout << "HIJO 2: ";
        for (const auto& a : hijos[1].individuo)
        {
            cout << a.id << " ";
        }cout << "fit: " << hijos[1].fitness << endl;*/

        // NUEVA POBLACION
        // 1. agrego los hijos a la poblacion
        poblacion.push_back(hijos[0]);
        poblacion.push_back(hijos[1]);
        // 2. ordeno toda la poblacion
        sort(poblacion.begin(), poblacion.end(), [](const Individuo& a, const Individuo& b) {
            return a.fitness < b.fitness; // ordena en orden descendente de fitness
            });

        // 3. Elimino los 2 ultimos
        poblacion.erase(poblacion.end() - 2, poblacion.end());


        verificar(); // mejora obligada

        printPoblacion();
        //  generacion++;
      //}

    }

    void vertices(float* v)
    {
        for (int i = 0; i < cantidad; i++)
        {
            v[i * 3] = ciudades[i].x;
            v[i * 3 + 1] = ciudades[i].y; // y
            v[i * 3 + 2] = 0.0f; // z (en este caso, 2D)
        }
    }

    void mejor(float* v)
    {
        for (int i = 0; i < cantidad; i++)
        {
            v[i * 3] = poblacion[0].individuo[i].x;
            v[i * 3 + 1] = poblacion[0].individuo[i].y; // y
            v[i * 3 + 2] = 0.0f; // z (en este caso, 2D)
        }
    }

};
/*
int main()
{
    int ciudades = 0;
    //cout << "Ingresa el numero de ciudades: "; cin >> ciudades;
    ciudades = 6;
    int individuos = 5;
    TSP tsp(ciudades, individuos);
    tsp.CrearPoblacion();
    tsp.printGrafo();
    tsp.LinkCiudades();
    tsp.printdistancias();
    tsp.generarPoblacion();
    tsp.printPoblacion();

    tsp.seleccion();
    tsp.printPoblacion();
    return 0;
}*/
