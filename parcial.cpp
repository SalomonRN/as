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
    int ganados;
    int empatados;
    int perdidos;
    int totalPartidos;
    int goleFavor;
    int golesContra;
    int golesDiferencia;
    int totalPuntos;
    bool final_ = false;
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
Team *head = nullptr;
pais *headP = nullptr;
int nClasificados = 2;
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
int nCaracteres(string str)
{
    // Convertir la cadena a wide string
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring wstr = converter.from_bytes(str);
    return wstr.size();
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
void mostrarP(pais *&headP)
{
    cout << endl;
    pais *current = headP;
    cout << "INFORMACION DE LOS PARTIDOS: " << endl;
    do
    {
        cout << current->nombre << " " << endl;
        cout << "GANADOS: " << current->ganados << " " << endl;
        cout << "PERDIDOS: " << current->perdidos << " " << endl;
        cout << "EMPATADOS: " << current->empatados << " " << endl;
        cout << "GOLES A FAVOR: " << current->goleFavor << " " << endl;
        cout << "GOLES EN CONTRA: " << current->golesContra << " " << endl;
        cout << "TOTAL DE PUNTOS : " << current->totalPuntos << " " << endl;
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
bool read()
{
    ifstream file;
    string group;
    string countries;
    file.open("txt.txt", ios::in);
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
    return true;
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
    int f = s[0]->golesDiferencia;
    for (int i = 1; i < s.size(); i++)
    {
        if (f == s[i]->golesDiferencia)
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
vector<int> goles()
{
    vector<int> v;
    v.push_back(rand() % 7);
    v.push_back(rand() % 7);
    return v;
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
        int f = s[0]->totalPuntos;
        for (int i = 0; i < nClasificados; i++)
        {
            for (int j = 1; j <= nClasificados; j++)
            {
                if (s[i]->totalPuntos == s[i + j]->totalPuntos)
                {
                    nRepetidasPts += 1;
                }
            }
            cout << "nRepetidasPts ES: " << nRepetidasPts << current->group << "................." << endl;
            if (nRepetidasPts >= nClasificados)
            {
                cout << "PUNTOS SIMILARES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
                s = orderByGolesDiferencia(s);
            }
            // current = current->next;
        }
        // SI EL PRIMER PUESTO TIENE -2 GOLES, PERO AUN
        // EN EL CASO QUE TODOS LOS PAISES TENGAN LOS MISMOS PUNTOS
        if (nRepetidasPts >= nClasificados)
        {
            cout << "PUNTOS SIMILARES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            s = orderByGolesDiferencia(s);
        }
        current->posicionClasificados = s;
        s.clear();
        current = current->next;

    } while (current != nullptr);
}
void calcularPuntos()
{
    // GANADOS = 3pts
    // EMPATADOS = 1pts
    // PERDIDOS = 0pts
    pais *current = headP;
    do
    {
        current->totalPuntos = (current->ganados * 3) + (current->empatados);
        current->totalPartidos = current->ganados + current->perdidos + current->empatados;
        current->golesDiferencia = current->goleFavor - current->golesContra;
        current = current->next;
    } while (current != nullptr);
}
void simularTodo(Team *&head)
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
                    current->pais[i]->perdidos += 1;
                    current->pais[j + a]->ganados += 1;
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
                    current->pais[i]->ganados += 1;
                    current->pais[j + a]->perdidos += 1;
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
                    current->pais[i]->empatados += 1;
                    current->pais[j + a]->empatados += 1;
                    break;
                }
                current->pais[i]->goleFavor += v[0];
                current->pais[i]->golesContra += v[1];
                current->pais[j + a]->golesContra += v[0];
                current->pais[j + a]->goleFavor += v[1];

                cout << "----------------------------------------------------------------------------------------------------------" << endl;
            }

            a += 1;
        }
        current = current->next;
        a = 0;
        cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    } while (current != nullptr);
    calcularPuntos();
}
void cuartosF()
{
    vector<pais *> grupoCuartos;
    Team *current = head;
    do
    {
        for (int i = 0; i < nClasificados; i++)
        {
            grupoCuartos.push_back(current->posicionClasificados[i]);
        }
        cout << "CLASIFICADOS: " << endl;
        for (int i = 0; i < nClasificados; i++)
        {
            cout << grupoCuartos[i]->nombre << endl;
        }
        current = current->next;
    } while (current != nullptr);
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
void mostrarPunteros()
{
    Team *current = head;
    do
    {
        cout << current->group << endl;
        for (int i = 0; i < current->pais.size(); i++)
        {
            cout << current->pais[i]->nombre << endl;
        }
        current = current->next;
        cout << endl;
    } while (current != nullptr);
}
void mostrarTabla()
{
    Team *current = head;
    current->posicionClasificados[1]->nombre;
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
            cout << "|    " << current->posicionClasificados[i]->totalPartidos << "    | " << current->posicionClasificados[i]->ganados << " | " << current->posicionClasificados[i]->empatados << " | " << current->posicionClasificados[i]->perdidos << " |   " << current->posicionClasificados[i]->totalPuntos << "    |";
            // 3
            if (current->posicionClasificados[i]->goleFavor >= 10)
            {
                cout << " " << current->posicionClasificados[i]->goleFavor << " |";
            }
            else
            {
                cout << " " << current->posicionClasificados[i]->goleFavor << "  |";
            }

            if (current->posicionClasificados[i]->golesContra >= 10)
            {
                cout << " " << current->posicionClasificados[i]->golesContra << " |";
            }
            else
            {
                cout << " " << current->posicionClasificados[i]->golesContra << "  |";
            }

            if (current->posicionClasificados[i]->golesDiferencia >= 10)
            {
                cout << " " << current->posicionClasificados[i]->golesDiferencia << " |" << endl;
            }
            else
            {
                cout << " " << current->posicionClasificados[i]->golesDiferencia << "  |" << endl;
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
int main()
{
    srand(time(0));
    if (read())
    {
        int choise = 1;
        cout << "OPCIONES: " << endl;
        cout << "1: Simular todos los partidos, desde fase de grupos hasta la final (TODO POR PROBABILIDAD)." << endl;
        // cin >> choise;

        switch (choise)
        {
        case 1:
            // mostrar(head);
            // mostrarP(headP);
            // mostrarPunteros();
            simularTodo(head);
            // mostrarP(headP);
            calcularPosicion();
            mostrarTabla();

            cuartosF();

            // clasi();
            break;

        default:
            break;
        }
        // mostrar(head);
    }
    else
    {
        cout << "ERROR AL LEER EL ARCHIVO TXT CON LOS GRUPOS. \n"
             << "REVISE EL MANUAL DE USUARIO.";
        return 0;
    }
}