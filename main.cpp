#include <ctime>
#include <cstdlib>

#include "ascenseur.hpp"



void modeManuel();
void modeAuto();
void appelRand(Ascenseur*);



int main()
{



    int mode;
    system("clear");

    cout<<"Bienvenue dans cette simulation d'ascenseur"<<endl<<endl;

    cout<<"Choix mode:"<<endl;
    cout<<"Manuel      : 1"<<endl;
    cout<<"Automatique : 2"<<endl;
    cin>>mode;

    switch (mode)
    {
    case 1:
        modeManuel();
        break;
    
    default:
        modeAuto();
        break;
    }


    return 0;
}



void modeManuel()
{

    system("clear");

    // 1 instance d'ascenseur à l'état EtatArret
    Ascenseur *ascenseur = new Ascenseur(new EtatArret);

    int depart;
    int destination;
    char appel;

    while(true)
    {
        system("clear");
        ascenseur->afficher();

        cout<<"Nouvel appel? O:oui | N:Non"<<endl;
        cin >> appel;
        

        switch (appel)
        {
        case 'O':
        case 'o':
            cout<<"Etage d'appel?:"<<endl;
            cin >> depart;
            cout<<"Etage de destination?:"<<endl;
            cin >> destination;
            ascenseur->appel(depart,destination);
            break;
        
        default:
            break;
        }

        ascenseur->bouge();

    }



    delete ascenseur;

}




void modeAuto()
{

    system("clear");

    // 1 instance d'ascenseur à l'état EtatArret
    Ascenseur *ascenseur = new Ascenseur(new EtatArret);

    while(true)
    {
        

        // on lance aléatoirement des appels aléatoirs.
        appelRand(ascenseur);

        // réafficher pour voir les nouveaux appels
        system("clear");
        ascenseur->afficher();
        sleep(1);

        // lancer 1 cycle
        ascenseur->bouge();
        
        // afficher ascenseur
        system("clear");
        ascenseur->afficher();
        sleep(1);

    }



    delete ascenseur;

}

void appelRand(Ascenseur* ascenseur)
{
    // Seed the random number generator with the current time
    srand(time(nullptr));
    // Generate a random integer between 0 and RAND_MAX
    int random_int = rand();
    // Convert the random integer to a bool value
    bool random_bool = (random_int % 2 == 0);


    if (random_bool)
    {
        srand(time(nullptr));

        int depart = rand() % (ETAGE_MAX - ETAGE_MIN + 1) + ETAGE_MIN;

        int destination;
        do {
            destination =rand() % (ETAGE_MAX - ETAGE_MIN + 1) + ETAGE_MIN;
        } while (depart==destination);
    
        ascenseur->appel(depart,destination); 
    }
}