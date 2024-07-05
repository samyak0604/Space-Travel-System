#ifndef STSCLASSES_CPP
#define STSCLASSES_CPP


#include <bits/stdc++.h>
#include "stsclasses.h"
using namespace std;


vector<Ticket> pending;
vector<SpaceTravel> travels;
map<int, Traveller> travellers;
map<int, Astronaut> astronauts;
map<int, Commander> commanders;
Astronaut *tempAstro;
Commander *tempComm;

Date::Date() {}

Date::Date(int yr, int mo, int dt)
{
    this->year = yr;
    this->month = mo;
    this->date = dt;
}

int Date::operator-(Date a)
{
    if(this->date < a.date)
    {
        this->date += 30;
        this->month--;
    }
    if(this->month < a.month)
    {
        this->month += 12;
        this->year--;
    }
    return (this->date - a.date + 30 * (this->month - a.month) + 365 * (this->year - a.year));
}

bool Date::operator==(Date a)
{
    return (this->date == a.date and this->month == a.month and this->year == a.year);
}

Date today(2040, 1, 1);

Planet::Planet() {}

Planet::Planet(int x, int y, int z, string nm)
{
    position.push_back(x);
    position.push_back(y);
    position.push_back(z);
    name = nm;
}

vector<int> Planet::getCoordinates()
{
    return position;
}

Traveller::Traveller() {}

Traveller::Traveller(string nm)
{
    Traveller::UID++;
    name = nm;
    ID = Traveller::UID;
    travellers[ID] = *this;
}

Traveller::Traveller(string nm, int id)
{
    this->name = nm;
    this->ID = id;
    travellers[ID] = *this;
}

string Traveller::getName()
{
    return this->name;
}

int Traveller::getId()
{
    return this->ID;
}

vector<SpaceTravel>::iterator Ticket::findTravel()
{
    for(auto i = travels.begin(); i != travels.end(); i++)
    {
        if(this->dt == i->dt and this->source.name == i->source.name and this->destination.name == i->destination.name)
        {
            return i; 
        }
    }
    return travels.end();
}

void Traveller::bookTicket()
{
    Ticket *t = new Ticket();

    cout << "Enter the date in dd mm yyyy format:" << endl;
    cin >> t->dt.date >> t->dt.month >> t->dt.year;
    if(t->dt - today > 3650)
    {
        cout << "Each ticket is valid only for 10 years. Please choose a valid travel date!\n";
        return;
    }
    Ticket::UID++;
    t->uid = Ticket::UID;
    cout << "Enter the source planet name:" << endl;
    cin >> t->source.name;
    cout << "Enter the destination planet name:" << endl;
    cin >> t->destination.name;
    t->setPrice();
    cout << "Unique Identification Number of your ticket is: " << t->uid << endl << endl;
    // this->listOfTickets[t->uid] = *t;
    t->traveller = new Traveller();
    *(t->traveller) = *this;
    this->listOfTickets[t->uid] = *t;

    this->bookTravel(t);

    string temp;
    cout << "Do you want a return ticket? (Yes/No)" << endl;
    cin >> temp;
    if(temp == "Yes")
    {
        Ticket *ticket = new Ticket();
        ticket->traveller = this;
        cout << "Enter the date in dd mm yyyy format:" << endl;
        cin >> ticket->dt.date >> ticket->dt.month >> ticket->dt.year;
        ticket->source.name = t->destination.name;
        ticket->destination.name = t->source.name;
        Ticket::UID++;
        ticket->uid = Ticket::UID;
        ticket->setPrice();
        cout << "Unique Identification Number of your return ticket is: " << ticket->uid << endl;
        this->bookTravel(ticket);
        this->listOfTickets[ticket->uid] = *ticket;
        pending.push_back(*ticket);
    }
    travellers[this->ID] = *this;
}

void Traveller::bookTravel(Ticket *t)
{
    int count = 0;
    vector<Ticket> arr;
    auto st = t->findTravel();
    if(st != travels.end())
    {
        st->addTraveller(*this);
    }
    else
    {
        bool flag = false;
        for(auto &i : pending)
        {
            if(i.dt == t->dt and t->source.name == i.source.name and t->destination.name == i.destination.name)
            {
                arr.push_back(i);
                count++;
            }
            if(count >= 2 and availableAstro(i.dt) and availableComm(i.dt))
            {
                flag = true;
                SpaceTravel *st = new SpaceTravel(i.dt, i.source.name, i.destination.name);
                SpaceTravel::UID++;
                st->uid = SpaceTravel::UID;
                Ticket t1 = arr[0];
                Ticket::UID++;
                t1.uid = UID;
                Ticket t2 = arr[0];
                Ticket::UID++;
                t2.uid = UID;
                
                tempAstro->listOfTickets[t1.uid] = t1;
                tempComm->listOfTickets[t2.uid] = t2;
                st->setAstronaut(*tempAstro);
                st->setCommander(*tempComm);
                st->setAstroId(tempAstro->ID);
                st->setCmdId(tempComm->ID);
                for(auto &ticket : arr)
                {
                    for(auto it = pending.begin(); it != pending.end(); it++)
                    {
                        if(*it == ticket)
                        {
                            st->addTraveller(*(ticket.traveller));
                            pending.erase(it);
                            break;
                        }
                    }
                }
                st->addTraveller(*this);
                travels.push_back(*st);

                break;
            }
        }
        if(!flag)
        {
            cout << "Your booking has been added to the waiting list. Your travel will be confirmed once we find more passengers!" << endl;
            pending.push_back(*t);
        }
    }
}

