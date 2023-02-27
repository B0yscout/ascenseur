#include "ascenseur.hpp"





void Ascenseur::changerEtat(Etat *etat)
{
  if (this->etat != nullptr)
    delete this->etat;
  
  this->etat = etat;
  this->etat->setAscenseur(this);
    
}

void Ascenseur::bouge()
{
  this->etat->bouge();
}

void Ascenseur::afficher()
{

  cout<<" ---------------------- étages."<<endl;
  cout<<" |  ------------------- état de l'ascenseur (montant ou descendant)"<<endl;
  cout<<" |  |      ------------ ascenseur avec passagers (représentés par leurs destinations)"<<endl;
  cout<<" |  |      |       ---- personnes en attente (représentées par leurs destinations)"<<endl;
  cout<<" |  |      |       |"<<endl<<endl;
  
  // ----- tic tac
  cout<<"           ";
  if (this->tick)
  {
    cout<<"\u25CB";
    tick=!tick;
  }
  else
  {
    cout<<"\u25CF";
    tick=!tick;
  }
  cout<<endl;
  // -----


  for (int etage_ = ETAGE_MAX ; etage_>=ETAGE_MIN; etage_--)
  {

    // ----- Numéro d'étage. En rouge si des passagers débarquent
    if ((this->aDebarque) && (etage_ == this->etage))
      cout<<" "<<printCouleur(etage_,1);
    else
      cout<<" "<<etage_;
    // -----
    
    cout<<"  ";

    // ----- Logo montant, arrêt, ou descendant 
    if (this->etage==etage_)
      cout<<this->etat->logo();
    else
      cout<<" ";
    // -----

    // ----- cage d'ascenseur Gauche
    if ((this->aDebarque) && (this->etage==etage_))
      cout<<" ";
    else
      cout<<'|';
    // -----


    // ----- ascenseur
    if (this->etage == etage_) {
      cout<<"[";

      // affiche les 8 places 
      // représente un passager par sa destination
      for (auto it = this->aboard.begin(); it != this->aboard.end(); ++it) {
        if (it->getDepart()>it->getDestination())
          cout<<printCouleur(it->getDestination(),2);
        else
          cout<<printCouleur(it->getDestination(),4);
      }    
      // for (int i=0;i<=ETAGE_MAX-this->aboard.size();i++)
      for (int i=this->aboard.size();i<PASSAGERS_MAX;i++)
      {
        cout<<"_";
      }

      cout<<"]";
    }
    else {
      for (int i=0;i<PASSAGERS_MAX+2;i++)
      {
        cout<<" ";
      }
    }
    // -----


    // cage d'ascenseur Droite
    if ((this->aEmbarque) && (this->etage==etage_))
      cout<<"  ";
    else
      cout<<"| ";
    // -----

    // affiche la destination des personnes ayant appelé l'ascenseur à cet étage
    for (auto it = this->queue.begin(); it != this->queue.end(); ++it) {

      if (it->getDepart()==etage_){
        if(it->getDepart()<it->getDestination())
          cout << printCouleur(it->getDestination(),4)<<" ";
        else
          cout << printCouleur(it->getDestination(),2)<<" ";

      }
    }

    cout<<endl;
  }
}

void Ascenseur::appel(int dep, int dest)
{
  this->queue.push_back(Personne(dep, dest));
}


void EtatArret::bouge()
{



  if (!this->monAscenseur->queue.empty() && (this->monAscenseur->queue.begin()->getDepart() >= this->monAscenseur->etage))
  {
    EtatMontant *nouvelEtat = new EtatMontant;
    this->monAscenseur->changerEtat(nouvelEtat);
    nouvelEtat->embarquer();

  }
  else if (!this->monAscenseur->queue.empty() && (this->monAscenseur->queue.begin()->getDepart() <= this->monAscenseur->etage))
  {
    EtatDescendant *nouvelEtat = new EtatDescendant;
    this->monAscenseur->changerEtat(nouvelEtat);
    nouvelEtat->embarquer();

  }  
  
}

