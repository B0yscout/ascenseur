#ifndef ASCENSEUR_H
#define ASCENSEUR_H

#include <string>
#include <list>
#include <typeinfo>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <unistd.h>
#include <map>

#define ETAGE_MAX 9
#define ETAGE_MIN 0
#define PASSAGERS_MAX 8

#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

using namespace std;


string printCouleur(string,int);
string printCouleur(int,int);


class Personne
{
    public:
        Personne(int dep,int dest) {
            setDepart(dep);
            setDestination(dest);
        }
        virtual ~Personne() {}
        void setDepart(int depart) {this->depart = depart; }
        void setDestination(int destination) {this->destination = destination;}
        int getDepart() {return this->depart;}
        int getDestination() {return this->destination;}
    private:
        int depart;
        int destination;
};


/* immplémentation du design pattern STATE */

class Ascenseur;

class Etat
{
    public:
        Etat() {

        }
        virtual ~Etat() {}
        void setAscenseur(Ascenseur* ascenseur) {this->monAscenseur=ascenseur;}
        virtual void bouge() = 0;
        virtual void embarquer() = 0;
        virtual void debarquer() = 0;
        virtual string logo() = 0;    
        
    protected:
        Ascenseur *monAscenseur;
        map<int,pair<string,string>> couleurs;

    private:

};



class EtatArret : public Etat
{
    public:
        EtatArret():Etat() {}
        void bouge() override;
        void embarquer() override;
        void debarquer() override;
        string logo() override {return "-";} 
    private:

};

class EtatMontant : public Etat
{
    public:
        EtatMontant():Etat() {}
        void bouge() override;
        void embarquer() override;
        void debarquer() override;
        string logo() override {return printCouleur("\u2191",4);} 



    private:
};

class EtatDescendant : public Etat
{
    public:
        EtatDescendant():Etat() {}
        void bouge() override;
        void embarquer() override;
        void debarquer() override;
        string logo() override {return printCouleur("\u2193",2);} 


    private:

};

class Ascenseur
{
    public:
        Ascenseur(Etat *state) : etat(nullptr) {
            this->changerEtat(state);
            this->etage = 0;
            this->aDebarque = false;
            this->aEmbarque = false;
            this->tick=false;

        }
        virtual ~Ascenseur() {
            delete etat;
        }
        void changerEtat(Etat *etat);
        virtual void bouge();
        
        void afficher();

        void appel(int,int);

        list<Personne> queue;
        list<Personne> aboard;
        int etage;
        bool aDebarque;
        bool aEmbarque;
        bool tick;
        

    protected:

    private:
        Etat *etat;


        
};

#endif