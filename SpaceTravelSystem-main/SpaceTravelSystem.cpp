#include <bits/stdc++.h>
#include "stsclasses.h"
using namespace std;

int Ticket::UID = 0;
int SpaceTravel::UID = 0;
int Traveller::UID = 0;

extern vector<SpaceTravel> travels;
extern map<int, Traveller> travellers;
extern map<int, Astronaut> astronauts;
extern map<int, Commander> commanders;

void createTravellers()
{
    string name = "Pass";
    for(int i = 0; i < 13; i++)
    {
        name += (char)(i + 48);
        Passenger pass(name);
        name.erase(name.size() - 1);
    }
    Astronaut a0(30, "Astro1", 1454);  // ID = 14
    Astronaut a1(23, "Astro2", 4645);  // ID = 15
    Commander c0("Comm1");  // ID = 16
    Commander c1("Comm2");  // ID = 17
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout << "Welcome to the future!" << endl;
    cout << "Today's date: 01-01-2040" << endl;
    cout << "Would you like to continue to your Intergalactic Space Travel? (Type Yes to continue)" << endl;
    string temp;
    cin >> temp;
    if(temp != "Yes") return 0;

    Planet p1(64, 625, 23, "Earth");
    Planet p2(685, 32, 95, "Jupiter");
    Planet p3(984, 51, 65, "Pluto");
    createTravellers();
    bool flag1 = true;
    while(flag1)
    {
        cout << "Choose your role:\n(1) Passenger\n(2) Astronaut\n(3) Commander\n(4) Quit" << endl;
        int in;
        cin >> in;
        if(in == 1)
        {
            bool flag2 = true;
            int uid;
            cout << "Enter your Traveller UID:" << endl;
            cin >> uid;
            if(travellers.find(uid) == travellers.end())
            {
                cout << "The UID for Passenger you entered does not exist. Kindly chose a valid Passenger ID!" << endl;
                continue;
            }
            Traveller trav = travellers[uid];
            while(flag2)
            {
                int input;
                cout << "Choose your option:\n(1) Book Ticket\n(2) View Ticket\n(3) Update Ticket\n(4) Delete Ticket\n(5) View All Tickets\n(6) Go Back" << endl;
                cin >> input;
                switch(input)
                {
                    case 1:
                        trav.bookTicket();
                        break;
                    case 2:
                        trav.viewTicket();
                        break;
                    case 3:
                        trav.updateTicket();
                        break;
                    case 4:
                        trav.deleteTicket();
                        break;
                    case 5:
                        trav.viewAllTickets();
                        break;
                    case 6:
                        flag2 = false;
                        break;
                    default: cout << "Please choose a valid option!" << endl;
                }
            }
        }
        else if(in == 2)
        {
            bool flag2 = true;
            int uid;
            cout << "Enter your Traveller UID:" << endl;
            cin >> uid;
            if(astronauts.find(uid) == astronauts.end())
            {
                cout << "You are not authorized to access the Spave Travel System as a commander!" << endl;
                continue;
            }
            Astronaut astro = astronauts[uid];
            while(flag2)
            {
                int input;
                cout << "Choose your options:\n(1) View Ticket\n(2) View all Tickets\n(3) Go back" << endl;
                cin >> input;
                switch(input)
                {
                    case 1:
                        astro.viewTicket();
                        break;
                    case 2:
                        astro.viewAllTickets();
                        break;
                    case 3:
                        flag2 = false;
                        break;
                    default: cout << "Please choose a valid option!" << endl;
                }
            }
        }
        else if(in == 3)
        {
            bool flag2 = true;
            int uid;
            cout << "Enter your Traveller UID:" << endl;
            cin >> uid;
            if(commanders.find(uid) == commanders.end())
            {
                cout << "You are not authorized to access the Spave Travel System as a commander!" << endl;
                continue;
            }
            Commander comm = commanders[uid];
            while(flag2)
            {
                int input;
                cout << "Choose your options:\n(1) View Ticket\n(2) View all upcoming travels\n(3) View Unappointed Tickets\n(4) Go back" << endl;
                cin >> input;
                switch(input)
                {
                    case 1:
                        comm.viewAllTickets();
                        break;
                    case 2:
                        comm.viewTravels();
                        break;
                    case 3:
                        comm.viewPending();
                        break;
                    case 4:
                        flag2 = false;
                        break;
                    default: cout << "Please choose a valid option!" << endl;
                }
            }
        }
        else if(in == 4)
        {
            cout << "Thank you for using the Space Travel System!" << endl;
            break;
        }
        else
        {
            cout << "Please choose a valid option!" << endl;
        }
    }
    return 0;
}