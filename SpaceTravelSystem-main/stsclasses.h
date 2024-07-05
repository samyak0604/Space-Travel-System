#ifndef STSCLASSES_H
#define STSCLASSES_H

#include <bits/stdc++.h>
using namespace std;


class Date
{
public:
    int year, month, date;

    Date();
    Date(int yr, int mo, int dt);
    bool operator==(Date a);
    int operator-(Date a);
};

class Planet
{
public:
    vector<int> position;
    string name;

    Planet();
    Planet(int x, int y, int z, string nm);
    vector<int>  getCoordinates();
};

class Traveller;
class SpaceTravel;

class Ticket
{
private:
    double price;

public:
    Planet source, destination;
    Date dt;
    Traveller *traveller;
    int validity;
    static int UID;
    int uid;

    Ticket();
    Ticket(string sn, string dn, Date d);
    void viewTicket();
    void updateTicket();
    bool operator==(Ticket);
    vector<SpaceTravel>::iterator findTravel();
    void setPrice();
    double getPrice();
};

class Traveller
{    
public:
    string name;
    int ID;
    Date dt;
    map<int, Ticket> listOfTickets;
    static int UID;

    Traveller();
    Traveller(string nm);
    Traveller(string nm, int id);
    string getName();
    int getId();
    void bookTicket();
    void updateTicket();
    void deleteTicket();
    void viewTicket();
    void viewAllTickets();
    void bookTravel(Ticket *t);
};

class Passenger: public Traveller
{
public:
    Passenger(string nm);
};

class Astronaut: public Traveller
{
private:
    int experience;
    int licenseId;

public:
    Astronaut();
    Astronaut(int exp, string nm, int liscId);
};

class Commander: public Traveller
{
public:
    Commander();
    Commander(string nm);
    int experience;
    void viewTravels();
    void viewPending();
};

class SpaceTravel
{
private:
    vector<Traveller> listOfTravellers;
    Astronaut astronaut;
    Commander commander;

public:
    static int UID;
    Date dt;
    Planet source, destination;
    int uid;

    SpaceTravel(Date dt, string src, string dest);
    void viewTravel();
    void addTraveller(Traveller trav);
    void deleteTravel();
    int removeTraveller(Ticket *t);
    void listTravellers();
    void setAstroId(int id);
    void setCmdId(int id);
    void updateAstroId(int id);
    void updateCmdId(int id);
    void setAstronaut(Astronaut astro);
    void setCommander(Commander comm);
};

bool availableAstro(Date dt);
bool availableComm(Date dt);

#endif