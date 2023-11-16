#include <iostream>
#include <vector>
#include <fstream>
#include <codecvt>
#include <locale>
using namespace std;
struct pais
{
    pais *next;
    string nombre;
    vector<int> ganados;
    vector<int> empatados;
    vector<int> perdidos;
    vector<int> totalPartidos;
    vector<int> goleFavor;
    vector<int> golesContra;
    vector<int> golesDiferencia;
    vector<int> totalPuntos;
    // LAS VARIABLES BOOL SE USAN PARA SABER SI CLASIFICÓ EN ESA FASE O NO
    bool octavosClas = false;
    bool cuartosClas = false;
    bool semifinalClas = false;
    bool winner = false;
    bool winnerT = false;
    // COLOCAR LOS INT COMO VECTORES Y QUE LAS POSICIONES DEL VECTOR DEPENDE DE QUE TAN LEJOS LLEGÓ EL EQUIPO, ES DECIR:
    // 0: FASE DE GRUPOS, 1: OCTAVOS, 2: CUARTOS, 3, FINAL O SEMIFINAL
    // PARA NO GENERAR CONFUCIONES LA VARIABLE 'final_' ES PARA CONFIRMAR SI
    // LOS EQUIPOS QUE NO PASAN SE LES PONE -1
};
struct Team
{
    Team *next;
    string group;
    vector<pais *> posicionClasificados;
    vector<string> countries;
    vector<pais *> pais; // GUARDAMOS LOS APUNTADORE DE CADA PAIS QUE ESTAN EN countries, para así apuntar a su informacion en pais strcuture
};
struct octavosF
{
    octavosF *next;
    int grupo; // GRUPO 1, GRUPO 2 Y ASI
    vector<pais *> clasificados;
};
struct cuartosF
{
    cuartosF *next;
    int grupo; // GRUPO 1, GRUPO 2 Y ASI
    vector<pais *> clasificados;
};
struct semifinal_
{
    semifinal_ *next;
    int grupo; // GRUPO 1, GRUPO 2 Y ASI
    vector<pais *> clasificados;
};
struct finalP
{
    finalP *next;
    vector<pais *> clasificados;
};
struct Estadio
{
    Estadio *next;
    string nombre;
};

// VARIABLES GLOBALES
int nClasificados = 2;
Team *head = nullptr;
pais *headP = nullptr;
octavosF *headO = nullptr;
cuartosF *headC = nullptr;
semifinal_ *headSF = nullptr;
finalP *headFP = nullptr;
Estadio *headEstadio = nullptr;
// DEFINO LAS FUNCIONES
void createEstadio(string nombreEstadio);
void iniciar();
vector<string> split(string txt);
void prueba();
int nCaracteres(string str);
vector<int> goles();
bool read();
void create(Team *&head, string group, vector<string> countries);
void createPais(Team *&head);
void calcularPuntos();
void calcularPosicion();
vector<pais *> orderByGolesDiferencia(vector<pais *> s);
void octavos();
void cuartos();
void semifinal();
void mostrarP(pais *&headP);
void mostrar(Team *&head);
void mostrarTeam();
void mostrarTabla();
void clasi();
void menu();