void Traveller::updateTicket()
{
    int uid;
    cout << "Enter Unique Identification Number of the ticket you want to update:" << endl;
    cin >> uid;
    Ticket t = this->listOfTickets[uid];
    t.updateTicket();
    if(listOfTickets.find(uid) != listOfTickets.end())
    {
        Ticket *toDelete =  &(this->listOfTickets[uid]);
        this->listOfTickets.erase(uid);
        for(auto i = pending.begin(); i != pending.end(); i++)
        {
            if(i->dt == toDelete->dt and i->source.name == toDelete->source.name and i->destination.name == toDelete->destination.name)
            {
                pending.erase(i);
                break;
            }
        }
        for(auto i = travels.begin(); i != travels.end(); i++)
        {
            int size = i->removeTraveller(toDelete);
            if(size < 3)
            {
                i->deleteTravel();
                travels.erase(i);
                break; 
            }
        }
    }
    else
    {
        cout << "The Ticket with the Unique Identification Number you entered does not exist!" << endl;
        cout << "Current number of tickets booked: " << this->listOfTickets.size() << endl;
    }
    // this->bookTicket()
    // this->bookTravel(&t);
    // auto st = t.findTravel();
    // if(st != travels.end())
    // {
    //     st->addTraveller(*this);
    // }
    listOfTickets[uid] = t;
}

void Traveller::deleteTicket()
{
    int uid;
    cout << "Enter Unique Identification Number of the ticket you want to delete:" << endl;
    cin >> uid;
    if(listOfTickets.find(uid) != listOfTickets.end())
    {
        Ticket *toDelete =  &(this->listOfTickets[uid]);
        for(auto i = pending.begin(); i != pending.end(); i++)
        {
            if(*i == *toDelete)
            {
                pending.erase(i);
                break;
            }
        }
        for(auto i = travels.begin(); i != travels.end(); i++)
        {
            int size = i->removeTraveller(toDelete);
            if(size < 3)
            {
                i->deleteTravel();
                travels.erase(i);
                break; 
            }
        }
        this->listOfTickets.erase(uid);
    }
    else
    {
        cout << "The Ticket with the Unique Identification Number you entered does not exist!" << endl;
        cout << "Current number of tickets booked: " << this->listOfTickets.size() << endl;
    }
}

void Traveller::viewTicket()
{
    int uid;
    cout << "Enter Unique Identification Number of the ticket you want to view:" << endl;
    cin >> uid;
    if(listOfTickets.find(uid) != listOfTickets.end())
    {
        Ticket t = this->listOfTickets[uid];
        t.viewTicket();
    }
    else
    {
        cout << "The Ticket with the Unique Identification Number you entered does not exist" << endl;
    }
}

void Traveller::viewAllTickets()
{
    if(this->listOfTickets.size() == 0)
    {
        cout << "You have no booked tickets!" << endl;
        return;
    }
    for(auto &i : this->listOfTickets)
    {
        i.second.viewTicket();
    }
}

Ticket::Ticket() {}

Ticket::Ticket(string sn, string dn, Date d)
{
    this->dt = d;
    this->source.name = sn;
    this->destination.name = dn;
}

void Ticket::setPrice()
{
    this->price = (double)1000000.0 / (this->dt - today + 1);
}

double Ticket::getPrice()
{
    return this->price;
}

bool Ticket::operator==(Ticket t)
{
    return (t.source.name == this->source.name and t.destination.name == this->destination.name and t.dt == this->dt and t.traveller->name == this->traveller->name and t.traveller->ID == this->traveller->ID);
}

void Ticket::viewTicket()
{
    cout << endl << "*******************************************************************" << endl << endl;
    cout << "\t\tSource Planet: " << source.name << endl;
    cout << "\t\tDestination Planet: " << destination.name << endl;
    cout << "\t\tDate of travel: " << dt.date << "-" << dt.month << "-" << dt.year << endl;
    cout << "\t\tName of Traveller: " << traveller->getName() << endl;
    cout << "\t\tID of Traveller: " << traveller->getId() << endl;
    cout << "\t\tTicket Unique Identification Number: " << uid << endl;
    cout << "\t\tPrice of Ticket: " << this->getPrice() << endl;
    cout << endl << "*******************************************************************" << endl << endl;
}

