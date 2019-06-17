

package part3;

simple Client
{
    @display("i=device/pc4");
    gates:
        inout inoutport1;
        inout inoutport2;
        inout inoutport3;
}

simple Server
{
    @display("i=device/server");
    gates:
        inout serv1;
        inout serv2;
        inout inoutport[];
}

network Part3
{
    parameters:
        int x;
        int i = 0;

        @display("bgl=2;bgb=389,312");
    submodules:
        serveur1: Server {
            @display("p=342,234");
        }
        serveur2: Server {
            @display("p=176,39");
        }
        serveur3: Server {
            @display("p=31,234");
        }
        client[x]: Client;
    connections:

        serveur1.serv1 <--> {  delay = 500 ms; } <--> serveur2.serv1;
        serveur2.serv2 <--> {  delay = 500 ms; } <--> serveur3.serv1;
        serveur3.serv2 <--> {  delay = 500 ms; } <--> serveur1.serv2;
        for i=0..x-1 {
            client[i].inoutport1 <--> {  delay = 500 ms; } <--> serveur1.inoutport++;
            client[i].inoutport2 <--> {  delay = 500 ms; } <--> serveur2.inoutport++;
            client[i].inoutport3 <--> {  delay = 500 ms; } <--> serveur3.inoutport++;
        }

}