void EtatMontant::embarquer()
{
  this->monAscenseur->aEmbarque=false;
  
  // prendre à bord les passagers qui montent:
  for (auto it = this->monAscenseur->queue.begin(); it != this->monAscenseur->queue.end();) {

    // si le passager est à l'étage courant ET (pas plus de PASSAGERS_MAX passagers) ET (passager monte OU l'ascenseur est à l'étage 0)
    if ((it->getDepart()==this->monAscenseur->etage) && (this->monAscenseur->aboard.size()<PASSAGERS_MAX) && ( (it->getDestination() >= this->monAscenseur->etage)  || (this->monAscenseur->etage== ETAGE_MIN) ))
    {

      // ajouter le passager à bord ("aboard")
      // et le supprimer des appels en attente ("queue")
      auto next_it = next(it);
      this->monAscenseur->aboard.splice(this->monAscenseur->aboard.end(),this->monAscenseur->queue, it);
      this->monAscenseur->aEmbarque=true;
      it = next_it;
    }

    else {
      ++it;
    }
  }

}

void EtatMontant::debarquer()
{
  this->monAscenseur->aDebarque=false;
  if(!this->monAscenseur->aboard.empty())
  {


    // déposer les passagers à bord qui ont atteint leur étage de destination
    for (auto it = this->monAscenseur->aboard.begin(); it != this->monAscenseur->aboard.end();) {

      // si le passager est à son étage de destination
      if (it->getDestination()==this->monAscenseur->etage) {

        // supprimer le passager de l'ascenseur ("aboard")
        auto next_it = next(it);
        this->monAscenseur->aboard.erase(it);
        this->monAscenseur->aDebarque=true;
        it = next_it;
      }

      else {
        ++it;
      }
    }

  }

}


void EtatDescendant::embarquer()
{

  this->monAscenseur->aEmbarque=false;

  // prendre à bord les passagers qui descendent OU le dernier passager le la file (seul appel restant):
  for (auto it = this->monAscenseur->queue.begin(); it != this->monAscenseur->queue.end();) {

    // si le passager est à l'étage courant ET (pas plus de MAX_PASSAGERS passagers) ET ( passager descend OU l'ascenseur est à l'étage MAX) : 
    if ((it->getDepart()==this->monAscenseur->etage) && (this->monAscenseur->aboard.size()<PASSAGERS_MAX ) && ((it->getDestination() <= this->monAscenseur->etage) || (this->monAscenseur->etage==ETAGE_MAX) )){

      // ajouter le passager à bord ("aboard")
      // et le supprimer des appels en attente ("queue")
      auto next_it = next(it);
      this->monAscenseur->aboard.splice(this->monAscenseur->aboard.end(),this->monAscenseur->queue, it);
      this->monAscenseur->aEmbarque=true;
      it = next_it;
    }

    else {
      ++it;
    }
  }


}

void EtatDescendant::debarquer()
{

  this->monAscenseur->aDebarque=false;
  if(!this->monAscenseur->aboard.empty())
  {

    // déposer les passagers à bord qui ont atteint leur étage de destination
    for (auto it = this->monAscenseur->aboard.begin(); it != this->monAscenseur->aboard.end();) {

      // si le passager est à son étage de destination
      if (it->getDestination()==this->monAscenseur->etage){

        // supprimer le passager de l'ascenseur ("aboard")
        auto next_it = next(it);
        this->monAscenseur->aboard.erase(it);
        this->monAscenseur->aDebarque=true;
        it = next_it;
      }

      else {
        ++it;
      }
    }

  }


}

void EtatArret::embarquer()
{
}
void EtatArret::debarquer()
{
}

void EtatMontant::bouge()
{
  this->monAscenseur->etage++;

  this->debarquer();
  this->embarquer();

  // changer d'état ? 
  bool ascenseurVide = (this->monAscenseur->aboard.size()==0) ? true : false;
  bool attenteVide = (this->monAscenseur->queue.size()==0) ? true : false;
  bool appelEnBas=false;

  if (!attenteVide)
  {
    auto it = this->monAscenseur->queue.begin();
    while (it != this->monAscenseur->queue.end() && it->getDepart() <= this->monAscenseur->etage) {
        ++it;
    }

    if (it != this->monAscenseur->queue.end()) {
        // aucun appel en bas
        appelEnBas = false;
    } else {
        // au moins 1 appel vers le bas
        appelEnBas = true;
    }
  }
  
  bool passagersDesc = false;

  for (auto it = this->monAscenseur->aboard.begin(); it != this->monAscenseur->aboard.end();it++) {

    // y-a-t-il des passagers à bord qui descendent?
    if (it->getDestination() <= this->monAscenseur->etage)
      passagersDesc = true;

  }
  

  // si plus aucun passager à bord ni aucun appel : passer à l'état ARRET
  if (ascenseurVide && attenteVide)
  {
    this->monAscenseur->changerEtat(new EtatArret);
  }

  // si plus aucun passager à bord mais des appels à des étages inférieurs : passer à l'état DESCENDANT
  else if ((ascenseurVide && appelEnBas) || passagersDesc )
  {
    EtatDescendant* nouvelEtat = new EtatDescendant;

    this->monAscenseur->changerEtat(nouvelEtat);

    nouvelEtat->embarquer();
    
  }

  

  
}

