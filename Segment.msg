packet Segment {
    int source;
    int destination;//id du groupe de destination
    int num_seq;
    int msg_dhcp;
    int address[4];
    
    int MAJ_DHCP;//booleen si = 1 donc cest un msg de maj de la table dadressage envoyé par un server à un autre
   
    int adress_maj;//id du noeud auquel son adresse va etre maj dans un autre server
    int ss_reseau;//valeur du troisieme octet de ladresse (idf groupe)
    int idf_mac;//valeur du 4eme octet de ladresse (idf machine)
    
    int reply;
}