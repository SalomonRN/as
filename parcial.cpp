#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

struct Team
{
    Team *next;
    string group;
    string countrie;
    vector<string> headData;
    vector<int> data;
};
struct Table
{
    Table *next;
    string table;
};

Team *head = nullptr;

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
void byOne(string countries, string group)
{
    vector<string> results = split(countries);
    for (int i = 0; i < results.size(); i++)
    {
        create(head, group, results[i]);
    }
}
void create(Team *&head, string group, string countrie)
{
    Team *new_node = new Team();
    new_node->group = group;
    snew_node->countrie = countrie;
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
    if (head == NULL)
    {
        cout << "NADA QUE MOSTRAR" << endl;
        return;
    }
    do
    {
        cout << current->group << endl;
        for (auto count : current->countries)
        {
            cout << count << " ";
        }
        cout << endl;
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
        byOne(countries, group);
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
    int numeroAleatorio, golesA, golesB, a = 0;
    do
    {
        cout << current->group << endl;
        for (int i = 0; i < current->countries.size(); i++)
        {
            for (int j = 1; j < current->countries.size(); j++)
            {
                if (j + a >= current->countries.size())
                {
                    break;
                }
                cout << current->countries[i] << " vs " << current->countries[j + a] << endl;
                numeroAleatorio = rand() % 3;
                switch (numeroAleatorio)
                {
                case 0:
                    do
                    {
                        v = goles();
                    } while (v[0] >= v[1]);

                    cout << "GOLES: " << current->countries[i] << ": " << v[0] << "\n"
                         << current->countries[j + a] << " : " << v[1] << endl;
                    cout << current->countries[i] << " PERDIO" << endl;
                    break;
                case 1:

                    do
                    {
                        v = goles();
                    } while (v[0] <= v[1]);

                    cout << "GOLES: " << current->countries[i] << ": " << v[0] << "\n"
                         << current->countries[j + a] << " : " << v[1] << endl;
                    cout << current->countries[i] << " GANÓ" << endl;
                    break;
                case 2:
                    do
                    {
                        v = goles();
                    } while (v[0] != v[1]);

                    cout << "GOLES: " << current->countries[i] << ": " << v[0] << "\n"
                         << current->countries[j + a] << " : " << v[1] << endl;
                    cout << current->countries[i] << " Y " << current->countries[j + a] << " EMPATARON" << endl;
                    break;
                }
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