void EtatDescendant::bouge()
{
  this->monAscenseur->etage--;

  this->debarquer();
  this->embarquer();

  // changer d'état ? 
  bool ascenseurVide = (this->monAscenseur->aboard.size()==0) ? true : false;
  bool attenteVide = (this->monAscenseur->queue.size()==0) ? true : false;
  bool appelEnHaut=false;

  if (!attenteVide)
  {
    auto it = this->monAscenseur->queue.begin();
    while (it != this->monAscenseur->queue.end() && it->getDepart() >= this->monAscenseur->etage) {
        ++it;
    }

    if (it != this->monAscenseur->queue.end()) {
        // aucun appel en haut
        appelEnHaut = false;
    } else {
        // au moins 1 appel vers le haut
        appelEnHaut = true;
    }
  }
  
  bool passagersMont = false;

  for (auto it = this->monAscenseur->aboard.begin(); it != this->monAscenseur->aboard.end();it++) {

    // y-a-t-il des passagers à bord qui montent?
    if (it->getDestination() >= this->monAscenseur->etage)
      passagersMont = true;

  }
  
  // si plus aucun passager à bord ni aucun appel : passer à l'état ARRET
  if (ascenseurVide && attenteVide)
  {
    this->monAscenseur->changerEtat(new EtatArret);

  }
  // si plus aucun passager à bord mais des appels à des étages supérieurs : passer à l'état MONTANT
  else if ((ascenseurVide && appelEnHaut) || passagersMont )
  {
    EtatMontant* nouvelEtat = new EtatMontant;

    this->monAscenseur->changerEtat(nouvelEtat);
    nouvelEtat->embarquer();
  }
  


  
}


string printCouleur(string texte,int couleur)
{
  map<int,pair<string,string>> couleurs;

  couleurs[BLACK]=make_pair("\033[30m","\033[0m");
  couleurs[RED]=make_pair("\033[31m","\033[0m");
  couleurs[GREEN]=make_pair("\033[32m","\033[0m");
  couleurs[YELLOW]=make_pair("\033[33m","\033[0m");
  couleurs[BLUE]=make_pair("\033[34m","\033[0m");
  couleurs[MAGENTA]=make_pair("\033[35m","\033[0m");
  couleurs[CYAN]=make_pair("\033[36m","\033[0m");
  couleurs[WHITE]=make_pair("\033[37m","\033[0m");

  return(couleurs[couleur].first.append(texte.append(couleurs[couleur].second)));
  // cout<<couleurs[couleur].first<<texte<<couleurs[couleur].second;
}

string printCouleur(int texte,int couleur)
{
  map<int,pair<string,string>> couleurs;

  couleurs[BLACK]=make_pair("\033[30m","\033[0m");
  couleurs[RED]=make_pair("\033[31m","\033[0m");
  couleurs[GREEN]=make_pair("\033[32m","\033[0m");
  couleurs[YELLOW]=make_pair("\033[33m","\033[0m");
  couleurs[BLUE]=make_pair("\033[34m","\033[0m");
  couleurs[MAGENTA]=make_pair("\033[35m","\033[0m");
  couleurs[CYAN]=make_pair("\033[36m","\033[0m");
  couleurs[WHITE]=make_pair("\033[37m","\033[0m");


  return(couleurs[couleur].first.append(to_string(texte).append(couleurs[couleur].second)));
  // cout<<couleurs[couleur].first<<texte<<couleurs[couleur].second;

}