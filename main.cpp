#define _CRT_SECURE_NO_WARNINGS
#include "ourvector.h"
#include<string>
#include<cmath>
#include<fstream>
#include<cstring>
#include<sstream>
using namespace std;
struct District
{
    int democraticVotes;
    int republicationVotes;
    int wastedDemocraticVotes;
    int wastedRepublicationVotes;
};
struct State
{
    string name;
    int eligibleVoters;
    ourvector<District> districts;
    double efficiency;
    int totalDemVotes;
    int totalrepVotes;
    int totalWastedDem;
    int totalWastedRep;
};
void plot(State& st);
void plot(District& dis);
void calculateEfficiency(ourvector<State>& vec);
double calculateEfficiency(State& st);
int getIndex(string name, ourvector<State>& vec);
void tokenize(string& stateName, ourvector<District>& dis, string line);
void display(string isLoaded, string stateName);
string convert2lower(string str);
string load(ourvector<State>& vec, string districtFilename, string eligibleVotersFilename, string stateName, bool& isSearchPossible);
void run();
void stats(ourvector<State>& vec, int index);
void search(string& isLoaded, string& stateName, ourvector<State>& vec, int& index, bool& isSearchPossible);
void plot(string stateName, string isLoaded, ourvector<State>& vec, int index);
void stats(string isLoaded, string stateName, ourvector<State>& vec, int index);


