#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct Team
{
    Team *next;
    string group;
    string countrie;
    vector<string> headData; // DEJAR FIJO
    vector<int> data;
};
struct Table
{
    Table *next;
    string table;
};

Team *head = nullptr;
vector<string> split(string txt);
void byOne(string countrie, string group);
void create(Team *&head, string group, string countrie);
void mostrar(Team *&head);
bool read();
vector<int> goles();
void simularTodo(Team *&head);
vector<int> contador(Team *&head);

vector<int> contador(Team *&head)
{
    vector<int> result, null;
    int n = 1, t = 0;
    Team *current = head;
    Team *aux = current->next;
    if (head == NULL)
    {
        cout << "NADA QUE CONTAR" << endl;
        return null;
    }
    do
    {
        n += 1;
        current = current->next;
        aux = aux->next;
    } while (current->group == aux->group);
    result.push_back(n);
    current = head;
    do
    {
        t += 1;
        current = current->next;

    } while (current != nullptr);
    result.push_back(t);
    return result;
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
void byOne(string countrie, string group)
{
    vector<string> results = split(countrie);
    for (int i = 0; i < results.size(); i++)
    {
        create(head, group, results[i]);
    }
}
void create(Team *&head, string group, string countrie)
{
    Team *new_node = new Team();
    new_node->group = group;
    new_node->countrie = countrie;
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
void mostrar(Team *&head)
{
    Team *current = head;
    Team *aux = current->next;
    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {

        cout << current->group << endl;
        do
        {
            cout << current->countrie << endl;
            current = current->next;
            if (aux->next != nullptr)
            {
                aux = aux->next;
            }

        } while (current->group == aux->group && current->next != nullptr);
        cout << current->countrie << endl;
        current = current->next;
        aux = aux->next;

    } while (current != nullptr);
}
bool read()
{
    ifstream file;
    string group;
    string countrie;
    file.open("txt.txt", ios::in);
    if (file.fail())
    {
        return false;
    }

    while (!file.eof())
    {
        getline(file, group);
        getline(file, countrie);
        byOne(countrie, group);
    }
    return true;
}
vector<int> goles()
{
    vector<int> v;
    v.push_back(rand() % 7);
    v.push_back(rand() % 7);
    return v;
}
void simularTodo(Team *&head)
{
    Team *current = head;
    vector<int> v;
    int numeroAleatorio, golesA, golesB, a = 0, j = 0;
    vector<int> cont = contador(head);
    cout << "PAISES POR EQUIPOS: " << cont[0] << " TOTAL DE paises: " << cont[1] << endl;
    do
    {
        cout << current->group << endl;
        vector<int> cont = contador(head);
        for (int i = 0; i < cont[0]; i++)
        
        
        {
            switch (numeroAleatorio)
            {
            case 0:
                do
                {
                    v = goles();
                } while (v[0] >= v[1]);

                cout << "GOLES: " << current->countrie[i] << ": " << v[0] << "\n"
                     << current->countrie[j + a] << " : " << v[1] << endl;
                cout << current->countrie[i] << " PERDIO" << endl;
                break;
            case 1:

                do
                {
                    v = goles();
                } while (v[0] <= v[1]);

                cout << "GOLES: " << current->countrie[i] << ": " << v[0] << "\n"
                     << current->countrie[j + a] << " : " << v[1] << endl;
                cout << current->countrie[i] << " GANÓ" << endl;
                break;
            case 2:
                do
                {
                    v = goles();
                } while (v[0] != v[1]);

                cout << "GOLES: " << current->countrie[i] << ": " << v[0] << "\n"
                     << current->countrie[j + a] << " : " << v[1] << endl;
                cout << current->countrie[i] << " Y " << current->countrie[j + a] << " EMPATARON" << endl;
                break;

                cout << "----------------------------------------------------------------------------------------------------------" << endl;
            }

            a += 1;
        }
        current = current->next;
        a = 0;
        cout << "///////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    } while (current != nullptr);
}

int main()
{
    srand(time(0));
    if (read())
    {
        int choise;
        cout << "OPCIONES: " << endl;
        cout << "1: Simular todos los partidos, desde fase de grupos hasta la final (TODO POR PROBABILIDAD)." << endl;
        cin >> choise;

        switch (choise)
        {
        case 1:
            simularTodo(head);
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