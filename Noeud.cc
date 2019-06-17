
#include "Noeud.h"
#include <string.h>
#include <omnetpp.h>
#include <Segment_m.h>

/////////////////////////////////////
int i=0;//gerer les numero de sequence par le server1
int z=0;//gestion des num de sequence par le server 2
int w=0;//gestion des num de seq par le server 3
//int nbr_noeud=9;
int add=0;
int adress_s1[253][3];//base de donnée de server ne peut contenir que 253 noeud
int adress_s2[253][3];
int adress_s3[253][3];

int adress_c[253][4];//ya au maximum 253 adress (253 noeud)
cQueue queue[253]; //construire 253 buffer (253 nbr de noeud max)

int reply=0;
///////////////////////////////////////////////////////////////////////////////////
Define_Module (Server);


void Server::initialize()
{if(strcmp("serveur1", getName())==0){
    int nbr_noeud = getParentModule() -> par("x");

    for (int j = 0; j < nbr_noeud; j++)
            {
        Segment *m = new Segment();

        //cest un segment dhcp
        m->msg_dhcp =1;
        m->address[0]=10;
        m->address[1]=0;
        //choix aleatoire de x entre 0 et 2
        int x = uniform(0,2);//grouper les client aléatoirement en 3 groupe selon l'identificateur ss réseau
        m->address[2]=x;
        add++;
        m->address[3]=add;

        //remplir la table bdd dhcp
         adress_s1[j][0]=j;//la premiere colonne contient l'id du client
         adress_s1[j][1]=x;//la deuxieme colonne contient l'id du sous reseau
         adress_s1[j][2]=add;//la 3eme colonne contient l'id hote de ladresse ip

         send(m, "inoutport$o", j);//attribuer ladresse au client

         //envoyer ladresse au autres servers pour qu'il maj leur table dadresse
         Segment *m2 = new Segment();
         m2->MAJ_DHCP=1;
         m2->adress_maj=j;
         m2->ss_reseau=x;
         m2->idf_mac=add;
         send(m2, "serv1$o");
         Segment *m3 = new Segment();
         m3->MAJ_DHCP=1;
         m3->adress_maj=j;
         m3->ss_reseau=x;
         m3->idf_mac=add;
         send(m3, "serv2$o");


    }

}
}
void Server::handleMessage (cMessage *msg)
{int nbr_noeud = getParentModule() -> par("x");
    //////////////////////// DEBUT SERVER 1 ////////////////////
    if(strcmp("serveur1", getName())==0){


    Segment *m = check_and_cast<Segment *>(msg);

    i++;
    m->num_seq = i;

//envoi des msg recu au noeud appartenant au groupe de destination précisé par lemetteur
    EV <<getName() << " >  Diffusion du message " << i << " a tous les noeuds clients appartenant au groupe " << m->destination << " \n";
    for (int j = 0; j < nbr_noeud; j++)
            {
               if((m->source!=j)&&( adress_s1[j][1]==m->destination)){//la condition sert a veridier que le message ne sera pas renvoyé à lemetteur, et que que le message ne sera envoyé qu'aux membre du groupe que l'emetteur a choisit comme destination
                Segment *copy = m->dup();
                send(copy, "inoutport$o", j);
            }
            }


        if(m->num_seq==3){
            //envoyer un message REPLY au noeud emmeteur noeud 0
            Segment *reply = new Segment();
            reply->reply=1;
            send(reply, "inoutport$o", 0);
            EV <<getName() << " >  Envoi d'un message d'acquittement au noeud emetteur\n";

            //supprimer le noeud emetteur du groupe
                    adress_s1[m->source][0]=NULL;
                    adress_s1[m->source][1]=NULL;
                    adress_s1[m->source][2]=NULL;
            EV <<getName() << " >  Le noeud " << adress_s1[m->source][0] << " a été supprimer de la liste\n";

        }
}
///////////////////////////////////FIN SERVEUR 1/////////////////////


////////////////////////////////////DEBUT SERVEUR 2

        if(strcmp("serveur2", getName())==0){
            Segment *m = check_and_cast<Segment *>(msg);
            if(m->MAJ_DHCP!=NULL){
            adress_s2[m->adress_maj][0]=m->adress_maj;
            adress_s2[m->adress_maj][1]=m->ss_reseau;
            adress_s2[m->adress_maj][2]=m->idf_mac;}
            else{
                z++;
                m->num_seq = z;

                //envoi des msg recu au noeud appartenant au groupe de destination précisé par lemetteur
                    EV <<getName() << " >  Diffusion du message " << i << " a tous les noeuds clients appartenant au groupe " << m->destination << " \n";
                for (int j = 0; j < nbr_noeud; j++)
                        {
                           if((m->source!=j)&&( adress_s1[j][1]==m->destination)){//la condition sert a veridier que le message ne sera pas renvoyé à lemetteur, et que que le message ne sera envoyé qu'aux membre du groupe que l'emetteur a choisit comme destination
                            Segment *copy = m->dup();
                            send(copy, "inoutport$o", j);
                        }
                        }
                    if(m->num_seq==3){
                        //envoyer un message REPLY au noeud emmeteur noeud 0
                        Segment *reply = new Segment();
                        reply->reply=1;
                        send(reply, "inoutport$o", 0);
                        EV <<getName() << " >  Envoi d'un message d'acquittement au noeud emetteur\n";

                        //supprimer le noeud emetteur du groupe
                                adress_s1[m->source][0]=NULL;
                                adress_s1[m->source][1]=NULL;
                                adress_s1[m->source][2]=NULL;
                        EV <<getName() << " >  Le noeud " << adress_s1[m->source][0] << " a été supprimer de la liste\n";
                    }
            }

        }
//////////////////////////////////FIN SERVER 2///////////////////////

////////////////////////////////////DEBUT SERVEUR 3////////////

                if(strcmp("serveur3", getName())==0){
                    Segment *m = check_and_cast<Segment *>(msg);
                    if(m->MAJ_DHCP!=NULL){
                    adress_s3[m->adress_maj][0]=m->adress_maj;
                    adress_s3[m->adress_maj][1]=m->ss_reseau;
                    adress_s3[m->adress_maj][2]=m->idf_mac;}
                    else{
                        w++;
                        m->num_seq = w;

                        //envoi des msg recu au noeud appartenant au groupe de destination précisé par lemetteur
                            EV <<getName() << " >  Diffusion du message " << i << " a tous les noeuds clients appartenant au groupe " << m->destination << " \n";
                        for (int j = 0; j < nbr_noeud; j++)
                                {
                                   if((m->source!=j)&&( adress_s1[j][1]==m->destination)){//la condition sert a veridier que le message ne sera pas renvoyé à lemetteur, et que que le message ne sera envoyé qu'aux membre du groupe que l'emetteur a choisit comme destination
                                    Segment *copy = m->dup();
                                    send(copy, "inoutport$o", j);
                                }
                                }


                            if(m->num_seq==3){
                                //envoyer un message REPLY au noeud emmeteur noeud 0
                                Segment *reply = new Segment();
                                reply->reply=1;
                                send(reply, "inoutport$o", 0);
                                EV <<getName() << " >  Envoi d'un message d'acquittement au noeud emetteur\n";


                                //supprimer le noeud emetteur du groupe
                                        adress_s1[m->source][0]=NULL;
                                        adress_s1[m->source][1]=NULL;
                                        adress_s1[m->source][2]=NULL;
                                EV <<getName() << " >  Le noeud " << adress_s1[m->source][0] << " a été supprimer de la liste\n";

                            }
                    }

                }
        //////////////////////////////////FIN SERVER 3///////////////////////
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
Define_Module (Client);
void Client::initialize()
{
}
void Client::handleMessage (cMessage *msg)
{

   Segment *m = check_and_cast<Segment *>(msg);
   if(m->reply==NULL){
    if(m->msg_dhcp!=1){
    queue[getIndex()].insert(m);
    }
    else{
        adress_c[getIndex()][0]=m->address[0];
        adress_c[getIndex()][1]=m->address[1];
        adress_c[getIndex()][2]=m->address[2];
        adress_c[getIndex()][3]=m->address[3];

        if (getIndex()==0){
            EV <<" Client " <<  getIndex() <<" > Envoi du 1er message...\n ";
              Segment *msg1 = new Segment();
              msg1->source=getIndex();
              msg1->destination=0;//choix du groupe de destination
              send(msg1, "inoutport1$o");

              EV <<" Client " <<  getIndex() <<" > Envoi du 2eme message...\n ";
              Segment *msg2 = new Segment();
              msg2->source=getIndex();
              msg2->destination=0;//choix du groupe de destination
              send(msg2, "inoutport1$o");

              EV <<" Client " <<  getIndex() <<" > Envoi du 3eme message...\n ";
              Segment *msg3 = new Segment();
              msg3->source=getIndex();
              msg3->destination=0;//choix du groupe de destination
              send(msg3, "inoutport1$o");


          }
    }
   }
   else{
       if(getIndex()==0){reply=1;}
   }
}
void Client::finish()
{
    //verifier si le server1 est foncitonnel
    if(getIndex()==0){
        if(reply==0){
            int server_aleatoire = uniform(0,1);
        EV <<" Client " <<  getIndex() <<" > Envoi du 1er message à un autre server...\n ";
        Segment *msg1 = new Segment();
        msg1->source=getIndex();
        msg1->destination=0;//choix du groupe de destination
        if(server_aleatoire==0){ send(msg1, "inoutport2$o");}else{send(msg1, "inoutport3$o");}

        EV <<" Client " <<  getIndex() <<" > Envoi du 2eme message un autre server...\n ";
        Segment *msg2 = new Segment();
        msg2->source=getIndex();
        msg2->destination=0;//choix du groupe de destination
        if(server_aleatoire==0){ send(msg2, "inoutport2$o");}else{send(msg2, "inoutport3$o");}


        EV <<" Client " <<  getIndex() <<" > Envoi du 3eme message un autre server...\n ";
        Segment *msg3 = new Segment();
        msg3->source=getIndex();
        msg3->destination=0;//choix du groupe de destination
        if(server_aleatoire==0){ send(msg3, "inoutport2$o");}else{send(msg3, "inoutport3$o");}

        }
        else{
            EV <<" Client " <<  getIndex() <<" > Le server1 été disponible\n ";
            //le noeud 0 desactive sa pile protocolaire apres lenvoi des message
                          adress_c[getIndex()][0]=0;
                          adress_c[getIndex()][1]=0;
                          adress_c[getIndex()][2]=0;
                          adress_c[getIndex()][3]=0;
                          EV <<" Client " <<  getIndex() <<" > Désactivation de ma pile protocolaire\n ";
        }
    }




//////////////////////////////////////////////////////////
    int nbr_noeud = getParentModule() -> par("x");
    Segment vect[50][50];
    int i=0;
    for(int affiche=0;affiche<nbr_noeud;affiche++){
    if((getIndex()==affiche)&&(getIndex()!=0)){
///////////////////////////////////////////////////////////
        EV <<" Client " <<  getIndex() <<" > Mon adresse est la suivante : "<<  adress_c[getIndex()][0] <<"."<< adress_c[getIndex()][1] <<"."<<  adress_c[getIndex()][2] <<"."<<  adress_c[getIndex()][3] <<" \n";

      while(!queue[getIndex()].isEmpty()){
          Segment *m = (Segment *)queue[getIndex()].pop();
      vect[getIndex()][i]= *m;i++;
      }
      int j=1;
      int k=0;
      while(j<=3)
      {
          while(k<50){
                       if(vect[getIndex()][k].num_seq==j){ EV <<" Client " <<  getIndex() <<" > Le message ayant un numero de séquence = "<<  vect[getIndex()][k].num_seq <<" est consommé \n";j++;k=0;}else{k++;}
          }
          j++;k=0;
      }

    /////////////////////////////////////////////////////////
      }
    }
    }