int main()
{
    run();
    return 0;
}
/*
this function will provide an interface to execute different commands like searxh load plot and stats
*/
void run()
{
    cout << "Welcome to the Gerrymandering App!\n\n";
    ourvector<State> vec;
    int index = -1;
    string isLoaded = "No";
    string stateName = "N/A";
    bool isSearchPossible = false;
    display(isLoaded, stateName);
    while (true)
    {
        string command, line;
        cin >> command;
        cin.ignore();
        if (command == "search")
            search(isLoaded, stateName, vec, index, isSearchPossible);
        else if (command == "load")
        {
            string districtFile, eligibleVotersFile;
            cin >> districtFile >> eligibleVotersFile;
            cin.ignore();
            isLoaded = load(vec, districtFile, eligibleVotersFile, stateName,isSearchPossible);
        }
        else if (command == "plot")
            plot(stateName, isLoaded, vec, index);
        else if (command == "exit")
            return;
        else if (command == "stats")
            stats(isLoaded, stateName, vec, index);
    }
}
/*
this function will take the service of getIndex function to find a state by name
then perform stats operation as specified
this function will manage incorrect output and take the service of overloaded stats function to print stats
*/
void stats(string isLoaded, string stateName, ourvector<State>& vec,int index)
{
    if (isLoaded == "Yes")
    {
        if (stateName != "")
        {
            stats(vec, index);
            display("Yes", stateName);
        }
        else
        {
            cout << "No state searched, please search state first.\n\n";
            display("Yes", "N/A");
        }
    }
    else
    {
        cout << "No data loaded, please load data first.\n\n";
        display("No", "N/A");
    }
}
/*
this function will take the service of getIndex function to find a state by name
then perform plot operation as specified
this function will manage incorrect output and take the service of overloaded plot function to plot
*/
void plot(string stateName, string isLoaded, ourvector<State>& vec, int index)
{
    if (isLoaded == "Yes")
    {
        if (stateName != "")
        {
            plot(vec[index]);
            cout << '\n';
            display("Yes", stateName);
        }
        else
        {
            cout << "No state indicated, please search for state first.\n\n";
            display("Yes", "N/A");
        }
    }
    else
    {
        cout << "No data loaded, please load data first.\n\n";
        display("No", "N/A");
    }
}
/*
this function will take the service of getIndex function to find a state by name
then perform search operation as specified
this function will manage incorrect output and take the service of overloaded getindex function to perform search operation
*/
void search(string& isLoaded, string& stateName,ourvector<State>& vec,int& index, bool& isSearchPossible)
{
    string name;
    getline(cin, name);
    if (!isSearchPossible)
    {
        cout << "No data loaded, please load data first.\n\n";
        display("No", "N/A");
    }
    else
    {
        index = getIndex(name, vec);
        if (index == -1)
        {
            cout << "State does not exist, search again.\n\n";
            display("Yes", "N/A");
        }
        else
        {
            stateName = name;
            stateName = vec[index].name;
            display(isLoaded, stateName);
        }
    }
}
/*
this function will take ourvector obj and the index of a state whose stats we are going to print
then prints its stats
*/
void stats(ourvector<State>& vec, int index)
{
    if (vec[index].districts.size() > 3 && vec[index].efficiency >= 7)
    {
        cout << "Gerrymandered: Yes\n";
        cout << "Gerrymandered against: " << (vec[index].totalWastedDem > vec[index].totalWastedRep ? "Democrats" : "Republicans");
        cout << "\nefficiency Factor: " << vec[index].efficiency << "%\n";
        cout << "Wasted Democratic votes: " << vec[index].totalWastedDem << '\n';
        cout << "Wasted Republican votes: " << vec[index].totalWastedRep << '\n';
        cout << "Eligible voters: " << vec[index].eligibleVoters << "\n\n";
    }
    else
    {
        cout << "Gerrymandered: No\n";
        cout << "Wasted Democratic votes: " << vec[index].totalWastedDem << '\n';
        cout << "Wasted Republican votes: " << vec[index].totalWastedRep << '\n';
        cout << "Eligible voters: " << vec[index].eligibleVoters << "\n\n";
    }
}
/*
this function will take string and returns new string by converting it to lower case
*/
string convert2lower(string str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + 32;
    }
    return str;
}
/*
this function display data such as
Data loaded? yes/no and state?N/A and enter command
*/
void display(string isLoaded, string stateName)
{
    cout << "Data loaded? " << isLoaded << '\n';
    cout << "State: " << stateName << "\n\nEnter command: ";
    cout << "\n-----------------------------\n\n";
}
/*
this function will take state obj, then pass each district data to the below function
this function will call the below function to plot each district`s data contained in the state passed to it
*/
void plot(State& st)
{
    int count = 1;
    int size = st.districts.size();
    for (int i=0;i<size;i++)
    {
        District& dis = st.districts[i];
        cout << "District: " << count++ << '\n';
        plot(dis);
    }
}
/*
this function will take District object and plot D`s and R`s
*/
void plot(District& dis)
{
    int percent = round((double)dis.democraticVotes / (dis.democraticVotes + dis.republicationVotes) * 100);
    for (int i = 0; i < percent; i++)
        cout << 'D';
    for (int i = percent; i < 100; i++)
        cout << 'R';
    cout << endl;
}
/*
this function will take ourvector<state> object 2 filenames
it then writes data in ourvector obj by reading from file
*/
string load(ourvector<State>& vec, string districtFilename, string eligibleVotersFilename, string stateName,bool& isSearchPossible)
{
    if (vec.size() != 0)
    {
        cout << "Already read data in, exit and start over.\n\n";
        display("Yes", stateName);
        return "Yes";
    }
    ifstream fin;
    fin.open(districtFilename);
    if (!fin)
    {
        string temp;
        cout << "\nInvalid first file, try again.\n\n";
        display("No", "N/A");
        return "No";
    }
    cout << "Reading: " << districtFilename << '\n';
    string line;
    while (!fin.eof())
    {
        State state;
        if (fin.peek() == EOF)
            break;
        getline(fin, line);
        tokenize(state.name, state.districts, line);
        string displayStr = "..." + state.name + "..." + to_string(state.districts.size()) + " districts total\n";
        cout << displayStr;
        vec.push_back(state);
    }
    isSearchPossible = true;
    cout << '\n';
    fin.close();
    calculateEfficiency(vec);
    fin.open(eligibleVotersFilename);
    if (!fin)
    {
        cout << "Invalid second file, try again.\n\n";
        display("No", "N/A");
        return "No";
    }
    cout << "Reading: " << eligibleVotersFilename << '\n';
    while (!fin.eof())
    {
        if (fin.peek() == EOF)
            break;
        getline(fin, line);
        char* str = (char*)line.c_str();
        char* name = strtok(str, ",");
        char* voters = strtok(NULL, ",");
        int index = getIndex(string(name), vec);
        if (index != -1)
        {
            vec[index].eligibleVoters = atoi(voters);
            string displayStr = "..." + string(name) + "..." + voters + " eligible voters\n";
            cout << displayStr;
        }
    }
    cout << '\n';
    fin.close();
    //calculateEfficiency(vec);
    display("Yes", "N/A");
    return "Yes";
}
/*
when we have read all the data in ourvector, then we pass ourvector to this function and this function
will calculate effieiency gap
*/
void calculateEfficiency(ourvector<State>& vec)
{
    int size = vec.size();
    for (int i = 0; i < size; i++)
    {
        State& st = vec[i];
        st.efficiency = calculateEfficiency(st);
    }
}
/*
* This function will get state struct and calculate its efficiency gap
*/
double calculateEfficiency(State& st)
{
    int size = st.districts.size();
    int wastedDemVotes = 0;
    int wastedRepVotes = 0;
    int totalVotesCast = 0;
    int demtotal = 0;
    int reptotal = 0;
    for (int i=0;i<size;i++)
    {
        District& dis = st.districts[i];
        demtotal += dis.democraticVotes;
        reptotal += dis.republicationVotes;
        int total = dis.democraticVotes + dis.republicationVotes;
        int overHalf = (total) / 2 + 1;
        if (dis.democraticVotes > overHalf)
        {
            dis.wastedDemocraticVotes = dis.democraticVotes - overHalf;
            dis.wastedRepublicationVotes = dis.republicationVotes;
        }
        else if (dis.democraticVotes < overHalf)
        {
            dis.wastedRepublicationVotes = dis.republicationVotes - overHalf;
            dis.wastedDemocraticVotes = dis.democraticVotes;
        }
        else
        {
            cerr << "Tie occur\n";
            exit(0);
        }
        wastedDemVotes += dis.wastedDemocraticVotes;
        wastedRepVotes += dis.wastedRepublicationVotes;
        totalVotesCast += total;
    }
    st.totalDemVotes = demtotal;
    st.totalrepVotes = reptotal;
    st.totalWastedDem = wastedDemVotes;
    st.totalWastedRep = wastedRepVotes;
    return abs(wastedDemVotes - wastedRepVotes) / (double)totalVotesCast * 100;
}
/*
This function will take state name and tells its index in ourvector<State>
it performs searching
*/

int getIndex(string name, ourvector<State>& vec)
{
    int size = vec.size();
    int i = 0;
    while (i<size)
    {
        if (convert2lower(name) == convert2lower(vec[i].name))
            return i;
        ++i;
    }
    return -1;
}
/*
This function will read a complete line from file.
That line contains state name and its districts info
This function will split this line and separates all the information
*/
void tokenize(string& stateName, ourvector<District>& dis, string line)
{
    char* str = (char*)line.c_str();
    char* temp;
    stateName = string(strtok(str, ","));
    temp = strtok(NULL, ",");
    int i = 0;
    District district;
    while (temp)
    {
        temp = strtok(NULL, ",");
        district.democraticVotes = atoi(temp);
        temp = strtok(NULL, ",");
        district.republicationVotes = atoi(temp);
        temp = strtok(NULL, ",");
        dis.push_back(district);
    }
}