// FUNCIONES QUE USO UNA VEZ O VARIAS, PERO NO TAN IMPORTANTES
void iniciar()
{

    pais *current = headP;
    do
    {
        current->ganados.push_back(0);
        current->empatados.push_back(0);
        current->perdidos.push_back(0);
        current->totalPartidos.push_back(0);
        current->goleFavor.push_back(0);
        current->golesContra.push_back(0);
        current->golesDiferencia.push_back(0);
        current->totalPuntos.push_back(0);
        current = current->next;
    } while (current != nullptr);
}
vector<string> split(string txt)
{
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while (posFound >= 0)
    {
        posFound = txt.find(",", posInit);
        splitted = txt.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}
void prueba()
{

    Team *current = head;
    vector<int> v;
    int numeroAleatorio, golesA, golesB, a = 0;
    do
    {
        cout << "----------------------------" << current->group << "----------------------------" << endl;
        for (int i = 0; i < current->countries.size(); i++)
        {
            for (int j = 1; j < current->countries.size(); j++)
            {
                if (j + a >= current->countries.size())
                {
                    break;
                }
                cout << current->pais[i]->nombre << " vs " << current->pais[j + a]->nombre << endl;
                numeroAleatorio = rand() % 3;
            }

            a += 1;
        }
        current = current->next;
        a = 0;
    } while (current != nullptr);
}
int nCaracteres(string str)
{
    // Convertir la cadena a wide string
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring wstr = converter.from_bytes(str);
    return wstr.size();
}
vector<int> goles()
{
    vector<int> v;
    v.push_back(rand() % 7);
    v.push_back(rand() % 7);
    return v;
}
bool read()
{
    ifstream file;
    string group;
    string countries;
    file.open("equipos.txt", ios::in);
    if (file.fail())
    {
        return false;
    }

    while (!file.eof())
    {
        getline(file, group);
        getline(file, countries);
        create(head, group, split(countries));
    }
    createPais(head);
    file.close();
    file.open("estadios.txt", ios::in);
    string nombreEstadio;
    if (file.fail())
    {
        return false;
    }
    while (!file.eof())
    {
        getline(file, nombreEstadio);
        createEstadio(nombreEstadio);
    }
    iniciar();
    return true;
}
void calcularPuntos()
{
    // GANADOS = 3pts
    // EMPATADOS = 1pts
    // PERDIDOS = 0pts
    pais *current = headP;
    do
    {
        current->totalPuntos[0] = (current->ganados[0] * 3) + (current->empatados[0]);
        current->totalPartidos[0] = current->ganados[0] + current->perdidos[0] + current->empatados[0];
        current->golesDiferencia[0] = current->goleFavor[0] - current->golesContra[0];
        current = current->next;
    } while (current != nullptr);
}
void calcularPosicion()
{
    /**
     * CASOS A TENER EN CUENTA :
     *          TRES SON IGUALES EN TOTAL DE PUNTOS
     *          DOS SON IGUALES EN TOTAL DE PUNTOS
     */
    Team *current = head;
    vector<pais *> s; // Vector que guarda los punteros de cada pais en orden de mayor a menor segun sus puntos.
    do
    {
        // DUPLICA EL VECTOR Y LO GUARDA EN s
        for (int i = 0; i < current->pais.size(); i++)
        {
            s.push_back(current->pais[i]);
        }
        pais *aux;
        // ORDENA EL VECTOR s DE MENOR A MAYOR
        for (int i = 0; i < size(s); i++)
        {
            for (int j = 0; j < size(s) - 1; j++)
            {
                if (s[j]->totalPuntos < s[j + 1]->totalPuntos)
                {

                    aux = s[j];
                    s[j] = s[j + 1];
                    s[j + 1] = aux;
                }
            }
        }
        /*
                for (int i = 0; i < size(s); i++)
                {
                    cout << "NOMBRE: " << s[i]->nombre << endl;
                    cout << "GOLES: " << s[i]->golesDiferencia << endl;
                }*/

        //
        // comprobaciones
        // SI TODAS LOS PUNTOS SON IGUALES
        int nRepetidasPts = 0;
        int f = s[0]->totalPuntos[0];
        for (int i = 0; i < nClasificados; i++)
        {
            for (int j = 1; j <= nClasificados; j++)
            {
                if (s[i]->totalPuntos == s[i + j]->totalPuntos)
                {
                    nRepetidasPts += 1;
                }
            }
            // cout << "nRepetidasPts ES: " << nRepetidasPts << current->group << "................." << endl;
            if (nRepetidasPts >= nClasificados)
            {
                // cout << "PUNTOS SIMILARES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                s = orderByGolesDiferencia(s);
            }
            // current = current->next;
        }
        // SI EL PRIMER PUESTO TIENE -2 GOLES, PERO AUN
        // EN EL CASO QUE TODOS LOS PAISES TENGAN LOS MISMOS PUNTOS
        if (nRepetidasPts >= nClasificados)
        {
            // cout << "PUNTOS SIMILARES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            s = orderByGolesDiferencia(s);
        }
        current->posicionClasificados = s;
        s.clear();
        current = current->next;

    } while (current != nullptr);
}
vector<pais *> orderByGolesDiferencia(vector<pais *> s)
{
    pais *aux;
    // ORDENAMOS POR METODO BURBUJA
    for (int i = 0; i < size(s); i++)
    {
        for (int j = 0; j < size(s) - 1; j++)
        {
            if (s[j]->golesDiferencia < s[j + 1]->golesDiferencia)
            {
                aux = s[j];
                s[j] = s[j + 1];
                s[j + 1] = aux;
            }
        }
    }
    // COMPROBAMOS CUANTAS VECES ESTÁ REPETIDO 'golesDiferencia' Y DE AHÍ HACEMOS CAMBIOS
    int nRepetidas = 0; // Varaible que guarda las veces que se está repitiendo el número mayor de
    int f = s[0]->golesDiferencia[0];
    for (int i = 1; i < s.size(); i++)
    {
        if (f == s[i]->golesDiferencia[0])
        {
            nRepetidas += 1;
        }
    }
    // HACERLO CON VECTOR QUE GUARDE n CANTIDAD DE CAMBIOS, QUE REPRESENTARIA LA n CANTIDAD DE CLASIFICADOS
    if (nRepetidas >= nClasificados)
    {
        vector<int> r; // PARA GUARDAR LA n CANTIDAD DE CAMBIOS
        for (int i = 0; i < nClasificados; i++)
        {
            r.push_back(rand() % (nRepetidas + 1));
        }

        for (int i = 0; i < r.size(); i++)
        {
            aux = s[i];
            s[i] = s[r[i]];
            s[r[i]] = aux;
        }
        return s;
    }

    return s;
}
float floatRandom()
{
    int numeroEntero = rand() % 100 + 1;
    float numeroDecimal = static_cast<float>(numeroEntero) / 100.0;

    return numeroDecimal;
}

// SECION DE CREACION DE NODOS
void create(Team *&head, string group, vector<string> countries)
{

    Team *new_node = new Team();
    new_node->group = group;
    new_node->countries = countries;
    new_node->next = nullptr;

    if (head == nullptr)
    {
        head = new_node;
    }
    else
    {
        Team *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}
void createPais(Team *&head)
{
    Team *current = head;
    pais *new_ = new pais();
    vector<pais *> vP;
    if (head == NULL)
    {
        cout << "NADA QUE HACER" << endl;
        return;
    }
    do
    {
        for (string Pais : current->countries)
        {
            pais *new_ = new pais();
            new_->nombre = Pais;

            if (headP == nullptr)
            {

                headP = new_;
            }
            else
            {
                pais *currentP = headP;
                while (currentP->next != nullptr)
                {
                    currentP = currentP->next;
                }
                currentP->next = new_;
            }

            vP.push_back(new_);

            current->pais = vP;
        }
        vP.clear();
        current = current->next;
    } while (current != nullptr);
}
void createEstadio(string nombreEstadio)
{
    Estadio *new_node = new Estadio();
    new_node->nombre = nombreEstadio;
    new_node->next = nullptr;

    if (headEstadio == nullptr)
    {
        cout << "SIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII" << endl;
        headEstadio = new_node;
        headEstadio->next = headEstadio;
    }
    else
    {
        Estadio *current = headEstadio;
        while (current->next != headEstadio)
        {
            current = current->next;
        }
        current->next = new_node;
        new_node->next = headEstadio;
    }
}
void createOctavos(int grupo, vector<pais *> countries)
{
    octavosF *new_node = new octavosF();
    new_node->grupo = grupo;
    new_node->clasificados = countries;
    new_node->next = nullptr;

    if (headO == nullptr)
    {
        headO = new_node;
    }
    else
    {
        octavosF *current = headO;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}
void createCuartos(int grupo, vector<pais *> countries)
{
    cuartosF *new_node = new cuartosF();
    new_node->grupo = grupo;
    new_node->clasificados = countries;
    new_node->next = nullptr;

    if (headC == nullptr)
    {
        headC = new_node;
    }
    else
    {
        cuartosF *current = headC;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}
void createSemifinal(int grupo, vector<pais *> countries)
{
    semifinal_ *new_node = new semifinal_();
    new_node->grupo = grupo;
    new_node->clasificados = countries;
    new_node->next = nullptr;

    if (headSF == nullptr)
    {
        headSF = new_node;
    }
    else
    {
        semifinal_ *current = headSF;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}
void createFinal(vector<pais *> countries)
{
    finalP *new_node = new finalP();
    new_node->clasificados = countries;
    new_node->next = nullptr;

    if (headFP == nullptr)
    {
        headFP = new_node;
    }
    else
    {
        finalP *current = headFP;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}
// SECCION DONDE CALCULA TODO DE MANERA ALEATORIA
void faseGrupos(Team *&head)
{
    Team *current = head;
    vector<int> v;
    int numeroAleatorio, golesA, golesB, a = 0;
    do
    {
        cout << "----------------------------" << current->group << "----------------------------" << endl;
        for (int i = 0; i < current->countries.size(); i++)
        {
            for (int j = 1; j < current->countries.size(); j++)
            {
                if (j + a >= current->countries.size())
                {
                    break;
                }
                cout << current->pais[i]->nombre << " vs " << current->pais[j + a]->nombre << endl;
                cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
                headEstadio = headEstadio->next;
                numeroAleatorio = rand() % 3;
                switch (numeroAleatorio)
                {
                case 0:
                    do
                    {
                        v = goles();
                    } while (v[0] >= v[1]);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << v[0] << "\n"
                         << current->pais[j + a]->nombre << " : " << v[1] << endl;
                    cout << current->pais[i]->nombre << " PERDIO" << endl;

                    // ACTULIZAR INFORMACION
                    current->pais[i]->perdidos[0] += 1;
                    current->pais[j + a]->ganados[0] += 1;
                    break;
                case 1:

                    do
                    {
                        v = goles();
                    } while (v[0] <= v[1]);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << v[0] << "\n"
                         << current->pais[j + a]->nombre << " : " << v[1] << endl;
                    cout << current->pais[i]->nombre << " GANÓ" << endl;

                    //
                    current->pais[i]->ganados[0] += 1;
                    current->pais[j + a]->perdidos[0] += 1;
                    break;
                case 2:
                    do
                    {
                        v = goles();
                    } while (v[0] != v[1]);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << v[0] << "\n"
                         << current->pais[j + a]->nombre << " : " << v[1] << endl;
                    cout << current->pais[i]->nombre << " Y " << current->pais[j + a]->nombre << " EMPATARON" << endl;

                    current->pais[i]->empatados[0] += 1;
                    current->pais[j + a]->empatados[0] += 1;
                    break;
                }
                cout << "----------------------------------------------------------------------------------------------------------------" << endl;
                current->pais[i]->goleFavor[0] += v[0];
                current->pais[i]->golesContra[0] += v[1];
                current->pais[j + a]->golesContra[0] += v[0];
                current->pais[j + a]->goleFavor[0] += v[1];
            }

            a += 1;
        }
        current = current->next;
        a = 0;
    } while (current != nullptr);
    calcularPuntos();
    calcularPosicion();
}
void octavos()
{
    vector<pais *> paisesOctavos;
    vector<pais *> aux_paisesOctavos;
    Team *currentT = head;
    Team *aux = currentT->next;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        aux = currentT->next;
        for (int i = 0; i < nClasificados; i = i + 2)
        {
            paisesOctavos.push_back(currentT->posicionClasificados[i]);
            paisesOctavos.push_back(aux->posicionClasificados[i]);
            //
            aux_paisesOctavos.push_back(currentT->posicionClasificados[i + 1]);
            aux_paisesOctavos.push_back(aux->posicionClasificados[i + 1]);
        }
        currentT = currentT->next->next;
        createOctavos(num, paisesOctavos);
        createOctavos(num + 1, aux_paisesOctavos);
        num += 2;
        paisesOctavos.clear();
        aux_paisesOctavos.clear();
    } while (currentT != nullptr);
    // CALCULAR Y ASÍ
    octavosF *current = headO;
    vector<int> v;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        numeroAleatorio = rand() % 2;
        switch (numeroAleatorio)
        {
        case 0:
            do
            {
                v = goles();
            } while (v[0] >= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;

            // ACTULIZAR INFORMACION
            current->clasificados[1]->octavosClas = true;
            break;
        case 1:

            do
            {
                v = goles();
            } while (v[0] <= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            //
            current->clasificados[0]->octavosClas = true;
            break;
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;

        current->clasificados[0]->goleFavor.push_back(v[0]);
        current->clasificados[1]->goleFavor.push_back(v[1]);
        current = current->next;
    } while (current != nullptr);
}
void cuartos()
{
    vector<pais *> paisesCuartos;
    octavosF *currentT = headO;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->octavosClas == true)
            {
                paisesCuartos.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->octavosClas == true)
            {
                paisesCuartos.push_back(currentT->clasificados[i]);
            }
        }
        createCuartos(num, paisesCuartos);
        num += 1;
        paisesCuartos.clear();
        currentT = currentT->next;
    } while (currentT != nullptr);
    cuartosF *current = headC;
    vector<int> v;
    do
    {
        /*
        cout << "---------------------------- GRUPO: " << current->grupo << "----------------------------" << endl;*/
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        numeroAleatorio = rand() % 2;
        switch (numeroAleatorio)
        {
        case 0:
            do
            {
                v = goles();
            } while (v[0] >= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;

            // ACTULIZAR INFORMACION
            current->clasificados[1]->cuartosClas = true;
            break;
        case 1:

            do
            {
                v = goles();
            } while (v[0] <= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            //
            current->clasificados[0]->cuartosClas = true;
            break;
        }
        current->clasificados[0]->goleFavor.push_back(v[0]);
        current->clasificados[1]->goleFavor.push_back(v[1]);
        current = current->next;
        // cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    } while (current != nullptr);
}
void semifinal()
{
    vector<pais *> paisesSemifinal;
    cuartosF *currentT = headC;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->cuartosClas == true)
            {
                paisesSemifinal.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->cuartosClas == true)
            {
                paisesSemifinal.push_back(currentT->clasificados[i]);
            }
        }
        createSemifinal(num, paisesSemifinal);
        num += 1;
        paisesSemifinal.clear();
        currentT = currentT->next;
    } while (currentT != nullptr);
    semifinal_ *current = headSF;
    vector<int> v;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        numeroAleatorio = rand() % 2;
        switch (numeroAleatorio)
        {
        case 0:
            do
            {
                v = goles();
            } while (v[0] >= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;

            // ACTULIZAR INFORMACION
            current->clasificados[1]->semifinalClas = true;
            break;
        case 1:

            do
            {
                v = goles();
            } while (v[0] <= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            //
            current->clasificados[0]->semifinalClas = true;
            break;
        }
        current->clasificados[0]->goleFavor.push_back(v[0]);
        current->clasificados[1]->goleFavor.push_back(v[1]);
        current = current->next;
        // cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    } while (current != nullptr);
}
void partidoFinal()
{
    vector<pais *> paisesFinal;
    semifinal_ *currentT = headSF;
    int numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->semifinalClas == true)
            {
                paisesFinal.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
    } while (currentT != nullptr);
    createFinal(paisesFinal);
    finalP *current = headFP;
    vector<int> v;
    cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
    cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
    headEstadio = headEstadio->next;
    numeroAleatorio = rand() % 2;
    switch (numeroAleatorio)
    {
    case 0:
        do
        {
            v = goles();
        } while (v[0] >= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " PERDIO" << endl;

        // ACTULIZAR INFORMACION
        current->clasificados[1]->winner = true;
        break;
    case 1:

        do
        {
            v = goles();
        } while (v[0] <= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " GANÓ" << endl;

        // ACTULIZAR INFORMACION
        current->clasificados[0]->winner = true;
        break;
    }
    current->clasificados[0]->goleFavor.push_back(v[0]);
    current->clasificados[1]->goleFavor.push_back(v[1]);
    // cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
}
void tercerPuesto()
{
    vector<pais *> paisesFinal;
    semifinal_ *current = headSF;
    int numeroAleatorio, a = 0;
    vector<int> v;
    do
    {
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            if (current->clasificados[i]->semifinalClas == false)
            {
                paisesFinal.push_back(current->clasificados[i]);
            }
        }
        current = current->next;
    } while (current != nullptr);
    //
    cout << paisesFinal[0]->nombre << " vs " << paisesFinal[1]->nombre << endl;
    cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
    headEstadio = headEstadio->next;
    numeroAleatorio = rand() % 2;
    current = headSF;
    switch (numeroAleatorio)
    {
    case 0:
        do
        {
            v = goles();
        } while (v[0] >= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " PERDIO" << endl;

        // ACTULIZAR INFORMACION
        paisesFinal[1]->winnerT = true;
        break;
    case 1:

        do
        {
            v = goles();
        } while (v[0] <= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " GANÓ" << endl;

        // ACTULIZAR INFORMACION
        paisesFinal[0]->winnerT = true;
        break;
    }
    paisesFinal[0]->goleFavor.push_back(v[0]);
    paisesFinal[1]->goleFavor.push_back(v[1]);
}
// EL USUARIOS INGRESA LOS RESULTADOS
void faseGruposIn()
{
    Team *current = head;
    vector<int> v;
    int choise, golesA, golesB, a = 0;
    do
    {
        cout << "----------------------------" << current->group << "----------------------------" << endl;
        for (int i = 0; i < current->countries.size(); i++)
        {
            for (int j = 1; j < current->countries.size(); j++)
            {
                if (j + a >= current->countries.size())
                {
                    break;
                }
                cout << current->pais[i]->nombre << " vs " << current->pais[j + a]->nombre << endl;
                cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
                headEstadio = headEstadio->next;
                //
                cout << "Opciones del partido: " << endl;
                cout << "0: Pierde " << current->pais[i]->nombre << " contra " << current->pais[j + a]->nombre << endl;
                cout << "1: Gana " << current->pais[i]->nombre << " contra " << current->pais[j + a]->nombre << endl;
                cout << "2: Empatan " << current->pais[i]->nombre << " y " << current->pais[j + a]->nombre << endl;
                cout << "Opcion: ";
                cin >> choise;
                switch (choise)
                {
                    // PIERDE EL PAIS A
                case 0:
                    do
                    {
                        cout << "Ingrese los goles de " << current->pais[i]->nombre << ": "; // 4
                        cin >> golesA;
                        cout << "Ingrese los goles de " << current->pais[j + a]->nombre << ": "; // 8
                        cin >> golesB;
                        if (golesA >= golesB)
                        {
                            cout << "LOS GOLES DE " << current->pais[i]->nombre << " DEBEN SER MENORES A LOS DE " << current->pais[j + a]->nombre << endl;
                        }
                    } while (golesA >= golesB);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << golesA << "\n"
                         << current->pais[j + a]->nombre << " : " << golesB << endl;
                    cout << current->pais[i]->nombre << " PERDIO" << endl;

                    // ACTULIZAR INFORMACION
                    current->pais[i]->perdidos[0] += 1;
                    current->pais[j + a]->ganados[0] += 1;
                    break;
                // GANA EL PAIS A
                case 1:
                    do
                    {
                        cout << "Ingrese los goles de " << current->pais[i]->nombre << ": "; // 4
                        cin >> golesA;
                        cout << "Ingrese los goles de " << current->pais[j + a]->nombre << ": "; // 8
                        cin >> golesB;
                        if (golesA <= golesB)
                        {
                            cout << "LOS GOLES DE " << current->pais[i]->nombre << " DEBEN SER MAYORES A LOS DE " << current->pais[j + a]->nombre << endl;
                        }
                    } while (golesA <= golesB);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << golesA << "\n"
                         << current->pais[j + a]->nombre << " : " << golesB << endl;
                    cout << current->pais[i]->nombre << " GANÓ" << endl;

                    //
                    current->pais[i]->ganados[0] += 1;
                    current->pais[j + a]->perdidos[0] += 1;
                    break;
                    // EMPATAN
                case 2:
                    do
                    {
                        cout << "Ingrese los goles de " << current->pais[i]->nombre << ": "; // 4
                        cin >> golesA;
                        cout << "Ingrese los goles de " << current->pais[j + a]->nombre << ": "; // 8
                        cin >> golesB;
                        if (golesA != golesB)
                        {
                            cout << "LOS GOLES DE " << current->pais[i]->nombre << " DEBEN SER IGUALES A LOS DE " << current->pais[j + a]->nombre << endl;
                        }
                    } while (golesA != golesB);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << golesA << "\n"
                         << current->pais[j + a]->nombre << " : " << golesB << endl;
                    cout << current->pais[i]->nombre << " Y " << current->pais[j + a]->nombre << " EMPATARON" << endl;

                    current->pais[i]->empatados[0] += 1;
                    current->pais[j + a]->empatados[0] += 1;
                    break;
                }
                cout << "----------------------------------------------------------------------------------------------------------------" << endl;
                current->pais[i]->goleFavor[0] += golesA;
                current->pais[i]->golesContra[0] += golesB;
                current->pais[j + a]->golesContra[0] += golesA;
                current->pais[j + a]->goleFavor[0] += golesB;
            }

            a += 1;
        }
        current = current->next;
        a = 0;
    } while (current != nullptr);
    calcularPuntos();
    calcularPosicion();
}
void octavosIn()
{
    vector<pais *> paisesOctavos;
    vector<pais *> aux_paisesOctavos;
    Team *currentT = head;
    Team *aux = currentT->next;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        aux = currentT->next;
        for (int i = 0; i < nClasificados; i = i + 2)
        {
            paisesOctavos.push_back(currentT->posicionClasificados[i]);
            paisesOctavos.push_back(aux->posicionClasificados[i]);
            //
            aux_paisesOctavos.push_back(currentT->posicionClasificados[i + 1]);
            aux_paisesOctavos.push_back(aux->posicionClasificados[i + 1]);
        }
        currentT = currentT->next->next;
        createOctavos(num, paisesOctavos);
        createOctavos(num + 1, aux_paisesOctavos);
        num += 2;
        paisesOctavos.clear();
        aux_paisesOctavos.clear();
    } while (currentT != nullptr);
    // CALCULAR Y ASÍ
    octavosF *current = headO;
    int choise, golesA, golesB;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        cout << "Opciones del partido: " << endl;
        cout << "0: Pierde " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "1: Gana " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "2: " << current->clasificados[0]->nombre << " y " << current->clasificados[1]->nombre << " empatan y el partido se define en el tiempo extra (Si ambos equipos realizan la misma cantidad de goles pasan a penales)" << endl;
        cout << "Opcion: ";
        cin >> choise;
        switch (choise)
        {
        case 0:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA >= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MENORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA >= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            //
            current->clasificados[1]->octavosClas = true;
            break;
        case 1:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA <= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MAYORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA <= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            current->clasificados[0]->octavosClas = true;
            break;

        // PROLOGA
        case 2:
            cout << "Ingrese los goles que realizaron los equipos en la prologa: " << endl;
            cout << "Goles de " << current->clasificados[0]->nombre << ": ";
            cin >> golesA;
            cout << "Goles de " << current->clasificados[1]->nombre << ": ";
            cin >> golesB;
            if (golesA != golesB)
            {
                if (golesA > golesB)
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                    current->clasificados[0]->octavosClas = true;
                }
                else
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                    current->clasificados[1]->octavosClas = true;
                }

                break;
            }
            cout << "Como los equipos anotaron la misma cantidad de goles el partido se definirá por panales realizados" << endl;
        // PENALES
        case 3:
            int penal;
            vector<int> penalesAnotados;
            penalesAnotados.push_back(0);
            penalesAnotados.push_back(0);
            do
            {
                int intento = 1;
                while (intento <= 5)
                {
                    cout << "Penal " << intento << " del equipo " << current->clasificados[0]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[0] += 1;
                    }
                    //
                    cout << "Penal " << intento << " del equipo " << current->clasificados[1]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[1] += 1;
                    }
                    intento++;
                }
                if (penalesAnotados[0] != penalesAnotados[1])
                {
                    if (penalesAnotados[0] > penalesAnotados[1])
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                        current->clasificados[0]->octavosClas = true;
                    }
                    else
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                        current->clasificados[1]->octavosClas = true;
                    }
                    break;
                }
                else
                {
                    cout << "LOS EQUIPOS REALIZARON LA MISMA CANTIDAD DE ANOTACIONES EN LOS PENALES" << endl;
                    cout << "POR ENDE SE VUELVE A REALIZAR OTRA TANDA DE PENALES" << endl;
                }
            } while (penalesAnotados[0] == penalesAnotados[1]);
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        current->clasificados[0]->goleFavor.push_back(golesA);
        current->clasificados[1]->goleFavor.push_back(golesB);
        current = current->next;
    } while (current != nullptr);
}
void cuartosIn()
{
    vector<pais *> paisesCuartos;
    octavosF *currentT = headO;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->octavosClas == true)
            {
                paisesCuartos.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->octavosClas == true)
            {
                paisesCuartos.push_back(currentT->clasificados[i]);
            }
        }
        createCuartos(num, paisesCuartos);
        num += 1;
        paisesCuartos.clear();
        currentT = currentT->next;
    } while (currentT != nullptr);
    cuartosF *current = headC;
    int choise, golesA, golesB;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        cout << "Opciones del partido: " << endl;
        cout << "0: Pierde " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "1: Gana " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "2: " << current->clasificados[0]->nombre << " y " << current->clasificados[1]->nombre << " empatan y el partido se define en el tiempo extra (Si ambos equipos realizan la misma cantidad de goles pasan a penales)" << endl;
        cout << "Opcion: ";
        cin >> choise;
        switch (choise)
        {
        case 0:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA >= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MENORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA >= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            //
            current->clasificados[1]->cuartosClas = true;
            break;
        case 1:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA <= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MAYORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA <= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            current->clasificados[0]->cuartosClas = true;
            break;

        // PROLOGA
        case 2:
            cout << "Ingrese los goles que realizaron los equipos en la prologa: " << endl;
            cout << "Goles de " << current->clasificados[0]->nombre << ": ";
            cin >> golesA;
            cout << "Goles de " << current->clasificados[1]->nombre << ": ";
            cin >> golesB;
            if (golesA != golesB)
            {
                if (golesA > golesB)
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                    current->clasificados[0]->cuartosClas = true;
                }
                else
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                    current->clasificados[1]->cuartosClas = true;
                }

                break;
            }
            cout << "Como los equipos anotaron la misma cantidad de goles el partido se definirá por panales realizados" << endl;
        // PENALES
        case 3:
            int penal;
            vector<int> penalesAnotados;
            penalesAnotados.push_back(0);
            penalesAnotados.push_back(0);
            do
            {
                int intento = 1;
                while (intento <= 5)
                {
                    cout << "Penal " << intento << " del equipo " << current->clasificados[0]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[0] += 1;
                    }
                    //
                    cout << "Penal " << intento << " del equipo " << current->clasificados[1]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[1] += 1;
                    }
                    intento++;
                }
                if (penalesAnotados[0] != penalesAnotados[1])
                {
                    if (penalesAnotados[0] > penalesAnotados[1])
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                        current->clasificados[0]->cuartosClas = true;
                    }
                    else
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                        current->clasificados[1]->cuartosClas = true;
                    }
                    break;
                }
                else
                {
                    cout << "LOS EQUIPOS REALIZARON LA MISMA CANTIDAD DE ANOTACIONES EN LOS PENALES" << endl;
                    cout << "POR ENDE SE VUELVE A REALIZAR OTRA TANDA DE PENALES" << endl;
                }
            } while (penalesAnotados[0] == penalesAnotados[1]);
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        current->clasificados[0]->goleFavor.push_back(golesA);
        current->clasificados[1]->goleFavor.push_back(golesB);
        current = current->next;
    } while (current != nullptr);
}
void semifinalIn()
{
    vector<pais *> paisesSemifinal;
    cuartosF *currentT = headC;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->cuartosClas == true)
            {
                paisesSemifinal.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->cuartosClas == true)
            {
                paisesSemifinal.push_back(currentT->clasificados[i]);
            }
        }
        createSemifinal(num, paisesSemifinal);
        num += 1;
        paisesSemifinal.clear();
        currentT = currentT->next;
    } while (currentT != nullptr);
    semifinal_ *current = headSF;
    int choise, golesA, golesB;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        cout << "Opciones del partido: " << endl;
        cout << "0: Pierde " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "1: Gana " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "2: " << current->clasificados[0]->nombre << " y " << current->clasificados[1]->nombre << " empatan y el partido se define en el tiempo extra (Si ambos equipos realizan la misma cantidad de goles pasan a penales)" << endl;
        cout << "Opcion: ";
        cin >> choise;
        switch (choise)
        {
        case 0:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA >= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MENORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA >= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            //
            current->clasificados[1]->semifinalClas = true;
            break;
        case 1:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA <= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MAYORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA <= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            current->clasificados[0]->semifinalClas = true;
            break;

        // PROLOGA
        case 2:
            cout << "Ingrese los goles que realizaron los equipos en la prologa: " << endl;
            cout << "Goles de " << current->clasificados[0]->nombre << ": ";
            cin >> golesA;
            cout << "Goles de " << current->clasificados[1]->nombre << ": ";
            cin >> golesB;
            if (golesA != golesB)
            {
                if (golesA > golesB)
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                    current->clasificados[0]->semifinalClas = true;
                }
                else
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                    current->clasificados[1]->semifinalClas = true;
                }

                break;
            }
            cout << "Como los equipos anotaron la misma cantidad de goles el partido se definirá por panales realizados" << endl;
        // PENALES
        case 3:

            int penal, totalA = 0, totalB;
            vector<int> penalesAnotados;
            penalesAnotados.push_back(0);
            penalesAnotados.push_back(0);
            do
            {
                int intento = 1;
                while (intento <= 5)
                {
                    cout << "Penal " << intento << " del equipo " << current->clasificados[0]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[0] += 1;
                    }
                    //
                    cout << "Penal " << intento << " del equipo " << current->clasificados[1]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[1] += 1;
                    }
                    intento++;
                }
                if (penalesAnotados[0] != penalesAnotados[1])
                {
                    if (penalesAnotados[0] > penalesAnotados[1])
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                        current->clasificados[0]->semifinalClas = true;
                    }
                    else
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                        current->clasificados[1]->semifinalClas = true;
                    }
                    break;
                }
                else
                {
                    cout << "LOS EQUIPOS REALIZARON LA MISMA CANTIDAD DE ANOTACIONES EN LOS PENALES" << endl;
                    cout << "POR ENDE SE VUELVE A REALIZAR OTRA TANDA DE PENALES" << endl;
                }
            } while (penalesAnotados[0] == penalesAnotados[1]);
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;

        current->clasificados[0]->goleFavor.push_back(golesA);
        current->clasificados[1]->goleFavor.push_back(golesB);
        current = current->next;
    } while (current != nullptr);
}
void partidoFinalIn()
{
    vector<pais *> paisesFinal;
    semifinal_ *currentT = headSF;
    int numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->semifinalClas == true)
            {
                paisesFinal.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
    } while (currentT != nullptr);
    createFinal(paisesFinal);
    finalP *current = headFP;
    int choise, golesA, golesB;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        cout << "Opciones del partido: " << endl;
        cout << "0: Pierde " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "1: Gana " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "2: " << current->clasificados[0]->nombre << " y " << current->clasificados[1]->nombre << " empatan y el partido se define en el tiempo extra (Si ambos equipos realizan la misma cantidad de goles pasan a penales)" << endl;
        cout << "Opcion: ";
        cin >> choise;
        switch (choise)
        {
        case 0:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA >= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MENORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA >= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            //
            current->clasificados[1]->winner = true;
            break;
        case 1:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA <= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MAYORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA <= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            current->clasificados[0]->winner = true;
            break;

        // PROLOGA
        case 2:
            cout << "Ingrese los goles que realizaron los equipos en la prologa: " << endl;
            cout << "Goles de " << current->clasificados[0]->nombre << ": ";
            cin >> golesA;
            cout << "Goles de " << current->clasificados[1]->nombre << ": ";
            cin >> golesB;
            if (golesA != golesB)
            {
                if (golesA > golesB)
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                    current->clasificados[0]->winner = true;
                }
                else
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                    current->clasificados[1]->winner = true;
                }

                break;
            }
            cout << "Como los equipos anotaron la misma cantidad de goles el partido se definirá por panales realizados" << endl;
        // PENALES
        case 3:
            int penal;
            vector<int> penalesAnotados;
            penalesAnotados.push_back(0);
            penalesAnotados.push_back(0);
            do
            {
                int intento = 1;
                while (intento <= 5)
                {
                    cout << "Penal " << intento << " del equipo " << current->clasificados[0]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[0] += 1;
                    }
                    //
                    cout << "Penal " << intento << " del equipo " << current->clasificados[1]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[1] += 1;
                    }
                    intento++;
                }
                if (penalesAnotados[0] != penalesAnotados[1])
                {
                    if (penalesAnotados[0] > penalesAnotados[1])
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                        current->clasificados[0]->winner = true;
                    }
                    else
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                        current->clasificados[1]->winner = true;
                    }
                    break;
                }
                else
                {
                    cout << "LOS EQUIPOS REALIZARON LA MISMA CANTIDAD DE ANOTACIONES EN LOS PENALES" << endl;
                    cout << "POR ENDE SE VUELVE A REALIZAR OTRA TANDA DE PENALES" << endl;
                }
            } while (penalesAnotados[0] == penalesAnotados[1]);
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        current->clasificados[0]->goleFavor.push_back(golesA);
        current->clasificados[1]->goleFavor.push_back(golesB);
        current = current->next;
    } while (current != nullptr);
}
void tercerPuestoIn()
{
    vector<pais *> paisesFinal;
    semifinal_ *current = headSF;
    int choise, golesA, golesB;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        cout << "Opciones del partido: " << endl;
        cout << "0: Pierde " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "1: Gana " << current->clasificados[0]->nombre << " contra " << current->clasificados[1]->nombre << endl;
        cout << "2: " << current->clasificados[0]->nombre << " y " << current->clasificados[1]->nombre << " empatan y el partido se define en el tiempo extra (Si ambos equipos realizan la misma cantidad de goles pasan a penales)" << endl;
        cout << "Opcion: ";
        cin >> choise;
        switch (choise)
        {
        case 0:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA >= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MENORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA >= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            //
            current->clasificados[1]->winnerT = true;
            break;
        case 1:
            do
            {
                cout << "Ingrese los goles de " << current->clasificados[0]->nombre << ": "; // 4
                cin >> golesA;
                cout << "Ingrese los goles de " << current->clasificados[1]->nombre << ": "; // 8
                cin >> golesB;
                if (golesA <= golesB)
                {
                    cout << "LOS GOLES DE " << current->clasificados[0]->nombre << " DEBEN SER MAYORES A LOS DE " << current->clasificados[1]->nombre << endl;
                }
            } while (golesA <= golesB);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << golesA << "\n"
                 << current->clasificados[1]->nombre << " : " << golesB << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            current->clasificados[0]->winnerT = true;
            break;

        // PROLOGA
        case 2:
            cout << "Ingrese los goles que realizaron los equipos en la prologa: " << endl;
            cout << "Goles de " << current->clasificados[0]->nombre << ": ";
            cin >> golesA;
            cout << "Goles de " << current->clasificados[1]->nombre << ": ";
            cin >> golesB;
            if (golesA != golesB)
            {
                if (golesA > golesB)
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                    current->clasificados[0]->winnerT = true;
                }
                else
                {
                    cout << "GOLES: \n"
                         << current->clasificados[0]->nombre << ": " << golesA << "\n"
                         << current->clasificados[1]->nombre << " : " << golesB << endl;
                    cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                    current->clasificados[1]->winnerT = true;
                }

                break;
            }
            cout << "Como los equipos anotaron la misma cantidad de goles el partido se definirá por panales realizados" << endl;
        // PENALES
        case 3:
            int penal;
            vector<int> penalesAnotados;
            penalesAnotados.push_back(0);
            penalesAnotados.push_back(0);
            do
            {
                int intento = 1;
                while (intento <= 5)
                {
                    cout << "Penal " << intento << " del equipo " << current->clasificados[0]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[0] += 1;
                    }
                    //
                    cout << "Penal " << intento << " del equipo " << current->clasificados[1]->nombre << endl;
                    cout << "Anotó? (No: 0 - Sí: 1)" << endl;
                    cin >> penal;
                    if (penal == 1)
                    {
                        penalesAnotados[1] += 1;
                    }
                    intento++;
                }
                if (penalesAnotados[0] != penalesAnotados[1])
                {
                    if (penalesAnotados[0] > penalesAnotados[1])
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                        current->clasificados[0]->winnerT = true;
                    }
                    else
                    {
                        cout << "GOLES: \n"
                             << current->clasificados[0]->nombre << ": " << golesA << "\n"
                             << current->clasificados[1]->nombre << " : " << golesB << endl;
                        cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                        current->clasificados[1]->winnerT = true;
                    }
                    break;
                }
                else
                {
                    cout << "LOS EQUIPOS REALIZARON LA MISMA CANTIDAD DE ANOTACIONES EN LOS PENALES" << endl;
                    cout << "POR ENDE SE VUELVE A REALIZAR OTRA TANDA DE PENALES" << endl;
                }
            } while (penalesAnotados[0] == penalesAnotados[1]);
        }
        cout << "----------------------------------------------------------------------------------------------------------------" << endl;
        current->clasificados[0]->goleFavor.push_back(golesA);
        current->clasificados[1]->goleFavor.push_back(golesB);
        current = current->next;
    } while (current != nullptr);
}
// POR PROBABILIDAD
void faseGruposProb()
{
    Team *current = head;
    vector<int> v;
    int numeroAleatorio, golesA, golesB, a = 0;
    double probPaisA, probPaisB, empate, chance;
    do
    {
        cout << "----------------------------" << current->group << "----------------------------" << endl;
        for (int i = 0; i < current->countries.size(); i++)
        {
            for (int j = 1; j < current->countries.size(); j++)
            {
                if (j + a >= current->countries.size())
                {
                    break;
                }
                cout << current->pais[i]->nombre << " vs " << current->pais[j + a]->nombre << endl;
                cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
                headEstadio = headEstadio->next;

                while (true)
                {

                    float probPaisA, probPaisB;
                    cout << "Ingrese la probabilidad de que " << current->pais[i]->nombre << " gane el partido" << endl;
                    cin >> probPaisA;
                    probPaisA /= 100;

                    cout << "Ingrese la probabilidad de que " << current->pais[j + a]->nombre << " gane el partido" << endl;
                    cin >> probPaisB;
                    probPaisB = (probPaisB / 100) + probPaisA;

                    float ram = floatRandom();
                    if (((probPaisA + (probPaisB - probPaisA)) <= 1.0))
                    {
                        if ((probPaisA == 0.0 && probPaisB == 0.0))
                        {
                            cout << "AMBOS PAISES NO PUEDEN TENER 0 PROBABILIDAD DE VICTORIA" << endl;
                        }
                        else
                        {
                            if (ram <= probPaisA)
                            {
                                numeroAleatorio = 1;
                            }
                            else if (ram <= probPaisB)
                            {
                                numeroAleatorio = 0;
                            }
                            else
                            {
                                numeroAleatorio = 2;
                            }
                            break;
                        }
                    }
                    else
                    {
                        cout << "LA SUMA DE AMBAS PROBABILIDADES SUPERA EL 100%, O UNA DE LOS PAISES " << endl;
                    }
                }

                switch (numeroAleatorio)
                {
                case 0:
                    do
                    {
                        v = goles();
                    } while (v[0] >= v[1]);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << v[0] << "\n"
                         << current->pais[j + a]->nombre << " : " << v[1] << endl;
                    cout << current->pais[i]->nombre << " PERDIO" << endl;

                    // ACTULIZAR INFORMACION
                    current->pais[i]->perdidos[0] += 1;
                    current->pais[j + a]->ganados[0] += 1;
                    break;
                case 1:

                    do
                    {
                        v = goles();
                    } while (v[0] <= v[1]);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << v[0] << "\n"
                         << current->pais[j + a]->nombre << " : " << v[1] << endl;
                    cout << current->pais[i]->nombre << " GANÓ" << endl;

                    //
                    current->pais[i]->ganados[0] += 1;
                    current->pais[j + a]->perdidos[0] += 1;
                    break;
                case 2:
                    do
                    {
                        v = goles();
                    } while (v[0] != v[1]);

                    cout << "GOLES: \n"
                         << current->pais[i]->nombre << ": " << v[0] << "\n"
                         << current->pais[j + a]->nombre << " : " << v[1] << endl;
                    cout << current->pais[i]->nombre << " Y " << current->pais[j + a]->nombre << " EMPATARON" << endl;

                    current->pais[i]->empatados[0] += 1;
                    current->pais[j + a]->empatados[0] += 1;
                    break;
                }
                cout << "----------------------------------------------------------------------------------------------------------------" << endl;
                current->pais[i]->goleFavor[0] += v[0];
                current->pais[i]->golesContra[0] += v[1];
                current->pais[j + a]->golesContra[0] += v[0];
                current->pais[j + a]->goleFavor[0] += v[1];
            }

            a += 1;
        }
        current = current->next;
        a = 0;
    } while (current != nullptr);
    calcularPuntos();
    calcularPosicion();
}
void octavosProb()
{
    vector<pais *> paisesOctavos;
    vector<pais *> aux_paisesOctavos;
    Team *currentT = head;
    Team *aux = currentT->next;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        aux = currentT->next;
        for (int i = 0; i < nClasificados; i = i + 2)
        {
            paisesOctavos.push_back(currentT->posicionClasificados[i]);
            paisesOctavos.push_back(aux->posicionClasificados[i]);
            //
            aux_paisesOctavos.push_back(currentT->posicionClasificados[i + 1]);
            aux_paisesOctavos.push_back(aux->posicionClasificados[i + 1]);
        }
        currentT = currentT->next->next;
        createOctavos(num, paisesOctavos);
        createOctavos(num + 1, aux_paisesOctavos);
        num += 2;
        paisesOctavos.clear();
        aux_paisesOctavos.clear();
    } while (currentT != nullptr);
    // CALCULAR Y ASÍ
    octavosF *current = headO;
    vector<int> v;

    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        while (true)
        {
            float probPaisA, probPaisB;
            cout << "Ingrese la probabilidad de que " << current->clasificados[0]->nombre << " gane el partido:" << endl;
            cin >> probPaisA;
            probPaisA /= 100;

            cout << "Ingrese la probabilidad de que " << current->clasificados[1]->nombre << " gane el partido: " << endl;
            cin >> probPaisB;
            probPaisB = (probPaisB / 100) + probPaisA;

            float ram = floatRandom();
            if (((probPaisA + (probPaisB - probPaisA)) <= 1.0))
            {
                if ((probPaisA == 0.0 && probPaisB == 0.0))
                {
                    cout << "AMBOS PAISES NO PUEDEN TENER 0 PROBABILIDAD DE VICTORIA" << endl;
                }
                else
                {
                    if (ram <= probPaisA)
                    {
                        numeroAleatorio = 1;
                    }
                    else if (ram <= probPaisB)
                    {
                        numeroAleatorio = 0;
                    }
                    else
                    {
                        numeroAleatorio = 2;
                    }
                    break;
                }
            }
            else
            {
                cout << "LA SUMA DE AMBAS PROBABILIDADES SUPERA EL 100%, O UNA DE LOS PAISES " << endl;
            }
        }
        switch (numeroAleatorio)
        {
        case 0:
            do
            {
                v = goles();
            } while (v[0] >= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;

            // ACTULIZAR INFORMACION
            current->clasificados[1]->octavosClas = true;
            break;
        case 1:

            do
            {
                v = goles();
            } while (v[0] <= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            //
            current->clasificados[0]->octavosClas = true;
            break;
        // PROLOGA
        case 2:

            do
            {
                v = goles();
            } while (v[0] == v[1]);

            if (v[0] > v[1])
            {
                cout << "GOLES: \n"
                     << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                     << current->clasificados[1]->nombre << " : " << v[1] << endl;
                cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                current->clasificados[0]->octavosClas = true;
            }
            else
            {
                cout << "GOLES: \n"
                     << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                     << current->clasificados[1]->nombre << " : " << v[1] << endl;
                cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                current->clasificados[1]->octavosClas = true;
            }
        }
        current->clasificados[0]->goleFavor.push_back(v[0]);
        current->clasificados[1]->goleFavor.push_back(v[1]);
        current = current->next;
    } while (current != nullptr);
}
void cuartosProb()
{
    vector<pais *> paisesCuartos;
    octavosF *currentT = headO;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->octavosClas == true)
            {
                paisesCuartos.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->octavosClas == true)
            {
                paisesCuartos.push_back(currentT->clasificados[i]);
            }
        }
        createCuartos(num, paisesCuartos);
        num += 1;
        paisesCuartos.clear();
        currentT = currentT->next;
    } while (currentT != nullptr);
    cuartosF *current = headC;
    vector<int> v;
    do
    {
        /*
        cout << "---------------------------- GRUPO: " << current->grupo << "----------------------------" << endl;*/
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        while (true)
        {
            float probPaisA, probPaisB;
            cout << "Ingrese la probabilidad de que " << current->clasificados[0]->nombre << " gane el partido:" << endl;
            cin >> probPaisA;
            probPaisA /= 100;

            cout << "Ingrese la probabilidad de que " << current->clasificados[1]->nombre << " gane el partido: " << endl;
            cin >> probPaisB;
            probPaisB = (probPaisB / 100) + probPaisA;

            float ram = floatRandom();
            if (((probPaisA + (probPaisB - probPaisA)) <= 1.0))
            {
                if ((probPaisA == 0.0 && probPaisB == 0.0))
                {
                    cout << "AMBOS PAISES NO PUEDEN TENER 0 PROBABILIDAD DE VICTORIA" << endl;
                }
                else
                {
                    if (ram <= probPaisA)
                    {
                        numeroAleatorio = 1;
                    }
                    else if (ram <= probPaisB)
                    {
                        numeroAleatorio = 0;
                    }
                    else
                    {
                        numeroAleatorio = 2;
                    }
                    break;
                }
            }
            else
            {
                cout << "LA SUMA DE AMBAS PROBABILIDADES SUPERA EL 100%, O UNA DE LOS PAISES " << endl;
            }
        }
        switch (numeroAleatorio)
        {
        case 0:
            do
            {
                v = goles();
            } while (v[0] >= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;

            // ACTULIZAR INFORMACION
            current->clasificados[1]->cuartosClas = true;
            break;
        case 1:

            do
            {
                v = goles();
            } while (v[0] <= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            //
            current->clasificados[0]->cuartosClas = true;
            break;
        case 2:

            do
            {
                v = goles();
            } while (v[0] == v[1]);

            if (v[0] > v[1])
            {
                cout << "GOLES: \n"
                     << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                     << current->clasificados[1]->nombre << " : " << v[1] << endl;
                cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                current->clasificados[0]->cuartosClas = true;
            }
            else
            {
                cout << "GOLES: \n"
                     << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                     << current->clasificados[1]->nombre << " : " << v[1] << endl;
                cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                current->clasificados[1]->cuartosClas = true;
            }
        }
        current->clasificados[0]->goleFavor.push_back(v[0]);
        current->clasificados[1]->goleFavor.push_back(v[1]);
        current = current->next;
        // cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    } while (current != nullptr);
}
void semifinalProb()
{
    vector<pais *> paisesSemifinal;
    cuartosF *currentT = headC;
    int num = 1, numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->cuartosClas == true)
            {
                paisesSemifinal.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->cuartosClas == true)
            {
                paisesSemifinal.push_back(currentT->clasificados[i]);
            }
        }
        createSemifinal(num, paisesSemifinal);
        num += 1;
        paisesSemifinal.clear();
        currentT = currentT->next;
    } while (currentT != nullptr);
    semifinal_ *current = headSF;
    vector<int> v;
    do
    {
        cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
        cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
        headEstadio = headEstadio->next;
        while (true)
        {
            float probPaisA, probPaisB;
            cout << "Ingrese la probabilidad de que " << current->clasificados[0]->nombre << " gane el partido:" << endl;
            cin >> probPaisA;
            probPaisA /= 100;

            cout << "Ingrese la probabilidad de que " << current->clasificados[1]->nombre << " gane el partido: " << endl;
            cin >> probPaisB;
            probPaisB = (probPaisB / 100) + probPaisA;

            float ram = floatRandom();
            if (((probPaisA + (probPaisB - probPaisA)) <= 1.0))
            {
                if ((probPaisA == 0.0 && probPaisB == 0.0))
                {
                    cout << "AMBOS PAISES NO PUEDEN TENER 0 PROBABILIDAD DE VICTORIA" << endl;
                }
                else
                {
                    if (ram <= probPaisA)
                    {
                        numeroAleatorio = 1;
                    }
                    else if (ram <= probPaisB)
                    {
                        numeroAleatorio = 0;
                    }
                    else
                    {
                        numeroAleatorio = 2;
                    }
                    break;
                }
            }
            else
            {
                cout << "LA SUMA DE AMBAS PROBABILIDADES SUPERA EL 100%, O UNA DE LOS PAISES " << endl;
            }
        }
        switch (numeroAleatorio)
        {
        case 0:
            do
            {
                v = goles();
            } while (v[0] >= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;

            // ACTULIZAR INFORMACION
            current->clasificados[1]->semifinalClas = true;
            break;
        case 1:

            do
            {
                v = goles();
            } while (v[0] <= v[1]);

            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;

            //
            current->clasificados[0]->semifinalClas = true;
            break;
        case 2:

            do
            {
                v = goles();
            } while (v[0] == v[1]);

            if (v[0] > v[1])
            {
                cout << "GOLES: \n"
                     << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                     << current->clasificados[1]->nombre << " : " << v[1] << endl;
                cout << current->clasificados[0]->nombre << " GANÓ" << endl;
                current->clasificados[0]->semifinalClas = true;
            }
            else
            {
                cout << "GOLES: \n"
                     << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                     << current->clasificados[1]->nombre << " : " << v[1] << endl;
                cout << current->clasificados[0]->nombre << " PERDIO" << endl;
                current->clasificados[1]->semifinalClas = true;
            }
        }
        current->clasificados[0]->goleFavor.push_back(v[0]);
        current->clasificados[1]->goleFavor.push_back(v[1]);
        current = current->next;
        // cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    } while (current != nullptr);
}
void partidoFinalProb()
{
    vector<pais *> paisesFinal;
    semifinal_ *currentT = headSF;
    int numeroAleatorio, a = 0;
    do
    {
        for (int i = 0; i < currentT->clasificados.size(); i++)
        {
            if (currentT->clasificados[i]->semifinalClas == true)
            {
                paisesFinal.push_back(currentT->clasificados[i]);
            }
        }
        currentT = currentT->next;
    } while (currentT != nullptr);
    createFinal(paisesFinal);
    finalP *current = headFP;
    vector<int> v;
    cout << current->clasificados[0]->nombre << " vs " << current->clasificados[1]->nombre << endl;
    cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
    headEstadio = headEstadio->next;
    while (true)
    {

        float probPaisA, probPaisB;
        cout << "Ingrese la probabilidad de que " << current->clasificados[0]->nombre << " gane el partido:" << endl;
        cin >> probPaisA;
        probPaisA /= 100;

        cout << "Ingrese la probabilidad de que " << current->clasificados[1]->nombre << " gane el partido: " << endl;
        cin >> probPaisB;
        probPaisB = (probPaisB / 100) + probPaisA;

        float ram = floatRandom();
        if ((probPaisA + (probPaisB - probPaisA)) <= 1.0)
        {
            if (ram <= probPaisA)
            {
                cout << "GANA A" << endl;
            }
            else if (ram <= probPaisB)
            {
                cout << "GANA B" << endl;
            }
            else
            {
                cout << "EMPATE" << endl;
            }
            break;
        }
        else
        {
            cout << "LA SUMA DE AMBAS PROBABILIDADES SUPERA EL 100%" << endl;
        }
    }
    switch (numeroAleatorio)
    {
    case 0:
        do
        {
            v = goles();
        } while (v[0] >= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " PERDIO" << endl;

        // ACTULIZAR INFORMACION
        current->clasificados[1]->winner = true;
        break;
    case 1:

        do
        {
            v = goles();
        } while (v[0] <= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " GANÓ" << endl;

        // ACTULIZAR INFORMACION
        current->clasificados[0]->winner = true;
        break;
    case 2:

        do
        {
            v = goles();
        } while (v[0] == v[1]);

        if (v[0] > v[1])
        {
            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;
            current->clasificados[0]->winner = true;
        }
        else
        {
            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            current->clasificados[1]->winner = true;
        }
    }
    current->clasificados[0]->goleFavor.push_back(v[0]);
    current->clasificados[1]->goleFavor.push_back(v[1]);
    // cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
}
void tercerPuestoProb()
{
    vector<pais *> paisesFinal;
    semifinal_ *current = headSF;
    int numeroAleatorio, a = 0;
    vector<int> v;
    do
    {
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            if (current->clasificados[i]->semifinalClas == false)
            {
                paisesFinal.push_back(current->clasificados[i]);
            }
        }
        current = current->next;
    } while (current != nullptr);
    //
    cout << paisesFinal[0]->nombre << " vs " << paisesFinal[1]->nombre << endl;
    cout << "PARTIDO JUGADO EN EL " << headEstadio->nombre << endl;
    headEstadio = headEstadio->next;
    while (true)
    {

        float probPaisA, probPaisB;
        cout << "Ingrese la probabilidad de que " << current->clasificados[0]->nombre << " gane el partido:" << endl;
        cin >> probPaisA;
        probPaisA /= 100;

        cout << "Ingrese la probabilidad de que " << current->clasificados[1]->nombre << " gane el partido: " << endl;
        cin >> probPaisB;
        probPaisB = (probPaisB / 100) + probPaisA;

        float ram = floatRandom();
        if ((probPaisA + (probPaisB - probPaisA)) <= 1.0)
        {
            if (ram <= probPaisA)
            {
                cout << "GANA A" << endl;
            }
            else if (ram <= probPaisB)
            {
                cout << "GANA B" << endl;
            }
            else
            {
                cout << "EMPATE" << endl;
            }
            break;
        }
        else
        {
            cout << "LA SUMA DE AMBAS PROBABILIDADES SUPERA EL 100%" << endl;
        }
    }
    current = headSF;
    switch (numeroAleatorio)
    {
    case 0:
        do
        {
            v = goles();
        } while (v[0] >= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " PERDIO" << endl;

        // ACTULIZAR INFORMACION
        paisesFinal[1]->winnerT = true;
        break;
    case 1:

        do
        {
            v = goles();
        } while (v[0] <= v[1]);

        cout << "GOLES: \n"
             << current->clasificados[0]->nombre << ": " << v[0] << "\n"
             << current->clasificados[1]->nombre << " : " << v[1] << endl;
        cout << current->clasificados[0]->nombre << " GANÓ" << endl;

        // ACTULIZAR INFORMACION
        paisesFinal[0]->winnerT = true;
        break;
    case 2:

        do
        {
            v = goles();
        } while (v[0] == v[1]);

        if (v[0] > v[1])
        {
            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " GANÓ" << endl;
            current->clasificados[0]->winnerT = true;
        }
        else
        {
            cout << "GOLES: \n"
                 << current->clasificados[0]->nombre << ": " << v[0] << "\n"
                 << current->clasificados[1]->nombre << " : " << v[1] << endl;
            cout << current->clasificados[0]->nombre << " PERDIO" << endl;
            current->clasificados[1]->winnerT = true;
        }
    }
    paisesFinal[0]->goleFavor.push_back(v[0]);
    paisesFinal[1]->goleFavor.push_back(v[1]);
}

// MOSTRAR ALGO
void mostrarP(pais *&headP)
{
    cout << endl;
    pais *current = headP;
    cout << "INFORMACION DE LOS PARTIDOS: " << endl;
    do
    {
        cout << current->nombre << " " << endl;
        cout << "GANADOS: " << current->ganados[0] << " " << endl;
        cout << "PERDIDOS: " << current->perdidos[0] << " " << endl;
        cout << "EMPATADOS: " << current->empatados[0] << " " << endl;
        cout << "GOLES A FAVOR: " << current->goleFavor[0] << " " << endl;
        cout << "GOLES EN CONTRA: " << current->golesContra[0] << " " << endl;
        cout << "TOTAL DE PUNTOS : " << current->totalPuntos[0] << " " << endl;
        current = current->next;
        cout << endl;
    } while (current != nullptr);
}
void mostrar(Team *&head)
{
    Team *current = head;

    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        cout << current->group << endl;
        for (int i = 0; i < current->pais.size(); i++)
        {
            cout << current->pais[i]->nombre << endl;
            cout << "I: " << i << endl;
        }

        current = current->next;
    } while (current != nullptr);

    cout << "\n";
}
void mostrarTeam()
{
    Team *current = head;
    do
    {
        cout << current->group << endl;
        for (int i = 0; i < current->countries.size(); i++)
        {
            cout << current->countries[i] << " ";
        }
        cout << endl;
        current = current->next;
    } while (current != nullptr);
}
void mostrarTabla()
{
    Team *current = head;
    do
    {

        cout << "----------------------------------" << current->group << "----------------------------------" << endl;
        //       10               18                 10   4   4   4      10    5     5    5
        cout << "| Puesto  | Equipo           | Jugados | G | E | P | Puntos | GF | GC | GD |" << endl;
        for (int i = 0; i < current->posicionClasificados.size(); i++)
        {
            cout << "|    " << i + 1 << "    |" << current->posicionClasificados[i]->nombre;
            //              12
            for (int j = nCaracteres(current->posicionClasificados[i]->nombre); j < 18; j++)
            {
                cout << " ";
            }
            cout << "|    " << current->posicionClasificados[i]->totalPartidos[0] << "    | " << current->posicionClasificados[i]->ganados[0] << " | " << current->posicionClasificados[i]->empatados[0] << " | " << current->posicionClasificados[i]->perdidos[0] << " |   " << current->posicionClasificados[i]->totalPuntos[0] << "    |";
            // 3
            if (current->posicionClasificados[i]->goleFavor[0] >= 10)
            {
                cout << " " << current->posicionClasificados[i]->goleFavor[0] << " |";
            }
            else
            {
                cout << " " << current->posicionClasificados[i]->goleFavor[0] << "  |";
            }

            if (current->posicionClasificados[i]->golesContra[0] >= 10)
            {
                cout << " " << current->posicionClasificados[i]->golesContra[0] << " |";
            }
            else
            {
                cout << " " << current->posicionClasificados[i]->golesContra[0] << "  |";
            }

            if (current->posicionClasificados[i]->golesDiferencia[0] >= 10)
            {
                cout << " " << current->posicionClasificados[i]->golesDiferencia[0] << " |" << endl;
            }
            else
            {
                cout << " " << current->posicionClasificados[i]->golesDiferencia[0] << "  |" << endl;
            }
        }
        current = current->next;
    } while (current != nullptr);
}
void clasi()
{
    Team *current = head;
    do
    {
        for (int i = 0; i < current->posicionClasificados.size() / 2; i++)
        {
            cout << "POS: " << current->posicionClasificados[i]->nombre << endl;
        }

        current = current->next;
    } while (current != nullptr);
}
void mostrarOctavos()
{
    octavosF *current = headO;
    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        cout << "----------------------------- Grupo " << current->grupo << endl;
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            cout << " *" << current->clasificados[i]->nombre << endl;
            cout << "Goles que hizo: " << current->clasificados[i]->goleFavor[1] << endl;
            string clasi = current->clasificados[i]->octavosClas ? "PASÓ" : "NO PASÓ";
            cout << "Pasó octavos: " << clasi << endl;
        }

        current = current->next;
    } while (current != nullptr);
}
void mostrarCuartos()
{
    cuartosF *current = headC;
    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        cout << "----------------------------- Grupo " << current->grupo << " en cuartos" << endl;
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            cout << current->clasificados[i]->nombre << endl;
            cout << "Goles que hizo: " << current->clasificados[i]->goleFavor[2] << endl;
            string clasi = current->clasificados[i]->cuartosClas ? "PASÓ" : "NO PASÓ";
            cout << "Pasó cuartos: " << clasi << endl;
        }
        current = current->next;
    } while (current != nullptr);
}
void mostrarSemiFinal()
{
    semifinal_ *current = headSF;
    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        cout << "----------------------------- Grupo " << current->grupo << " en la SemiFinal" << endl;
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            cout << current->clasificados[i]->nombre << endl;
            cout << "Goles que hizo: " << current->clasificados[i]->goleFavor[3] << endl;
            string clasi = current->clasificados[i]->semifinalClas ? "PASÓ" : "NO PASÓ";
            cout << "Pasó semifinales: " << clasi << endl;
        }
        current = current->next;
    } while (current != nullptr);
}
void mostrarFinal()
{
    finalP *current = headFP;
    if (headFP == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            cout << current->clasificados[i]->nombre << endl;
            cout << "Goles que hizo: " << current->clasificados[i]->goleFavor[4] << endl;
            string clasi = current->clasificados[i]->winner ? "GANÓ" : "NO GANÓ";
            cout << clasi << " LA FINAL" << endl;
        }
        current = current->next;
    } while (current != nullptr);
}
void mostrarTercerpuesto()
{
    semifinal_ *current = headSF;
    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        for (int i = 0; i < current->clasificados.size(); i++)
        {
            if (current->clasificados[i]->semifinalClas == false)
            {
                cout << current->clasificados[i]->nombre << endl;
                cout << "Goles que hizo: " << current->clasificados[i]->goleFavor[4] << endl;
                string clasi = current->clasificados[i]->winnerT ? "GANÓ" : "NO GANÓ";
                cout << clasi << " EL TERCER PUESTO" << endl;
            }
        }

        current = current->next;
    } while (current != nullptr);
}
//
void menu()
{

    if (read())
    {
        int choise;
        cout << "OPCIONES: " << endl;
        cout << "1: Simular todos los partidos, desde fase de grupos hasta la final (TODO POR PROBABILIDAD)." << endl;
        cout << "2: Ingresar los resultados " << endl;
        cout << "3: Por probabilidad" << endl;
        cout << "Opcion: ";
        cin >> choise;

        switch (choise)
        {
        case 1:
            cout << "-------------------------------  FASE DE GRUPO -------------------------------" << endl;
            faseGrupos(head);
            mostrarTabla();
            cout << "------------------------------- OCTAVOS DE FINAL -------------------------------" << endl;
            octavos();
            mostrarOctavos();
            cout << "------------------------------- CUARTOS DE FINAL -------------------------------" << endl;
            cuartos();
            mostrarCuartos();
            cout << "------------------------------- SEMI-FINAL -------------------------------" << endl;
            semifinal();
            mostrarSemiFinal();
            cout << "------------------------------- FINAL -------------------------------" << endl;
            partidoFinal();
            mostrarFinal();
            cout << "------------------------------- TERCER PUESTO -------------------------------" << endl;
            tercerPuesto();
            mostrarTercerpuesto();
            break;
        case 2:
            cout << "-------------------------------  FASE DE GRUPO -------------------------------" << endl;
            faseGruposIn();
            mostrarTabla();
            cout << "------------------------------- OCTAVOS DE FINAL -------------------------------" << endl;
            octavosIn();
            mostrarOctavos();
            cout << "------------------------------- CUARTOS DE FINAL -------------------------------" << endl;
            cuartosIn();
            mostrarCuartos();
            cout << "------------------------------- SEMI-FINAL -------------------------------" << endl;
            semifinalIn();
            mostrarSemiFinal();
            cout << "------------------------------- FINAL -------------------------------" << endl;
            partidoFinalIn();
            mostrarFinal();
            cout << "------------------------------- TERCER PUESTO -------------------------------" << endl;
            tercerPuestoIn();
            mostrarTercerpuesto();
            break;
        case 3:
            cout << "-------------------------------  FASE DE GRUPO -------------------------------" << endl;
            faseGruposProb();
            mostrarTabla();
            cout << "------------------------------- OCTAVOS DE FINAL -------------------------------" << endl;
            octavosProb();
            mostrarOctavos();
            cout << "------------------------------- CUARTOS DE FINAL -------------------------------" << endl;
            cuartosProb();
            mostrarCuartos();
            cout << "------------------------------- SEMI-FINAL -------------------------------" << endl;
            semifinalProb();
            mostrarSemiFinal();
            cout << "------------------------------- FINAL -------------------------------" << endl;
            partidoFinalProb();
            mostrarFinal();
            cout << "------------------------------- TERCER PUESTO -------------------------------" << endl;
            tercerPuestoProb();
            mostrarTercerpuesto();
            break;
        }

        system("PAUSE");
        system("cls");
        head = nullptr;
        headP = nullptr;
        headO = nullptr;
        headC = nullptr;
        headSF = nullptr;
        headFP = nullptr;
        headEstadio = nullptr;
    }
    else
    {
        cout << "ERROR AL LEER EL ARCHIVO TXT CON LOS GRUPOS. \n"
             << "REVISE EL MANUAL DE USUARIO.";
    }
}
int main()
{
    srand(time(0));
    menu();
}