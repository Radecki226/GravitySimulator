#include<Windows.h>
#include<SFML/Graphics.hpp>
#include<time.h>
#include<cmath>
#include<iostream>
#include <string>
#include<sstream>
using namespace sf;
using namespace std;

#define PI 3.14159265

float cosd(float x){
    return cos(x*PI/180);
}

float sind(float x){
    return sin(x*PI/180);
}

int main(){

    //stale
    float skala=1722857142.8;
    float oddalenie=10;
    int srodekx=800;
    int srodeky=450;
    float G=6.67e-11;
    int N=11;
    int J=4;//liczba wyswietalnych pol
    int kierunek=+1;
    int czas=1987;
    float licznik=1; //liczy dni
    float predkosc=0.1;
    int dt=3600*24*predkosc;
    int sel = 0;

    cout << "Wartosc kroku calkowania[moze byc zmiennoprzecinkowa]: ";
    cin >> predkosc;


    //okno
    RenderWindow okno(sf::VideoMode(1600,900,32) ,"Symulacja ukladu slonecznego");

    //tekstury
    Texture planety_tex[N];

    planety_tex[0].loadFromFile("./slonce.jpg");
    planety_tex[1].loadFromFile("./merkury.jpg");
    planety_tex[2].loadFromFile("./wenus.jpg");
    planety_tex[3].loadFromFile("./ziemia.jpg");
    planety_tex[4].loadFromFile("./mars.jpg");
    planety_tex[5].loadFromFile("./jowisz.jpg");
    planety_tex[6].loadFromFile("./saturn.jpg");
    planety_tex[7].loadFromFile("./uran.jpg");
    planety_tex[8].loadFromFile("./neptun.png");
    planety_tex[9].loadFromFile("./pluton.jpg");
    planety_tex[10].loadFromFile("./kometa.jpg");


    //tekst
    Font myFont;
    myFont.loadFromFile("./PIXEARG_.ttf");

    Text rok;
    rok.setFont(myFont);
    rok.setString(to_string(czas));
    rok.setCharacterSize(30);
    rok.setFillColor(Color::White);

    Text dzien;
    dzien.setFont(myFont);
    dzien.setString(to_string(licznik));
    dzien.setCharacterSize(20);
    dzien.setFillColor(Color::White);
    dzien.setPosition(10,30);

    Text szybkosc;
    szybkosc.setFont(myFont);
    szybkosc.setString(to_string(predkosc));
    szybkosc.setCharacterSize(20);
    szybkosc.setFillColor(Color::White);
    szybkosc.setPosition(1200,0);

    Text podpis;
    podpis.setFont(myFont);
    podpis.setString("Piotr Radecki 2020");
    podpis.setCharacterSize(10);
    podpis.setFillColor(Color::White);
    podpis.setPosition(1450,840);
    Text dane[N][J];

    for (int i=0;i<N;i++){
        for (int j=0;j<J;j++){
            dane[i][j].setFont(myFont);
            dane[i][j].setCharacterSize(20);
            dane[i][j].setFillColor(Color::White);
            dane[i][j].setPosition(0,670+30*j);
        }
    }
    dane[0][0].setString("Slonce");
    dane[1][0].setString("Merkury");
    dane[2][0].setString("Wenus");
    dane[3][0].setString("Ziemia");
    dane[4][0].setString("Mars");
    dane[5][0].setString("Jowisz");
    dane[6][0].setString("Saturn");
    dane[7][0].setString("Uran");
    dane[8][0].setString("Neptun");
    dane[9][0].setString("Pluton");
    dane[10][0].setString("Nieistniejaca Kometa");


    //Stworzenie obiektow
    CircleShape planety[N];

    //rozmiary
    for (int i=0;i<N;i++){
        planety[i].setRadius(10);
    }

    //przypisanie tekstur
    for (int i=0;i<N;i++){

        planety[i].setTexture(&planety_tex[i]);
    }

    //wektory: vm-predkosc i masa r-polozenie
    vector<vector<float> > vm;
    vm.resize(N, std::vector<float>(3));

    vector<vector<float> > r;
    r.resize(N, std::vector<float>(2));

    vector<float> fi{0,-90,140,115,60,15,-95,-85,-80,-135};


    //masy
    vm[0][2]=1.989e30;
    vm[1][2]=3.28e23;
    vm[2][2]=4.68e24;
    vm[3][2]=5.97e24;
    vm[4][2]=6.39e23;
    vm[5][2]=1.9e27;
    vm[6][2]=5.68e26;
    vm[7][2]=8.68e25;
    vm[8][2]=1.024e26;
    vm[9][2]=1.303e22;
    vm[10][2]=1e16;

    //polozenia poczatkowe
    r[0][0] = 0;
    r[0][1] = 0;

    r[1][0] = 5.79e10*cosd(fi[1]);
    r[1][1] = -5.79e10*sind(fi[1]);

    r[2][0] = 1.08e11*cosd(fi[2]);
    r[2][1] = -1.08e11*sind(fi[2]);

    r[3][0] = 1.496e11*cosd(fi[3]);
    r[3][1] = -1.496e11*sind(fi[3]);

    r[4][0] = 2.279e11*cosd(fi[4]);
    r[4][1] = -2.279e11*sind(fi[4]);

    r[5][0] = 7.79e11*cosd(fi[5]);
    r[5][1] = -7.79e11*sind(fi[5]);

    r[6][0] = 1.434e12*cosd(fi[6]);
    r[6][1] = -1.434e12*sind(fi[6]);

    r[7][0] = 2.9e12*cosd(fi[7]);
    r[7][1] = -2.9e12*sind(fi[7]);

    r[8][0] = 4.495e12*cosd(fi[8]);
    r[8][1] = -4.495e12*sind(fi[8]);

    r[9][0] = 4.438e12*cosd(fi[9]);
    r[9][1] = -4.438e12*sind(fi[9]); //peryhelium plutona

    r[10][0] = 2e12;
    r[10][1] = 0;

    //predkosci poczatkowe

    for (int i=1;i<N-2;i++){

        vm[i][0]=sqrt(G*vm[0][2]/sqrt(pow(r[i][0]-r[0][0],2)+pow(r[i][1]-r[0][1],2)))*(-sind(fi[i]));
        vm[i][1]=sqrt(G*vm[0][2]/sqrt(pow(r[i][0]-r[0][0],2)+pow(r[i][1]-r[0][1],2)))*(-cosd(fi[i]));
    }


    //predkosc plutona
    vm[N-2][0] = -6.1e3*sind(fi[N-2]);
    vm[N-2][1] = -6.1e3*cosd(fi[N-2]);

    //predkosc komety
    vm[N-1][0] = -3e3;
    vm[N-1][1] = 2e3;

    //srodek masy ukladu słonecznego
    CircleShape masscentre;
    masscentre.setFillColor(Color::Blue);
    masscentre.setRadius(5);


    vector<float> masscentre_r{0,0};
    float temp[3] = {0,0,0};
    for (int i=0;i<N;i++){
        temp[2] = temp[2] + vm[i][2];
    }
    for (int i=0;i<N;i++){
        temp[0] = temp[0] + vm[i][2]/temp[2]*r[i][0];
        temp[1] = temp[1] + vm[i][2]/temp[2]*r[i][1];

    }
    masscentre_r[0]=temp[0];
    masscentre_r[1]=temp[1];

     //kontrola pozycji ziemii

    CircleShape ziemia0;
    ziemia0.setFillColor(Color::Blue);
    ziemia0.setRadius(10);

    vector<float> ziemia0_r{r[3][0],r[3][1]};



    //petla głowna
    while(okno.isOpen()){

        Event zdarzenie;

        while(okno.pollEvent(zdarzenie)){

                 if( zdarzenie.type == sf::Event::Closed )
                   okno.close();

                 else if (zdarzenie.type == Event::MouseButtonPressed){
                    if (zdarzenie.mouseButton.button == Mouse::Right){
                        kierunek=-kierunek;
                    }
                    else if(zdarzenie.mouseButton.button == Mouse::Left){
                        for (int i=0; i<N; i++){
                            if (sqrt(pow(Mouse::getPosition().x-planety[i].getPosition().x-planety[i].getRadius(),2)+pow(Mouse::getPosition().y-planety[i].getPosition().y-planety[i].getRadius(),2))<planety[i].getRadius())
                                sel=i;

                        }
                    }
                 }



                 else if (zdarzenie.type==sf::Event::MouseWheelMoved){
                    if (zdarzenie.mouseWheel.delta==1){
                        oddalenie = oddalenie/2;
                    }
                    else if (zdarzenie.mouseWheel.delta==-1){
                        oddalenie = oddalenie*2;
                    }

                 }


        }



            for (int n=0; n< N; n++){

                //a-przyspieszenie
                vector<float> a(2);
                a[0]=0;a[1]=0;

                for (int k=0;k<N;k++){
                    if (k != n){

                        //zwiekszanie predkosc w zaleznosci od odzia³ywan grawitacyjnych
                        a[0] = a[0]-G*vm[k][2]*(r[n][0]-r[k][0])/pow(sqrt(pow(r[n][0]-r[k][0],2)+pow(r[n][1]-r[k][1],2)),3);
                        a[1] = a[1]-G*vm[k][2]*(r[n][1]-r[k][1])/pow(sqrt(pow(r[n][0]-r[k][0],2)+pow(r[n][1]-r[k][1],2)),3);
                    }
                }



            //v = v- + a*dt
            vm[n][0] = vm[n][0] + kierunek*dt*a[0];
            vm[n][1] = vm[n][1] + kierunek*dt*a[1];

            //r = r- + v*dt
            r[n][0] = r[n][0]+dt*kierunek*vm[n][0];
            r[n][1] = r[n][1]+dt*kierunek*vm[n][1];

            //przelozenie r na polozenie
            planety[n].setPosition(srodekx+(r[n][0]/skala)/oddalenie-planety[n].getRadius(),srodeky+(r[n][1]/skala)/oddalenie-planety[n].getRadius());



    }
    //Srodek masy
    temp[0]=temp[1]=temp[2]=0;
    for (int i=0;i<N;i++){
        temp[2] = temp[2] + vm[i][2];
    }
    for (int i=0;i<N;i++){
        temp[0] = temp[0] + vm[i][2]/temp[2]*r[i][0];
        temp[1] = temp[1] + vm[i][2]/temp[2]*r[i][1];

    }
    masscentre_r[0]=temp[0];
    masscentre_r[1]=temp[1];
    masscentre.setPosition(srodekx+(masscentre_r[0]/skala)/oddalenie-masscentre.getRadius(),srodeky+(masscentre_r[1]/skala)/oddalenie-masscentre.getRadius());

    //kontrola pozycji ziemii
    ziemia0.setPosition(srodekx+(ziemia0_r[0]/skala)/oddalenie-ziemia0.getRadius(),srodeky+(ziemia0_r[1]/skala)/oddalenie-ziemia0.getRadius());

    //poruszanie całoscia
    if( Keyboard::isKeyPressed( Keyboard::Up ) ){
        for (int i=0; i<N;i++){
            r[i][1] = r[i][1] + 3e9*oddalenie;
        }
        ziemia0_r[1] = ziemia0_r[1] + 3e9*oddalenie;
    }
    if( Keyboard::isKeyPressed( Keyboard::Down ) ){
        for (int i=0; i<N;i++){
            r[i][1] = r[i][1] - 3e9*oddalenie;
        }
        ziemia0_r[1] = ziemia0_r[1] - 3e9*oddalenie;
    }
    if( Keyboard::isKeyPressed( Keyboard::Right ) ){
        for (int i=0; i<N;i++){
            r[i][0] = r[i][0] - 3e9*oddalenie;
        }
        ziemia0_r[0] = ziemia0_r[0] - 3e9*oddalenie;
    }
    if( Keyboard::isKeyPressed( Keyboard::Left ) ){
        for (int i=0; i<N;i++){
            r[i][0] = r[i][0] + 3e9*oddalenie;
        }
        ziemia0_r[0] = ziemia0_r[0] + 3e9*oddalenie;
    }


    //inkrementacja masy wybranej planety
    if( Keyboard::isKeyPressed( Keyboard::W ) ){
        vm[sel][2] = vm[sel][2]*1.05;
    }
    if( Keyboard::isKeyPressed( Keyboard::S ) ){
        vm[sel][2] = vm[sel][2]/1.05;
    }
    //zwiekszanie predkosci symulacji
    if( Keyboard::isKeyPressed( Keyboard::A ) ){
        predkosc = predkosc*1.05;
    }
    if( Keyboard::isKeyPressed( Keyboard::D ) ){
        predkosc = predkosc/1.05;
    }
    dt=24*3600*predkosc;



    //rozmiary
    for (int i=0;i<N;i++){
        planety[i].setRadius(10/oddalenie);
    }
    masscentre.setRadius(0.1/oddalenie);
    ziemia0.setRadius(10/oddalenie);

    //wyswietlanie roku
    //Przestepny:
    if (czas%4==0){
        if (kierunek==1){
            if (licznik>=367){
                licznik=1;
                czas++;
            }
            else
                licznik=licznik+predkosc;
        }
        else{
            if (licznik<=1){
                licznik=366;
                czas--;
            }
            else
                licznik=licznik-predkosc;
        }
    }
    //rok po przestepnym
   else if ((czas-1)%4==0){
        if (kierunek==1){
            if (licznik>=366){
                licznik=1;
                czas++;
            }
            else
                licznik=licznik+predkosc;
        }
        else{
            if (licznik<=1){
                licznik=367;
                czas--;
            }
            else
                licznik=licznik-predkosc;
        }
    }
    //kazdy inny
    else{
        if (kierunek==1){
            if (licznik>=366){
                licznik=1;
                czas++;
            }
            else
                licznik=licznik+predkosc;
        }
        else{
            if (licznik<=1){
                licznik=366;
                czas--;
            }
            else
                licznik=licznik-predkosc;
        }
    }


    //tekst
    rok.setString(to_string(czas));
    dzien.setString(to_string(int(licznik)));

    ostringstream ostr;
    ostr << predkosc;

    szybkosc.setString("Szybkosc symulacji: "+ostr.str());

    ostr.str("");
    ostr.clear();

    for (int i=0;i<N;i++){
        ostr << vm[i][2];
        dane[i][1].setString("Masa[kg]: " + ostr.str());

        ostr.str("");
        ostr.clear();
    }
    for (int i=0;i<N;i++){
        ostr << (sqrt(pow(r[i][0]-r[0][0],2)+pow(r[i][1]-r[0][1],2))/1.496e11);
        dane[i][2].setString("Odleglosc od slonca[au]: " + ostr.str());

        ostr.str("");
        ostr.clear();
    }

    for (int i=0;i<N;i++){
        ostr << sqrt(pow(vm[i][0],2)+pow(vm[i][1],2))/1000;
        dane[i][3].setString("Predkosc[km/s]: " + ostr.str());

        ostr.str("");
        ostr.clear();
    }

    //wyswietlanie
    okno.clear(Color::Black);

    for (int i=0;i<N;i++){
        okno.draw(planety[i]);
    }
    for (int j=0;j<J;j++){
        okno.draw(dane[sel][j]);
    }
    okno.draw(szybkosc);
    okno.draw(masscentre);
    okno.draw(rok);
    okno.draw(dzien);
    okno.draw(ziemia0);
    okno.draw(podpis);
    //cout << "Odleglosc srodka slonca od srodka masy[km]: " << sqrt(pow(r[0][0]-masscentre_r[0],2)+pow(r[0][1]-masscentre_r[1],2))/1000 << endl;
    //cout << "Odleglosc plutona[au]" <<  sqrt(pow(r[0][0]-r[9][0],2)+pow(r[0][1]-r[9][1],2))/1.496e11 << endl;
    cout << Mouse::getPosition().x <<"  " << Mouse::getPosition().y << endl;
    okno.display();



}
}