void Ticket::updateTicket()
{
    cout << "Enter new date:" << endl;
    cin >> this->dt.date >> this->dt. month >> this->dt.year;
    cout << "Enter new destination planet name:" << endl;
    cin >> this->destination.name;
    this->setPrice();
}

SpaceTravel::SpaceTravel(Date dt, string src, string dest) 
{
    this->dt = dt;
    this->source.name = src;
    this->destination.name = dest;
}

void SpaceTravel::addTraveller(Traveller trav)
{
    this->listOfTravellers.push_back(trav);
}

void SpaceTravel::viewTravel()
{
    cout << "*******************************************************************" << endl << endl;
    if(travels.size() == 0)
    {
        cout << "No Space Travels exist!" << endl;
    }
    else
    {
        cout << "\t\tDate of Travel: " << this->dt.date << " " << this->dt.month << " " << this->dt.year << endl;
        cout << "\t\tSource Planet Name: " << this->source.name << endl;
        cout << "\t\tDestination Planet Name: " << this->destination.name << endl;
        cout << "\t\tUnique Travel ID: " << this->uid << endl;
        cout << "\t\tList of travellers:" << endl;
        for(auto t : this->listOfTravellers)
        {
            cout << "\t\tName: " << t.name << "\tId: " << t.ID << endl;
        }
        cout << "\t\tAstronaut: " << this->astronaut.name << endl;
        cout << "\t\tCommander: " << this->commander.name << endl;
    }
    cout << endl << "*******************************************************************" << endl << endl;
}

void SpaceTravel::listTravellers()
{
    int count = 1;
    for(auto traveller: this->listOfTravellers)
    {
        cout << "Traveller #" << count++ << endl;
        cout << "Name: " << traveller.getName() << endl;
        cout << "Traveller ID: " << traveller.getId() << endl;
    }
}

void SpaceTravel::setAstronaut(Astronaut astro)
{
    this->astronaut = astro;
}

void SpaceTravel::setCommander(Commander comm)
{
    this->commander = comm;
}

void SpaceTravel::setAstroId(int id)
{
    this->astronaut.ID = id;
}

void SpaceTravel::setCmdId(int id)
{
    this->commander.ID = id;
}

void SpaceTravel::updateAstroId(int id)
{
    this->astronaut.ID = id;
}

void SpaceTravel::updateCmdId(int id)
{
    this->commander.ID = id;
}

int SpaceTravel::removeTraveller(Ticket *t)
{
    if(t->dt == this->dt and t->source.name == this->source.name and t->destination.name == this->destination.name)
    {
        for(auto it = this->listOfTravellers.begin(); it != this->listOfTravellers.end(); it++)
        {
            if(t->traveller->ID == it->ID)
            {
                this->listOfTravellers.erase(it);
                break;
            }
        }
    }
    return this->listOfTravellers.size();
}

void SpaceTravel::deleteTravel()
{
    // cout << this->listOfTravellers.size() << endl;
    for(auto &i : this->listOfTravellers)
    {
        // cout << i.listOfTickets.size() << endl;
        for(auto &it : i.listOfTickets)
        {
            if(it.second.dt == this->dt and it.second.source.name == this->source.name and it.second.destination.name == this->destination.name)
            {
                pending.push_back(it.second);
                break;
            }
        }
    }
}

Passenger::Passenger(string nm):Traveller(nm) {}

Astronaut::Astronaut() {}

Astronaut::Astronaut(int exp, string nm, int liscId):Traveller(nm)
{
    this->experience = exp;
    this->licenseId = liscId;
    astronauts[ID] = *this;
}

Commander::Commander() {}

Commander::Commander(string nm):Traveller(nm)
{
    commanders[ID] = *this;
}

void Commander::viewTravels()
{
    if(travels.size() == 0)
    {
        cout << "There are no current Space Travels!" << endl;
        return;
    }
    for(auto i : travels)
    {
        i.viewTravel();
    }
}

void Commander::viewPending()
{
    if(pending.size() == 0)
    {
        cout << "There are no unappointed tickets!" << endl;
        return;
    }
    for(auto i : pending)
    {
        i.viewTicket();
    }
}

bool availableAstro(Date dt)
{
    for(auto &astro : astronauts)
    {
        bool flag = true;
        for(auto &i : astro.second.listOfTickets)
        {
            if(i.second.dt == dt)
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            tempAstro = &(astro.second);
            return true;
        }
    }
    return false;
}

bool availableComm(Date dt)
{
    for(auto &comm : commanders)
    {
        bool flag = true;
        for(auto &i : comm.second.listOfTickets)
        {
            if(i.second.dt == dt)
            {
                flag = false;
                break;
            }
        }
        if(flag)
        {
            tempComm = &(comm.second);
            return true;
        }
    }
    return false;
}



#endif